#include "Player.hpp"

Player::Player(vector<move_callback *> *move_objects,
               vector<render_callback *> *render_objects,
               vector<event_handle_callback *> *event_listeners,
               vector<collision_detector *> *collision_detectors,
               vector<blowup_callback *> *blowable_objects,
               player_control_type_enum control_type, Renderer *renderer,
               Map *map) {
    // Load image resources
    this->images[player_state_enum::up] = resources.player_up;
    this->images[player_state_enum::down] = resources.player_down;
    this->images[player_state_enum::left] = resources.player_left;
    this->images[player_state_enum::right] = resources.player_right;
    this->images[player_state_enum::stop] = resources.player_stop;

    // Initialize some variables
    this->control_type = control_type;
    this->renderer = renderer;
    this->move_objects = move_objects;
    this->render_objects = render_objects;
    this->event_listeners = event_listeners;
    this->blowable_objects = blowable_objects;
    this->map = map;
    this->collision_detectors = collision_detectors;
    this->position.x = this->map->position.x;
    this->position.y = this->map->position.y;
    this->position.w = 50;
    this->position.h = 50;
    this->blb = new BloodBar(
        renderer, render_objects,
        this->control_type == wasd ? new Color(0, 255, 0)
                                   : new Color(0, 0, 255),
        new Rect(this->control_type == wasd
                     ? 0
                     : this->renderer->GetOutputWidth() / 2,
                 map->position.GetY2(), this->renderer->GetOutputWidth() / 2,
                 this->renderer->GetOutputHeight() - this->map->position.h),
        &this->blood);

    // Move callback
    this->movecb = [=](Uint64 time, Uint64 dtime) -> void {
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
        this->renderer->Copy(*this->images[this->state], NullOpt,
                             Rect(this->position.x, this->position.y,
                                  this->position.w, this->position.h));
    };
    render_objects->push_back(&this->rendercb);

    // Event callback
    this->eventcb = [=](SDL_Event *event) -> void {
        if (!((event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) &&
              ((SDL_KeyboardEvent *)event)->repeat == 0)) {
            return;
        }

        switch (event->type) {
        case SDL_KEYDOWN:
            switch (this->control_type) {
            case player_control_type_enum::wasd:
                switch (event->key.keysym.sym) {
                case SDLK_w:
                    this->state = player_state_enum::up;
                    break;
                case SDLK_s:
                    this->state = player_state_enum::down;
                    break;
                case SDLK_a:
                    this->state = player_state_enum::left;
                    break;
                case SDLK_d:
                    this->state = player_state_enum::right;
                    break;

                case SDLK_SPACE:
                    // Put Bomb
                    new Bomb(this->renderer, &this->position,
                             this->move_objects, this->render_objects,
                             this->blowable_objects);
                    break;

                default:
                    break;
                }
                break;

            case player_control_type_enum::arrow:
                switch (event->key.keysym.sym) {
                case SDLK_UP:
                    this->state = player_state_enum::up;
                    break;
                case SDLK_DOWN:
                    this->state = player_state_enum::down;
                    break;
                case SDLK_LEFT:
                    this->state = player_state_enum::left;
                    break;
                case SDLK_RIGHT:
                    this->state = player_state_enum::right;
                    break;

                case SDLK_RCTRL:
                    // Put Bomb
                    new Bomb(this->renderer, &this->position,
                             this->move_objects, this->render_objects,
                             this->blowable_objects);
                    break;

                default:
                    break;
                }

            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (this->control_type) {
            case player_control_type_enum::wasd:
                switch (event->key.keysym.sym) {
                case SDLK_w:
                    if (this->state == player_state_enum::up)
                        this->state = player_state_enum::stop;
                    break;
                case SDLK_s:
                    if (this->state == player_state_enum::down)
                        this->state = player_state_enum::stop;
                    break;
                case SDLK_a:
                    if (this->state == player_state_enum::left)
                        this->state = player_state_enum::stop;
                    break;
                case SDLK_d:
                    if (this->state == player_state_enum::right)
                        this->state = player_state_enum::stop;
                default:
                    break;
                }
                break;

            case player_control_type_enum::arrow:
                switch (event->key.keysym.sym) {
                case SDLK_UP:
                    if (this->state == player_state_enum::up)
                        this->state = player_state_enum::stop;
                    break;
                case SDLK_DOWN:
                    if (this->state == player_state_enum::down)
                        this->state = player_state_enum::stop;
                    break;
                case SDLK_LEFT:
                    if (this->state == player_state_enum::left)
                        this->state = player_state_enum::stop;
                    break;
                case SDLK_RIGHT:
                    if (this->state == player_state_enum::right)
                        this->state = player_state_enum::stop;
                default:
                    break;
                }

            default:
                break;
            }
            break;
        }
    };
    event_listeners->push_back(&this->eventcb);

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
            this->lose();
    };
    blowable_objects->push_back(&this->blowupcb);
}

void Player::lose() {
    int *lostPlayer = new int(this->control_type);
    // Texture *screenshot=new Texture(*this->renderer, SDL_PIXELFORMAT_RGB24,
    //                SDL_TEXTUREACCESS_TARGET, this->renderer->GetOutputWidth(),
    //                this->renderer->GetOutputHeight());
    // this->renderer->SetTarget(*screenshot);
    // for(auto r : *this->render_objects)
    //     (*r)();
    // this->renderer->SetTarget();
    Surface screenshot_surface(0,this->renderer->GetOutputWidth(),this->renderer->GetOutputHeight(),32,0x00ff0000,0x0000ff00,0x000000ff,0xff000000);
    this->renderer->ReadPixels(NullOpt,SDL_PIXELFORMAT_ARGB8888,screenshot_surface.Get()->pixels,screenshot_surface.Get()->pitch);
    Texture *screenshot=new Texture(*this->renderer,screenshot_surface);

    SDL_UserEvent *e = new SDL_UserEvent{.type = ChangeGameState,
                                         .timestamp = 0,
                                         .windowID = 0,
                                         .code = changeGameStateAction::endGame,
                                         .data1 = lostPlayer,
                                         .data2 = screenshot};
    SDL_PushEvent((SDL_Event *)e);
    printf("Lose!\n");
}
