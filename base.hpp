#ifndef base_hpp
#define base_hpp

#include <SDL2pp/SDL2pp.hh>
#include <functional>

using namespace SDL2pp;

#define RESOURCE_PATH "res/"
#define REMOVEFROMONCE(elem, vect)                                             \
    for (auto e = vect->begin(); e != vect->end(); e++) {                      \
        if (*e == elem) {                                                      \
            vect->erase(e);                                                    \
            break;                                                             \
        }                                                                      \
    }

// The render callback type
typedef std::function<void()> render_callback;

// The move callback type
/* Argument 1: result of SDL_GetTick64()
   Argument 2: delta of SDL_GetTick64() from last move() call to this
 */
typedef std::function<void(Uint64,Uint64)> move_callback;

// The event handling callback type
typedef std::function<void(SDL_Event *)> event_handle_callback;

// Will you be influenced by a bomb?
typedef std::function<bool(Rect*)> bomb_detection_callback;

// When a bomb blows, all objects it influences will get this callback called.
typedef std::function<void(bomb_detection_callback)> blowup_callback;

// Collision detector
/*
   param 1: The rect to test
   param 2: What object is requesting to test its collision
            This is used to avoid colliding with oneself
*/
typedef std::function<bool(Rect *, void *)> collision_detector;

typedef struct resources_container {
    // Block
    Texture *block_ground, *block_hard_wall, *block_soft_wall;

    // Player
    Texture *player_up, *player_down, *player_right, *player_left, *player_stop;

    // Bomb
    Texture *bomb;
    std::vector<Texture*> bomb_animation_frames;
} resources_container;

extern resources_container resources;

#endif
