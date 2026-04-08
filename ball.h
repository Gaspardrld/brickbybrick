#ifndef BALL_H
#define BALL_H

#include "tools.h"
#include "constants.h"

class Ball {
private :
    Circle form;
    Point delta;

    double norm_delta() const; // mise en private car uniquement pour valid ball
    double norm_delta_squared() const;  // idem que norm_Delta

public :
    bool valid_ball() const;

    Circle get_circle() const;
    Point  get_delta()  const;

    Ball(double x, double y, double radius, double d_x, double d_y) 
    : form({{x,y}, radius}), delta({d_x, d_y}) {}
};

#endif