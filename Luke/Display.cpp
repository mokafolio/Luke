#include <Luke/Display.hpp>

#if defined(LUKE_IMPLEMENTATION_GLFW)
#include <Luke/GLFW/GLFWDisplayImpl.hpp>
#elif defined(LUKE_IMPLEMENTATION_SDL)
#include <Luke/SDL/SDLDisplayImpl.hpp>
#endif

#include <cmath>

namespace luke
{
    using namespace stick;

    Display::Display() :
        m_pimpl(makeUnique<detail::DisplayImpl>(defaultAllocator()))
    {

    }

    Display::~Display()
    {

    }

    Display::Display(const Display & _other) :
        m_pimpl(_other.m_pimpl->clone())
    {

    }

    Display & Display::operator = (const Display & _other)
    {
        m_pimpl = _other.m_pimpl->clone();
        return *this;
    }

    Error Display::setDisplayMode(const DisplayMode & _mode)
    {
        // if (isValidDisplayMode(_mode))
        //     detail::DisplayImpl::setDisplayMode(m_id, _mode, _error);
        // else
        //     _error = ErrorReport(error::InvalidDisplayMode, "The display mode is unsupported by this display", __FILE__, __LINE__);

        return m_pimpl->setDisplayMode(_mode);
    }

    bool Display::isValidDisplayMode(const DisplayMode & _mode) const
    {
        DisplayModeArray modes = displayModes();
        return std::find(modes.begin(), modes.end(), _mode) != modes.end();
        // return m_pimpl->isValidDisplayMode(_mode);
    }

    DisplayMode Display::currentDisplayMode() const
    {
        // return detail::DisplayImpl::currentDisplayMode(m_id);
        return m_pimpl->currentDisplayMode();
    }

    DisplayModeArray Display::displayModes() const
    {
        // return detail::DisplayImpl::displayModes(m_id);
        return m_pimpl->displayModes();
    }

    DisplayMode Display::findBestDisplayMode(Float32 _width, Float32 _height,
            UInt32 _redBits, UInt32 _greenBits, UInt32 _blueBits,
            UInt32 _refreshRate) const
    {
        DisplayModeArray modes = displayModes();
        DisplayMode ret = modes[0];
        Float32 bestWidthDiff = std::numeric_limits<Float32>::max();
        Float32 bestHeightDiff = std::numeric_limits<Float32>::max();

        //@TODO: This is not very good yet as it requires perfect color depth matching
        //to even consider the dimensions.
        for (auto & mode : modes)
        {
            if (_refreshRate == RefreshRate::DontCare || mode.refreshRate() == _refreshRate)
            {
                if (_redBits == mode.redBits() && _greenBits == mode.greenBits() && _blueBits == mode.blueBits())
                {
                    Float32 widthDiff = std::abs(mode.width() - _width);
                    Float32 heightDiff = std::abs(mode.height() - _height);

                    //perfect match found
                    if (mode.width() == _width && mode.height() == _height)
                        return mode;

                    if (widthDiff <= bestWidthDiff && heightDiff <= bestHeightDiff)
                    {
                        ret = mode;
                        bestWidthDiff = widthDiff;
                        bestHeightDiff = heightDiff;
                    }
                }
            }
        }

        return ret;
    }

    Display Display::mainDisplay()
    {
        return detail::DisplayImpl::mainDisplay();
    }

    DisplayArray Display::displays()
    {
        return detail::DisplayImpl::displays();
    }

    bool Display::isValid() const
    {
        return m_pimpl->isValid();
    }

    Float32 Display::x() const
    {
        return m_pimpl->x();
    }

    Float32 Display::y() const
    {
        return m_pimpl->y();
    }
}
