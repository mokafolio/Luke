#ifndef LUKE_WINDOW_HPP
#define LUKE_WINDOW_HPP

#include <Luke/KeyCodes.hpp>
#include <Luke/MouseState.hpp>
#include <Luke/WindowSettings.hpp>
#include <Stick/EventForwarder.hpp>

namespace luke
{
namespace detail
{
class WindowImpl;
using WindowImplUniquePtr = stick::UniquePtr<WindowImpl>;
} // namespace detail

STICK_API_ENUM_CLASS(FullscreenMode){
    Borderless, // borderless fullscreenwindow in the current desktop resolution
    Fullscreen  // actually changs the display mode
};

STICK_API_ENUM_CLASS(CursorType){ Cursor,   TextInput,  ResizeAll,  ResizeNS,
                                  ResizeEW, ResizeNESW, ResizeNWSE,
                                  Custom, // not used yet
                                  _Count };

class STICK_API Window : public stick::EventForwarderT<stick::Event,
                                                       stick::detail::ForwardingPolicyBasic,
                                                       stick::detail::PublishingPolicyBasic>
{
  public:
    Window();

    // virtual so its ok to derive from window and stuff
    virtual ~Window();

    stick::Error open(const WindowSettings & _settings = WindowSettings(),
                              Window * _shared = nullptr);

    void close();

    void setShouldClose(bool _b);

    void move(stick::Float32 _x, stick::Float32 _y);

    void moveToCenter();

    void show();

    void hide();

    void resize(stick::Float32 _width, stick::Float32 _height);

    void maximize();

    //@TODO add minimize/iconify to API

    void focus();

    stick::Error enableRenderContext();

    // enters fullscreen.
    //_mode allow to specify if its "real" fullscreen or borderless fullscreen window (sometimes
    // called desktop fullscreen). If _mode is borderless the display mode won't be changed and the
    // window will take on the desktop resolution. If _mode is fullscreen, it will try to switch to
    // the display mode that most closely matches the current window resolution. _display allows you
    // to specify a display to go fullscreen on. If not provided, window goes fullscreen on the
    // display that it is on.
    stick::Error enterFullscreen(FullscreenMode _mode = FullscreenMode::Borderless,
                                 const Display & _display = Display());

    stick::Error enterFullscreen(const DisplayMode & _mode, const Display & _display = Display());

    stick::Error enterFullscreen(stick::Float32 _width,
                                 stick::Float32 _height,
                                 const Display & _display = Display());

    stick::Error exitFullscreen();

    void setVerticalSync(bool _b);

    void setCursor(CursorType _cursor);

    void hideCursor();

    void showCursor();

    void setTitle(const stick::String & _str);

    void swapBuffers();

    stick::Error setClipboardText(const char * _text);

    stick::String clipboardText() const;

    bool hasClipboardText() const;

    bool shouldClose() const;

    bool isVisible() const;

    bool isFocussed() const;

    bool isFullscreen() const;

    //@TODO: Should this return the settings that the window was opened with or the current ones???
    const WindowSettings & settings() const;

    CursorType cursor() const;

    bool isCursorVisible() const;

    bool verticalSync() const;

    const stick::String & title() const;

    Display display() const;

    stick::Float32 backingScaleFactor() const;

    stick::Float32 width() const;

    stick::Float32 height() const;

    stick::Float32 widthInPixels() const;

    stick::Float32 heightInPixels() const;

    stick::Float32 x() const;

    stick::Float32 y() const;

  private:
    detail::WindowImplUniquePtr m_pimpl;
};

//@TODO: Should these sit somewhere else?

STICK_API stick::Error pollEvents();

STICK_API stick::Error setClipboardText(const char * _txt);

STICK_API const char * clipboardText();

STICK_API bool hasClipboardText();

STICK_API const MouseState & mouseState();

STICK_API bool isKeyDown(KeyCode _key);

STICK_API stick::UInt32 modifiers();

STICK_API bool modifier(KeyModifier _mod);
} // namespace luke

#endif // LUKE_WINDOW_HPP
