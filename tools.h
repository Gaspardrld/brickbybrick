#ifndef TOOLS_H
#define TOOLS_H

#include <cmath>
#include "graphic.h"


constexpr double epsil_zero = 0.125;


struct Point {
    double x = 0.0;
    double y = 0.0;
};

struct Circle {
    Point  center;
    double radius = 0.0;
    void draw(Color color, bool filled = true) const;
};

struct Square {
    Point  center;
    double side   = 0.0;
    void draw(Color color) const;
};


double norm(Point v);

double norm_squared(Point v);

double dot_product(Point a, Point b);

double distance(Point a, Point b);

bool is_circle_arc_in_bounds(double x_center, double y_center, 
                    double radius, double x_max , bool use_tolerance = true);

bool circles_intersect(const Circle& c1, const Circle& c2, 
                    bool use_tolerance = true);

bool circle_square_intersect(const Circle& c, const Square& s,
                    bool use_tolerance = true);

bool squares_intersect(const Square& s1, const Square& s2, 
                    bool use_tolerance = true);

bool point_in_square(Point p, const Square& s,bool use_tolerance = true);

bool circle_in_square(const Circle& c, const Square& arena,
                        bool use_tolerance = true, bool ignore_bottom = false);

bool square_in_square(const Square& inner, const Square& outer,
                                            bool use_tolerance = true);

Point closest_point_on_square(Point p, const Square& s);

void draw_cross(double x, double y, double size, Color color);

#endif