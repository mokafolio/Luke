#include <Luke/Display.hpp>
#include <Luke/Window.hpp>
#include <Luke/MouseEvents.hpp>
#include <Luke/KeyEvents.hpp>
#include <Luke/WindowEvents.hpp>
#include <GLAnnotate/GLAnnotate.hpp>

using namespace luke;

int main(int _argc, const char * _args[])
{
    printf("START\n");
    Display disp = Display::mainDisplay();
    printf("B\n");
    auto current = disp.currentDisplayMode();
    printf("C\n");
    auto modes = disp.displayModes();
    printf("D\n");
    for (auto & mode : modes)
    {
        printf("DISPLAY MODE W %f H %f RR %i R %i G %i B %i\n", mode.width(), mode.height(), mode.refreshRate(), mode.redBits(), mode.greenBits(), mode.blueBits());
    }

    auto displays = Display::displays();
    disp.setDisplayMode(modes.last());
    printf("DISPLAY COUNT %lu\n", displays.count());

    Window window;
    WindowSettings settings;
    settings.setResizeable(true);
    settings.setSize(800, 600);
    auto err = window.open(settings);

    window.setVerticalSync(true);
    window.addEventCallback([](const MouseScrollEvent & _event)
    {
        printf("MOUSE SCROLL BROOO %f %f\n", _event.scrollX(), _event.scrollY());
    });

    window.addEventCallback([&](const MouseDownEvent & _event)
    {
        printf("MOUSE DOWN BROOO\n");
        if (_event.button() == MouseButton::Left)
        {
            printf("LEFT BUTTON\n");
            // window.enterFullscreen(800, 600);
            // if (!window.isFullscreen())
            //     window.enterFullscreen();
            // else
            //     window.exitFullscreen();
            // window.move(400, 400);
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

    window.addEventCallback([](const WindowIconifyEvent & _event)
    {
        printf("WINDOW ICOOOOOONIFYYY\n");
    });

    window.addEventCallback([](const WindowRestoreEvent & _event)
    {
        printf("WINDOW RESTOROOOOROOROROREEE\n");
    });

    window.addEventCallback([&](const KeyUpEvent & _event)
    {
        printf("KEY UP %i\n", _event.key());

        if (_event.key() == KeyCode::Subtract)
        {
            printf("DA SUBTRACT\n");
        }
        else if (_event.key() == KeyCode::Slash)
        {
            printf("DA SLAAASH\n");
        }
        else if (_event.key() == KeyCode::Space)
        {
            if (!window.isFullscreen())
                window.enterFullscreen();
            else
                window.exitFullscreen();
        }
    });

    window.addEventCallback([](const KeyDownEvent & _event)
    {
        // printf("KEY DOWN\n");
    });

    while (!window.shouldClose())
    {
        window.enableRenderContext();
        printf("WS %f %f\n", window.widthInPixels(), window.heightInPixels());
        glViewport(0, 0, window.widthInPixels(), window.heightInPixels());
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        luke::pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}
