#ifndef BRICK_H
#define BRICK_H 

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
    virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr) const = 0; // méthode virtuelle pure 
    //pour forcer les classes dérivées à implémenter leur propre méthode de dessin
    virtual ~Brick() = default; // pour éviter les problèmes de memory leak 
                                // avec les pointeurs de type Brick* dans Game
    virtual int get_type() const = 0;
};


class Rainbow_Brick : public Brick {
private :
    int hit_points;
public :
    Rainbow_Brick(double x, double y, double side, int hp)
    : Brick(x,y,side), hit_points(hp) {}
    int get_hit_points() const;
    void draw(const Cairo::RefPtr<Cairo::Context>& cr) const;
    int get_type() const override;
};


class Ball_Brick : public Brick {
public :
    Ball_Brick(double x, double y, double side)
    : Brick(x,y,side) {}
    void draw(const Cairo::RefPtr<Cairo::Context>& cr) const;
    int get_type() const override;
};

class Split_Brick : public Brick {
public :
    Split_Brick(double x, double y, double side)
    : Brick(x,y,side) {}
    void draw(const Cairo::RefPtr<Cairo::Context>& cr) const;
    int get_type() const override;
};

#endif