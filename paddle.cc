#include "paddle.h"

Circle Paddle::get_circle() const {
    return form;
}

bool Paddle::validate_paddle() const {
    if (form.radius <= 0.0) {
        return false;
    }
    if (not

    if (x < 0 || x > arena_size || y > 0 || y+radius <= 0 || radius <= 0 || !is_circle_arc_in_bounds(x, y, radius, arena_size)) {
                    cout << paddle_outside(x, y);
                    return false;
                }    