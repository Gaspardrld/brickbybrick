// graphic.h
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <cmath>

enum Color
{
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    BLACK,
    GREY,
    WHITE,
};

void set_color(Color color);

void draw_circle(double x, double y, double radius, bool filled = true, 
                            double angle_start=0, double angle_end=2 * M_PI);

void draw_rectangle(double x, double y, double width, double height);

void draw_line(double x1, double y1, double x2, double y2);

#endif
