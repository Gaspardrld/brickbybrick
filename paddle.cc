// paddle.cc
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#include <iostream>
#include "paddle.h"
#include "message.h"

void Paddle::set_theta(double new_theta) {
    theta = new_theta;
}

void Paddle::set_x(double x) {
    form.center.x = x;
}

Circle& Paddle::get_circle() {
    return form;
}

const Circle& Paddle::get_circle() const {
    return form;
}

bool Paddle::validate_paddle() const {
    if (form.radius <= 0.0) {
        return false;
    }

    bool out_x = form.center.x < 0 or form.center.x > arena_size;
    bool out_y = form.center.y > 0 or form.center.y + form.radius <= 0;
    bool out_arc = !is_circle_arc_in_bounds(form.center.x, form.center.y,
                                            form.radius, arena_size, false);

    if (out_x or out_y or out_arc) {
        std::cout << message::paddle_outside(form.center.x, form.center.y);
        return false;
    }
    return true;
}

void Paddle::draw() const {
    form.draw(BLACK, false, -theta, M_PI + theta);
}

void Paddle::move() {    
    // limiter la vitesse
    double delta = target - form.center.x;
    if (abs(delta) > delta_norm_max) {
        delta = (delta > 0) ? delta_norm_max : -delta_norm_max;
    }
    double new_x = form.center.x + delta;

    form.center.x = new_x;
    last_delta.x = delta;  
}

void Paddle::set_target(double x) {
    target = x;
}

void Paddle::clamp_to_arena() {
    double half_width = form.radius * cos(theta);
    form.center.x = std::max(half_width + epsil_zero,
                             std::min(arena_size - half_width - epsil_zero,
                                      form.center.x));
}