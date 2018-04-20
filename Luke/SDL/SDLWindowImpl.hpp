#ifndef LUKE_SDL_SDLWINDOWIMPL_HPP
#define LUKE_SDL_SDLWINDOWIMPL_HPP

#include <Luke/Window.hpp>
#include <Luke/MouseEvents.hpp>

// struct SDL_Window;
// struct SDL_GLContext;

#include "SDL.h"

namespace luke
{
    namespace detail
    {
        class STICK_LOCAL WindowImpl
        {
        public:

            WindowImpl(Window * _window);

            ~WindowImpl();

            stick::Error open(const WindowSettings & _settings, WindowImpl * _shared);

            void close();

            void setShouldClose(bool _b);


            void move(stick::Float32 _x, stick::Float32 _y);

            void moveToCenter();

            void show();

            void hide();

            void resize(stick::Float32 _width, stick::Float32 _height);

            void maximize();

            void focus();


            void enableRenderContext();

            void disableRenderContext();

            void swapBuffers();


            void setVerticalSync(bool _b);

            stick::Error enterFullscreen(FullScreenMode _mode, const Display & _display);

            stick::Error enterFullscreen(const DisplayMode & _mode, const Display & _display);

            void exitFullscreen();

            void hideCursor();

            void showCursor();

            void setTitle(const stick::String & _str);


            bool shouldClose() const;

            bool isVisible() const;

            bool isFocussed() const;

            const WindowSettings & settings() const;

            bool isCursorVisible() const;

            bool verticalSync() const;

            bool isFullscreen() const;

            bool isKeyDown(KeyCode _code) const;

            const stick::String & title() const;

            stick::Float32 backingScaleFactor() const;

            stick::Float32 width() const;

            stick::Float32 height() const;

            stick::Float32 widthInPixels() const;

            stick::Float32 heightInPixels() const;

            stick::Float32 x() const;

            stick::Float32 y() const;

            Display display() const;

            static stick::Error pollEvents();

            void deallocateSDLWindowAndContext();

            stick::UInt32 sdlWindowID() const;


            SDL_Window * m_sdlWindow;
            SDL_GLContext m_sdlGLContext;
            Window * m_window;
            MouseState m_mouseState;
            bool m_bShouldClose;
            mutable stick::String m_title;
        };
    }
}

#endif //LUKE_SDL_SDLWINDOWIMPL_HPP
