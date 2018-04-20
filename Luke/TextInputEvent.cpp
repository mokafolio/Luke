#include <Luke/TextInputEvent.hpp>

namespace luke
{
    TextInputEvent::TextInputEvent(const char * _utf8) :
    m_utf8(_utf8)
    {

    }

    const String & TextInputEvent::text() const
    {
        return m_utf8;
    }
}
