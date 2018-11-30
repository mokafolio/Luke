#include <Luke/KeyEvents.hpp>

namespace luke
{
using namespace stick;

KeyEvent::KeyEvent(KeyCode _key, Int32 _scancode) : m_key(_key), m_scancode(_scancode)
{
}

KeyCode KeyEvent::key() const
{
    return m_key;
}

Int32 KeyEvent::scancode() const
{
    return m_scancode;
}

KeyDownEvent::KeyDownEvent(KeyCode _key, Int32 _scancode, bool _bIsRepeat) :
    KeyEvent(_key, _scancode),
    m_bIsRepeat(_bIsRepeat)
{
}

bool KeyDownEvent::isRepeat() const
{
    return m_bIsRepeat;
}

KeyUpEvent::KeyUpEvent(KeyCode _key, Int32 _scancode) : KeyEvent(_key, _scancode)
{
}
} // namespace luke
