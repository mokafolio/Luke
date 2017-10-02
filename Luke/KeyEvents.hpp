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

        KeyCode key() const;


    private:

        KeyCode m_key;
    };

    class STICK_API KeyDownEvent :
        public KeyboardEvent,
        public stick::EventT<KeyDownEvent>
    {
    public:

        KeyDownEvent(KeyCode _key, bool _bRepeat);

        bool isRepeat() const;

    private:

        bool m_bIsRepeat;
    };

    class STICK_API KeyUpEvent :
        public KeyboardEvent,
        public stick::EventT<KeyUpEvent>
    {
    public:

        KeyUpEvent(KeyCode _key);
    };
}

#endif //LUKE_KEYEVENTS_HPP
