#include <Luke/SDL/SDLWindowImpl.hpp>
#include <Luke/SDL/SDLDisplayImpl.hpp>
#include <Luke/SDL/SDLInitializer.hpp>
#include <Luke/WindowEvents.hpp>
#include <Luke/KeyEvents.hpp>
#include <Luke/TextInputEvent.hpp>
#include <Stick/Maybe.hpp>

//@TODO Proper error code
#define RETURN_SDL_ERROR(_item) \
do \
{ \
    if(!_item) \
    { \
        return Error(ec::InvalidOperation, sdlError(), STICK_FILE, STICK_LINE); \
    } \
} while(0)

namespace luke
{
    namespace detail
    {
        using namespace stick;

        static const String sdlError()
        {
            String ret;
            const char * error = SDL_GetError();
            if (*error != '\0')
            {
                ret = String::formatted("SDL Error: %s\n", error);
                SDL_ClearError();
            }
            return ret;
        }

        //@TODO: We might want to lock this for thread safety/portability reasons
        static DynamicArray<WindowImpl *> g_sdlWindows;
        static MouseState g_mouseState;
        static Maybe<MouseButton> g_initialDragButton;

        WindowImpl::WindowImpl(Window * _window) :
            m_sdlWindow(NULL),
            m_window(_window),
            m_sdlGLContext(NULL),
            m_bShouldClose(false),
            m_sdlWindowID(0)
        {

        }

        WindowImpl::~WindowImpl()
        {
            deallocateSDLWindowAndContext();
        }

        void WindowImpl::deallocateSDLWindowAndContext()
        {
            if (m_sdlWindow)
            {
                SDL_GL_DeleteContext(m_sdlGLContext);
                SDL_DestroyWindow(m_sdlWindow);

                auto it = stick::find(g_sdlWindows.begin(), g_sdlWindows.end(), this);
                if (it != g_sdlWindows.end())
                    g_sdlWindows.remove(it);

                m_sdlGLContext = NULL;
                m_sdlWindow = NULL;

                for (Size i = 0; i < m_cursors.count(); ++i)
                {
                    SDL_FreeCursor(m_cursors[i]);
                }

                m_sdlWindowID = 0;
            }
        }

        Error WindowImpl::open(const WindowSettings & _settings, WindowImpl * _shared)
        {
            SDLInitializer::instance();

            deallocateSDLWindowAndContext();

            SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, "1");

