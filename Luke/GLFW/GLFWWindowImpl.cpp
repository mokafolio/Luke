#include <Luke/GLFW/GLFWWindowImpl.hpp>
#include <Luke/GLFW/GLFWDisplayImpl.hpp>
#include <Luke/GLFW/GLFWInitializer.hpp>
#include <Luke/WindowEvents.hpp>

namespace luke
{
    namespace detail
    {
        using namespace stick;

        WindowImpl::WindowImpl(Window * _window) :
            m_glfwWindow(NULL),
            m_window(_window),
            m_preFullscreenWidth(0),
            m_preFullscreenHeight(0),
            m_preFullscreenX(0),
            m_preFullscreenY(0)
        {

        }

        WindowImpl::~WindowImpl()
        {
            if (m_glfwWindow)
                glfwDestroyWindow(m_glfwWindow);
        }

        static void mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods)
        {
            WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
            STICK_ASSERT(window);

            MouseButton btn = MouseButton::None;
            if (_button == GLFW_MOUSE_BUTTON_LEFT)
                btn = MouseButton::Left;
            else if (_button == GLFW_MOUSE_BUTTON_RIGHT)
                btn = MouseButton::Right;
            else if (_button == GLFW_MOUSE_BUTTON_MIDDLE)
                btn = MouseButton::Middle;
            else if (_button == GLFW_MOUSE_BUTTON_3)
                btn = MouseButton::Button3;
            else if (_button == GLFW_MOUSE_BUTTON_4)
                btn = MouseButton::Button4;
            else if (_button == GLFW_MOUSE_BUTTON_5)
                btn = MouseButton::Button5;
            else if (_button == GLFW_MOUSE_BUTTON_6)
                btn = MouseButton::Button6;
            else if (_button == GLFW_MOUSE_BUTTON_7)
                btn = MouseButton::Button7;

            double xpos, ypos;
            glfwGetCursorPos(_window, &xpos, &ypos);

            //TODO: Actually keep track of a button bitmask per window.
            MouseState state(xpos, ypos, 0);

            if (_action == GLFW_PRESS)
            {
                window->m_mouseState.setButtonBitMask(window->m_mouseState.buttonBitMask() | (UInt32)btn);
                printf("DA BITMASK %i\n", window->m_mouseState.buttonBitMask());
                window->m_window->publish(MouseDownEvent(window->m_mouseState, btn), true);
            }
            else if (_action == GLFW_RELEASE)
            {
                window->m_mouseState.setButtonBitMask(window->m_mouseState.buttonBitMask() & ~(UInt32)btn);
                window->m_window->publish(MouseUpEvent(window->m_mouseState, btn), true);
            }
        }

