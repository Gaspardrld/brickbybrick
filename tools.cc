#include "tools.h"
#include <algorithm>  // std::min, std::max
#include <cmath>      // std::sqrt, std::abs


double Norm(Point v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

double Norm_Squared(Point v) {
    return v.x * v.x + v.y * v.y;
}

double Dot_Product(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

double Distance(Point a, Point b) {
    return Norm({a.x - b.x, a.y - b.y});
}


Point closest_point_on_square(Point p, const Square& s) {
    double half = s.side/2.0;
    double closest_x = std::max(s.center.x - half,std::min(p.x, s.center.x + half));
    double closest_y = std::max(s.center.y - half,std::min(p.y, s.center.y + half));
    return {closest_x, closest_y};
}

bool point_in_square(Point p, const Square& s, bool use_tolerance) {
    double tol= use_tolerance ? epsil_zero : 0.0;
    double half = s.side/2.0;

    bool in_x = p.x >= s.center.x - half - tol and p.x <= s.center.x + half + tol;
    bool in_y = p.y >= s.center.y - half - tol and p.y <= s.center.y + half + tol;

    return in_x and in_y;
}

bool is_circle_arc_in_bounds(double x_center, double y_center, double radius, double x_max , bool use_tolerance) {
    double tol = use_tolerance ? epsil_zero : 0.0;
    double radicand = radius * radius - y_center * y_center;
    if (radicand < 0) return false;

    double L = sqrt(radicand);
    double x1 = x_center - L;
    double x2 = x_center + L;

    return (x1 >= 0 and x1 < x_max-tol and x2 >= 0 and x2 < x_max-tol);
}

bool circles_intersect(const Circle& c1, const Circle& c2, bool use_tolerance) {
    double tol = use_tolerance ? epsil_zero : 0.0;
    double dist = Distance(c1.center, c2.center);

    return dist<(c1.radius + c2.radius + tol);
}

bool circle_square_intersect(const Circle& c, const Square& s, bool use_tolerance) {
    double tol = use_tolerance ? epsil_zero : 0.0;
    double radius_with_tol = c.radius + tol;
    Point closest = closest_point_on_square(c.center, s);
    Point diff = {c.center.x - closest.x, c.center.y - closest.y};

    return Norm_Squared(diff)<(radius_with_tol * radius_with_tol);
}

bool squares_intersect(const Square& s1, const Square& s2, bool use_tolerance) {
    double tol = use_tolerance ? epsil_zero : 0.0;
    double half1 = s1.side/2.0;
    double half2 = s2.side/2.0;

    bool sep_x = s1.center.x + half1 + tol < s2.center.x - half2 or s2.center.x + half2 + tol < s1.center.x - half1;
    bool sep_y = s1.center.y + half1 + tol < s2.center.y - half2 or s2.center.y + half2 + tol < s1.center.y - half1;

    return not(sep_x or sep_y);
}

bool circle_in_square(const Circle& c, const Square& arena,bool use_tolerance, bool ignore_bottom) {
    double tol  = use_tolerance ? epsil_zero : 0.0;
    double half = arena.side / 2.0;

    bool in_x   = c.center.x - c.radius >= arena.center.x - half - tol and c.center.x + c.radius <= arena.center.x + half + tol;
    bool in_top = c.center.y + c.radius <= arena.center.y + half + tol;
    bool in_bot = ignore_bottom ? true : c.center.y - c.radius >= arena.center.y - half - tol;

    return in_x and in_top and in_bot;
}

bool square_in_square(const Square& inner, const Square& outer, bool use_tolerance) {
    double half = inner.side / 2.0;
    Point  bottom_left ={inner.center.x - half, inner.center.y - half};
    Point  bottom_right={inner.center.x + half, inner.center.y - half};
    Point  top_left= {inner.center.x - half, inner.center.y + half};
    Point  top_right ={inner.center.x + half, inner.center.y + half};

    return point_in_square(bottom_left,outer, use_tolerance) and 
        point_in_square(bottom_right,outer, use_tolerance) and 
        point_in_square(top_left,outer, use_tolerance) and 
        point_in_square(top_right,outer, use_tolerance);
}