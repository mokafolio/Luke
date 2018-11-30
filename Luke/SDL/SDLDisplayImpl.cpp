#include <Luke/SDL/SDLDisplayImpl.hpp>
#include <Luke/SDL/SDLInitializer.hpp>

#include "SDL.h"

namespace luke
{
namespace detail
{
using namespace stick;

DisplayImpl::DisplayImpl() : m_displayID(-1)
{
}

DisplayImpl::~DisplayImpl()
{
}

DisplayImplUniquePtr DisplayImpl::clone() const
{
    auto ret = makeUnique<DisplayImpl>(defaultAllocator());
    ret->m_displayID = m_displayID;
    return ret;
}

Error DisplayImpl::setDisplayMode(const DisplayMode & _mode)
{
    return Error(
        ec::Unsupported,
        "SDL only allows you to change the display mode when entering fullscreen with a window",
        STICK_FILE,
        STICK_LINE);
}

static DisplayMode fromSDLDisplaymode(SDL_DisplayMode & _mode)
{
    //@TODO: make this way more complete;
    UInt32 redBits, greenBits, blueBits;
    redBits = greenBits = blueBits = 0;
    if (_mode.format == SDL_PIXELFORMAT_RGB444 || _mode.format == SDL_PIXELFORMAT_ARGB4444 ||
        _mode.format == SDL_PIXELFORMAT_RGBA4444 || _mode.format == SDL_PIXELFORMAT_ABGR4444 ||
        _mode.format == SDL_PIXELFORMAT_BGRA4444)
    {
        redBits = greenBits = blueBits = 4;
    }
    else if (_mode.format == SDL_PIXELFORMAT_RGB24 || _mode.format == SDL_PIXELFORMAT_BGR24 ||
             _mode.format == SDL_PIXELFORMAT_RGB888 || _mode.format == SDL_PIXELFORMAT_RGBX8888 ||
             _mode.format == SDL_PIXELFORMAT_BGRX8888 || _mode.format == SDL_PIXELFORMAT_ARGB8888 ||
             _mode.format == SDL_PIXELFORMAT_RGBA8888 || _mode.format == SDL_PIXELFORMAT_ABGR8888 ||
             _mode.format == SDL_PIXELFORMAT_BGRA8888 || _mode.format == SDL_PIXELFORMAT_RGBA32 ||
             _mode.format == SDL_PIXELFORMAT_ARGB32 || _mode.format == SDL_PIXELFORMAT_BGRA32 ||
             _mode.format == SDL_PIXELFORMAT_ABGR32)
    {
        redBits = greenBits = blueBits = 8;
    }

    return DisplayMode(_mode.w, _mode.h, redBits, greenBits, blueBits, _mode.refresh_rate);
}

DisplayMode DisplayImpl::currentDisplayMode() const
{
    if (m_displayID != -1)
    {
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(m_displayID, &mode);
        return fromSDLDisplaymode(mode);
    }

    return DisplayMode();
}

bool DisplayImpl::isValid() const
{
    return m_displayID != -1;
}

DisplayModeArray DisplayImpl::displayModes() const
{
    SDLInitializer::instance();
    DisplayModeArray ret;
    if (m_displayID >= 0)
    {
        SDL_DisplayMode mode;
        for (Size i = 0; i < SDL_GetNumDisplayModes(m_displayID); ++i)
        {
            SDL_GetDisplayMode(m_displayID, i, &mode);
            ret.append(fromSDLDisplaymode(mode));
        }
    }
    return ret;
}

Display DisplayImpl::mainDisplay()
{
    Display ret;
    ret.m_pimpl->m_displayID = 0;
    return ret;
}

Float32 DisplayImpl::x() const
{
    if (m_displayID != -1)
    {
        SDL_Rect rect;
        SDL_GetDisplayBounds(m_displayID, &rect);
        return rect.x;
    }
    return 0;
}

Float32 DisplayImpl::y() const
{
    if (m_displayID != -1)
    {
        SDL_Rect rect;
        SDL_GetDisplayBounds(m_displayID, &rect);
        return rect.y;
    }
    return 0;
}

DisplayArray DisplayImpl::displays()
{
    DisplayArray ret;

    SDLInitializer::instance();

    for (Size i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        Display disp;
        disp.m_pimpl->m_displayID = i;
        ret.append(disp);
    }

    return ret;
}
} // namespace detail
} // namespace luke
