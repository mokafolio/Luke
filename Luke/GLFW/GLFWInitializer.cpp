#include <Luke/GLFW/GLFWInitializer.hpp>
#include <GLFW/glfw3.h>

namespace luke
{
    namespace detail
    {
        GLFWInitializer::GLFWInitializer()
        {
            STICK_ASSERT(glfwInit());
        }

        GLFWInitializer::~GLFWInitializer()
        {
            glfwTerminate();
        }

        GLFWInitializer & GLFWInitializer::instance()
        {
            static GLFWInitializer s_instance;
            return s_instance;
        }
    }
}
