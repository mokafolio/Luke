#include <Luke/MouseEvents.hpp>

namespace luke
{
    using namespace stick;

    MouseEvent::MouseEvent(const MouseState & _state, MouseButton _button) :
        m_state(_state),
        m_modifiedButton(_button)
    {
    }

    MouseEvent::~MouseEvent()
    {

    }

    Float32 MouseEvent::x() const
    {
        return m_state.x();
    }

    Float32 MouseEvent::y() const
    {
        return m_state.y();
    }

    MouseScrollEvent::MouseScrollEvent(const MouseState & _state, Float32 _scrollX, Float32 _scrollY) :
    MouseEvent(_state, MouseButton::None),
    m_scrollX(_scrollX),
    m_scrollY(_scrollY)
    {

    }

    Float32 MouseScrollEvent::scrollX() const
    {
        return m_scrollX;
    }

    Float32 MouseScrollEvent::scrollY() const
    {
        return m_scrollY;
    }

    MouseButton MouseEvent::button() const
    {
        return m_modifiedButton;
    }

    const MouseState & MouseEvent::mouseState() const
    {
        return m_state;
    }

    MouseMoveEvent::MouseMoveEvent(const MouseState & _state) :
        MouseEvent(_state, MouseButton::None)
    {

    }

    MouseDragEvent::MouseDragEvent(const MouseState & _state, MouseButton _button) :
        MouseEvent(_state, _button)
    {

    }

    MouseDownEvent::MouseDownEvent(const MouseState & _state, MouseButton _button) :
        MouseEvent(_state, _button)
    {

    }

    MouseUpEvent::MouseUpEvent(const MouseState & _state, MouseButton _button) :
        MouseEvent(_state, _button)
    {

    }
}
