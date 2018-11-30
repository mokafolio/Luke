#ifndef LUKE_GLFW_GLFWINITIALIZER_HPP
#define LUKE_GLFW_GLFWINITIALIZER_HPP

#include <Stick/Platform.hpp>

namespace luke
{
namespace detail
{
struct STICK_LOCAL GLFWInitializer
{
    GLFWInitializer();

    ~GLFWInitializer();

    static GLFWInitializer & instance();
};
} // namespace detail
} // namespace luke

#endif // LUKE_GLFW_GLFWINITIALIZER_HPP
