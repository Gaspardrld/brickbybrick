// brick.h
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#ifndef BRICK_H
#define BRICK_H 

#include <cmath>
#include "tools.h"
#include "constants.h"
#include "graphic_gui.h"

class Brick {
protected :
    Square form;
public :
    const Square& get_form() const;
    bool valid_brick() const;
    Brick(double x, double y, double side) : form({{x,y}, side}) {}
    virtual void draw() const = 0;
    virtual ~Brick() = default;
    virtual int get_type() const = 0;
};


class RainbowBrick : public Brick {
private :
    int hit_points;
public :
    RainbowBrick(double x, double y, double side, int hp)
    : Brick(x,y,side), hit_points(hp) {}
    int get_hit_points() const;
    void draw() const override;
    int get_type() const override;
};


class BallBrick : public Brick {
private :
    Circle ball_in_brick;
public :
    BallBrick(double x, double y, double side);
    void draw() const override;
    int get_type() const override;
    void create_ball_in_brick();
};


class SplitBrick : public Brick {
private :
    std::vector<std::unique_ptr<SplitBrick>> splitBricks; 
public :
    SplitBrick(double x, double y, double side);
    void draw() const override;
    void draw(Color color) const;
    int get_type() const override;
};

#endif