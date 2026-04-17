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

void Split_Brick::draw() const {
    draw(RED);
}

void Split_Brick :: draw(Color color) const {
    form.draw(color);
        switch(color) {
        case RED: color= ORANGE; break; 
        case ORANGE: color= YELLOW; break;
        case YELLOW: color= GREEN;  break;
        default: color = color; break;
    }   
    for (auto& brick : splitBricks) {
        brick->draw(color);
    }
}

Split_Brick::Split_Brick(double x, double y, double side)
: Brick(x, y, side)
{
    double new_side = (side - split_brick_gap) / 2;
    if (new_side < brick_size_min) return;

    double left = x - side / 2;
    double top  = y - side / 2;

    // HG
    splitBricks.push_back(std::make_unique<Split_Brick>(
        left + new_side / 2,
        top  + new_side / 2,
        new_side
    ));

    // HD
    splitBricks.push_back(std::make_unique<Split_Brick>(
        left + new_side + split_brick_gap + new_side / 2,
        top  + new_side / 2,
        new_side
    ));

    // BG
    splitBricks.push_back(std::make_unique<Split_Brick>(
        left + new_side / 2,
        top  + new_side + split_brick_gap + new_side / 2,
        new_side
    ));

    // BD
    splitBricks.push_back(std::make_unique<Split_Brick>(
        left + new_side + split_brick_gap + new_side / 2,
        top  + new_side + split_brick_gap + new_side / 2,
        new_side
    )); 
}

int Rainbow_Brick :: get_type() const { return 0; }
int Ball_Brick :: get_type() const { return 1; }
int Split_Brick :: get_type() const { return 2; }