// paddle.h
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"
#include "constants.h"
#include "graphic_gui.h"


class Paddle {
private :
    Circle form;
    double target = arena_size/2.0;
    double theta;
    Point last_delta = {0.0, 0.0};
public :
    Paddle() = default; 
    Paddle(double x, double y, double radius)
        : form({{x,y}, radius}), theta(asin(y / radius)) {}
    void draw() const;
    Circle& get_circle();
    const Circle& get_circle() const;
    double get_theta() const { return theta; }
    void set_theta(double new_theta);
    bool validate_paddle() const;
    void move();
    void set_target(double x);
    Point get_last_delta() const { return last_delta; }
    void set_last_delta(Point d);
    void set_x(double x);
    void clamp_to_arena();
};

#endif