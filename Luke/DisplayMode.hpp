#ifndef LUKE_DISPLAYMODE_HPP
#define LUKE_DISPLAYMODE_HPP

#include <Stick/DynamicArray.hpp>
#include <Crunch/Rectangle.hpp>

namespace luke
{
    struct STICK_API RefreshRate
    {
        enum
        {
            DontCare = -1
        };
    };

    class STICK_API DisplayMode
    {
    public:

        DisplayMode(stick::Float32 _width, stick::Float32 _height,
                    stick::UInt32 _redBits, stick::UInt32 _greenBits, stick::UInt32 _blueBits,
                    stick::UInt32 _refreshRate);

        DisplayMode();

        ~DisplayMode();


        bool isValid() const;

        bool operator == (const DisplayMode & _other) const;

        bool operator != (const DisplayMode & _other) const;


        bool operator < (const DisplayMode & _other) const;

        bool operator > (const DisplayMode & _other) const;


        bool operator <= (const DisplayMode & _other) const;

        bool operator >= (const DisplayMode & _other) const;


        stick::Float32 width() const;

        stick::Float32 height() const;

        stick::UInt32 redBits() const;

        stick::UInt32 greenBits() const;

        stick::UInt32 blueBits() const;

        stick::UInt32 refreshRate() const;

    private:

        stick::Float32 m_width, m_height;
        stick::UInt32 m_redBits, m_greenBits, m_blueBits;
        stick::UInt32 m_refreshRate;
    };

    using DisplayModeArray = stick::DynamicArray<DisplayMode>;
}

#endif //LUKE_DISPLAYMODE_HPP
