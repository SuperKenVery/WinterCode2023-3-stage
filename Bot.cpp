#include "Bot.hpp"

Bot::Bot(vector<move_callback *> *move_objects,
         vector<render_callback *> *render_objects,
         vector<event_handle_callback *> *event_listeners,
         vector<collision_detector *> *collision_detectors,
         vector<blowup_callback *> *blowable_objects, Renderer *renderer,
         Map *map) {
    // Load image resources
    this->image = resources.bot;

    // Initialize some variables
    this->renderer = renderer;
    this->move_objects = move_objects;
    this->render_objects = render_objects;
    this->blowable_objects = blowable_objects;
    this->map = map;
    this->collision_detectors = collision_detectors;
    this->position.x = this->map->position.x;
    this->position.y = this->map->position.y;
    this->position.w = 50;
    this->position.h = 50;

    // Move callback
    this->device = new random_device();
    this->generator = new mt19937((*this->device)());
    this->distribution = new uniform_int_distribution<int>(1, 3);
    this->movecb = [=](Uint64 time, Uint64 dtime) -> void {
        if (time % 3000 == 0) {
            vector<player_state_enum> choices(
                {player_state_enum::up, player_state_enum::down,
                 player_state_enum::left, player_state_enum::right,
                 player_state_enum::stop});
            shuffle(choices.begin(), choices.end(),*this->generator);
            this->state = choices[0];

            if ((*this->distribution)(*this->generator) == 0)
                new Bomb(this->renderer, new Rect(this->position),
                         this->move_objects, this->render_objects,
                         this->blowable_objects);
        }

        const int speed = 5;
        Rect new_position = this->position;
        switch (this->state) {
        case player_state_enum::up:
            new_position.y -= speed;
            break;
        case player_state_enum::down:
            new_position.y += speed;
            break;
        case player_state_enum::left:
            new_position.x -= speed;
            break;
        case player_state_enum::right:
            new_position.x += speed;
            break;
        case stop:
            // Do nothing
            return;
        }
        bool intersects = false;
        for (auto detect : *this->collision_detectors)
            if (detect != &this->collisiondet)
                intersects |= (*detect)(&new_position, this);
        if (!intersects)
            this->position = new_position;
        // Don't move out of map
        if (this->position.y < this->map->position.GetY())
            this->position.y = this->map->position.GetY();
        if (this->position.x < this->map->position.GetX())
            this->position.x = this->map->position.GetX();
        if (this->position.y > this->map->position.GetY2() - this->position.h)
            this->position.y = this->map->position.GetY2() - this->position.h;
        if (this->position.x > this->map->position.GetX2() - this->position.w)
            this->position.x = this->map->position.GetX2() - this->position.w;
    };
    move_objects->push_back(&this->movecb);

    // Render callback
    this->rendercb = [=]() -> void {
        this->renderer->Copy(*this->image, NullOpt,
                             Rect(this->position.x, this->position.y,
                                  this->position.w, this->position.h));
    };
    render_objects->push_back(&this->rendercb);

    // Collide detection
    this->collisiondet = [=](Rect *other, void *from) -> bool {
        if (from == this)
            return false;
        return this->position.Intersects(*other);
    };
    collision_detectors->push_back(&this->collisiondet);

    // Blow up callback
    this->blowupcb = [=](bomb_detection_callback willBeBlownUp) -> void {
        if (!willBeBlownUp(&this->position))
            return;
        this->blood -= BOMB_HURT;
        if (this->blood <= 0)
            delete this;
    };
    blowable_objects->push_back(&this->blowupcb);
}


Bot::~Bot() {
    // Remove callbacks from callback chains
    REMOVEFROMONCE(&this->movecb, this->move_objects)
    REMOVEFROMONCE(&this->rendercb, this->render_objects)
    REMOVEFROMONCE(&this->collisiondet, this->collision_detectors)
    REMOVEFROMONCE(&this->blowupcb, this->blowable_objects)

    delete this->device;
    delete this->generator;
    delete this->distribution;
}
