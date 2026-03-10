#ifndef BRICK_H
#define BRICK_H 

#include "tools.h"
#include "constante.h"

using namespace std;

class Brick {
protected :
    Square form;
public :
    Square get_form() const;
    bool valid_Brick() const;
    Brick(double x, double y, double side) : form({{x,y}, side}) {}
};


class Rainbow_Brick : public Brick {
private :
    int hit_points;
public :
    Rainbow_Brick(double x, double y, double side, int hp)
    : Brick(x,y,side), hit_points(hp) {}
    int get_hit_points() const;
};


class Ball_Brick : public Brick {
public :
    Ball_Brick(double x, double y, double side)
    : Brick(x,y,side) {}
};

class Split_Brick : public Brick {
public :
    Split_Brick(double x, double y, double side)
    : Brick(x,y,side) {}
};

#endif