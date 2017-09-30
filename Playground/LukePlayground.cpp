#include <Luke/Display.hpp>
#include <Luke/Window.hpp>
#include <Luke/MouseEvents.hpp>
#include <Luke/WindowEvents.hpp>
#include <GLAnnotate/GLAnnotate.hpp>

using namespace luke;

int main(int _argc, const char * _args[])
{
    Display disp = Display::mainDisplay();
    auto current = disp.currentDisplayMode();
    auto modes = disp.displayModes();
    auto displays = Display::displays();
    disp.setDisplayMode(modes.last());
    printf("DISPLAY COUNT %lu\n", displays.count());

    Window window;
    auto err = window.open();

    window.addEventCallback([](const MouseScrollEvent & _event)
    {
        printf("MOUSE SCROLL BROOO %f %f\n", _event.scrollX(), _event.scrollY());
    });

    window.addEventCallback([](const MouseDownEvent & _event)
    {
        printf("MOUSE DOWN BROOO\n");
        if (_event.button() == MouseButton::Left)
        {
            printf("LEFT BUTTON\n");
        }
        if (_event.button() == MouseButton::Middle)
        {
            printf("MIDDLE BUTTON\n");
        }
        if (_event.button() == MouseButton::Right)
        {
            printf("RIGHT BUTTON\n");
        }
    });

    window.addEventCallback([](const MouseUpEvent & _event)
    {
        printf("MOUSE UP BROOO\n");
        if (_event.button() == MouseButton::Left)
        {
            printf("LEFT BUTTON\n");
        }
        if (_event.button() == MouseButton::Middle)
        {
            printf("MIDDLE BUTTON\n");
        }
        if (_event.button() == MouseButton::Right)
        {
            printf("RIGHT BUTTON\n");
        }
    });

    window.addEventCallback([](const MouseMoveEvent & _event)
    {
        printf("MOUSE MOOOOOOOOOOOOVE BROOO %f %f\n", _event.x(), _event.y());
    });

    window.addEventCallback([](const MouseDragEvent & _event)
    {
        printf("MOUSE DRAAAG BROOO\n");
        if (_event.button() == MouseButton::Left)
        {
            printf("LEFT BUTTON\n");
        }
        if (_event.button() == MouseButton::Middle)
        {
            printf("MIDDLE BUTTON\n");
        }
        if (_event.button() == MouseButton::Right)
        {
            printf("RIGHT BUTTON\n");
        }
    });

    if (err)
    {
        printf("%s\n", err.message().cString());
        return EXIT_FAILURE;
    }

    printf("W %f H %f\n", window.width(), window.height());
    printf("W2 %f H2 %f\n", window.widthInPixels(), window.heightInPixels());

    window.addEventCallback([](const WindowResizeEvent & _event)
    {
        printf("WINDOW RESIZED %f %f\n", _event.width(), _event.height());
    });

    window.addEventCallback([](const WindowMoveEvent & _event)
    {
        printf("WINDOW MOVED %f %f\n", _event.x(), _event.y());
    });

    window.addEventCallback([](const WindowFocusEvent & _event)
    {
        printf("WINDOW FOCUSED\n");
    });

    window.addEventCallback([](const WindowLostFocusEvent & _event)
    {
        printf("WINDOW LOOOOOOST FOCUSED\n");
    });

    while (!window.shouldClose())
    {
        window.enableRenderContext();
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}
