#include "Player.hpp"

Player::Player(std::vector<move_callback> *movers,
               std::vector<render_callback> *render_objects,
               std::vector<event_handle_callback> *event_listeners,
               control_type_enum control_type,
               Renderer *renderer) {
    // Load image resources

    // Initialize some variables
    // this->width = image width
    // this->height = image height
    this->control_type=control_type;
    this->renderer=renderer;

    // Move callback
    movers->push_back([=](Uint64 time)->void {
        if(this->go_up) this->y-=5;
        if(this->go_down) this->y+=5;
        if(this->go_left) this->x-=5;
        if(this->go_right) this->x+=5;

        if(this->y<0) this->y=0;
        if(this->x<0) this->x=0;
        if(this->y>this->renderer->GetOutputHeight()-this->height) this->y = this->renderer->GetOutputHeight()-this->height;
        if(this->x>this->renderer->GetOutputWidth()-this->width) this->x = this->renderer->GetOutputWidth()-this->width;

    });

    // Render callback
    render_objects->push_back([=]()->void {
        // Should use image resource, but let's use rectangle for now
        this->renderer->SetDrawColor(255,255,255,255);
        this->renderer->FillRect(this->x,this->y,this->x+this->width,this->y+this->height);

    });

    // Event callback
    event_listeners->push_back([=](SDL_Event* event)->void{
        if(!(event->type==SDL_KEYDOWN||event->type==SDL_KEYUP)){
            return;
        }

        switch (this->control_type)
        {
        case wasd:
            switch(event->key.keysym.sym){
                case SDLK_w:
                    this->go_up=event->type==SDL_KEYDOWN?true:false;
                    break;
                case SDLK_s:
                    this->go_down = event->type == SDL_KEYDOWN ? true : false;
                    break;
                case SDLK_a:
                    this->go_left = event->type == SDL_KEYDOWN ? true : false;
                    break;
                case SDLK_d:
                    this->go_right = event->type == SDL_KEYDOWN ? true : false;
                default:
                    break;
            }
            break;

        case arrow:
            switch (event->key.keysym.sym) {
            case SDLK_UP:
                    this->go_up = event->type == SDL_KEYDOWN ? true : false;
                    break;
            case SDLK_DOWN:
                    this->go_down = event->type == SDL_KEYDOWN ? true : false;
                    break;
            case SDLK_LEFT:
                    this->go_left = event->type == SDL_KEYDOWN ? true : false;
                    break;
            case SDLK_RIGHT:
                    this->go_right = event->type == SDL_KEYDOWN ? true : false;
            default:
                    break;
            }

        default:
            break;
        }
    });
}