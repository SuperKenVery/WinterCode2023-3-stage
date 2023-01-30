#ifndef BloodBar_hpp
#define BloodBar_hpp

#include "base.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace SDL2pp;
using namespace std;

class BloodBar{
    Renderer *renderer;
    Color *color;
    Rect *position;
    double *blood;

    // Callbacks
    render_callback rendercb;
    // vector<callback>s
    vector<render_callback *> *render_objects;

  public:
    BloodBar(Renderer *renderer, vector<render_callback *> *render_objects,Color *color, Rect *position, double *blood);
    ~BloodBar();
};

#endif
