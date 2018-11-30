#include <Luke/GLFW/GLFWDisplayImpl.hpp>
#include <Luke/GLFW/GLFWInitializer.hpp>
#include <Luke/GLFW/GLFWWindowImpl.hpp>
#include <Luke/KeyEvents.hpp>
#include <Luke/WindowEvents.hpp>

#include <GLFW/glfw3.h>

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

    // TODO: Actually keep track of a button bitmask per window.
    MouseState state(xpos, ypos, 0);

    if (_action == GLFW_PRESS)
    {
        window->m_mouseState.setButtonBitMask(window->m_mouseState.buttonBitMask() | (UInt32)btn);
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
    if (window->m_mouseState.buttonBitMask() != 0)
    {
        // drag event, emit one for every button that is down
        if (window->m_mouseState.isButtonDown(MouseButton::Left))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Left),
                                      true);
        }
        if (window->m_mouseState.isButtonDown(MouseButton::Middle))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Middle),
                                      true);
        }
        if (window->m_mouseState.isButtonDown(MouseButton::Right))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Right),
                                      true);
        }
        if (window->m_mouseState.isButtonDown(MouseButton::Button3))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button3),
                                      true);
        }
        if (window->m_mouseState.isButtonDown(MouseButton::Button4))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button4),
                                      true);
        }
        if (window->m_mouseState.isButtonDown(MouseButton::Button5))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button5),
                                      true);
        }
        if (window->m_mouseState.isButtonDown(MouseButton::Button6))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button6),
                                      true);
        }
        if (window->m_mouseState.isButtonDown(MouseButton::Button7))
        {
            window->m_window->publish(MouseDragEvent(window->m_mouseState, MouseButton::Button7),
                                      true);
        }
    }
    else
    {
        // move event
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
        window->m_window->publish(WindowIconifyEvent(), true);
    }
    else
    {
        // The window was restored
        window->m_window->publish(WindowRestoreEvent(), true);
    }
}

