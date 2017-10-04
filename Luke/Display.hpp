#ifndef LUKE_DISPLAY_HPP
#define LUKE_DISPLAY_HPP

#include <Stick/Error.hpp>
#include <Stick/UniquePtr.hpp>
#include <Luke/DisplayMode.hpp>

namespace luke
{
    class Display;
    using DisplayArray = stick::DynamicArray<Display>;

    namespace detail
    {
        class WindowImpl;
        class DisplayImpl;
        using DisplayImplUniquePtr = stick::UniquePtr<DisplayImpl>;
    }

    class STICK_API Display
    {
        friend class detail::DisplayImpl;
        friend class detail::WindowImpl;

    public:

        Display();

        ~Display();

        Display(const Display & _other);

        Display & operator = (const Display & _other);

        stick::Error setDisplayMode(const DisplayMode & _mode);

        DisplayMode currentDisplayMode() const;

        DisplayMode findBestDisplayMode(stick::Float32 _width, stick::Float32 _height,
                                        stick::UInt32 _redBits = 8, stick::UInt32 _greenBits = 8, stick::UInt32 _blueBits = 8,
                                        stick::UInt32 _refreshRate = RefreshRate::DontCare) const;

        DisplayModeArray displayModes() const;

        bool isValidDisplayMode(const DisplayMode & _mode) const;

        bool isValid() const;

        stick::Float32 x() const;

        stick::Float32 y() const;


        static Display mainDisplay();

        static DisplayArray displays();


    private:

        detail::DisplayImplUniquePtr m_pimpl;
    };
}

#endif //LUKE_DISPLAY_HPP
