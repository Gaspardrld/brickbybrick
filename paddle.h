#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"
#include "constants.h"
#include "graphic_gui.h"


class Paddle {
private :
    Circle form;
    double target;
public :
    Paddle() = default; 
    Paddle(double x, double y, double radius) : form({{x,y}, radius}) {}
    void draw() const;
    Circle& get_circle();
    bool validate_paddle() const;
    void move();
    void set_target(double x);
};

#endif