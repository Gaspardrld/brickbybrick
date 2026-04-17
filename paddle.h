#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"
#include "constants.h"
#include "graphic_gui.h"


class Paddle {
private :
    Circle form;
public :
    Paddle() = default; 
    Paddle(double x, double y, double radius) : form({{x,y}, radius}) {}
    void draw() const;
    const Circle get_circle() const;
    bool validate_paddle() const;
    void move(double x);
};

#endif