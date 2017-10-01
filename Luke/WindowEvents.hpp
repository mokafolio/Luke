#ifndef LUKE_WINDOWEVENTS_HPP
#define LUKE_WINDOWEVENTS_HPP

#include <Stick/Event.hpp>

namespace luke
{
    class STICK_API WindowMoveEvent :
        public stick::EventT<WindowMoveEvent>
    {
    public:

        WindowMoveEvent(stick::Float32 _x, stick::Float32 _y);

        // WindowMoveEvent(const WindowMoveEvent &) = default;
        // WindowMoveEvent(WindowMoveEvent &&) = default;
        // WindowMoveEvent & operator = (const WindowMoveEvent &) = default;
        // WindowMoveEvent & operator = (WindowMoveEvent &&) = default;

        stick::Float32 x() const;

        stick::Float32 y() const;

    private:

        stick::Float32 m_x;
        stick::Float32 m_y;
    };

    class STICK_API WindowResizeEvent :
        public stick::EventT<WindowResizeEvent>
    {
    public:

        WindowResizeEvent(stick::Float32 _width, stick::Float32 _height);

        // WindowResizeEvent(const WindowResizeEvent &) = default;
        // WindowResizeEvent(WindowResizeEvent &&) = default;
        // WindowResizeEvent & operator = (const WindowResizeEvent &) = default;
        // WindowResizeEvent & operator = (WindowResizeEvent &&) = default;

        stick::Float32 width() const;

        stick::Float32 height() const;

    private:

        stick::Float32 m_width;
        stick::Float32 m_height;
    };

    class STICK_API WindowFocusEvent :
        public stick::EventT<WindowFocusEvent>
    {
    public:

        WindowFocusEvent();
        // WindowFocusEvent(const WindowFocusEvent &) = default;
        // WindowFocusEvent(WindowFocusEvent &&) = default;
        // WindowFocusEvent & operator = (const WindowFocusEvent &) = default;
        // WindowFocusEvent & operator = (WindowFocusEvent &&) = default;
    };

    class STICK_API WindowLostFocusEvent :
        public stick::EventT<WindowLostFocusEvent>
    {
    public:

        WindowLostFocusEvent();
        // WindowLostFocusEvent(const WindowLostFocusEvent &) = default;
        // WindowLostFocusEvent(WindowLostFocusEvent &&) = default;
        // WindowLostFocusEvent & operator = (const WindowLostFocusEvent &) = default;
        // WindowLostFocusEvent & operator = (WindowLostFocusEvent &&) = default;
    };

    class STICK_API WindowIconifyEvent :
        public stick::EventT<WindowIconifyEvent>
    {
    public:

        WindowIconifyEvent();
        // WindowLostFocusEvent(const WindowLostFocusEvent &) = default;
        // WindowLostFocusEvent(WindowLostFocusEvent &&) = default;
        // WindowLostFocusEvent & operator = (const WindowLostFocusEvent &) = default;
        // WindowLostFocusEvent & operator = (WindowLostFocusEvent &&) = default;
    };

    class STICK_API WindowRestoreEvent :
        public stick::EventT<WindowRestoreEvent>
    {
    public:

        WindowRestoreEvent();
        // WindowLostFocusEvent(const WindowLostFocusEvent &) = default;
        // WindowLostFocusEvent(WindowLostFocusEvent &&) = default;
        // WindowLostFocusEvent & operator = (const WindowLostFocusEvent &) = default;
        // WindowLostFocusEvent & operator = (WindowLostFocusEvent &&) = default;
    };
}

#endif //LUKE_WINDOWEVENTS_HPP
