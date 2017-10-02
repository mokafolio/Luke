#include <Luke/KeyEvents.hpp>

namespace luke
{
    using namespace stick;

    KeyboardEvent::KeyboardEvent(KeyCode _key) :
        m_key(_key)
    {

    }

    KeyCode KeyboardEvent::key() const
    {
        return m_key;
    }

    KeyDownEvent::KeyDownEvent(KeyCode _key, bool _bIsRepeat) :
        KeyboardEvent(_key),
        m_bIsRepeat(_bIsRepeat)
    {

    }

    bool KeyDownEvent::isRepeat() const
    {
        return m_bIsRepeat;
    }

    KeyUpEvent::KeyUpEvent(KeyCode _key) :
        KeyboardEvent(_key)
    {

    }
}
