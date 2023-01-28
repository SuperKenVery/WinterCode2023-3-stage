#ifndef Player_hpp
#define Player_hpp

#include <SDL2pp/SDL2pp.hh>
#include <vector>
#include "base.hpp"

using namespace SDL2pp;

enum control_type_enum{
    wasd,
    arrow
};

class Player{
    Renderer *renderer;
    std::vector<SDL_Surface*> images;
    int x=0,y=0;
    int width=20,height=20;
    control_type_enum control_type;
    bool go_up=false,go_down=false,go_left=false,go_right=false;

public:
    Player(std::vector<move_callback>* movers,std::vector<render_callback>* render_objects,std::vector<event_handle_callback>* event_listeners,control_type_enum control_type,Renderer *renderer);
};



#endif
