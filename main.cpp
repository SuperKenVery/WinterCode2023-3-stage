#include "Map/Map.hpp"
#include "Player.hpp"
#include "base.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace SDL2pp;

resources_container resources;

void loadResources(Renderer &r) {
    try {
        // Block
        resources.block_ground = new Texture(r, RESOURCE_PATH "block-ground.png");
        resources.block_hard_wall =
            new Texture(r, RESOURCE_PATH "block-hard-wall.png");
        resources.block_soft_wall =
            new Texture(r, RESOURCE_PATH "block-soft-wall.png");

        // Player
        resources.player_up = new Texture(r, RESOURCE_PATH "player-up.png");
        resources.player_down = new Texture(r, RESOURCE_PATH "player-down.png");
        resources.player_left = new Texture(r, RESOURCE_PATH "player-left.png");
        resources.player_right = new Texture(r, RESOURCE_PATH "player-right.png");
        resources.player_stop = new Texture(r, RESOURCE_PATH "player-stop.png");

        // Bomb
        resources.bomb = new Texture(r, RESOURCE_PATH "bomb.png");
        // TODO: Load bomb animation
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cout << "Failed to load resources, quitting now..." << std::endl;
        exit(1);
    }
}

int main() {
    SDL sdl(SDL_INIT_VIDEO);

    std::vector<move_callback*> movers;
    std::vector<render_callback*> render_objects;
    std::vector<event_handle_callback*> event_listeners;
    std::vector<blowup_callback*> blowable_objects;
    std::vector<collision_detector*> collision_detectors;

    Window window("泡泡堂", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  1920, 1080, SDL_WINDOW_RESIZABLE);

    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    renderer.Clear();
    renderer.Present();

    loadResources(renderer);

    // The order of initializing Map and Player is important
    // This is the render order

    Map map(&render_objects, &renderer, &collision_detectors, &blowable_objects);

    Player left_player(&movers, &render_objects, &event_listeners,
                       &collision_detectors, &blowable_objects, wasd, &renderer, &map),
        right_player(&movers, &render_objects, &event_listeners,
                     &collision_detectors, &blowable_objects, arrow, &renderer, &map);
    right_player.position.x +=
        left_player.position.w + 1; // Or no one can move!

    Uint64 last_time=SDL_GetTicks64(),time=SDL_GetTicks64();
    while (true) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Quit();
                return 0;
            }
            for (auto event_listener : event_listeners)
                (*event_listener)(&event);
        }

        // Draw the animation
        renderer.SetDrawColor(0, 0, 0, 255);
        renderer.Clear();
        last_time=time;
        time=SDL_GetTicks64();


        for (auto mover : movers)
            (*mover)(time,time-last_time);

        for (auto render_object : render_objects)
            (*render_object)();

        // Show them on screen
        renderer.Present();

        // Control framerate
        const Uint32 frame_rate = 60;
        Uint64 delay = 1000 / frame_rate - (time-last_time);
        // What if on a garbage machine delay<0 and wraps to very big?
        if(delay<=1000/frame_rate) SDL_Delay(delay);
    }
}