        static void mouseMoveCallback(GLFWwindow * _window, double _xpos, double _ypos)
        {
            WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
            STICK_ASSERT(window);

            window->m_mouseState.setPosition(_xpos, _ypos);
            if (window->m_mouseState.buttonBitMask() != 0 )
            {
                //drag event, emit one for every button that is down
                if (window->m_mouseState.isButtonDown(MouseButton::Left))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Left), true);
                }
                if (window->m_mouseState.isButtonDown(MouseButton::Middle))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Middle), true);
                }
                if (window->m_mouseState.isButtonDown(MouseButton::Right))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Right), true);
                }
                if (window->m_mouseState.isButtonDown(MouseButton::Button3))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button3), true);
                }
                if (window->m_mouseState.isButtonDown(MouseButton::Button4))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button4), true);
                }
                if (window->m_mouseState.isButtonDown(MouseButton::Button5))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button5), true);
                }
                if (window->m_mouseState.isButtonDown(MouseButton::Button6))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button6), true);
                }
                if (window->m_mouseState.isButtonDown(MouseButton::Button7))
                {
                    window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button7), true);
                }
            }
            else
            {
                //move event
                window->m_window->publish(MouseMoveEvent(window->m_mouseState), true);
            }
        }

        static void mouseScrollCallback(GLFWwindow * _window, double _xoffset, double _yoffset)
        {
            WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
            STICK_ASSERT(window);
            window->m_window->publish(MouseScrollEvent(window->m_mouseState, _xoffset, _yoffset), true);
        }

        static void windowSizeCallback(GLFWwindow * _window, int _width, int _height)
        {
            WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
            STICK_ASSERT(window);
            printf("GLFW RESIZE\n");
            window->m_window->publish(WindowResizeEvent(_width, _height), true);
        }

        static void windowPositionCallback(GLFWwindow * _window, int _x, int _y)
        {
            WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
            STICK_ASSERT(window);
            window->m_window->publish(WindowMoveEvent(_x, _y), true);
        }

        static void windowFocusCallback(GLFWwindow * _window, int _focused)
        {
            WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
            STICK_ASSERT(window);
            if (_focused)
            {
                // The window gained input focus
                window->m_window->publish(WindowFocusEvent(), true);
            }
            else
            {
                // The window lost input focus
                window->m_window->publish(WindowLostFocusEvent(), true);
            }
        }

        static void windowIconifyCallback(GLFWwindow * _window, int _iconified)
        {
            WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
            STICK_ASSERT(window);
            if (_iconified)
            {
                // The window was iconified
                printf("ICONIFIED\n");
            }
            else
            {
                // The window was restored
                printf("RESTORED\n");
            }
        }

        Error WindowImpl::open(const WindowSettings & _settings, WindowImpl * _shared)
        {
            if (m_glfwWindow)
                glfwDestroyWindow(m_glfwWindow);

            // and set some hints to get the correct opengl versions/profiles
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwWindowHint(GLFW_DEPTH_BITS, _settings.depthPrecision());
            glfwWindowHint(GLFW_STENCIL_BITS, _settings.stencilPrecision());
            glfwWindowHint(GLFW_RED_BITS, _settings.colorPrecision());
            glfwWindowHint(GLFW_GREEN_BITS, _settings.colorPrecision());
            glfwWindowHint(GLFW_BLUE_BITS, _settings.colorPrecision());
            glfwWindowHint(GLFW_ALPHA_BITS, _settings.alphaPrecision());
            glfwWindowHint(GLFW_SAMPLES, _settings.sampleCount());
            glfwWindowHint(GLFW_RESIZABLE, _settings.isResizeable());
            glfwWindowHint(GLFW_DECORATED, _settings.isDecorated());

            //create the window
            m_glfwWindow = glfwCreateWindow(_settings.width(), _settings.height(),
                                            _settings.title().length() ? _settings.title().cString() : "Luke Window", 
                                            _settings.display().isValid() ? _settings.display().m_pimpl->m_glfwMonitor : NULL, 
                                            _shared ? _shared->m_glfwWindow : NULL);

            if (!m_glfwWindow)
            {
                return Error(ec::InvalidOperation, "Could not create GLFW window", STICK_FILE, STICK_LINE);
            }

            m_preFullscreenWidth = _settings.width();
            m_preFullscreenHeight = _settings.height();
            m_preFullscreenX = x();
            m_preFullscreenY = y();

            glfwSetWindowUserPointer(m_glfwWindow, this);
            glfwSetMouseButtonCallback(m_glfwWindow, &mouseButtonCallback);
            glfwSetCursorPosCallback(m_glfwWindow, &mouseMoveCallback);
            glfwSetScrollCallback(m_glfwWindow, &mouseScrollCallback);
            glfwSetWindowSizeCallback(m_glfwWindow, &windowSizeCallback);
            glfwSetWindowPosCallback(m_glfwWindow, &windowPositionCallback);
            glfwSetWindowFocusCallback(m_glfwWindow, &windowFocusCallback);
            glfwSetWindowIconifyCallback(m_glfwWindow, &windowIconifyCallback);
            glfwDefaultWindowHints();

            return Error();
        }

        void WindowImpl::close()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwSetWindowShouldClose(m_glfwWindow, GLFW_TRUE);
        }

        bool WindowImpl::shouldClose() const
        {
            STICK_ASSERT(m_glfwWindow);
            return glfwWindowShouldClose(m_glfwWindow);
        }

        void WindowImpl::move(Float32 _x, Float32 _y)
        {
            STICK_ASSERT(m_glfwWindow);
            glfwSetWindowPos(m_glfwWindow, _x, _y);
        }

        void WindowImpl::moveToCenter()
        {

        }

        void WindowImpl::show()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwShowWindow(m_glfwWindow);
        }

        void WindowImpl::hide()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwHideWindow(m_glfwWindow);
        }

        void WindowImpl::resize(Float32 _width, Float32 _height)
        {
            STICK_ASSERT(m_glfwWindow);
            glfwSetWindowSize(m_glfwWindow, _width, _height);
        }

        void WindowImpl::maximize()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwMaximizeWindow(m_glfwWindow);
        }

        void WindowImpl::focus()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwFocusWindow(m_glfwWindow);
        }

        void WindowImpl::enableRenderContext()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwMakeContextCurrent(m_glfwWindow);
        }

        void WindowImpl::disableRenderContext()
        {
            glfwMakeContextCurrent(NULL);
        }

        void WindowImpl::swapBuffers()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwSwapBuffers(m_glfwWindow);
        }

        void WindowImpl::setVerticalSync(bool _b)
        {
            enableRenderContext();
            glfwSwapInterval(_b ? 1 : 0);
        }

        //GLFW does not keep track about which monitor a window is on.
        //This routine gets the monitor based on the window position,
        //based on this: https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
        static GLFWmonitor * currentMonitor(GLFWwindow * _window)
        {
            int nmonitors, i;
            int wx, wy, ww, wh;
            int mx, my, mw, mh;
            int overlap, bestoverlap;
            GLFWmonitor * bestmonitor;
            GLFWmonitor ** monitors;
            const GLFWvidmode * mode;

            bestoverlap = 0;
            bestmonitor = NULL;

            glfwGetWindowPos(_window, &wx, &wy);
            glfwGetWindowSize(_window, &ww, &wh);
            monitors = glfwGetMonitors(&nmonitors);

            for (i = 0; i < nmonitors; ++i)
            {
                mode = glfwGetVideoMode(monitors[i]);
                glfwGetMonitorPos(monitors[i], &mx, &my);
                mw = mode->width;
                mh = mode->height;

                overlap =  std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
                           std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

                if (bestoverlap < overlap)
                {
                    bestoverlap = overlap;
                    bestmonitor = monitors[i];
                }
            }

            return bestmonitor;
        }

        //@TODO: Merge the code for both enter fullscreen functions into a helper
        Error WindowImpl::enterFullscreen(const Display & _display)
        {
            STICK_ASSERT(m_glfwWindow);
            GLFWmonitor * monitor = NULL;
            if (!_display.isValid())
            {
                monitor = currentMonitor(m_glfwWindow);
            }
            else
            {
                monitor = _display.m_pimpl->m_glfwMonitor;
            }
            m_preFullscreenWidth = width();
            m_preFullscreenHeight = height();
            m_preFullscreenX = x();
            m_preFullscreenY = y();
            STICK_ASSERT(monitor);
            const GLFWvidmode * mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(m_glfwWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            return Error();
        }

        Error WindowImpl::enterFullscreen(const DisplayMode & _mode, const Display & _display)
        {
            STICK_ASSERT(m_glfwWindow);
            GLFWmonitor * monitor = NULL;
            if (!_display.isValid())
            {
                monitor = currentMonitor(m_glfwWindow);
            }
            else
            {
                monitor = _display.m_pimpl->m_glfwMonitor;
            }
            m_preFullscreenWidth = width();
            m_preFullscreenHeight = height();
            m_preFullscreenX = x();
            m_preFullscreenY = y();
            STICK_ASSERT(monitor);
            glfwSetWindowMonitor(m_glfwWindow, monitor, 0, 0, _mode.width(), _mode.height(), _mode.refreshRate());
            return Error();
        }

        void WindowImpl::exitFullscreen()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwSetWindowMonitor(m_glfwWindow, NULL, m_preFullscreenX, m_preFullscreenY, m_preFullscreenWidth, m_preFullscreenHeight, GLFW_DONT_CARE);
        }

        void WindowImpl::hideCursor()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        void WindowImpl::showCursor()
        {
            STICK_ASSERT(m_glfwWindow);
            glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        void WindowImpl::setTitle(const String & _str)
        {

        }

        bool WindowImpl::isVisible() const
        {
            STICK_ASSERT(m_glfwWindow);
            return glfwGetWindowAttrib(m_glfwWindow, GLFW_VISIBLE);
        }

        bool WindowImpl::isFocussed() const
        {
            STICK_ASSERT(m_glfwWindow);
            return glfwGetWindowAttrib(m_glfwWindow, GLFW_FOCUSED);
        }

        const WindowSettings & WindowImpl::settings() const
        {

        }

        bool WindowImpl::isCursorVisible() const
        {
            return glfwGetInputMode(m_glfwWindow, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
        }

        bool WindowImpl::verticalSync() const
        {

        }

        bool WindowImpl::isFullscreen() const
        {
            STICK_ASSERT(m_glfwWindow);
            return glfwGetWindowMonitor(m_glfwWindow) != NULL;
        }

        const String & WindowImpl::title() const
        {

        }

        Float32 WindowImpl::backingScaleFactor() const
        {
            return widthInPixels() / width();
        }

        Float32 WindowImpl::width() const
        {
            if (m_glfwWindow)
            {
                int w, h;
                glfwGetWindowSize(m_glfwWindow, &w, &h);
                return w;
            }
            return 0;
        }

        Float32 WindowImpl::height() const
        {
            if (m_glfwWindow)
            {
                int w, h;
                glfwGetWindowSize(m_glfwWindow, &w, &h);
                return h;
            }
            return 0;
        }

        Float32 WindowImpl::widthInPixels() const
        {
            if (m_glfwWindow)
            {
                int w, h;
                glfwGetFramebufferSize(m_glfwWindow, &w, &h);
                return w;
            }
            return 0;
        }

        Float32 WindowImpl::heightInPixels() const
        {
            if (m_glfwWindow)
            {
                int w, h;
                glfwGetFramebufferSize(m_glfwWindow, &w, &h);
                return h;
            }
            return 0;
        }

        Float32 WindowImpl::x() const
        {
            if (m_glfwWindow)
            {
                int x, y;
                glfwGetWindowPos(m_glfwWindow, &x, &y);
                return x;
            }
            return 0;
        }

        Float32 WindowImpl::y() const
        {
            if (m_glfwWindow)
            {
                int x, y;
                glfwGetWindowPos(m_glfwWindow, &x, &y);
                return y;
            }
            return 0;
        }

        Display WindowImpl::display() const
        {
            if (m_glfwWindow)
            {
                GLFWmonitor * monitor = currentMonitor(m_glfwWindow);
                STICK_ASSERT(monitor);
                Display ret;
                ret.m_pimpl->m_glfwMonitor = monitor;
                return ret;
            }
            return Display();
        }

        Error WindowImpl::pollEvents()
        {
            glfwPollEvents();
            return Error();
        }
    }
}
