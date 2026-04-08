#ifndef BRICK_H
#define BRICK_H 

#include "tools.h"
#include "constants.h"

class Brick {
protected :
    Square form;
public :
    Square get_form() const;
    bool valid_brick() const;
    Brick(double x, double y, double side) : form({{x,y}, side}) {}
    virtual ~Brick() = default; // pour éviter les problèmes de memory leak 
                                // avec les pointeurs de type Brick* dans Game
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