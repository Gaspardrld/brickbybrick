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
    double target;
    double theta;
public :
    Paddle() = default; 
    Paddle(double x, double y, double radius)
        : form({{x,y}, radius}), theta(asin(y / radius)) {}
    void draw() const;
    Circle& get_circle();
    double get_theta() const { return theta; }
    void set_theta(double new_theta);
    bool validate_paddle() const;
    void move();
    void set_target(double x);
};

#endif