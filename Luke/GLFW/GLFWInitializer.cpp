#include <GLFW/glfw3.h>
#include <Luke/GLFW/GLFWInitializer.hpp>

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
} // namespace detail
} // namespace luke
