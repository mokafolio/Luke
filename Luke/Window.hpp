#ifndef LUKE_WINDOW_HPP
#define LUKE_WINDOW_HPP

#include <Stick/EventForwarder.hpp>
#include <Luke/WindowSettings.hpp>

namespace luke
{
    namespace detail
    {
        class WindowImpl;
        using WindowImplUniquePtr = stick::UniquePtr<WindowImpl>;
    }

    class STICK_API Window :
        public stick::EventForwarderT<stick::Event, stick::detail::ForwardingPolicyBasic, stick::detail::PublishingPolicyBasic>
    {
    public:

        Window();

        ~Window();

        stick::Error open(const WindowSettings & _settings = WindowSettings(), Window * _shared = nullptr);

        void close();


        void move(stick::Float32 _left, stick::Float32 _bottom);

        void moveToCenter();

        void show();

        void hide();

        void resize(stick::Float32 _width, stick::Float32 _height);

        void focus();


        void enableRenderContext();

        void disableRenderContext();


        void setVerticalSync(bool _b);

        void hideCursor();

        void showCursor();

        void setTitle(const stick::String & _str);

        void swapBuffers();


        bool shouldClose() const;

        bool isVisible() const;

        bool isFocussed() const;

        const WindowSettings & settings() const;

        bool isCursorVisible() const;

        bool verticalSync() const;

        const stick::String & title() const;

        stick::Float32 backingScaleFactor() const;

        stick::Float32 width() const;

        stick::Float32 height() const;

        stick::Float32 widthInPixels() const;

        stick::Float32 heightInPixels() const;

    private:

        detail::WindowImplUniquePtr m_pimpl;
    };

    STICK_API stick::Error pollEvents();
}

#endif //LUKE_WINDOW_HPP
