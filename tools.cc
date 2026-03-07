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





