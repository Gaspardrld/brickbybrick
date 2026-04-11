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

const Square& Brick::get_form() const {
    return form;
}

int Rainbow_Brick::get_hit_points() const {
    return hit_points;
}

void Ball_Brick::draw(const Cairo::RefPtr<Cairo::Context>& cr) const {
    set_color(RED);
    cr->rectangle(form.center.x - form.side/2,
                  form.center.y - form.side/2,
                  form.side, form.side);
    cr->fill_preserve();
    cr->stroke();

    set_color(BLACK);
    cr->arc(form.center.x, form.center.y, new_ball_radius, 0, 2 * M_PI);
    cr->fill();
}

void Rainbow_Brick::draw(const Cairo::RefPtr<Cairo::Context>& cr) const {
    switch (hit_points) {
        case 1: set_color(RED);    break;
        case 2: set_color(ORANGE); break;
        case 3: set_color(YELLOW); break;
        case 4: set_color(GREEN);  break;
        case 5: set_color(CYAN);   break;
        case 6: set_color(BLUE);   break;
        case 7: set_color(PURPLE); break;
    }
    cr->rectangle(form.center.x - form.side/2,
                  form.center.y - form.side/2,
                  form.side, form.side);
    cr->fill_preserve();
    cr->stroke();
}

void Split_Brick::draw(const Cairo::RefPtr<Cairo::Context>& cr) const {
    // carré principal rouge
    set_color(RED);
    cr->rectangle(form.center.x - form.side/2,
                  form.center.y - form.side/2,
                  form.side, form.side);
    cr->fill_preserve();
    cr->stroke();

    // 4 coins oranges
    double small_side = (form.side - split_brick_gap) / 2;
    if (small_side < brick_size_min) return;

    double offset = split_brick_gap/2 + small_side/2;
    set_color(ORANGE);
    
    double centers[4][2] = {
        {form.center.x - offset, form.center.y + offset},
        {form.center.x + offset, form.center.y + offset},
        {form.center.x - offset, form.center.y - offset},
        {form.center.x + offset, form.center.y - offset},
    };
    
    for (auto& c : centers) {
        cr->rectangle(c[0] - small_side/2, c[1] - small_side/2, small_side, small_side);
        cr->fill_preserve();
        cr->stroke();
    }
}