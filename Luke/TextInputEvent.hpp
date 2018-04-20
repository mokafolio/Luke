#ifndef LUKE_TEXTINPUTEVENT_HPP
#define LUKE_TEXTINPUTEVENT_HPP

#include <Stick/Event.hpp>

namespace luke
{
    class TextInputEvent : public stick::EventT<TextInputEvent>
    {
    public:

        TextInputEvent(const char * _utf8);

        const String & text() const;

    private:

        String m_utf8;
    };
}

#endif //LUKE_TEXTINPUTEVENT_HPP
