#if defined(LUKE_IMPLEMENTATION_GLFW)
#include <Luke/GLFW/GLFWWindowImpl.hpp>
#elif defined(LUKE_IMPLEMENTATION_SDL)
#include <Luke/SDL/SDLWindowImpl.hpp>
#endif

namespace luke
{
    using namespace stick;

    Window::Window() :
        m_pimpl(makeUnique<detail::WindowImpl>(defaultAllocator(), this))
    {

    }

    Window::~Window()
    {

    }

    Error Window::open(const WindowSettings & _settings, Window * _shared)
    {
        return m_pimpl->open(_settings, _shared ? _shared->m_pimpl.get() : nullptr);
    }

    void Window::close()
    {
        m_pimpl->close();
    }

    void Window::setShouldClose(bool _b)
    {
        m_pimpl->setShouldClose(_b);
    }

    bool Window::shouldClose() const
    {
        return m_pimpl->shouldClose();
    }

    void Window::move(Float32 _x, Float32 _y)
    {
        m_pimpl->move(_x, _y);
    }

    void Window::moveToCenter()
    {
        m_pimpl->moveToCenter();
    }

    void Window::show()
    {
        m_pimpl->show();
    }

    void Window::hide()
    {
        m_pimpl->hide();
    }

    void Window::resize(Float32 _width, Float32 _height)
    {
        m_pimpl->resize(_width, _height);
    }

    void Window::maximize()
    {
        m_pimpl->maximize();
    }

    void Window::focus()
    {
        m_pimpl->focus();
    }

    void Window::enableRenderContext()
    {
        m_pimpl->enableRenderContext();
    }

    void Window::disableRenderContext()
    {
        m_pimpl->disableRenderContext();
    }

    void Window::setVerticalSync(bool _b)
    {
        m_pimpl->setVerticalSync(_b);
    }

    Error Window::enterFullscreen(FullScreenMode _mode, const Display & _display)
    {
        return m_pimpl->enterFullscreen(_mode, _display);
    }

    Error Window::enterFullscreen(const DisplayMode & _mode, const Display & _display)
    {
        return m_pimpl->enterFullscreen(_mode, _display);
    }

    Error Window::enterFullscreen(Float32 _width, Float32 _height, const Display & _display)
    {
        DisplayMode mode;
        if (_display.isValid())
        {
            DisplayMode current = _display.currentDisplayMode();
            mode = _display.findBestDisplayMode(_width, _height,
                                                current.redBits(), current.greenBits(), current.blueBits(), current.refreshRate());
        }
        else
        {
            DisplayMode current = display().currentDisplayMode();
            mode = display().findBestDisplayMode(_width, _height,
                                                 current.redBits(), current.greenBits(), current.blueBits(), current.refreshRate());
        }
        if (mode.isValid())
        {
            return enterFullscreen(mode, _display);
        }
        else
        {
            //@TODO: Create Luke error codes that are more informative
            return Error(ec::InvalidOperation, "Could not find display mode", STICK_FILE, STICK_LINE);
        }
    }

    void Window::exitFullscreen()
    {
        m_pimpl->exitFullscreen();
    }

    void Window::hideCursor()
    {
        m_pimpl->hideCursor();
    }

    void Window::showCursor()
    {
        m_pimpl->showCursor();
    }

    void Window::setTitle(const String & _str)
    {
        m_pimpl->setTitle(_str);
    }

    void Window::swapBuffers()
    {
        m_pimpl->swapBuffers();
    }

    bool Window::isVisible() const
    {
        return m_pimpl->isVisible();
    }

    bool Window::isFocussed() const
    {
        return m_pimpl->isFocussed();
    }

    bool Window::isFullscreen() const
    {
        return m_pimpl->isFullscreen();
    }

    bool Window::isKeyDown(KeyCode _key) const
    {
        return m_pimpl->isKeyDown(_key);
    }

    const WindowSettings & Window::settings() const
    {
        return m_pimpl->settings();
    }

    bool Window::isCursorVisible() const
    {
        return m_pimpl->isCursorVisible();
    }

    bool Window::verticalSync() const
    {
        return m_pimpl->verticalSync();
    }

    const String & Window::title() const
    {
        return m_pimpl->title();
    }

    Float32 Window::backingScaleFactor() const
    {
        return m_pimpl->backingScaleFactor();
    }

    Float32 Window::width() const
    {
        return m_pimpl->width();
    }

    Float32 Window::height() const
    {
        return m_pimpl->height();
    }

    Float32 Window::widthInPixels() const
    {
        return m_pimpl->widthInPixels();
    }

    Float32 Window::heightInPixels() const
    {
        return m_pimpl->heightInPixels();
    }

    Error pollEvents()
    {
        return detail::WindowImpl::pollEvents();
    }

    Float32 Window::x() const
    {
        return m_pimpl->x();
    }

    Float32 Window::y() const
    {
        return m_pimpl->y();
    }

    Display Window::display() const
    {
        return m_pimpl->display();
    }
}
