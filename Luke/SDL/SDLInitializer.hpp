#ifndef LUKE_SDL_SDLINITIALIZER_HPP
#define LUKE_SDL_SDLINITIALIZER_HPP

#include <Stick/Platform.hpp>

namespace luke
{
namespace detail
{
struct STICK_LOCAL SDLInitializer
{
    SDLInitializer();

    ~SDLInitializer();

    static SDLInitializer & instance();
};
} // namespace detail
} // namespace luke

#endif // LUKE_SDL_SDLINITIALIZER_HPP
