#ifndef LUKE_GLFW_GLFWDISPLAYIMPL_HPP
#define LUKE_GLFW_GLFWDISPLAYIMPL_HPP

#include <Luke/Display.hpp>

struct GLFWmonitor;

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

            crunch::Vec2f position() const;

            static Display mainDisplay();

            static DisplayArray displays();

            DisplayImplUniquePtr clone() const;


            GLFWmonitor * m_glfwMonitor;
        };
    }
}

#endif //LUKE_GLFW_GLFWDISPLAYIMPL_HPP
