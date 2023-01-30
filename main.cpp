#include "Map/Map.hpp"
#include "Player.hpp"
#include "base.hpp"
#include "gameScenes.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace SDL2pp;

resources_container resources;
Uint32 ChangeGameState;

int main() {
    SDL sdl(SDL_INIT_VIDEO);
    ChangeGameState = SDL_RegisterEvents(1);

    Window window("泡泡堂", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  1920, 1080, SDL_WINDOW_RESIZABLE);

    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    event.user = SDL_UserEvent{.type = ChangeGameState,
                               .timestamp = 0,
                               .windowID = 0,
                               .code = changeGameStateAction::startGame,
                               .data1 = nullptr,
                               .data2 = nullptr};

    while (true) {
        // Can't use switch because ChangeGameState is not constant
        if (event.type == SDL_QUIT)
            return 0;
        else if (event.type == ChangeGameState) {
            switch (event.user.code) {
            case changeGameStateAction::startGame:
                mainGameScene(renderer, &event);
                break;
            case changeGameStateAction::endGame:
                endGameScene(renderer, &event, &window);
                break;
            }
        } else {
            printf("Can't handle event whose type is %d\n", event.type);
            break;
        }
    }
}