static void keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods)
{
    WindowImpl * window = reinterpret_cast<WindowImpl *>(glfwGetWindowUserPointer(_window));
    STICK_ASSERT(window);

    KeyCode code;

    switch (_key)
    {
    case GLFW_KEY_0:
        code = KeyCode::Zero;
        break;
    case GLFW_KEY_1:
        code = KeyCode::One;
        break;
    case GLFW_KEY_2:
        code = KeyCode::Two;
        break;
    case GLFW_KEY_3:
        code = KeyCode::Three;
        break;
    case GLFW_KEY_4:
        code = KeyCode::Four;
        break;
    case GLFW_KEY_5:
        code = KeyCode::Five;
        break;
    case GLFW_KEY_6:
        code = KeyCode::Six;
        break;
    case GLFW_KEY_7:
        code = KeyCode::Seven;
        break;
    case GLFW_KEY_8:
        code = KeyCode::Eight;
        break;
    case GLFW_KEY_9:
        code = KeyCode::Nine;
        break;

    case GLFW_KEY_A:
        code = KeyCode::A;
        break;
    case GLFW_KEY_B:
        code = KeyCode::B;
        break;
    case GLFW_KEY_C:
        code = KeyCode::C;
        break;
    case GLFW_KEY_D:
        code = KeyCode::D;
        break;
    case GLFW_KEY_E:
        code = KeyCode::E;
        break;
    case GLFW_KEY_F:
        code = KeyCode::F;
        break;
    case GLFW_KEY_G:
        code = KeyCode::G;
        break;
    case GLFW_KEY_H:
        code = KeyCode::H;
        break;
    case GLFW_KEY_I:
        code = KeyCode::I;
        break;
    case GLFW_KEY_J:
        code = KeyCode::J;
        break;
    case GLFW_KEY_K:
        code = KeyCode::K;
        break;
    case GLFW_KEY_L:
        code = KeyCode::L;
        break;
    case GLFW_KEY_M:
        code = KeyCode::M;
        break;
    case GLFW_KEY_N:
        code = KeyCode::N;
        break;
    case GLFW_KEY_O:
        code = KeyCode::O;
        break;
    case GLFW_KEY_P:
        code = KeyCode::P;
        break;
    case GLFW_KEY_Q:
        code = KeyCode::Q;
        break;
    case GLFW_KEY_R:
        code = KeyCode::R;
        break;
    case GLFW_KEY_S:
        code = KeyCode::S;
        break;
    case GLFW_KEY_T:
        code = KeyCode::T;
        break;
    case GLFW_KEY_U:
        code = KeyCode::U;
        break;
    case GLFW_KEY_V:
        code = KeyCode::V;
        break;
    case GLFW_KEY_W:
        code = KeyCode::W;
        break;
    case GLFW_KEY_X:
        code = KeyCode::X;
        break;
    case GLFW_KEY_Y:
        code = KeyCode::Y;
        break;
    case GLFW_KEY_Z:
        code = KeyCode::Z;
        break;

    case GLFW_KEY_ENTER:
        code = KeyCode::Return;
        break;
    case GLFW_KEY_ESCAPE:
        code = KeyCode::Escape;
        break;
    case GLFW_KEY_DELETE:
        code = KeyCode::Delete;
        break;
    case GLFW_KEY_TAB:
        code = KeyCode::Tab;
        break;
    case GLFW_KEY_SPACE:
        code = KeyCode::Space;
        break;
    case GLFW_KEY_MINUS:
        code = KeyCode::Subtract;
        break;
    case GLFW_KEY_EQUAL:
        code = KeyCode::Equal;
        break;
    case GLFW_KEY_LEFT_BRACKET:
        code = KeyCode::LeftBracket;
        break;
    case GLFW_KEY_RIGHT_BRACKET:
        code = KeyCode::RightBracket;
        break;
    case GLFW_KEY_BACKSLASH:
        code = KeyCode::Backslash;
        break;
    case GLFW_KEY_WORLD_1:
        code = KeyCode::WorldOne;
        break;
    case GLFW_KEY_SEMICOLON:
        code = KeyCode::Semicolon;
        break;
    case GLFW_KEY_APOSTROPHE:
        code = KeyCode::Apostrophe;
        break;
    case GLFW_KEY_GRAVE_ACCENT:
        code = KeyCode::GraveAccent;
        break;
    case GLFW_KEY_COMMA:
        code = KeyCode::Comma;
        break;
    case GLFW_KEY_PERIOD:
        code = KeyCode::Period;
        break;
    case GLFW_KEY_SLASH:
        code = KeyCode::Slash;
        break;
    case GLFW_KEY_CAPS_LOCK:
        code = KeyCode::CapsLock;
        break;

    case GLFW_KEY_F1:
        code = KeyCode::F1;
        break;
    case GLFW_KEY_F2:
        code = KeyCode::F2;
        break;
    case GLFW_KEY_F3:
        code = KeyCode::F3;
        break;
    case GLFW_KEY_F4:
        code = KeyCode::F4;
        break;
    case GLFW_KEY_F5:
        code = KeyCode::F5;
        break;
    case GLFW_KEY_F6:
        code = KeyCode::F6;
        break;
    case GLFW_KEY_F7:
        code = KeyCode::F7;
        break;
    case GLFW_KEY_F8:
        code = KeyCode::F8;
        break;
    case GLFW_KEY_F9:
        code = KeyCode::F9;
        break;
    case GLFW_KEY_F10:
        code = KeyCode::F10;
        break;
    case GLFW_KEY_F11:
        code = KeyCode::F11;
        break;
    case GLFW_KEY_F12:
        code = KeyCode::F12;
        break;
    case GLFW_KEY_F13:
        code = KeyCode::F13;
        break;
    case GLFW_KEY_F14:
        code = KeyCode::F14;
        break;
    case GLFW_KEY_F15:
        code = KeyCode::F15;
        break;
    case GLFW_KEY_F16:
        code = KeyCode::F16;
        break;

    case GLFW_KEY_LEFT_CONTROL:
        code = KeyCode::LeftControl;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        code = KeyCode::LeftShift;
        break;
    case GLFW_KEY_LEFT_ALT:
        code = KeyCode::LeftAlt;
        break;
    case GLFW_KEY_LEFT_SUPER:
        code = KeyCode::LeftCommand;
        break;
    case GLFW_KEY_RIGHT_CONTROL:
        code = KeyCode::RightControl;
        break;
    case GLFW_KEY_RIGHT_SHIFT:
        code = KeyCode::RightShift;
        break;
    case GLFW_KEY_RIGHT_ALT:
        code = KeyCode::RightAlt;
        break;
    case GLFW_KEY_RIGHT_SUPER:
        code = KeyCode::RightCommand;
        break;

    case GLFW_KEY_RIGHT:
        code = KeyCode::Right;
        break;
    case GLFW_KEY_LEFT:
        code = KeyCode::Left;
        break;
    case GLFW_KEY_DOWN:
        code = KeyCode::Down;
        break;
    case GLFW_KEY_UP:
        code = KeyCode::Up;
        break;

    case GLFW_KEY_KP_DIVIDE:
        code = KeyCode::KPDivide;
        break;
    case GLFW_KEY_KP_MULTIPLY:
        code = KeyCode::KPMultiply;
        break;
    case GLFW_KEY_KP_SUBTRACT:
        code = KeyCode::KPSubtract;
        break;
    case GLFW_KEY_KP_ADD:
        code = KeyCode::KPAdd;
        break;
    case GLFW_KEY_KP_ENTER:
        code = KeyCode::KPReturn;
        break;
    case GLFW_KEY_KP_1:
        code = KeyCode::KPOne;
        break;
    case GLFW_KEY_KP_2:
        code = KeyCode::KPTwo;
        break;
    case GLFW_KEY_KP_3:
        code = KeyCode::KPThree;
        break;
    case GLFW_KEY_KP_4:
        code = KeyCode::KPFour;
        break;
    case GLFW_KEY_KP_5:
        code = KeyCode::KPFive;
        break;
    case GLFW_KEY_KP_6:
        code = KeyCode::KPSix;
        break;
    case GLFW_KEY_KP_7:
        code = KeyCode::KPSeven;
        break;
    case GLFW_KEY_KP_8:
        code = KeyCode::KPEight;
        break;
    case GLFW_KEY_KP_9:
        code = KeyCode::KPNine;
        break;
    case GLFW_KEY_KP_0:
        code = KeyCode::KPZero;
        break;
    case GLFW_KEY_KP_EQUAL:
        code = KeyCode::KPEqual;
        break;

    default:
        code = KeyCode::Unknown;
    }

    if (_action == GLFW_PRESS || _action == GLFW_REPEAT)
    {
        window->m_window->publish(KeyDownEvent(code, _scancode, _action == GLFW_REPEAT), true);
    }
    else if (_action == GLFW_RELEASE)
    {
        window->m_window->publish(KeyUpEvent(code, _scancode), true);
    }
}

