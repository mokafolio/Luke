#ifndef LUKE_SDL_SDLDISPLAYIMPL_HPP
#define LUKE_SDL_SDLDISPLAYIMPL_HPP

#include <Luke/Display.hpp>

// struct GLFWmonitor;

namespace luke
{
    namespace detail
    {
        class STICK_LOCAL DisplayImpl
        {
        public:

            DisplayImpl();

            ~DisplayImpl();

            stick::Error setDisplayMode(const DisplayMode & _mode);

            DisplayMode currentDisplayMode() const;

            DisplayModeArray displayModes() const;

            bool isValid() const;

            stick::Float32 x() const;

            stick::Float32 y() const;

            static Display mainDisplay();

            static DisplayArray displays();

            DisplayImplUniquePtr clone() const;


            int m_displayID;
        };
    }
}

#endif //LUKE_SDL_SDLDISPLAYIMPL_HPP
