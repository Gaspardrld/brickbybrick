#include <iostream>
#include "paddle.h"
#include "message.h"

Circle Paddle::get_circle() const {
    return form;
}

bool Paddle::validate_paddle() const {
    if (form.radius <= 0.0) {
        return false;
    }

    bool out_x = form.center.x < 0 or form.center.x > arena_size;
    bool out_y = form.center.y > 0 or form.center.y + form.radius <= 0;
    bool out_arc = !is_circle_arc_in_bounds(form.center.x, form.center.y,
                                            form.radius, arena_size, false);

    if (out_x or out_y or out_arc) {
        std::cout << message::paddle_outside(form.center.x, form.center.y);
        return false;
    }
    return true;
}

void Paddle::draw(const Cairo::RefPtr<Cairo::Context>& cr) const {
    set_color(BLACK);
    cr->arc(form.center.x, form.center.y, form.radius, 0, 2 * M_PI);
    cr->fill_preserve();
    cr->stroke();
}