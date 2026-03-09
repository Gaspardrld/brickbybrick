#include <iostream>
#include "ball.h"
#include "message.h"

bool Ball::valid_Ball() const {
    if (form.radius <= 0.0) {
        std::cout << message::ball_outside(form.center.x, form.center.y);
        return false;
    }
    Square arena = { {arena_size/2, arena_size/2}, arena_size};
    if (not (circle_in_square(form, arena, false, true))) {
        std::cout << message::ball_outside(form.center.x, form.center.y);
        return false;
    }
    if (norm_Delta() > delta_norm_max) {
        std::cout << message::invalid_delta(delta.x, delta.y);
        return false;
    }
    return true;
}

double Ball::norm_Delta() const {
    return Norme(delta);
}

double Ball::norm_Delta_Squared() const {
    return Norme_Au_Carre(delta);
}

Circle Ball::get_circle() const {
    return form;
}

Point Ball::get_delta() const {
    return delta;
}

 