Error WindowImpl::open(const WindowSettings & _settings, WindowImpl * _shared)
{
    GLFWInitializer::instance();

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

    // create the window
    m_glfwWindow = glfwCreateWindow(
        _settings.width(),
        _settings.height(),
        _settings.title().length() ? _settings.title().cString() : "Luke Window",
        _settings.display().isValid() ? _settings.display().m_pimpl->m_glfwMonitor : NULL,
        _shared ? _shared->m_glfwWindow : NULL);

    //@TODO Proper error code
    if (!m_glfwWindow)
    {
        return Error(ec::InvalidOperation, "Could not create GLFW window", STICK_FILE, STICK_LINE);
    }

    //@TODO move the window to the position specified in settings?

    m_preFullscreenWidth = _settings.width();
    m_preFullscreenHeight = _settings.height();
    m_preFullscreenX = x();
    m_preFullscreenY = y();

    glfwSetWindowUserPointer(m_glfwWindow, this);
    glfwSetMouseButtonCallback(m_glfwWindow, &mouseButtonCallback);
    glfwSetCursorPosCallback(m_glfwWindow, &mouseMoveCallback);
    glfwSetScrollCallback(m_glfwWindow, &mouseScrollCallback);
    glfwSetKeyCallback(m_glfwWindow, &keyCallback);
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
    //@TODO
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

// GLFW does not keep track about which monitor a window is on.
// This routine gets the monitor based on the window position,
// based on this:
// https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
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

        overlap = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
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
    glfwSetWindowMonitor(
        m_glfwWindow, monitor, 0, 0, _mode.width(), _mode.height(), _mode.refreshRate());
    return Error();
}

void WindowImpl::exitFullscreen()
{
    STICK_ASSERT(m_glfwWindow);
    glfwSetWindowMonitor(m_glfwWindow,
                         NULL,
                         m_preFullscreenX,
                         m_preFullscreenY,
                         m_preFullscreenWidth,
                         m_preFullscreenHeight,
                         GLFW_DONT_CARE);
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
    //@TODO!!!!!!
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
    //@TODO!!!!!! or remove from api?
}

