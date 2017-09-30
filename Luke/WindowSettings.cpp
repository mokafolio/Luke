#include <Luke/WindowSettings.hpp>

namespace luke
{
    using namespace stick;

    WindowSettings::WindowSettings() :
        m_bIsResizeableByDragging(false),
        m_bDecorated(true),
        m_initialWidth(1280),
        m_initialHeight(720),
        m_initialX(0),
        m_initialY(0),
        m_samples(0),
        m_colorPrecision(32),
        m_alphaPrecision(8),
        m_depthPrecision(24),
        m_stencilPrecision(8)
    {

    }

    void WindowSettings::setResizeable(bool _b)
    {
        m_bIsResizeableByDragging = _b;
    }

    void WindowSettings::setDecorated(bool _b)
    {
        m_bDecorated = _b;
    }

    void WindowSettings::setTitle(const String & _title)
    {
        m_title = _title;
    }

    void WindowSettings::setPosition(Float32 _x, Float32 _y)
    {
        m_initialX = _x;
        m_initialY = _y;
    }

    void WindowSettings::setSize(Float32 _width, Float32 _height)
    {
        m_initialWidth = _width;
        m_initialHeight = _height;
    }

    void WindowSettings::setDisplay(const Display & _display)
    {
        m_display = _display;
    }

    bool WindowSettings::isResizeable() const
    {
        return m_bIsResizeableByDragging;
    }

    bool WindowSettings::isDecorated() const
    {
        return m_bDecorated;
    }

    const String & WindowSettings::title() const
    {
        return m_title;
    }

    const Display & WindowSettings::display() const
    {
        return m_display;
    }

    Float32 WindowSettings::x() const
    {
        return m_initialX;
    }

    Float32 WindowSettings::y() const
    {
        return m_initialY;
    }

    Float32 WindowSettings::width() const
    {
        return m_initialWidth;
    }

    Float32 WindowSettings::height() const
    {
        return m_initialHeight;
    }

    void WindowSettings::setSampleCount(UInt32 _num)
    {
        m_samples = _num;
    }

    void WindowSettings::setColorPrecision(UInt32 _colorDepth)
    {
        m_colorPrecision = _colorDepth;
    }

    void WindowSettings::setAlphaPrecision(UInt32 _alphaDepth)
    {
        m_alphaPrecision = _alphaDepth;
    }

    void WindowSettings::setDepthPrecision(UInt32  _depth)
    {
        m_depthPrecision = _depth;
    }

    void WindowSettings::setStencilPrecision(UInt32 _stencil)
    {
        m_stencilPrecision = _stencil;
    }

    UInt32 WindowSettings::sampleCount() const
    {
        return m_samples;
    }

    UInt32 WindowSettings::colorPrecision() const
    {
        return m_colorPrecision;
    }

    UInt32 WindowSettings::alphaPrecision() const
    {
        return m_alphaPrecision;
    }

    UInt32 WindowSettings::depthPrecision() const
    {
        return m_depthPrecision;
    }

    UInt32 WindowSettings::stencilPrecision() const
    {
        return m_stencilPrecision;
    }
}