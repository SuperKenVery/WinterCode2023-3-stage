#include "Block.hpp"

Block::Block(Renderer *renderer, Rect *position,
             vector<blowup_callback *> *blowup_callbacks,
             vector<move_callback *> *move_callbacks, block_type_enum type) {
    // Initialize some variables
    this->position = position;
    this->renderer = renderer;
    this->blowup_callbacks = blowup_callbacks;
    this->move_callbacks = move_callbacks;
    this->type = type;
    switch (this->type) {
    case block_type_enum::hard_wall:
        this->image = resources.block_hard_wall;
        break;
    case block_type_enum::soft_wall:
        this->image = resources.block_soft_wall;
        break;
    case block_type_enum::ground:
        this->image = resources.block_ground;
        break;
    }

    // Blow callback
    this->blowupcb = [=](bomb_detection_callback willBeBlownUp) -> void {
        if (!willBeBlownUp(this->position))
            return;
        {
            // Make a private copy of the block texture
            // to do animation
            this->animating_image =
                new Texture(*this->renderer, SDL_PIXELFORMAT_RGB24,
                            SDL_TEXTUREACCESS_TARGET, this->image->GetWidth(),
                            this->image->GetHeight());
            this->renderer->SetTarget(*this->animating_image);
            this->renderer->Copy(*this->image);
            this->renderer->SetTarget();
        }
        this->destroying = true;
        this->need_render = true;
    };
    if (this->type == soft_wall) {
        blowup_callbacks->push_back(&this->blowupcb);
    }
}

void Block::render() {
    if(this->destroying){
        this->animating_image->SetColorMod(
            255 - (1 - this->countdown / 30.0) * 100,
            255 - (1 - this->countdown / 30.0) * 255,
            255 - (1 - this->countdown / 30.0) * 255);
        this->renderer->Copy(*this->animating_image, NullOpt, *this->position);
    }else{
        this->renderer->Copy(*this->image, NullOpt, *this->position);
    }
    this->need_render = false;
}

void Block::move(Uint64 time,Uint64 dtime) {
    if (this->destroying == false)
        return;
    this->countdown--;
    this->need_render = true;

    if (this->countdown == 0) {
        this->destroying=false;
        this->type = ground; // Implies removed from render() call chain
        this->image = resources.block_ground;
        REMOVEFROMONCE(&this->blowupcb, this->blowup_callbacks)
    }
}

bool Block::intersects(Rect *other) {
    if (this->type == soft_wall || this->type == hard_wall)
        return this->position->Intersects(*other);
    else
        return false; // ground never collides with someone
}

Block::~Block() {
    // Remove itself from blow callback
    if (this->type == soft_wall)
        REMOVEFROMONCE(&this->blowupcb, this->blowup_callbacks)

    // renderer is shared
    // image is shared
    delete this->position;
    // blowup_callbacks is managed by other code
}
