#include "tools.h"
#include <algorithm>  // std::min, std::max
#include <cmath>      // std::sqrt, std::abs




double Norme(Point v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double Norme_Au_Carre(Point v) {
    return v.x * v.x + v.y * v.y;
}

double Produit_Scalaire(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

double Distance(Point a, Point b) {
    return Norme({a.x - b.x, a.y - b.y});
}

bool circles_intersect(const Circle& c1, const Circle& c2, bool use_tolerance = true){
    double tol = use_tolerance ? epsil_zero : 0.0;
    if (Distance(c1.center , c2.center) < (c1.radius + c2.radius + tol)){
        return true;
    }    
    else {
        return false;
    }
}

bool circle_square_intersect(const Circle& c, const Square& s, bool use_tolerance = true) {
    double tol = use_tolerance ? epsil_zero : 0.0;

    double half  = s.side / 2.0;
    double x_min = s.center.x - half;
    double x_max = s.center.x + half;
    double y_min = s.center.y - half;
    double y_max = s.center.y + half;

    double closest_x = std::max(x_min, std::min(c.center.x, x_max));
    double closest_y = std::max(y_min, std::min(c.center.y, y_max));

    double dx = c.center.x - closest_x;
    double dy = c.center.y - closest_y;

    double radius_with_tol = c.radius + tol;
    return (dx * dx + dy * dy) < (radius_with_tol * radius_with_tol);
}

Point closest_point_on_square(Point p, const Square& s) {
    double half  = s.side / 2.0;
    double x_min = s.center.x - half;
    double x_max = s.center.x + half;
    double y_min = s.center.y - half;
    double y_max = s.center.y + half;

    double closest_x = std::max(x_min, std::min(p.x, x_max));
    double closest_y = std::max(y_min, std::min(p.y, y_max));

    return {closest_x, closest_y};
}

bool point_in_square(Point p, const Square& s, bool use_tolerance = true) {
    double tol   = use_tolerance ? epsil_zero : 0.0;
    double half  = s.side / 2.0;

    return p.x >= s.center.x - half - tol && p.x <= s.center.x + half + tol && p.y >= s.center.y - half - tol && p.y <= s.center.y + half + tol;
}

bool squares_intersect(const Square& s1, const Square& s2, bool use_tolerance = true) {
    double tol    = use_tolerance ? epsil_zero : 0.0;
    double half1  = s1.side / 2.0;
    double half2  = s2.side / 2.0;

    return !(s1.center.x + half1 + tol < s2.center.x - half2 || s2.center.x + half2 + tol < s1.center.x - half1 || s1.center.y + half1 + tol < s2.center.y - half2 || s2.center.y + half2 + tol < s1.center.y - half1);
}

bool circle_in_square(const Circle& c, const Square& arena, bool use_tolerance = true, bool ignore_bottom = false) {
    double tol  = use_tolerance ? epsil_zero : 0.0;
    double half = arena.side / 2.0;

    bool in_x     = c.center.x - c.radius >= arena.center.x - half - tol && c.center.x + c.radius <= arena.center.x + half + tol;
    bool in_top   = c.center.y + c.radius <= arena.center.y + half + tol;
    bool in_bot   = ignore_bottom ? true : c.center.y - c.radius >= arena.center.y - half - tol;

    return in_x && in_top && in_bot;
}

bool square_in_square(const Square& inner, const Square& outer, bool use_tolerance = true) {
    double tol        = use_tolerance ? epsil_zero : 0.0;
    double half_in    = inner.side / 2.0;
    double half_out   = outer.side / 2.0;

    return inner.center.x - half_in >= outer.center.x - half_out - tol && inner.center.x + half_in <= outer.center.x + half_out + tol && inner.center.y - half_in >= outer.center.y - half_out - tol && inner.center.y + half_in <= outer.center.y + half_out + tol;
}





