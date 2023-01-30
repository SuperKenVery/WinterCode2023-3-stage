#ifndef Bot_hpp
#define Bot_hpp

#include "BloodBar.hpp"
#include "Bomb.hpp"
#include "Map/Map.hpp"
#include "base.hpp"
#include "Player.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>

#define BOMB_HURT (double)5 / 100


class Bot {
    Renderer *renderer;
    Map *map;
    player_state_enum state = stop;
    Texture *image;
    double blood = 1;
    // Random generator
    random_device *device;
    mt19937 *generator;
    uniform_int_distribution<int> *distribution;

    // The callbacks
    move_callback movecb;
    render_callback rendercb;
    collision_detector collisiondet;
    blowup_callback blowupcb;

    // The vector<callback>s
    vector<move_callback *> *move_objects;
    vector<render_callback *> *render_objects;
    vector<collision_detector *> *collision_detectors;
    vector<blowup_callback *> *blowable_objects;

  public:
    Rect position;

  public:
    Bot(vector<move_callback *> *move_objects,
           vector<render_callback *> *render_objects,
           vector<event_handle_callback *> *event_listeners,
           vector<collision_detector *> *collision_detectors,
           vector<blowup_callback *> *blowable_objects,
            Renderer *renderer, Map *map);
    ~Bot();


};

#endif
