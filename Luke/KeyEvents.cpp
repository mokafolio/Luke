#include <Luke/KeyEvents.hpp>

namespace luke
{
    using namespace stick;

    KeyboardEvent::KeyboardEvent(KeyCode _key, Int32 _scancode) :
        m_key(_key),
        m_scancode(_scancode)
    {

    }

    KeyCode KeyboardEvent::key() const
    {
        return m_key;
    }

    Int32 KeyboardEvent::scancode() const
    {
        return m_scancode;
    }

    KeyDownEvent::KeyDownEvent(KeyCode _key, Int32 _scancode, bool _bIsRepeat) :
        KeyboardEvent(_key, _scancode),
        m_bIsRepeat(_bIsRepeat)
    {

    }

    bool KeyDownEvent::isRepeat() const
    {
        return m_bIsRepeat;
    }

    KeyUpEvent::KeyUpEvent(KeyCode _key, Int32 _scancode) :
        KeyboardEvent(_key, _scancode)
    {

    }
}
