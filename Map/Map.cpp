#include "Map.hpp"

Map::Map(vector<render_callback *> *render_objects, Renderer *renderer,
         vector<collision_detector *> *collision_detectors,
         vector<blowup_callback *> *blow_callbacks,
         vector<move_callback *> *move_callbacks) {
    // Initialize some variables
    this->renderer = renderer;
    this->collision_detectors = collision_detectors;
    this->ground = new Texture(*renderer, RESOURCE_PATH "block-ground.png");
    this->hard_wall =
        new Texture(*renderer, RESOURCE_PATH "block-hard-wall.png");
    this->soft_wall =
        new Texture(*renderer, RESOURCE_PATH "block-soft-wall.png");

    // Initialize all blocks
    /* TODO: Dynamically load from file*/
    /* 16x9
        0-ground
        1-hard wall
        2-soft wall
    */
    string map = "00000000000000000000000001000000000000\n"
                 "00000000000000000000000001000000000000\n"
                 "00000000000000000000000001000000000000\n"
                 "00000000000000000000000001000222222222\n"
                 "00000000000000000000000000000000000000\n"
                 "00000000000100000000001000000111111111\n"
                 "00000000000100000000000000000100000000\n"
                 "00222000000100000022002000000100000000\n"
                 "00000000000100000000000000000100000000\n"
                 "00000000000000000000022200000000000000\n"
                 "00000000000000000000022200000000000000\n"
                 "00000000000000000000022200000000000000\n"
                 "00000000000111110000000000000111111111\n"
                 "00000000000000000000000000000000000000\n"
                 "12222222222222201111001000000200000000\n"
                 "00000000000100000000000000000200000000\n"
                 "00222000000100000022002000000200000000\n"
                 "00222000000100000022002000000200000000\n"
                 "00222000000100000022002000000200000000\n"
                 "00000000000100000000000000000200000000\n";

    // Initialize all blocks - Process the string
    vector<string> lines;
    int last_index = 0;
    for (int i = 0; i < map.length(); i++) {
        if (map[i] == '\n') {
            lines.push_back(map.substr(last_index, i - last_index));
            last_index = i + 1;
        }
    }
    int height = lines.size(), width = lines[0].size();
    this->cache = new Texture(*this->renderer, SDL_PIXELFORMAT_RGB24,
                              SDL_TEXTUREACCESS_TARGET, BLOCK_WIDTH * width,
                              BLOCK_HEIGHT * height);

    // Vertical: Top  Horizontal: Middle
    this->position.y = 0;
    this->position.h = this->cache->GetHeight();
    this->position.x =
        (this->renderer->GetOutputWidth() - this->cache->GetWidth()) / 2;
    this->position.SetX2(this->position.x + this->cache->GetWidth());
    // Initialize all blocks - Actually create the blocks
    Rect pos(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
    for (auto line : lines) {
        vector<Block *> *row = new vector<Block *>;
        for (auto block_char : line) {
            switch (block_char) {
            case block_type_for_map::ground_block:
                row->push_back(new Block(this->renderer, new Rect(pos),
                                         blow_callbacks, move_callbacks,
                                         block_type_enum::ground));
                break;
            case block_type_for_map::hard_wall_block:
                row->push_back(new Block(this->renderer, new Rect(pos),
                                         blow_callbacks, move_callbacks,
                                         block_type_enum::hard_wall));
                break;
            case block_type_for_map::soft_wall_block:
                row->push_back(new Block(this->renderer, new Rect(pos),
                                         blow_callbacks, move_callbacks,
                                         block_type_enum::soft_wall));
                break;
            default:
                printf("Encountered unknown block %c\n", block_char);
                break;
            }
            pos.x += BLOCK_WIDTH;
        }
        this->blocks.push_back(row);
        pos.x = 0;
        pos.y += BLOCK_HEIGHT;
    }

    // Render callback
    this->rendercb = [=]() -> void {
        bool need_render = false;
        for (auto line : this->blocks) {
            for (auto block : *line) {
                need_render |= block->need_render;
            }
        }

        if (need_render) {
            this->renderer->SetTarget(*this->cache);
            // renderers doesn't delete themselves
            for (auto line : this->blocks)
                for (auto block : *line)
                    if (block->need_render)
                        block->render();
        }

        this->renderer->SetTarget();

        this->renderer->Copy(*this->cache, NullOpt,
                             this->position.GetTopLeft());
    };
    render_objects->push_back(&this->rendercb);

    // Move callback
    this->movecb = [=](Uint64 time, Uint64 dtime) -> void {
        for (auto line : this->blocks) {
            for (auto block : *line) {
                block->move(time, dtime);
            }
        }
    };
    move_callbacks->push_back(&this->movecb);

    // Collision callback
    this->collisiondet = [=](Rect *other, void *from) -> bool {
        if (from == this)
            return false;

        // Convert the coordinate system from window to map
        Rect map_coordinate =
            Rect(other->x - this->position.x, other->y - this->position.y,
                 // No scaling occurred when rendering map onto screen
                 other->w, other->h);
        bool intersects = false;
        for (auto line : this->blocks) {
            for (auto block : *line) {
                intersects |= block->intersects(&map_coordinate);
            }
        }
        return intersects;
    };
    collision_detectors->push_back(&this->collisiondet);
}
