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
    if (form.center.x < 0 or form.center.x > arena_size or form.center.y > 0 or form.center.y+form.radius <= 0 or !is_circle_arc_in_bounds(form.center.x, form.center.y, form.radius, arena_size, false)) {
        std::cout << message::paddle_outside(form.center.x, form.center.y);
        return false;
    }
    return true;
}