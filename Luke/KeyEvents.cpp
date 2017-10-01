#include <Luke/KeyEvents.hpp>

namespace luke
{
    using namespace stick;

    KeyboardEvent::KeyboardEvent(KeyCode _key) :
        m_key(_key)
    {

    }

    KeyboardEvent::KeyboardEvent(KeyCode _key, const String & _unicode) :
        m_key(_key),
        m_unicode(_unicode)
    {

    }

    KeyboardEvent::~KeyboardEvent()
    {

    }

    String KeyboardEvent::unicode() const
    {
        return m_unicode;
    }

    KeyCode KeyboardEvent::key() const
    {
        return m_key;
    }

    KeyDownEvent::KeyDownEvent(KeyCode _key, const String & _unicode, bool _bIsRepeat) :
        KeyboardEvent(_key, _unicode),
        m_bIsRepeat(_bIsRepeat)
    {

    }

    bool KeyDownEvent::isRepeat() const
    {
        return m_bIsRepeat;
    }

    KeyUpEvent::KeyUpEvent(KeyCode _key, const String & _unicode) :
        KeyboardEvent(_key, _unicode)
    {

    }
}
