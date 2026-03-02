#include "tools.h" // epsil_zero et enum Color de graphic.h

constexpr double arena_size = 100.0;
constexpr double new_ball_radius = 1.0;
constexpr double new_ball_delta_norm = 0.8;
constexpr double delta_norm_max = 3.0;
constexpr double split_brick_gap = 3.0;
constexpr double brick_size_min = 3.0;
constexpr unsigned nb_bounce_max = 5; // limite pour eviter les boucles infinies
constexpr unsigned score_per_hit = 10;
constexpr unsigned score_per_life = 300;
constexpr unsigned dt = 25; // intervalle de temps entre mise à jour en millisecondes