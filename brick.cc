#include <iostream>
#include "brick.h"
#include "message.h"

bool Brick::valid_brick() const {
    if (form.side < brick_size_min) {
        std::cout << message::invalid_brick_size(form.side);
        return false;
    }
    if (not(square_in_square(form, 
            {{arena_size/2, arena_size/2}, arena_size}, false))) {
        std::cout << message::brick_outside(form.center.x, form.center.y);
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