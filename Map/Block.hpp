#ifndef Block_hpp
#define Block_hpp

#include "base.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace std;
using namespace SDL2pp;

enum block_type_enum { hard_wall, soft_wall, ground };
class Block {
    Renderer *renderer;
    Texture *image;
    Rect *position;
    block_type_enum type;

    // The callbacks
    blowup_callback blowupcb;

    // The vector<callback>s
    vector<blowup_callback*> *blowup_callbacks;

  public:
    bool need_render = true;

  public:
    Block(Renderer *renderer, Rect *position,
          vector<blowup_callback*> *blow_callbacks,
          block_type_enum type);

    /* Render the block on Map

       Before calling, Map may set the render target to its cache
       The work of checking need_render should be done by Map
    */
    void render();

    /* move

       Might be used if we want to render animation for block
    */
    void move(Uint64 time);

    bool intersects(Rect *other);

    ~Block();
};

#endif
