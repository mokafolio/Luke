#include <Luke/SDL/SDLInitializer.hpp>

#include "SDL.h"
#include <cstdlib>

namespace luke
{
namespace detail
{
SDLInitializer::SDLInitializer()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }
}

SDLInitializer::~SDLInitializer()
{
    SDL_Quit();
}

SDLInitializer & SDLInitializer::instance()
{
    static SDLInitializer s_instance;
    return s_instance;
}
} // namespace detail
} // namespace luke
