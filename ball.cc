#include <iostream>
#include "ball.h"
#include "message.h"

bool Ball::valid_ball() const {
    if (form.radius <= 0.0) {
        std::cout << message::ball_outside(form.center.x, form.center.y);
        return false;
    }
    Square arena = { {arena_size/2, arena_size/2}, arena_size};
    if (not (circle_in_square(form, arena, false, true))) {
        std::cout << message::ball_outside(form.center.x, form.center.y);
        return false;
    }
    if (norm_delta() > delta_norm_max) {
        std::cout << message::invalid_delta(delta.x, delta.y);
        return false;
    }
    return true;
}

double Ball::norm_delta() const {
    return norm(delta);
}

double Ball::norm_delta_squared() const {
    return norm_squared(delta);
}

Circle Ball::get_circle() const {
    return form;
}

Point Ball::get_delta() const {
    return delta;
}

 