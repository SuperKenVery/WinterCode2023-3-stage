#include "Bomb.hpp"

Bomb::Bomb(Renderer *renderer, Rect *position,
           vector<move_callback *> *move_objects,
           vector<render_callback *> *render_objects,
           vector<blowup_callback *> *blow_listeners) {
    this->renderer = renderer;
    this->position = *position;
    this->blow_listeners = blow_listeners;
    this->move_objects = move_objects;
    this->render_objects = render_objects;
    this->center = Point(this->position.x + this->position.w / 2,
                         this->position.y + this->position.h / 2);

    this->movecb = [=](Uint64 time, Uint64 dtime) -> void {
        if (this->countdown > 0) {
            this->countdown -= dtime;
        } else {
            animation_frame_index++;
        }
    };
    move_objects->push_back(&this->movecb);

    this->rendercb = [=]() -> void {
        if (this->countdown > 0) {
            // Don't blow up yet
            this->renderer->Copy(*resources.bomb, NullOpt, this->position);
        } else {
            if (animation_frame_index <
                resources.bomb_animation_frames.size()) {
                // Play animation
                Point center = Point(this->position.x + this->position.w / 2,
                                     this->position.y + this->position.h / 2);
                Texture *frame =
                    resources.bomb_animation_frames[animation_frame_index];
                this->renderer->Copy(*frame, NullOpt,
                                     Point(center.x - frame->GetWidth() / 2,
                                           center.y - frame->GetHeight() / 2));
            } else {
                // Blow up now
                this->blow_up();
            }
        }
    };
    render_objects->push_back(&this->rendercb);
}

void Bomb::blow_up() {
    bomb_detection_callback bdcb = [=](Rect *pos) -> bool {
        Point center = Point(pos->x + pos->w / 2, pos->y + pos->h / 2);
        Uint32 dx = abs(center.x - this->center.x),
               dy = abs(center.y - this->center.y);
        if (dx > BOMB_RADIUS || dy > BOMB_RADIUS)
            return false;

        Uint32 distance = sqrt(dx * dx + dy * dy);

        if (distance <= BOMB_RADIUS) {
            return true;
        } else {
            return false;
        }
    };

    CALLTHROUGH(this->blow_listeners,bdcb);

    // This is not recommended, but is very convenient here.
    delete this;
}

Bomb::~Bomb() {
    REMOVEFROMONCE(&this->movecb, this->move_objects)
    REMOVEFROMONCE(&this->rendercb, this->render_objects)
}
