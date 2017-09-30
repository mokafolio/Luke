#include <Luke/Display.hpp>
#include <Luke/GLFW/GLFWDisplayImpl.hpp>

namespace luke
{
    using namespace stick;
    using namespace crunch;
    
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

    stick::Error Display::setDisplayMode(const DisplayMode & _mode)
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

    DisplayMode Display::findBestDisplayMode(Float32 _width, Float32 _height, UInt32 _colorDepth, Float32 _backingScale) const
    {
        // DisplayModeArray modes = displayModes();
        // DisplayMode ret = modes[0];
        // Float32 bestBackingScaleDiff = std::numeric_limits<Float32>::max();
        // Float32 bestWidthDiff = std::numeric_limits<Float32>::max();
        // Float32 bestHeightDiff = std::numeric_limits<Float32>::max();

        // DisplayModeArray::iterator it = modes.begin();

        // for (; it != modes.end(); ++it)
        // {
        //     DisplayMode & mode = (*it);
        //     if (mode.colorDepth() == _colorDepth)
        //     {
        //         Float32 bsDiff = std::abs(_backingScale - (*it).backingScaleFactor());
        //         Float32 widthDiff = std::abs((*it).width() - _width);
        //         Float32 heightDiff = std::abs((*it).height() - _height);

        //         //perfect match found
        //         if ((*it).width() == _width && (*it).height() == _height && (*it).backingScaleFactor() == _backingScale)
        //             return *it;

        //         //found a better backing scale match
        //         if (bsDiff < bestBackingScaleDiff)
        //         {
        //             ret = *it;
        //             bestBackingScaleDiff = bsDiff;
        //             bestWidthDiff = widthDiff;
        //             bestHeightDiff = heightDiff;
        //         }
        //         //found a better size match
        //         else if (bsDiff == bestBackingScaleDiff)
        //         {
        //             if (widthDiff <= bestWidthDiff && heightDiff <= bestHeightDiff)
        //             {
        //                 ret = *it;
        //                 bestWidthDiff = widthDiff;
        //                 bestHeightDiff = heightDiff;
        //             }
        //         }
        //     }
        // }

        // return ret;
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

    Vec2f Display::position() const
    {
        return m_pimpl->position();
    }
}