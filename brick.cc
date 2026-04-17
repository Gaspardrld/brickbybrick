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

void Ball_Brick::draw() const {
    form.draw(RED);
    ball_in_brick.draw(BLACK, true);
}

void Ball_Brick::create_ball_in_brick() {
    ball_in_brick = {{form.center.x, form.center.y}, new_ball_radius};
}

void Rainbow_Brick::draw() const {
    Color color;
    switch (hit_points) {
        case 1: color = RED;    break;
        case 2: color = ORANGE; break;
        case 3: color = YELLOW; break;
        case 4: color = GREEN;  break;
        case 5: color = CYAN;   break;
        case 6: color = BLUE;   break;
        case 7: color = PURPLE; break;
    }
    form.draw(color);
}


void Split_Brick::compute_split_points() {
    double ratio = static_cast<double>(form.side + split_brick_gap) / 
                   (brick_size_min + split_brick_gap);
    split_points = static_cast<int>(std::floor(std::log2(ratio))) + 1;
    if (split_points < 1) split_points = 1;
    if (split_points > 4) split_points = 4;
}

void Split_Brick :: draw() const {
    Color color;
    switch (split_points) {
        case 4: color = GREEN;  break;
        case 3: color = YELLOW; break;
        case 2: color = ORANGE; break;
        case 1: color = RED;    break;
    }
    form.draw(color);
    if (split_points == 4) {
       for (int i = 0; i < 4; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        double offset_x = (i - 1.5) * (form.side / 4);
                        double offset_y = (j - 1.5) * (form.side / 4);
                        draw_cross(form.center.x + offset_x, form.center.y + offset_y,
                                form.side / 8, YELLOW);
                    }
        }
    }
    if (split_points >=3) {
        double cross_size = form.side / 4;
        draw_cross(form.center.x - form.side/4, form.center.y - form.side/4, 
                                                                cross_size, ORANGE);
        draw_cross(form.center.x + form.side/4, form.center.y - form.side/4, 
                                                                cross_size, ORANGE);
        draw_cross(form.center.x - form.side/4, form.center.y + form.side/4, 
                                                                cross_size, ORANGE);
        draw_cross(form.center.x + form.side/4, form.center.y + form.side/4, 
                                                                cross_size, ORANGE);
    }
    if (split_points >= 2) {
        draw_cross(form.center.x, form.center.y, form.side/2, RED);
    }
}

int Rainbow_Brick :: get_type() const { return 0; }
int Ball_Brick :: get_type() const { return 1; }
int Split_Brick :: get_type() const { return 2; }