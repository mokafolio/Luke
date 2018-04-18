#include <Luke/SDL/SDLInitializer.hpp>

#include <cstdlib>
#include "SDL.h"

namespace luke
{
    namespace detail
    {
        SDLInitializer::SDLInitializer()
        {
            if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
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
    }
}
