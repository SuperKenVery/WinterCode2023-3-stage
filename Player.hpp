#ifndef Player_hpp
#define Player_hpp

#include "Bomb.hpp"
#include "Map/Map.hpp"
#include "base.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <string>
#include <unordered_map>
#include <vector>

using namespace SDL2pp;
using namespace std;

enum player_control_type_enum { wasd, arrow };
enum player_state_enum { up, down, left, right, stop };

class Player {
    Renderer *renderer;
    Map *map;
    player_control_type_enum control_type;
    player_state_enum state = stop;
    unordered_map<player_state_enum,Texture *> images;

    // The callbacks
    move_callback movecb;
    render_callback rendercb;
    event_handle_callback eventcb;
    collision_detector collisiondet;
    blowup_callback blowupcb;

    // The vector<callback>s
    vector<move_callback *> *move_objects;
    vector<render_callback *> *render_objects;
    vector<event_handle_callback *> *event_listeners;
    vector<collision_detector *> *collision_detectors;
    vector<blowup_callback *> *blowable_objects;
    // TODO: destructor

  public:
    Rect position;

  public:
    Player(vector<move_callback *> *move_objects,
           vector<render_callback *> *render_objects,
           vector<event_handle_callback *> *event_listeners,
           vector<collision_detector *> *collision_detectors,
           vector<blowup_callback *> *blowable_objects,
           player_control_type_enum control_type, Renderer *renderer, Map *map);
};

#endif
