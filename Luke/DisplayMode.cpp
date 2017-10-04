#include <Luke/DisplayMode.hpp>

namespace luke
{
    using namespace stick;
    
    DisplayMode::DisplayMode() :
        m_width(0),
        m_height(0),
        m_redBits(0),
        m_greenBits(0),
        m_blueBits(0),
        m_refreshRate(0)
    {

    }

    DisplayMode::DisplayMode(Float32 _width, Float32 _height,
                             stick::UInt32 _redBits, stick::UInt32 _greenBits, stick::UInt32 _blueBits,
                             stick::UInt32 _refreshRate) :
        m_width(_width),
        m_height(_height),
        m_redBits(_redBits),
        m_greenBits(_greenBits),
        m_blueBits(_blueBits),
        m_refreshRate(_refreshRate)
    {

    }

    DisplayMode::~DisplayMode()
    {

    }

    bool DisplayMode::isValid() const
    {
        return m_width > 0 && m_height > 0 &&
               (m_refreshRate == RefreshRate::DontCare || m_refreshRate > 0) &&
               m_redBits > 0 && m_greenBits > 0 && m_blueBits > 0;
    }

    bool DisplayMode::operator == (const DisplayMode & _other) const
    {
        return m_refreshRate == _other.m_refreshRate &&
               m_redBits == _other.m_redBits &&
               m_greenBits == _other.m_greenBits &&
               m_blueBits == _other.m_blueBits &&
               m_width == _other.m_width &&
               m_height == _other.m_height;
    }

    bool DisplayMode::operator != (const DisplayMode & _other) const
    {
        return !(*this == _other);
    }

    bool DisplayMode::operator < (const DisplayMode & _other) const
    {
        if (m_refreshRate == _other.m_refreshRate)
        {
            if (m_redBits == _other.m_redBits && m_greenBits == _other.m_greenBits && m_blueBits == _other.m_greenBits)
            {
                if (m_width == _other.m_width)
                {
                    return m_height < _other.m_height;
                }
                else
                {
                    return m_width < _other.m_width;
                }
            }
            else
            {
                return m_redBits < _other.m_redBits;
            }
        }
        else
        {
            return m_refreshRate < _other.m_refreshRate;
        }
    }

    bool DisplayMode::operator > (const DisplayMode & _other) const
    {
        return _other < *this;
    }

    bool DisplayMode::operator <= (const DisplayMode & _other) const
    {
        return !(_other < *this);
    }

    bool DisplayMode::operator >= (const DisplayMode & _other) const
    {
        return !(*this < _other);
    }

    Float32 DisplayMode::width() const
    {
        return m_width;
    }

    Float32 DisplayMode::height() const
    {
        return m_height;
    }

    UInt32 DisplayMode::redBits() const
    {
        return m_redBits;
    }

    UInt32 DisplayMode::greenBits() const
    {
        return m_greenBits;
    }

    UInt32 DisplayMode::blueBits() const
    {
        return m_blueBits;
    }

    UInt32 DisplayMode::refreshRate() const
    {
        return m_refreshRate;
    }
}
