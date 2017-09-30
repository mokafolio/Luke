#ifndef LUKE_WINDOWSETTINGS_HPP
#define LUKE_WINDOWSETTINGS_HPP

#include <Luke/Display.hpp>

namespace luke
{
    class WindowSettings
    {
    public:

        WindowSettings();

        void setResizeable(bool _b);

        void setTitle(const stick::String & _title);

        void setPosition(stick::Float32 _left, stick::Float32 _bottom);

        void setSize(stick::Float32 _width, stick::Float32 _height);

        bool isResizeable() const;

        void setDisplay(const Display & _display);

        void setSampleCount(stick::UInt32 _num);

        void setColorPrecision(stick::UInt32 _colorDepth);

        void setAlphaPrecision(stick::UInt32 _alphaDepth);

        void setDepthPrecision(stick::UInt32  _depth);

        void setStencilPrecision(stick::UInt32 _stencil);


        const Display & display() const;

        const stick::String & title() const;

        stick::Float32 x() const;

        stick::Float32 y() const;

        stick::Float32 width() const;

        stick::Float32 height() const;

        stick::UInt32 sampleCount() const;

        stick::UInt32 colorPrecision() const;

        stick::UInt32 alphaPrecision() const;

        stick::UInt32 depthPrecision() const;

        stick::UInt32 stencilPrecision() const;


    private:

        bool m_bIsResizeableByDragging; //if true, the window can be resized by dragging
        stick::String m_title;
        stick::Float32 m_initialWidth;
        stick::Float32 m_initialHeight;
        stick::Float32 m_initialX;
        stick::Float32 m_initialY;
        Display m_display;
        stick::UInt32 m_samples;
        stick::UInt32 m_colorPrecision;
        stick::UInt32 m_alphaPrecision;
        stick::UInt32 m_stencilPrecision;
        stick::UInt32 m_depthPrecision;
    };
}

#endif //LUKE_WINDOWSETTINGS_HPP
