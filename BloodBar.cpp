#include "BloodBar.hpp"

BloodBar::BloodBar(Renderer *renderer,
                   vector<render_callback *> *render_objects, Color *color,
                   Rect *position, double *blood) {
    // Initialize some variables
    this->renderer = renderer;
    this->color = color;
    this->render_objects = render_objects;
    this->position = position;
    this->blood = blood;

    // Render callback
    this->rendercb = [=]() -> void {
        Rect fill_rect =
            Rect(this->position->x, this->position->y,
                 this->position->w * (*(this->blood)), this->position->h);
        if (fill_rect.w < 0)
            fill_rect.w = 0;
        this->renderer->SetDrawColor(*this->color);
        this->renderer->FillRect(fill_rect);
    };
    render_objects->push_back(&this->rendercb);
}

BloodBar::~BloodBar() {
    // Remove itself from render callback chain
    REMOVEFROMONCE(&this->rendercb, this->render_objects)

    // renderer is shared
    delete this->color;
    delete this->position;
    // render_objects is managed by other code
}
