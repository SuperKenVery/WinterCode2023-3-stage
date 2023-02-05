#ifndef Map_hpp
#define Map_hpp

#include "Block.hpp"
#include "base.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace std;
using namespace SDL2pp;
#define BLOCK_HEIGHT 50
#define BLOCK_WIDTH 50

enum block_type_for_map {
    ground_block = '0',
    hard_wall_block = '1',
    soft_wall_block = '2'
};
class Map {
    Renderer *renderer;
    vector<vector<Block *> *> blocks;
    Texture *cache;
    vector<collision_detector*> *collision_detectors;

    // Callbacks
    render_callback rendercb;
    collision_detector collisiondet;
    blowup_callback blowcb;
    move_callback movecb;

  public:
    Texture *ground, *soft_wall, *hard_wall;
    Rect position;

  public:
    Map(vector<render_callback *> *render_objects, Renderer *renderer,
        vector<collision_detector *> *collision_detectors,
        vector<blowup_callback *> *blow_callbacks,
        vector<move_callback *> *move_callbacks);
};

#endif
