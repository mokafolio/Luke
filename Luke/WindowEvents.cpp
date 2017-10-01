#include <Luke/WindowEvents.hpp>

namespace luke
{
    using namespace stick;

    WindowMoveEvent::WindowMoveEvent(Float32 _x, Float32 _y) :
        m_x(_x),
        m_y(_y)
    {

    }

    Float32 WindowMoveEvent::x() const
    {
        return m_x;
    }

    Float32 WindowMoveEvent::y() const
    {
        return m_y;
    }

    WindowResizeEvent::WindowResizeEvent(Float32 _width, Float32 _height) :
        m_width(_width),
        m_height(_height)
    {

    }

    Float32 WindowResizeEvent::width() const
    {
        return m_width;
    }

    Float32 WindowResizeEvent::height() const
    {
        return m_height;
    }

    WindowFocusEvent::WindowFocusEvent()
    {

    }

    WindowLostFocusEvent::WindowLostFocusEvent()
    {

    }
    
    WindowIconifyEvent::WindowIconifyEvent()
    {

    }

    WindowRestoreEvent::WindowRestoreEvent()
    {

    }
}
