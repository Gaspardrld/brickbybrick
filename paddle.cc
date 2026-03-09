#include "paddle.h"

Circle Paddle::get_circle() const {
    return form;
}

bool Paddle::validate_paddle() const {
    if (form.radius <= 0.0) {
        return false;
    }
    if (not(point_in_square(form.center, {{arena_size/2, arena_size/2}, arena_size}, false, true))