#ifndef LUKE_TEXTINPUTEVENT_HPP
#define LUKE_TEXTINPUTEVENT_HPP

#include <Stick/Event.hpp>

namespace luke
{
    class TextInputEvent : public stick::EventT<TextInputEvent>
    {
    public:

        TextInputEvent(const char * _utf8);

        const stick::String & text() const;

    private:

        stick::String m_utf8;
    };
}

#endif //LUKE_TEXTINPUTEVENT_HPP
