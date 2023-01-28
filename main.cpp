#include <SDL2pp/SDL2pp.hh>
#include <vector>
#include "base.hpp"
#include "Player.hpp"

using namespace SDL2pp;

int main(){
    SDL sdl(SDL_INIT_VIDEO);

    std::vector<move_callback> movers;
    std::vector<render_callback> render_objects;
    std::vector<event_handle_callback> event_listeners;


    Window window("paopaotang",
        SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
        800*2,600*2,
        SDL_WINDOW_RESIZABLE
    );

    Renderer renderer(window,-1,SDL_RENDERER_ACCELERATED);
    renderer.Clear();
    renderer.Present();

    Player left_player(&movers, &render_objects, &event_listeners, wasd,&renderer),
           right_player(&movers, &render_objects, &event_listeners, arrow,&renderer);



    while(true){
        // Handle events
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                SDL_Quit();
                return 0;
            }
            for(auto event_listener:event_listeners) event_listener(&event);
        }

        // Draw the animation
        renderer.SetDrawColor(0,0,0,255);
        renderer.Clear();
        Uint64 ticks=SDL_GetTicks64();

        for(auto mover:movers) mover(ticks);
        for(auto render_object:render_objects) render_object();

        // Show them on screen
        renderer.Present();

        // Control framerate
        const Uint32 frame_rate=60;
        Uint64 after_render=SDL_GetTicks64();
        Uint64 delay=1000/frame_rate-(after_render-ticks);
        SDL_Delay(delay);

    }
}