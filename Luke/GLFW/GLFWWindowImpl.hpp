#ifndef LUKE_GLFW_GLFWWINDOWIMPL_HPP
#define LUKE_GLFW_GLFWWINDOWIMPL_HPP

#include <Luke/Window.hpp>
#include <Luke/MouseEvents.hpp>

struct GLFWwindow;

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

            void enterFullscreen(const Display & _display);

            void enterFullscreen(const DisplayMode & _mode, const Display & _display);

            void hideCursor();

            void showCursor();

            void setTitle(const stick::String & _str);


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

            stick::Float32 x() const;

            stick::Float32 y() const;

            Display display() const;

            static stick::Error pollEvents();


            GLFWwindow * m_glfwWindow;
            Window * m_window;
            MouseState m_mouseState;
        };
    }
}

#endif //LUKE_GLFW_GLFWWINDOWIMPL_HPP
