#ifndef Bomb_hpp
#define Bomb_hpp

#include "base.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace SDL2pp;
using namespace std;

#define BOMB_RADIUS 150

class Bomb {
    Renderer *renderer;
    int countdown = 2000; // Wait 2 seconds before blowing up
    int animation_frame_index = 0;
    Rect position;
    Point center;
    move_callback movecb;
    render_callback rendercb;
    vector<blowup_callback *> *blow_listeners;
    vector<move_callback *> *move_objects;
    vector<render_callback *> *render_objects;

  public:
    Bomb(Renderer *renderer, Rect *position,
         vector<move_callback *> *move_objects,
         vector<render_callback *> *render_objects,
         vector<blowup_callback *> *blow_listeners);

    void blow_up();
    ~Bomb();
};

#endif