            //@TODO allow to pass in opengl version in the settings
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, _settings.depthPrecision());
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, _settings.stencilPrecision());
            SDL_GL_SetAttribute(SDL_GL_RED_SIZE, _settings.colorPrecision());
            SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, _settings.colorPrecision());
            SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, _settings.colorPrecision());
            SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, _settings.alphaPrecision());
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
            if (_settings.sampleCount() > 1)
            {
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, _settings.sampleCount());
            }

            UInt32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

            if (_settings.isResizeable()) flags |= SDL_WINDOW_RESIZABLE;
            if (!_settings.isDecorated()) flags |= SDL_WINDOW_BORDERLESS;

            //@TODO take settings poistion into account
            m_sdlWindow = SDL_CreateWindow(_settings.title().length() ? _settings.title().cString() : "Luke Window",
                                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           _settings.width(), _settings.height(), flags);
            RETURN_SDL_ERROR(m_sdlWindow);

            m_sdlGLContext = SDL_GL_CreateContext(m_sdlWindow);
            RETURN_SDL_ERROR(m_sdlGLContext);

            g_sdlWindows.append(this);
            m_bShouldClose = false;

            //create the default cursors
            static_assert(CursorArray::capacity() == (Size)CursorType::_Count - 1, "Not all default cursors satisfied!");

            m_cursors[(Size)CursorType::Cursor] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            m_cursors[(Size)CursorType::TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
            m_cursors[(Size)CursorType::ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
            m_cursors[(Size)CursorType::ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
            m_cursors[(Size)CursorType::ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
            m_cursors[(Size)CursorType::ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
            m_cursors[(Size)CursorType::ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);


            m_sdlWindowID = SDL_GetWindowID(m_sdlWindow);
            return Error();
        }

        void WindowImpl::close()
        {
            STICK_ASSERT(m_sdlWindow);
            deallocateSDLWindowAndContext();
        }

        void WindowImpl::setShouldClose(bool _b)
        {
            m_bShouldClose = _b;
        }

        bool WindowImpl::shouldClose() const
        {
            STICK_ASSERT(m_sdlWindow);
            return m_bShouldClose;
        }

        void WindowImpl::move(Float32 _x, Float32 _y)
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_SetWindowPosition(m_sdlWindow, _x, _y);
        }

        void WindowImpl::moveToCenter()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_SetWindowPosition(m_sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        }

        void WindowImpl::show()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_ShowWindow(m_sdlWindow);
        }

        void WindowImpl::hide()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_HideWindow(m_sdlWindow);
        }

        void WindowImpl::resize(Float32 _width, Float32 _height)
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_SetWindowSize(m_sdlWindow, _width, _height);
        }

        void WindowImpl::maximize()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_MaximizeWindow(m_sdlWindow);
        }

        void WindowImpl::focus()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_RaiseWindow(m_sdlWindow);
        }

        Error WindowImpl::enableRenderContext()
        {
            STICK_ASSERT(m_sdlWindow);
            int res = SDL_GL_MakeCurrent(m_sdlWindow, m_sdlGLContext);
            RETURN_SDL_ERROR(res);
            return Error();
        }

        void WindowImpl::swapBuffers()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_GL_SwapWindow(m_sdlWindow);
        }

        void WindowImpl::setVerticalSync(bool _b)
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_GL_SetSwapInterval(_b);
        }

        //@TODO: Merge the code for both enter fullscreen functions into a helper
        Error WindowImpl::enterFullscreen(FullScreenMode _mode, const Display & _display)
        {
            STICK_ASSERT(m_sdlWindow);

            SDL_SetWindowFullscreen(m_sdlWindow,
                                    _mode == FullScreenMode::Borderless ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN);

            return Error();
        }

        Error WindowImpl::enterFullscreen(const DisplayMode & _mode, const Display & _display)
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_SetWindowFullscreen(m_sdlWindow, SDL_WINDOW_FULLSCREEN);

            return Error();
        }

        Error WindowImpl::exitFullscreen()
        {
            STICK_ASSERT(m_sdlWindow);
            int res = SDL_SetWindowFullscreen(m_sdlWindow, 0);
            RETURN_SDL_ERROR(res);
            return Error();
        }

        void WindowImpl::setCursor(CursorType _cursor)
        {
            STICK_ASSERT((Size)_cursor < m_cursors.count());
            SDL_SetCursor(m_cursors[(Size)_cursor]);
        }

        void WindowImpl::hideCursor()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_ShowCursor(SDL_DISABLE);
        }

        void WindowImpl::showCursor()
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_ShowCursor(SDL_ENABLE);
        }

        void WindowImpl::setTitle(const String & _str)
        {
            STICK_ASSERT(m_sdlWindow);
            SDL_SetWindowTitle(m_sdlWindow, _str.cString());
        }

        bool WindowImpl::isVisible() const
        {
            STICK_ASSERT(m_sdlWindow);
            return (SDL_GetWindowFlags(m_sdlWindow) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_SHOWN;
        }

        bool WindowImpl::isFocussed() const
        {
            STICK_ASSERT(m_sdlWindow);
            return (SDL_GetWindowFlags(m_sdlWindow) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_INPUT_FOCUS;
        }

        const WindowSettings & WindowImpl::settings() const
        {

        }

        bool WindowImpl::isCursorVisible() const
        {
            return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
        }

        bool WindowImpl::verticalSync() const
        {
            return SDL_GL_GetSwapInterval();
        }

        bool WindowImpl::isFullscreen() const
        {
            STICK_ASSERT(m_sdlWindow);
            return (SDL_GetWindowFlags(m_sdlWindow) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;
        }

        const String & WindowImpl::title() const
        {
            STICK_ASSERT(m_sdlWindow);
            m_title = SDL_GetWindowTitle(m_sdlWindow);
            return m_title;
        }

        Float32 WindowImpl::backingScaleFactor() const
        {
            return widthInPixels() / width();
        }

        Float32 WindowImpl::width() const
        {
            if (m_sdlWindow)
            {
                int w;
                SDL_GetWindowSize(m_sdlWindow, &w, NULL);
                return w;
            }
            return 0;
        }

        Float32 WindowImpl::height() const
        {
            if (m_sdlWindow)
            {
                int h;
                SDL_GetWindowSize(m_sdlWindow, NULL, &h);
                return h;
            }
            return 0;
        }

        Float32 WindowImpl::widthInPixels() const
        {
            if (m_sdlWindow)
            {
                int w;
                SDL_GL_GetDrawableSize(m_sdlWindow, &w, NULL);
                return w;
            }
            return 0;
        }

        Float32 WindowImpl::heightInPixels() const
        {
            if (m_sdlWindow)
            {
                int h;
                SDL_GL_GetDrawableSize(m_sdlWindow, NULL, &h);
                return h;
            }
            return 0;
        }

        Float32 WindowImpl::x() const
        {
            if (m_sdlWindow)
            {
                int x;
                SDL_GetWindowPosition(m_sdlWindow, &x, NULL);
                return x;
            }
            return 0;
        }

        Float32 WindowImpl::y() const
        {
            if (m_sdlWindow)
            {
                int y;
                SDL_GetWindowPosition(m_sdlWindow, NULL, &y);
                return y;
            }
            return 0;
        }

        Display WindowImpl::display() const
        {
            if (m_sdlWindow)
            {
                int idx = SDL_GetWindowDisplayIndex(m_sdlWindow);
                if (idx >= 0)
                {
                    Display ret;
                    ret.m_pimpl->m_displayID = idx;
                    return ret;
                }
            }
            return Display();
        }

        UInt32 WindowImpl::sdlWindowID() const
        {
            return m_sdlWindowID;
        }

        static WindowImpl * windowForEvent(SDL_Event * _event)
        {
            auto it = g_sdlWindows.begin();
            for (; it != g_sdlWindows.end(); ++it)
            {
                if ((*it)->sdlWindowID() == _event->window.windowID)
                    return *it;
            }
            return nullptr;
        }

        static void handleWindowEvent(SDL_Event * _event)
        {
            STICK_ASSERT(_event->type == SDL_WINDOWEVENT);

            WindowImpl * window = windowForEvent(_event);
            if (!window) return;

            switch (_event->window.event)
            {
                case SDL_WINDOWEVENT_SHOWN:
                    // SDL_Log("Window %d shown", _event->window.windowID);
                    break;
                case SDL_WINDOWEVENT_HIDDEN:
                    // SDL_Log("Window %d hidden", _event->window.windowID);
                    break;
                case SDL_WINDOWEVENT_EXPOSED:
                    // SDL_Log("Window %d exposed", _event->window.windowID);
                    //this should not be necessary but on osx the window flickers after
                    //exitting from fullscreen unless we move it (or force a refresh somehow)
                    break;
                case SDL_WINDOWEVENT_MOVED:
                    // SDL_Log("Window %d moved to %d,%d",
                    //         _event->window.windowID, _event->window.data1,
                    //         _event->window.data2);
                    window->m_window->publish(WindowMoveEvent(_event->window.data1, _event->window.data2), true);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    // SDL_Log("Window %d resized to %dx%d",
                    //         _event->window.windowID, _event->window.data1,
                    //         _event->window.data2);
                    window->m_window->publish(WindowResizeEvent(_event->window.data1, _event->window.data2), true);
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    // SDL_Log("Window %d minimized", _event->window.windowID);
                    window->m_window->publish(WindowIconifyEvent(), true);
                    break;
                case SDL_WINDOWEVENT_MAXIMIZED:
                    // SDL_Log("Window %d maximized", _event->window.windowID);
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    // SDL_Log("Window %d restored", _event->window.windowID);
                    window->m_window->publish(WindowRestoreEvent(), true);
                    break;
                case SDL_WINDOWEVENT_ENTER:
                    // SDL_Log("Mouse entered window %d",
                    // _event->window.windowID);
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    // SDL_Log("Mouse left window %d", _event->window.windowID);
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    // SDL_Log("Window %d gained keyboard focus",
                    //         _event->window.windowID);
                    window->m_window->publish(WindowFocusEvent(), true);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    // SDL_Log("Window %d lost keyboard focus",
                    //         _event->window.windowID);
                    window->m_window->publish(WindowLostFocusEvent(), true);
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    // SDL_Log("Window %d closed", _event->window.windowID);
                    window->m_bShouldClose = true;
                    window->hide();
                    break;
                default:
                    // SDL_Log("Window %d got unknown event %d",
                    //         _event->window.windowID, _event->window.event);
                    break;
            }
        }

        static void handkeKeyEvent(SDL_Event * _event)
        {
            STICK_ASSERT(_event->type == SDL_KEYDOWN || _event->type == SDL_KEYUP);

            WindowImpl * window = windowForEvent(_event);
            if (!window) return;

            KeyCode code;
            switch (_event->key.keysym.sym)
            {
                case SDLK_0:
                    code = KeyCode::Zero;
                    break;
                case SDLK_1:
                    code = KeyCode::One;
                    break;
                case SDLK_2:
                    code = KeyCode::Two;
                    break;
                case SDLK_3:
                    code = KeyCode::Three;
                    break;
                case SDLK_4:
                    code = KeyCode::Four;
                    break;
                case SDLK_5:
                    code = KeyCode::Five;
                    break;
                case SDLK_6:
                    code = KeyCode::Six;
                    break;
                case SDLK_7:
                    code = KeyCode::Seven;
                    break;
                case SDLK_8:
                    code = KeyCode::Eight;
                    break;
                case SDLK_9:
                    code = KeyCode::Nine;
                    break;

                case SDLK_a:
                    code = KeyCode::A;
                    break;
                case SDLK_b:
                    code = KeyCode::B;
                    break;
                case SDLK_c:
                    code = KeyCode::C;
                    break;
                case SDLK_d:
                    code = KeyCode::D;
                    break;
                case SDLK_e:
                    code = KeyCode::E;
                    break;
                case SDLK_f:
                    code = KeyCode::F;
                    break;
                case SDLK_g:
                    code = KeyCode::G;
                    break;
                case SDLK_h:
                    code = KeyCode::H;
                    break;
                case SDLK_i:
                    code = KeyCode::I;
                    break;
                case SDLK_j:
                    code = KeyCode::J;
                    break;
                case SDLK_k:
                    code = KeyCode::K;
                    break;
                case SDLK_l:
                    code = KeyCode::L;
                    break;
                case SDLK_m:
                    code = KeyCode::M;
                    break;
                case SDLK_n:
                    code = KeyCode::N;
                    break;
                case SDLK_o:
                    code = KeyCode::O;
                    break;
                case SDLK_p:
                    code = KeyCode::P;
                    break;
                case SDLK_q:
                    code = KeyCode::Q;
                    break;
                case SDLK_r:
                    code = KeyCode::R;
                    break;
                case SDLK_s:
                    code = KeyCode::S;
                    break;
                case SDLK_t:
                    code = KeyCode::T;
                    break;
                case SDLK_u:
                    code = KeyCode::U;
                    break;
                case SDLK_v:
                    code = KeyCode::V;
                    break;
                case SDLK_w:
                    code = KeyCode::W;
                    break;
                case SDLK_x:
                    code = KeyCode::X;
                    break;
                case SDLK_y:
                    code = KeyCode::Y;
                    break;
                case SDLK_z:
                    code = KeyCode::Z;
                    break;

                case SDLK_RETURN:
                    code = KeyCode::Return;
                    break;
                case SDLK_ESCAPE:
                    code = KeyCode::Escape;
                    break;
                case SDLK_BACKSPACE:
                    code = KeyCode::Backspace;
                    break;
                case SDLK_TAB:
                    code = KeyCode::Tab;
                    break;
                case SDLK_SPACE:
                    code = KeyCode::Space;
                    break;
                case SDLK_MINUS:
                    code = KeyCode::Subtract;
                    break;
                case SDLK_EQUALS:
                    code = KeyCode::Equal;
                    break;
                case SDLK_LEFTBRACKET:
                    code = KeyCode::LeftBracket;
                    break;
                case SDLK_RIGHTBRACKET:
                    code = KeyCode::RightBracket;
                    break;
                case SDLK_BACKSLASH:
                    code = KeyCode::Backslash;
                    break;
                case SDLK_SEMICOLON:
                    code = KeyCode::Semicolon;
                    break;
                case SDLK_QUOTE:
                    code = KeyCode::Apostrophe;
                    break;
                case SDLK_BACKQUOTE:
                    code = KeyCode::GraveAccent; //Not sure about this one
                    break;
                case SDLK_COMMA:
                    code = KeyCode::Comma;
                    break;
                case SDLK_PERIOD:
                    code = KeyCode::Period;
                    break;
                case SDLK_SLASH:
                    code = KeyCode::Slash;
                    break;
                case SDLK_CAPSLOCK:
                    code = KeyCode::CapsLock;
                    break;

                case SDLK_F1:
                    code = KeyCode::F1;
                    break;
                case SDLK_F2:
                    code = KeyCode::F2;
                    break;
                case SDLK_F3:
                    code = KeyCode::F3;
                    break;
                case SDLK_F4:
                    code = KeyCode::F4;
                    break;
                case SDLK_F5:
                    code = KeyCode::F5;
                    break;
                case SDLK_F6:
                    code = KeyCode::F6;
                    break;
                case SDLK_F7:
                    code = KeyCode::F7;
                    break;
                case SDLK_F8:
                    code = KeyCode::F8;
                    break;
                case SDLK_F9:
                    code = KeyCode::F9;
                    break;
                case SDLK_F10:
                    code = KeyCode::F10;
                    break;
                case SDLK_F11:
                    code = KeyCode::F11;
                    break;
                case SDLK_F12:
                    code = KeyCode::F12;
                    break;
                case SDLK_F13:
                    code = KeyCode::F13;
                    break;
                case SDLK_F14:
                    code = KeyCode::F14;
                    break;
                case SDLK_F15:
                    code = KeyCode::F15;
                    break;
                case SDLK_F16:
                    code = KeyCode::F16;
                    break;

                case SDLK_INSERT:
                    code = KeyCode::Insert;
                    break;
                case SDLK_HOME:
                    code = KeyCode::Home;
                    break;
                case SDLK_PAGEUP:
                    code = KeyCode::PageUp;
                    break;
                case SDLK_DELETE:
                    code = KeyCode::Delete;
                    break;
                case SDLK_END:
                    code = KeyCode::End;
                    break;
                case SDLK_PAGEDOWN:
                    code = KeyCode::PageDown;
                    break;

                case SDLK_LCTRL:
                    code = KeyCode::LeftControl;
                    break;
                case SDLK_LSHIFT:
                    code = KeyCode::LeftShift;
                    break;
                case SDLK_LALT:
                    code = KeyCode::LeftAlt;
                    break;
                case SDLK_LGUI:
                    code = KeyCode::LeftCommand;
                    break;
                case SDLK_RCTRL:
                    code = KeyCode::RightControl;
                    break;
                case SDLK_RSHIFT:
                    code = KeyCode::RightShift;
                    break;
                case SDLK_RALT:
                    code = KeyCode::RightAlt;
                    break;
                case SDLK_RGUI:
                    code = KeyCode::RightCommand;
                    break;

                case SDLK_RIGHT:
                    code = KeyCode::Right;
                    break;
                case SDLK_LEFT:
                    code = KeyCode::Left;
                    break;
                case SDLK_DOWN:
                    code = KeyCode::Down;
                    break;
                case SDLK_UP:
                    code = KeyCode::Up;
                    break;

                case SDLK_KP_DIVIDE:
                    code = KeyCode::KPDivide;
                    break;
                case SDLK_KP_MULTIPLY:
                    code = KeyCode::KPMultiply;
                    break;
                case SDLK_KP_MINUS:
                    code = KeyCode::KPSubtract;
                    break;
                case SDLK_KP_PLUS:
                    code = KeyCode::KPAdd;
                    break;
                case SDLK_KP_ENTER:
                    code = KeyCode::KPReturn;
                    break;
                case SDLK_KP_1:
                    code = KeyCode::KPOne;
                    break;
                case SDLK_KP_2:
                    code = KeyCode::KPTwo;
                    break;
                case SDLK_KP_3:
                    code = KeyCode::KPThree;
                    break;
                case SDLK_KP_4:
                    code = KeyCode::KPFour;
                    break;
                case SDLK_KP_5:
                    code = KeyCode::KPFive;
                    break;
                case SDLK_KP_6:
                    code = KeyCode::KPSix;
                    break;
                case SDLK_KP_7:
                    code = KeyCode::KPSeven;
                    break;
                case SDLK_KP_8:
                    code = KeyCode::KPEight;
                    break;
                case SDLK_KP_9:
                    code = KeyCode::KPNine;
                    break;
                case SDLK_KP_0:
                    code = KeyCode::KPZero;
                    break;
                case SDLK_KP_EQUALS:
                    code = KeyCode::KPEqual;
                    break;

                default:
                    code = KeyCode::Unknown;
            }

            if (_event->type == SDL_KEYUP)
                window->m_window->publish(KeyUpEvent(code, _event->key.keysym.scancode), true);
            else
                window->m_window->publish(KeyDownEvent(code, _event->key.repeat > 0, _event->key.keysym.scancode), true);
        }

        static void handleMouseEvent(SDL_Event * _event)
        {
            STICK_ASSERT(_event->type == SDL_MOUSEMOTION ||
                         _event->type == SDL_MOUSEWHEEL ||
                         _event->type == SDL_MOUSEBUTTONDOWN ||
                         _event->type == SDL_MOUSEBUTTONUP);

            WindowImpl * window = windowForEvent(_event);
            if (!window) return;

            if (_event->type == SDL_MOUSEBUTTONDOWN ||
                    _event->type == SDL_MOUSEBUTTONUP)
            {
                MouseButton btn;

                switch (_event->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        btn = MouseButton::Left;
                        break;
                    case SDL_BUTTON_RIGHT:
                        btn = MouseButton::Right;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        btn = MouseButton::Middle;
                        break;
                    case SDL_BUTTON_X1:
                        btn = MouseButton::Button3;
                        break;
                    case SDL_BUTTON_X2:
                        btn = MouseButton::Button4;
                        break;
                    //@TODO: We should mostly still propagate the button value as
                    //something implementation dependant
                    default:
                        btn = MouseButton::None;
                }

                if (_event->type == SDL_MOUSEBUTTONDOWN)
                {
                    g_mouseState.setButtonBitMask(g_mouseState.buttonBitMask() | (UInt32)btn);
                    window->m_window->publish(MouseDownEvent(g_mouseState, btn), true);
                    if (!g_initialDragButton)
                    {
                        g_initialDragButton = btn;
                        SDL_CaptureMouse(SDL_TRUE);
                    }
                }
                else
                {
                    g_mouseState.setButtonBitMask(g_mouseState.buttonBitMask() & ~(UInt32)btn);
                    window->m_window->publish(MouseUpEvent(g_mouseState, btn), true);
                    if (g_initialDragButton && btn == *g_initialDragButton)
                    {
                        g_initialDragButton.reset();
                        SDL_CaptureMouse(SDL_FALSE);
                    }
                }
            }
            else if (_event->type == SDL_MOUSEMOTION)
            {
                g_mouseState.setPosition(_event->motion.x, _event->motion.y);
                window->m_window->publish(MouseMoveEvent(g_mouseState), true);
            }
            else if (_event->type == SDL_MOUSEWHEEL)
            {
                window->m_window->publish(MouseScrollEvent(g_mouseState, _event->wheel.x, _event->wheel.y), true);
            }
        }

        static void handleTextInputEvent(SDL_Event * _event)
        {
            STICK_ASSERT(_event->type == SDL_TEXTEDITING ||
                         _event->type == SDK: SDL_TEXTINPUT);

            WindowImpl * window = windowForEvent(_event);
            if (!window) return;

            window->m_window->publish(TextInputEvent(_event->text.text), true);
        }

        Error WindowImpl::pollEvents()
        {
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0)
            {
                switch (e.type)
                {
                    case SDL_QUIT:
                        for (WindowImpl * wnd : g_sdlWindows)
                            wnd->setShouldClose(true);
                        break;
                    case SDL_WINDOWEVENT:
                        handleWindowEvent(&e);
                        break;
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        handkeKeyEvent(&e);
                        break;
                    case SDL_MOUSEMOTION:
                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEWHEEL:
                        handleMouseEvent(&e);
                        break;
                    case SDL_TEXTINPUT:
                        // case SDL_TEXTEDITING:
                        handleTextInputEvent(&e);
                        break;
                }
            }
            return Error();
        }

        Error WindowImpl::setClipboardText(const char * _text)
        {
            int res = SDL_SetClipboardText(_text);
            RETURN_SDL_ERROR(res);
            return Error();
        }

        const char * WindowImpl::clipboardText()
        {
            return SDL_GetClipboardText();
        }

        bool WindowImpl::hasClipboardText()
        {
            return SDL_HasClipboardText();
        }

        const MouseState & WindowImpl::mouseState()
        {
            return g_mouseState;
        }

        bool WindowImpl::isKeyDown(KeyCode _code)
        {
            int code;
            switch (_code)
            {
                case KeyCode::Zero:
                    code = SDLK_0;
                    break;
                case KeyCode::One:
                    code = SDLK_1;
                    break;
                case KeyCode::Two:
                    code = SDLK_2;
                    break;
                case KeyCode::Three:
                    code = SDLK_3;
                    break;
                case KeyCode::Four:
                    code = SDLK_4;
                    break;
                case KeyCode::Five:
                    code = SDLK_5;
                    break;
                case KeyCode::Six:
                    code = SDLK_6;
                    break;
                case KeyCode::Seven:
                    code = SDLK_7;
                    break;
                case KeyCode::Eight:
                    code = SDLK_8;
                    break;
                case KeyCode::Nine:
                    code = SDLK_9;
                    break;

                case KeyCode::A:
                    code = SDLK_a;
                    break;
                case KeyCode::B:
                    code = SDLK_b;
                    break;
                case KeyCode::C:
                    code = SDLK_c;
                    break;
                case KeyCode::D:
                    code = SDLK_d;
                    break;
                case KeyCode::E:
                    code = SDLK_e;
                    break;
                case KeyCode::F:
                    code = SDLK_f;
                    break;
                case KeyCode::G:
                    code = SDLK_g;
                    break;
                case KeyCode::H:
                    code = SDLK_h;
                    break;
                case KeyCode::I:
                    code = SDLK_i;
                    break;
                case KeyCode::J:
                    code = SDLK_j;
                    break;
                case KeyCode::K:
                    code = SDLK_k;
                    break;
                case KeyCode::L:
                    code = SDLK_l;
                    break;
                case KeyCode::M:
                    code = SDLK_m;
                    break;
                case KeyCode::N:
                    code = SDLK_n;
                    break;
                case KeyCode::O:
                    code = SDLK_o;
                    break;
                case KeyCode::P:
                    code = SDLK_p;
                    break;
                case KeyCode::Q:
                    code = SDLK_q;
                    break;
                case KeyCode::R:
                    code = SDLK_r;
                    break;
                case KeyCode::S:
                    code = SDLK_s;
                    break;
                case KeyCode::T:
                    code = SDLK_t;
                    break;
                case KeyCode::U:
                    code = SDLK_u;
                    break;
                case KeyCode::V:
                    code = SDLK_v;
                    break;
                case KeyCode::W:
                    code = SDLK_w;
                    break;
                case KeyCode::X:
                    code = SDLK_x;
                    break;
                case KeyCode::Y:
                    code = SDLK_y;
                    break;
                case KeyCode::Z:
                    code = SDLK_z;
                    break;

                case KeyCode::Return:
                    code = SDLK_RETURN;
                    break;
                case KeyCode::Escape:
                    code = SDLK_ESCAPE;
                    break;
                case KeyCode::Backspace:
                    code = SDLK_BACKSPACE;
                    break;
                case KeyCode::Tab:
                    code = SDLK_TAB;
                    break;
                case KeyCode::Space:
                    code = SDLK_SPACE;
                    break;
                case KeyCode::Subtract:
                    code = SDLK_MINUS;
                    break;
                case KeyCode::Equal:
                    code = SDLK_EQUALS;
                    break;
                case KeyCode::LeftBracket:
                    code = SDLK_LEFTBRACKET;
                    break;
                case KeyCode::RightBracket:
                    code = SDLK_RIGHTBRACKET;
                    break;
                case KeyCode::Backslash:
                    code = SDLK_BACKSLASH;
                    break;
                case KeyCode::Semicolon:
                    code = SDLK_SEMICOLON;
                    break;
                case KeyCode::Apostrophe:
                    code = SDLK_QUOTE;
                    break;
                case KeyCode::GraveAccent:
                    code = SDLK_BACKQUOTE; //Not sure about this one
                    break;
                case KeyCode::Comma:
                    code = SDLK_COMMA;
                    break;
                case KeyCode::Period:
                    code = SDLK_PERIOD;
                    break;
                case KeyCode::Slash:
                    code = SDLK_SLASH;
                    break;
                case KeyCode::CapsLock:
                    code = SDLK_CAPSLOCK;
                    break;

                case KeyCode::F1:
                    code = SDLK_F1;
                    break;
                case KeyCode::F2:
                    code = SDLK_F2;
                    break;
                case KeyCode::F3:
                    code = SDLK_F3;
                    break;
                case KeyCode::F4:
                    code = SDLK_F4;
                    break;
                case KeyCode::F5:
                    code = SDLK_F5;
                    break;
                case KeyCode::F6:
                    code = SDLK_F6;
                    break;
                case KeyCode::F7:
                    code = SDLK_F7;
                    break;
                case KeyCode::F8:
                    code = SDLK_F8;
                    break;
                case KeyCode::F9:
                    code = SDLK_F9;
                    break;
                case KeyCode::F10:
                    code = SDLK_F10;
                    break;
                case KeyCode::F11:
                    code = SDLK_F11;
                    break;
                case KeyCode::F12:
                    code = SDLK_F12;
                    break;
                case KeyCode::F13:
                    code = SDLK_F13;
                    break;
                case KeyCode::F14:
                    code = SDLK_F14;
                    break;
                case KeyCode::F15:
                    code = SDLK_F15;
                    break;
                case KeyCode::F16:
                    code = SDLK_F16;
                    break;

                case KeyCode::Insert:
                    code = SDLK_INSERT;
                    break;
                case KeyCode::Home:
                    code = SDLK_HOME;
                    break;
                case KeyCode::PageUp:
                    code = SDLK_PAGEUP;
                    break;
                case KeyCode::Delete:
                    code = SDLK_DELETE;
                    break;
                case KeyCode::End:
                    code = SDLK_END;
                    break;
                case KeyCode::PageDown:
                    code = SDLK_PAGEDOWN;
                    break;

                case KeyCode::LeftControl:
                    code = SDLK_LCTRL;
                    break;
                case KeyCode::LeftShift:
                    code = SDLK_LSHIFT;
                    break;
                case KeyCode::LeftAlt:
                    code = SDLK_LALT;
                    break;
                case KeyCode::LeftCommand:
                    code = SDLK_LGUI;
                    break;
                case KeyCode::RightControl:
                    code = SDLK_RCTRL;
                    break;
                case KeyCode::RightShift:
                    code = SDLK_RSHIFT;
                    break;
                case KeyCode::RightAlt:
                    code = SDLK_RALT;
                    break;
                case KeyCode::RightCommand:
                    code = SDLK_RGUI;
                    break;

                case KeyCode::Right:
                    code = SDLK_RIGHT;
                    break;
                case KeyCode::Left:
                    code = SDLK_LEFT;
                    break;
                case KeyCode::Down:
                    code = SDLK_DOWN;
                    break;
                case KeyCode::Up:
                    code = SDLK_UP;
                    break;

                case KeyCode::KPDivide:
                    code = SDLK_KP_DIVIDE;
                    break;
                case KeyCode::KPMultiply:
                    code = SDLK_KP_MULTIPLY;
                    break;
                case KeyCode::KPSubtract:
                    code = SDLK_KP_MINUS;
                    break;
                case KeyCode::KPAdd:
                    code = SDLK_KP_PLUS;
                    break;
                case KeyCode::KPReturn:
                    code = SDLK_KP_ENTER;
                    break;
                case KeyCode::KPOne:
                    code = SDLK_KP_1;
                    break;
                case KeyCode::KPTwo:
                    code = SDLK_KP_2;
                    break;
                case KeyCode::KPThree:
                    code = SDLK_KP_3;
                    break;
                case KeyCode::KPFour:
                    code = SDLK_KP_4;
                    break;
                case KeyCode::KPFive:
                    code = SDLK_KP_5;
                    break;
                case KeyCode::KPSix:
                    code = SDLK_KP_6;
                    break;
                case KeyCode::KPSeven:
                    code = SDLK_KP_7;
                    break;
                case KeyCode::KPEight:
                    code = SDLK_KP_8;
                    break;
                case KeyCode::KPNine:
                    code = SDLK_KP_9;
                    break;
                case KeyCode::KPZero:
                    code = SDLK_KP_0;
                    break;
                case KeyCode::KPEqual:
                    code = SDLK_KP_EQUALS;
                    break;

                default:
                    code = -1;
            }

            if(code == -1) return false;

            const Uint8 * state = SDL_GetKeyboardState(NULL);
            return state[code];
        }

        UInt32 WindowImpl::modifiers()
        {
            UInt32 ret = 0;
            SDL_Keymod mods = SDL_GetModState();

            if(mods & KMOD_LSHIFT)
            {
                ret |= (UInt32)KeyModifier::LeftShift;
            }
            else if(mods & KMOD_RSHIFT)
            {
                ret |= (UInt32)KeyModifier::RightShift;
            }
            else if(mods & KMOD_LGUI)
            {
                ret |= (UInt32)KeyModifier::LeftCommand;
            }
            else if(mods & KMOD_RGUI)
            {
                ret |= (UInt32)KeyModifier::RightCommand;
            }
            else if(mods & KMOD_LCTRL)
            {
                ret |= (UInt32)KeyModifier::LeftControl;
            }
            else if(mods & KMOD_RCTRL)
            {
                ret |= (UInt32)KeyModifier::RightControl;
            }
            else if(mods & KMOD_LALT)
            {
                ret |= (UInt32)KeyModifier::LeftAlt;
            }
            else if(mods & KMOD_RALT)
            {
                ret |= (UInt32)KeyModifier::RightAlt;
            }
            else if(mods & KMOD_CAPS)
            {
                ret |= (UInt32)KeyModifier::CapsLock;
            }
            else if(mods & KMOD_NUM)
            {
                ret |= (UInt32)KeyModifier::Numbers;
            }

            return ret;
        }

        bool WindowImpl::modifier(KeyModifier _mod)
        {
            return (modifiers() & (UInt32)_mod) != 0;
        }
    }
}
