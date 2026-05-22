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
    bool living = true;
public :
    Brick(double x, double y, double side) : form({{x,y}, side}) {}
    virtual ~Brick() = default;

    const Square& get_form() const;
    virtual void draw() const = 0;
    virtual int get_type() const = 0;
    virtual void draw(Color color) const;
    virtual Circle get_ball_in_brick() const {return {{0,0}, 0};}
    virtual void hit() = 0;
    virtual std::vector<std::unique_ptr<Brick>> get_children() { return {}; }
    bool is_living() const { return living; }
    bool valid_brick() const;
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

    void hit() override;
};


class BallBrick : public Brick {
private :
    Circle ball_in_brick;
public :
    BallBrick(double x, double y, double side);

    Circle get_ball_in_brick() const override;
    void draw() const override;
    int get_type() const override;
    void create_ball_in_brick();
    void hit() override;
};


class SplitBrick : public Brick {
private :
    std::vector<std::unique_ptr<Brick>> splitBricks;
public :
    SplitBrick(double x, double y, double side);

    void draw() const override;
    void draw(Color color) const override;
    int get_type() const override;
    void hit() override;
    std::vector<std::unique_ptr<Brick>> get_children() override;
};

#endif