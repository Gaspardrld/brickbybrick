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
    virtual void hit(Point delta) = 0;
    virtual std::vector<std::unique_ptr<Brick>> get_children() const { return {}; }

    bool valid_brick() const;
    bool is_living() const;
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

    void hit(Point delta) override;
};


class BallBrick : public Brick {
private :
    Circle ball_in_brick;
public :
    BallBrick(double x, double y, double side);

    Circle get_ball_in_brick() const;
    void draw() const override;
    int get_type() const override;
    void create_ball_in_brick();
    void hit(Point delta) override;
};


class SplitBrick : public Brick {
private :
    std::vector<std::unique_ptr<SplitBrick>> splitBricks; 
    Point last_delta;
public :
    SplitBrick(double x, double y, double side);

    void draw() const override;
    void set_delta(Point delta);
    void get_delta(Point delta) const;
    void draw(Color color) const;
    int get_type() const override;
    void hit(Point delta) override;
    std::vector<std::unique_ptr<Brick>> get_children() const override;
};

#endif