#ifndef LUKE_GLFW_GLFWINITIALIZER_HPP
#define LUKE_GLFW_GLFWINITIALIZER_HPP

#include <GLFW/glfw3.h>

namespace luke
{
    namespace detail
    {
        struct STICK_LOCAL GLFWInitializer
        {
            GLFWInitializer()
            {
                STICK_ASSERT(glfwInit());
            }

            ~GLFWInitializer()
            {
                glfwTerminate();
            }

            static GLFWInitializer & instance()
            {
                static GLFWInitializer s_instance;
                return s_instance;
            }
        };
    }
}

#endif //LUKE_GLFW_GLFWINITIALIZER_HPP
