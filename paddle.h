#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"
#include "constants.h"


class Paddle {
private :
    Circle form;
public :
    Paddle() = default; 
    Paddle(double x, double y, double radius) : form({{x,y}, radius}) {}
    Circle get_circle() const;
    bool validate_paddle() const;
};

#endif