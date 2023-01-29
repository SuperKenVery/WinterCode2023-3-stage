#include "Block.hpp"

Block::Block(Renderer *renderer, Rect position,
             vector<blowup_callback *> *blowup_callbacks,
             block_type_enum type) {
    // Initialize some variables
    this->position = position;
    this->renderer = renderer;
    this->blowup_callbacks = blowup_callbacks;
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
        if (!willBeBlownUp(&this->position))
            return;
        switch (this->type) {
        case ground:
            break;
        case hard_wall:
            break;
        case soft_wall:
            this->type = ground;
            this->image = resources.block_ground;
            this->need_render = true;
            break;
        }
    };
    blowup_callbacks->push_back(&this->blowupcb);
}

void Block::render() {
    this->renderer->Copy(*this->image, NullOpt, this->position);
    this->need_render = false;
}

void Block::move(Uint64 time) {}

bool Block::intersects(Rect *other) {
    if (this->type == soft_wall || this->type == hard_wall)
        return this->position.Intersects(*other);
    else
        return false; // ground never collides with someone
}
