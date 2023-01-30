#ifndef gameScenes_hpp
#define gameScenes_hpp

#include "Player.hpp"
#include "base.hpp"
#include "Bot.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <string>

using namespace SDL2pp;
using namespace std;

void mainGameScene(Renderer &renderer, SDL_Event *event);
void endGameScene(Renderer &renderer, SDL_Event *event, Window *w);

#endif
