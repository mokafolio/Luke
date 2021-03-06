#ifndef LUKE_MOUSEEVENTS_HPP
#define LUKE_MOUSEEVENTS_HPP

#include <Luke/Constants.hpp>
#include <Luke/MouseState.hpp>
#include <Stick/Event.hpp>

namespace luke
{
class STICK_API MouseEventCategory
{
};

class STICK_API MouseEvent
{
  public:
    MouseEvent(const MouseState & _state, MouseButton _button = MouseButton::None);

    virtual ~MouseEvent();

    stick::Float32 x() const;

    stick::Float32 y() const;

    // returns the button associated with this event
    MouseButton button() const;

    const MouseState & mouseState() const;

  private:
    MouseState m_state;
    MouseButton m_modifiedButton;
};

class STICK_API MouseScrollEvent
    : public MouseEvent,
      public stick::EventT<MouseScrollEvent, LukeEventCategory, MouseEventCategory>
{
  public:
    MouseScrollEvent(const MouseState & _state, stick::Float32 _scrollX, stick::Float32 _scrollY);

    stick::Float32 scrollX() const;

    stick::Float32 scrollY() const;

  private:
    // scroll data is not in MouseStae since it is only a relative change and thus only valid during
    // the event
    stick::Float32 m_scrollX, m_scrollY;
};

class STICK_API MouseMoveEvent
    : public MouseEvent,
      public stick::EventT<MouseMoveEvent, LukeEventCategory, MouseEventCategory>
{
  public:
    MouseMoveEvent(const MouseState & _state);
};

class STICK_API MouseDownEvent
    : public MouseEvent,
      public stick::EventT<MouseDownEvent, LukeEventCategory, MouseEventCategory>
{
  public:
    MouseDownEvent(const MouseState & _state, MouseButton _button);
};

class STICK_API MouseUpEvent
    : public MouseEvent,
      public stick::EventT<MouseUpEvent, LukeEventCategory, MouseEventCategory>
{
  public:
    MouseUpEvent(const MouseState & _state, MouseButton _button);
};
} // namespace luke

#endif // LUKE_MOUSEEVENTS_HPP