bool WindowImpl::isCursorVisible() const
{
    return glfwGetInputMode(m_glfwWindow, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
}

bool WindowImpl::verticalSync() const
{
    //@TODO!!!!!!
}

bool WindowImpl::isFullscreen() const
{
    STICK_ASSERT(m_glfwWindow);
    return glfwGetWindowMonitor(m_glfwWindow) != NULL;
}

bool WindowImpl::isKeyDown(KeyCode _code) const
{
    int code;
    switch (_code)
    {
    case KeyCode::Zero:
        code = GLFW_KEY_0;
        break;
    case KeyCode::One:
        code = GLFW_KEY_1;
        break;
    case KeyCode::Two:
        code = GLFW_KEY_2;
        break;
    case KeyCode::Three:
        code = GLFW_KEY_3;
        break;
    case KeyCode::Four:
        code = GLFW_KEY_4;
        break;
    case KeyCode::Five:
        code = GLFW_KEY_5;
        break;
    case KeyCode::Six:
        code = GLFW_KEY_6;
        break;
    case KeyCode::Seven:
        code = GLFW_KEY_7;
        break;
    case KeyCode::Eight:
        code = GLFW_KEY_8;
        break;
    case KeyCode::Nine:
        code = GLFW_KEY_9;
        break;

    case KeyCode::A:
        code = GLFW_KEY_A;
        break;
    case KeyCode::B:
        code = GLFW_KEY_B;
        break;
    case KeyCode::C:
        code = GLFW_KEY_C;
        break;
    case KeyCode::D:
        code = GLFW_KEY_D;
        break;
    case KeyCode::E:
        code = GLFW_KEY_E;
        break;
    case KeyCode::F:
        code = GLFW_KEY_F;
        break;
    case KeyCode::G:
        code = GLFW_KEY_G;
        break;
    case KeyCode::H:
        code = GLFW_KEY_H;
        break;
    case KeyCode::I:
        code = GLFW_KEY_I;
        break;
    case KeyCode::J:
        code = GLFW_KEY_J;
        break;
    case KeyCode::K:
        code = GLFW_KEY_K;
        break;
    case KeyCode::L:
        code = GLFW_KEY_L;
        break;
    case KeyCode::M:
        code = GLFW_KEY_M;
        break;
    case KeyCode::N:
        code = GLFW_KEY_N;
        break;
    case KeyCode::O:
        code = GLFW_KEY_O;
        break;
    case KeyCode::P:
        code = GLFW_KEY_P;
        break;
    case KeyCode::Q:
        code = GLFW_KEY_Q;
        break;
    case KeyCode::R:
        code = GLFW_KEY_R;
        break;
    case KeyCode::S:
        code = GLFW_KEY_S;
        break;
    case KeyCode::T:
        code = GLFW_KEY_T;
        break;
    case KeyCode::U:
        code = GLFW_KEY_U;
        break;
    case KeyCode::V:
        code = GLFW_KEY_V;
        break;
    case KeyCode::W:
        code = GLFW_KEY_W;
        break;
    case KeyCode::X:
        code = GLFW_KEY_X;
        break;
    case KeyCode::Y:
        code = GLFW_KEY_Y;
        break;
    case KeyCode::Z:
        code = GLFW_KEY_Z;
        break;

    case KeyCode::Return:
        code = GLFW_KEY_ENTER;
        break;
    case KeyCode::Escape:
        code = GLFW_KEY_ESCAPE;
        break;
    case KeyCode::Delete:
        code = GLFW_KEY_DELETE;
        break;
    case KeyCode::Tab:
        code = GLFW_KEY_TAB;
        break;
    case KeyCode::Space:
        code = GLFW_KEY_SPACE;
        break;
    case KeyCode::Subtract:
        code = GLFW_KEY_MINUS;
        break;
    case KeyCode::Equal:
        code = GLFW_KEY_EQUAL;
        break;
    case KeyCode::LeftBracket:
        code = GLFW_KEY_LEFT_BRACKET;
        break;
    case KeyCode::RightBracket:
        code = GLFW_KEY_RIGHT_BRACKET;
        break;
    case KeyCode::Backslash:
        code = GLFW_KEY_BACKSLASH;
        break;
    case KeyCode::WorldOne:
        code = GLFW_KEY_WORLD_1;
        break;
    case KeyCode::Semicolon:
        code = GLFW_KEY_SEMICOLON;
        break;
    case KeyCode::Apostrophe:
        code = GLFW_KEY_APOSTROPHE;
        break;
    case KeyCode::GraveAccent:
        code = GLFW_KEY_GRAVE_ACCENT;
        break;
    case KeyCode::Comma:
        code = GLFW_KEY_COMMA;
        break;
    case KeyCode::Period:
        code = GLFW_KEY_PERIOD;
        break;
    case KeyCode::Slash:
        code = GLFW_KEY_SLASH;
        break;
    case KeyCode::CapsLock:
        code = GLFW_KEY_CAPS_LOCK;
        break;

    case KeyCode::F1:
        code = GLFW_KEY_F1;
        break;
    case KeyCode::F2:
        code = GLFW_KEY_F2;
        break;
    case KeyCode::F3:
        code = GLFW_KEY_F3;
        break;
    case KeyCode::F4:
        code = GLFW_KEY_F4;
        break;
    case KeyCode::F5:
        code = GLFW_KEY_F5;
        break;
    case KeyCode::F6:
        code = GLFW_KEY_F6;
        break;
    case KeyCode::F7:
        code = GLFW_KEY_F7;
        break;
    case KeyCode::F8:
        code = GLFW_KEY_F8;
        break;
    case KeyCode::F9:
        code = GLFW_KEY_F9;
        break;
    case KeyCode::F10:
        code = GLFW_KEY_F10;
        break;
    case KeyCode::F11:
        code = GLFW_KEY_F11;
        break;
    case KeyCode::F12:
        code = GLFW_KEY_F12;
        break;
    case KeyCode::F13:
        code = GLFW_KEY_F13;
        break;
    case KeyCode::F14:
        code = GLFW_KEY_F14;
        break;
    case KeyCode::F15:
        code = GLFW_KEY_F15;
        break;
    case KeyCode::F16:
        code = GLFW_KEY_F16;
        break;

    case KeyCode::LeftControl:
        code = GLFW_KEY_LEFT_CONTROL;
        break;
    case KeyCode::LeftShift:
        code = GLFW_KEY_LEFT_SHIFT;
        break;
    case KeyCode::LeftAlt:
        code = GLFW_KEY_LEFT_ALT;
        break;
    case KeyCode::LeftCommand:
        code = GLFW_KEY_LEFT_SUPER;
        break;
    case KeyCode::RightControl:
        code = GLFW_KEY_RIGHT_CONTROL;
        break;
    case KeyCode::RightShift:
        code = GLFW_KEY_RIGHT_SHIFT;
        break;
    case KeyCode::RightAlt:
        code = GLFW_KEY_RIGHT_ALT;
        break;
    case KeyCode::RightCommand:
        code = GLFW_KEY_RIGHT_SUPER;
        break;

    case KeyCode::Right:
        code = GLFW_KEY_RIGHT;
        break;
    case KeyCode::Left:
        code = GLFW_KEY_LEFT;
        break;
    case KeyCode::Down:
        code = GLFW_KEY_DOWN;
        break;
    case KeyCode::Up:
        code = GLFW_KEY_UP;
        break;

    case KeyCode::KPDivide:
        code = GLFW_KEY_KP_DIVIDE;
        break;
    case KeyCode::KPMultiply:
        code = GLFW_KEY_KP_MULTIPLY;
        break;
    case KeyCode::KPSubtract:
        code = GLFW_KEY_KP_SUBTRACT;
        break;
    case KeyCode::KPAdd:
        code = GLFW_KEY_KP_ADD;
        break;
    case KeyCode::KPReturn:
        code = GLFW_KEY_KP_ENTER;
        break;
    case KeyCode::KPOne:
        code = GLFW_KEY_KP_1;
        break;
    case KeyCode::KPTwo:
        code = GLFW_KEY_KP_2;
        break;
    case KeyCode::KPThree:
        code = GLFW_KEY_KP_3;
        break;
    case KeyCode::KPFour:
        code = GLFW_KEY_KP_4;
        break;
    case KeyCode::KPFive:
        code = GLFW_KEY_KP_5;
        break;
    case KeyCode::KPSix:
        code = GLFW_KEY_KP_6;
        break;
    case KeyCode::KPSeven:
        code = GLFW_KEY_KP_7;
        break;
    case KeyCode::KPEight:
        code = GLFW_KEY_KP_8;
        break;
    case KeyCode::KPNine:
        code = GLFW_KEY_KP_9;
        break;
    case KeyCode::KPZero:
        code = GLFW_KEY_KP_0;
        break;
    case KeyCode::KPEqual:
        code = GLFW_KEY_KP_EQUAL;
        break;

    default:
        code = -1;
    }

    return glfwGetKey(m_glfwWindow, code) == GLFW_PRESS;
}

const String & WindowImpl::title() const
{
    //@TODO
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
} // namespace detail
} // namespace luke
