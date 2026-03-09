#include "brick.h"

bool Brick::valid_Brick() const {
    if (form.side < brick_size_min) {
        return false;
    }
    if (not(square_in_square(form, {{arena_size/2, arena_size/2}, arena_size}, false))) {
        return false;
    }
    return true;
}

Square Brick::get_form() const {
    return form;
}

int Rainbow_Brick::get_hit_points() const {
    return hit_points;
}