#ifndef LUKE_KEYEVENTS_HPP
#define LUKE_KEYEVENTS_HPP

#include <Luke/Constants.hpp>
#include <Luke/KeyCodes.hpp>
#include <Stick/Event.hpp>

namespace luke
{
struct STICK_API KeyEventCategory
{
};

class STICK_API KeyEvent
{
  public:
    KeyEvent(KeyCode _key, stick::Int32 _scancode);

    KeyCode key() const;

    stick::Int32 scancode() const;

  private:
    KeyCode m_key;
    stick::Int32 m_scancode;
};

class STICK_API KeyDownEvent
    : public KeyEvent,
      public stick::EventT<KeyDownEvent, LukeEventCategory, KeyEventCategory>
{
  public:
    KeyDownEvent(KeyCode _key, stick::Int32 _scancode, bool _bRepeat);

    bool isRepeat() const;

  private:
    bool m_bIsRepeat;
};

class STICK_API KeyUpEvent : public KeyEvent,
                             public stick::EventT<KeyUpEvent, LukeEventCategory, KeyEventCategory>
{
  public:
    KeyUpEvent(KeyCode _key, stick::Int32 _scancode);
};
} // namespace luke

#endif // LUKE_KEYEVENTS_HPP
