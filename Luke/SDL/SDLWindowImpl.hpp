#ifndef LUKE_SDL_SDLWINDOWIMPL_HPP
#define LUKE_SDL_SDLWINDOWIMPL_HPP

#include <Luke/Window.hpp>
#include <Luke/MouseEvents.hpp>

#include <Stick/FixedArray.hpp>

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


            stick::Error enableRenderContext();

            void swapBuffers();


            void setVerticalSync(bool _b);

            stick::Error enterFullscreen(FullscreenMode _mode, const Display & _display);

            stick::Error enterFullscreen(const DisplayMode & _mode, const Display & _display);

            stick::Error exitFullscreen();

            void setCursor(CursorType _cursor);

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

            const stick::String & title() const;

            stick::Float32 backingScaleFactor() const;

            stick::Float32 width() const;

            stick::Float32 height() const;

            stick::Float32 widthInPixels() const;

            stick::Float32 heightInPixels() const;

            stick::Float32 x() const;

            stick::Float32 y() const;

            Display display() const;

            void deallocateSDLWindowAndContext();

            stick::UInt32 sdlWindowID() const;


            static stick::Error pollEvents();

            static stick::Error setClipboardText(const char * _text);

            static const char * clipboardText();

            static bool hasClipboardText();

            static const MouseState & mouseState();

            static bool isKeyDown(KeyCode _code);

            static stick::UInt32 modifiers();

            static bool modifier(KeyModifier _mod);


            using CursorArray = stick::FixedArray<SDL_Cursor *, 7>;

            SDL_Window * m_sdlWindow;
            SDL_GLContext m_sdlGLContext;
            Window * m_window;
            bool m_bShouldClose;
            stick::UInt32 m_sdlWindowID;
            mutable stick::String m_title;
            CursorArray m_cursors;
            WindowSettings m_settings;
        };
    }
}

#endif //LUKE_SDL_SDLWINDOWIMPL_HPP
