// ball.h
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#ifndef BALL_H
#define BALL_H

#include "tools.h"
#include "constants.h"
#include "graphic_gui.h"

class Ball {
private :
    Circle form;
    Point delta;
    Point pre_move_center;

    double norm_delta() const;
    bool living = true;

public :
    bool valid_ball() const;

    const Circle get_circle() const;
    Point  get_delta()  const;
    
    Ball(double x, double y, double radius, double d_x, double d_y) 
    : form({{x,y}, radius}), delta({d_x, d_y}) {}

    void draw() const;
    void move();

    bool is_living() const { return living; }
    void undo_move();
    void set_delta(Point d);
    void set_center(Point c);
};

#endif