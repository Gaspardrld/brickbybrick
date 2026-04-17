#include "graphic.h"
#include "graphic_gui.h"
#include "constants.h"

using namespace std;

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}

// local functions
void set_color(Color color)
{
    double r(0.), g(0.), b(0.);

    switch (color)
    {
    case RED:
        r = 1.0;
        break;
    case ORANGE:
        r = 1.0;
        g = 0.5;
        break;
    case YELLOW:
        r = 1.0;
        g = 1.0;
        break;
    case GREEN:
        g = 1.0;
        break;
    case CYAN:
        g = 1.0;
        b = 1.0;
        break;
    case BLUE:
        b = 1.0;
        break;
    case PURPLE:
        r = 0.5;
        b = 1.0;
        break;
    case BLACK:
        r = g = b = 0.0;
        break;
    case GREY:
        r = g = b = 0.5;
        break;
    case WHITE:
        r = g = b = 1.0;
        break;
    default:
        break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}

void draw_circle(double x, double y, double radius,bool filled) {
    (*ptcr)->arc(x, y, radius, 0, 2 * M_PI);
    if (filled) {
        (*ptcr)->fill();
    } else {
        (*ptcr)->set_line_width(line_width_default);
        (*ptcr)->stroke();
    }
}

void draw_rectangle(double x, double y, double width, double height)
{
    (*ptcr)->rectangle(x, y, width, height);
    (*ptcr)->fill();
}

void draw_line(double x1, double y1, double x2, double y2)
{
    (*ptcr)->set_line_width(split_brick_gap);
    (*ptcr)->move_to(x1, y1);
    (*ptcr)->line_to(x2, y2);
    (*ptcr)->stroke();
}
