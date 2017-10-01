#ifndef LUKE_KEYEVENTS_HPP
#define LUKE_KEYEVENTS_HPP

#include <Stick/Event.hpp>
#include <Luke/KeyCodes.hpp>

namespace luke
{
    class STICK_API KeyboardEvent
    {
        friend class Keyboard;

    public:

        KeyboardEvent(KeyCode _key);

        KeyboardEvent(KeyCode _key, const stick::String & _unicode);

        ~KeyboardEvent();


        stick::String unicode() const;

        KeyCode key() const;


    private:

        KeyCode m_key;
        stick::String m_unicode; //holds unicode characters if unicode translation is activated
    };

    class STICK_API KeyDownEvent :
        public KeyboardEvent,
        public stick::EventT<KeyDownEvent>
    {
    public:

        KeyDownEvent(KeyCode _key, const stick::String & _unicode, bool _bRepeat);

        bool isRepeat() const;

    private:

        bool m_bIsRepeat;
    };

    class STICK_API KeyUpEvent :
        public KeyboardEvent,
        public stick::EventT<KeyUpEvent>
    {
    public:

        KeyUpEvent(KeyCode _key, const stick::String & _unicode);
    };
}

#endif //LUKE_KEYEVENTS_HPP
