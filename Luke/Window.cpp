#include <Luke/GLFW/GLFWWindowImpl.hpp>

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
}
