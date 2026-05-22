// game.cc
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "game.h"

using namespace std;

namespace {
    enum BrickType { RAINBOW = 0, BALL_BRICK = 1, SPLIT_BRICK = 2 };
    constexpr double ball_spawn_gap = 1.0; // gap minimal entre raquette et balle spawned
}

Game::Game() : 
    total_score(0), 
    nb_lives(0), 
    nb_bricks(0), 
    nb_balls(0), 
    nb_bricks_read(0), 
    nb_balls_read(0), 
    current_state(EXPECT_SCORE)
{    
}

Game::~Game() {
    reset();
}

bool Game::decode_line(const string& line) {
    istringstream iss(line);

    switch (current_state) {
        case EXPECT_SCORE:
            if (!verif_score(iss)) return false;
            current_state = EXPECT_LIVES;
            break;

        case EXPECT_LIVES:
            if (!verif_lives(iss)) return false;
            current_state = EXPECT_PADDLE;
            break;

        case EXPECT_PADDLE:
            if (!verif_paddle(iss)) return false;
            current_state = EXPECT_NB_BRICKS;
            break;

        case EXPECT_NB_BRICKS:
            if (!verif_nb_bricks(iss)) return false;
            break;

        case EXPECT_BRICKS:
            if (!verif_brick(iss)) return false;
            break;

        case EXPECT_NB_BALLS:
            if (!verif_nb_balls(iss)) return false;
            break;

        case EXPECT_BALLS:
            if (!verif_balls(iss)) return false;
            break;

        case FINISH:
            return true;
    }
    return true;
}



void Game::reset() {
    total_score = 0;
    nb_lives = 0;
    current_state = EXPECT_SCORE;
    status = ONGOING;
    nb_bricks_read = 0;
    nb_balls_read = 0;

    bricks.clear();
    balls.clear();
    pending_balls.clear();
    paddle = Paddle();
}

Game::ReadResult Game::read(const char* file_name) {
    last_file = file_name;
    reset();
    ifstream file(file_name);
    if (!file) {
        std::string alt = std::string("tests/") + file_name;
        file.open(alt);
        if (!file) return FILE_NOT_FOUND;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string first_word;

        // ignorer les lignes vides et commentaires
        if (!(iss >> first_word) || first_word[0] == '#') {
            continue;
        }
        if (!decode_line(line)) {
            reset();
            return INVALID_CONTENT;
        }
    }
    file.close();
    // vérification que le fichier est complet
    if (current_state != FINISH) {
        reset();
        return INVALID_CONTENT;
    }
    cout << message::success();
    return OK;
}

bool Game::save(const std::string& file_name ) {
    ofstream file(file_name);
    if (!file) {
        return false;
    }
    file << total_score << "\n";
    file << nb_lives << "\n";
    file << paddle.get_circle().center.x << " "<< paddle.get_circle().center.y << " "
         << paddle.get_circle().radius << "\n";
    file << bricks.size() << "\n";
    for (const auto& brick : bricks){
        if (brick->get_type() == RAINBOW){
            file << brick->get_type() << " "<< brick->get_form().center.x 
                 << " "<< brick->get_form().center.y << " "<< brick->get_form().side 
                 << " "
                 << static_cast<const RainbowBrick*>(brick.get())->get_hit_points() 
                 << "\n";
        }
        else {
            file << brick->get_type() << " "<< brick->get_form().center.x 
             << " "<< brick->get_form().center.y << " "<< brick->get_form().side 
             << "\n";
        }
    }
    file << balls.size() << "\n";
    for (const auto& ball : balls){
        file << ball.get_circle().center.x << " " << ball.get_circle().center.y << " "
             << ball.get_circle().radius << " " << ball.get_delta().x << " "
             << ball.get_delta().y << "\n";
    }
    return true;
}

int Game::get_score() const {
    return total_score;
}

int Game::get_nb_lives() const {
    return nb_lives;
}

int Game::get_nb_bricks() const {
    return bricks.size();
}

int Game::get_nb_balls() const {
    return balls.size();
}



void Game::step() {
    if (status != ONGOING) return;
    size_t n = balls.size();
    for (size_t i = 0; i < n; ) {
        balls[i].move();
        if (balls[i].get_circle().center.y < 0) {
            --n;
            if (i < n) balls[i] = std::move(balls.back());
            balls.pop_back();
            if (i >= n) break;
            continue;
        }
        unsigned int nb_rebonds = 0;
        while (has_collision(balls[i])) {
            if (nb_rebonds < nb_bounce_max) {
                nb_rebonds++;
                check_types_collisions(balls[i]);
                balls[i].undo_move();
                balls[i].move();
            } else {
                break;
            }
        }
        for (const auto& brick : bricks) {
            if (!brick->is_living() || !circle_square_intersect(balls[i].get_circle(), brick->get_form())) continue;
            Point c = balls[i].get_circle().center;
            double r = balls[i].get_circle().radius;
            double half = brick->get_form().side / 2.0;
            double ox = (half + r) - std::abs(c.x - brick->get_form().center.x);
            double oy = (half + r) - std::abs(c.y - brick->get_form().center.y);
            if (ox < oy) balls[i].set_center({c.x + (c.x < brick->get_form().center.x ? -ox : ox), c.y});
            else         balls[i].set_center({c.x, c.y + (c.y < brick->get_form().center.y ? -oy : oy)});
            break;
        }
        ++i;
    }
    move_paddle();
    n = balls.size();
    for (size_t i = 0; i < n; ++i) {
        unsigned int nb_rebonds = 0;
        while (has_collision(balls[i])) {
            if (nb_rebonds < nb_bounce_max) {
                nb_rebonds++;
                check_types_collisions(balls[i]);
                balls[i].undo_move();
                balls[i].move();
            } else {
                break;
            }
        }
        for (const auto& brick : bricks) {
            if (!brick->is_living() || !circle_square_intersect(balls[i].get_circle(), brick->get_form())) continue;
            Point c = balls[i].get_circle().center;
            double r = balls[i].get_circle().radius;
            double half = brick->get_form().side / 2.0;
            double ox = (half + r) - std::abs(c.x - brick->get_form().center.x);
            double oy = (half + r) - std::abs(c.y - brick->get_form().center.y);
            if (ox < oy) balls[i].set_center({c.x + (c.x < brick->get_form().center.x ? -ox : ox), c.y});
            else         balls[i].set_center({c.x, c.y + (c.y < brick->get_form().center.y ? -oy : oy)});
            break;
        }
    }
    for (auto& b : pending_balls) balls.push_back(std::move(b));
    pending_balls.clear();
    update_entities();
    update_status();
}


void Game::move_paddle() {
    double x_previous = paddle.get_circle().center.x;
    paddle.move();

    for (auto& brick : bricks) {
        if (circle_square_intersect(paddle.get_circle(), brick->get_form())) {
            paddle.set_x(x_previous);
            paddle.set_last_delta({0.0, 0.0});
            return;
        }
    }
    paddle.clamp_to_arena();
    // last_delta = déplacement RÉEL après clamp (sinon les balles voient une
    // vitesse paddle inexistante et le rebond va dans le mauvais sens)
    paddle.set_last_delta({paddle.get_circle().center.x - x_previous, 0.0});
}


bool Game::restart() {
    return read(last_file.c_str()) == OK;
}


const std::vector<std::unique_ptr<Brick>>& Game::get_bricks() const {
    return bricks;
}


const std::vector<Ball>& Game::get_balls() const {
    return balls;
}


const Paddle& Game::get_paddle() const {
    return paddle;
}


void Game::set_target_paddle(double x) {
    paddle.set_target(x);
}


void Game::new_ball(){
    double pos_x = paddle.get_circle().center.x;
    // positionné juste au dessus de la raquette
    double pos_y = paddle.get_circle().center.y
            + paddle.get_circle().radius
            + new_ball_radius + ball_spawn_gap + epsil_zero;
    Ball new_b(pos_x, pos_y, new_ball_radius, 0, new_ball_delta_norm);
    balls.push_back(new_b);
}


void Game::new_ball(double x, double y, double radius, double delta_x, double delta_y){
    Ball new_b(x, y, radius, delta_x, delta_y);
    if (new_b.valid_ball()) {
        pending_balls.push_back(new_b);
    }
}


bool Game::verif_score(istringstream& iss) {
    if (!(iss >> total_score)) { 
        return false;
    }
    if (total_score < 0) {
        cout << message::invalid_score(total_score);
        return false;
    }
    return true;
}


bool Game::verif_lives(istringstream& iss) {
    if (!(iss >> nb_lives)) {
        return false;
    }
    if (nb_lives < 0) {
        cout << message::invalid_lives(nb_lives);
        return false;
    }
    return true;
}


bool Game::verif_paddle(istringstream& iss) {
    double x, y, radius;
    if (!(iss >> x >> y >> radius)) {
        return false;
    }
    paddle = Paddle(x, y, radius);
    
    if (!paddle.validate_paddle()) return false;
    return true;
}


bool Game::verif_nb_bricks(istringstream& iss) {
    if (!(iss >> nb_bricks)) {
        return false;
    }
    if (nb_bricks < 0) {
        return false;
    }
    
    nb_bricks_read = 0;
    
    if (nb_bricks > 0) {
        current_state = EXPECT_BRICKS;
    } else {
        current_state = EXPECT_NB_BALLS;
    }
    return true;
}


std::unique_ptr<Brick> Game::create_brick(int type, double x, double y,
                                          double side, istringstream& iss) {
    switch (type) {
        case RAINBOW: {
            int hit_points;
            if (!(iss >> hit_points)) return nullptr;
            if (hit_points < 1 || hit_points > 7) {
                cout << message::invalid_hit_points(hit_points);
                return nullptr;
            }
            return std::make_unique<RainbowBrick>(x, y, side, hit_points);
        }
        case BALL_BRICK:
            return std::make_unique<BallBrick>(x, y, side);
        case SPLIT_BRICK:
            return std::make_unique<SplitBrick>(x, y, side);
        default:
            cout << message::invalid_brick_type(type);
            return nullptr;
    }
}

bool Game::verif_brick(istringstream& iss) {
    int type;
    double x, y, side;
    if (!(iss >> type >> x >> y >> side)) return false;

    auto new_brick = create_brick(type, x, y, side, iss);
    if (!new_brick || !new_brick->valid_brick()) return false;

    for (size_t i = 0; i < bricks.size(); ++i) {
        if (squares_intersect(new_brick->get_form(),
                              bricks[i]->get_form(), false)) {
            cout << message::collision_bricks(i, nb_bricks_read);
            return false;
        }
    }
    if (circle_square_intersect(paddle.get_circle(), new_brick->get_form())) {
        cout << message::collision_paddle_brick(nb_bricks_read);
        return false;
    }
    bricks.push_back(std::move(new_brick));
    ++nb_bricks_read;
    if (nb_bricks_read == nb_bricks) current_state = EXPECT_NB_BALLS;
    return true;
}


bool Game::verif_nb_balls(istringstream& iss) {
    if (!(iss >> nb_balls)) {
        return false;
    }
    if (nb_balls < 0) {
        return false;
    }
    
    nb_balls_read = 0;
    
    if (nb_balls > 0) {
        current_state = EXPECT_BALLS;
    } else {
        current_state = FINISH;
    }
    return true;
}


bool Game::verif_balls(istringstream& iss) {
    double x, y, radius, delta_x, delta_y;   
    if (!(iss >> x >> y >> radius >> delta_x >> delta_y)) {
        return false;
    }
    Ball ball(x, y, radius, delta_x, delta_y);
    if (!ball.valid_ball()) {
        return false;
    }

    for (size_t i = 0; i < bricks.size(); ++i) {
        if (circle_square_intersect(ball.get_circle(),
                                    bricks[i]->get_form())) {
            cout << message::collision_ball_brick(nb_balls_read, i);
            return false;
        }
    }

    for (size_t i = 0; i < balls.size(); ++i) {
        if (circles_intersect(ball.get_circle(), balls[i].get_circle())) {
            cout << message::collision_balls(nb_balls_read, i);
            return false;
        }
    }

    if (circles_intersect(paddle.get_circle(), ball.get_circle())) {
        cout << message::collision_paddle_ball(nb_balls_read);
        return false;
    }

    balls.push_back(ball);
    nb_balls_read++;
    if (nb_balls_read == nb_balls) {
        current_state = FINISH;
    }
    return true;
}


bool Game::has_collision(const Ball& ball) const {
    if (!circle_in_square(ball.get_circle(), arena, true, true)) return true;
    for (const auto& brick : bricks)
        if (brick->is_living() && circle_square_intersect(ball.get_circle(), 
                                                            brick->get_form())) {
             return true;
        }
    for (auto& other : balls)
        if (&other != &ball && circles_intersect(ball.get_circle(), 
                                                        other.get_circle())) {
            return true;
        }
    if (circles_intersect(ball.get_circle(), paddle.get_circle())) return true;
    return false;
}

void Game::check_types_collisions(Ball& ball) {
    for (auto& brick : bricks) {
        if (brick->is_living() && circle_square_intersect(ball.get_circle(),
                                                            brick->get_form())) {
            hit_colliding_brick(ball, *brick);
            return;
        }
    }
    for (auto& other : balls) {
        if (&other != &ball && 
                    circles_intersect(ball.get_circle(), other.get_circle())) {
            hit_colliding_ball(ball, other);
            return;
        }
    }
    if (circles_intersect(ball.get_circle(), paddle.get_circle())) {
        hit_colliding_paddle(ball);
        return;
    }
    hit_collisions_wall(ball);
}

void Game::hit_colliding_brick(Ball& ball, Brick& brick) {
    call_behavior(brick, ball);
    Point c = ball.get_circle().center;
    double r = ball.get_circle().radius;
    const Square& s = brick.get_form();
    double half = s.side / 2.0;
    Point d = ball.get_delta();
    double ox = (half + r) - std::abs(c.x - s.center.x);
    double oy = (half + r) - std::abs(c.y - s.center.y);
    if (ox <= 0.0 && oy <= 0.0) return;
    if (ox < oy) ball.set_delta({-d.x,  d.y});
    else         ball.set_delta({ d.x, -d.y});
}

void Game::hit_colliding_ball(Ball& ball, Ball& other_ball) {
    Point centre_ball  = ball.get_circle().center;
    Point centre_other = other_ball.get_circle().center;
    double r_ball  = ball.get_circle().radius;
    double r_other = other_ball.get_circle().radius;
    Point delta_ball  = ball.get_delta();
    Point delta_other = other_ball.get_delta();

    Point n = { centre_other.x - centre_ball.x, centre_other.y - centre_ball.y };
    double n_norm = norm(n);
    if (n_norm < epsil_zero) return;
    n.x /= n_norm;
    n.y /= n_norm;

    double v_n       = dot_product(delta_ball,  n);
    double v_other_n = dot_product(delta_other, n);
    double r2        = r_ball  * r_ball;
    double r_other2  = r_other * r_other;

    // update ball (incident)
    double impulsion_a = (-v_n + v_other_n) * 2.0 * r_other2 / (r2 + r_other2);
    Point new_delta_a = {delta_ball.x + impulsion_a * n.x,
                         delta_ball.y + impulsion_a * n.y};
    double norm_a = norm(new_delta_a);
    if (norm_a > delta_norm_max) {
        double f = delta_norm_max / norm_a;
        new_delta_a.x *= f;
        new_delta_a.y *= f;
    }
    ball.set_delta(new_delta_a);

    // symmetric update for other_ball (elastic collision conserves momentum)
    double impulsion_b = (v_n - v_other_n) * 2.0 * r2 / (r2 + r_other2);
    Point new_delta_b = {delta_other.x + impulsion_b * n.x,
                         delta_other.y + impulsion_b * n.y};
    double norm_b = norm(new_delta_b);
    if (norm_b > delta_norm_max) {
        double f = delta_norm_max / norm_b;
        new_delta_b.x *= f;
        new_delta_b.y *= f;
    }
    other_ball.set_delta(new_delta_b);

    // Eject ball away from other_ball so the while(has_collision) loop exits cleanly
    double sum_r = r_ball + r_other;
    if (n_norm < sum_r) {
        double depth = sum_r - n_norm + epsil_zero;
        ball.set_center({centre_ball.x - depth * n.x,
                         centre_ball.y - depth * n.y});
    }
}

void Game::hit_collisions_wall(Ball& ball) {
    bool vertical_check = ball.get_circle().center.y + ball.get_circle().radius > arena_size;
    bool horizontal_check = ball.get_circle().center.x - ball.get_circle().radius < 0
                or ball.get_circle().center.x + ball.get_circle().radius > arena_size;
    if (vertical_check and horizontal_check) {
        double dx = std::abs(ball.get_circle().center.x - arena_size / 2.0);
        double dy = std::abs(ball.get_circle().center.y - arena_size / 2.0);
        if (dx > dy)
            ball.set_delta({-ball.get_delta().x,  ball.get_delta().y});
        else
            ball.set_delta({ ball.get_delta().x, -ball.get_delta().y});
    } else if (vertical_check) {
        ball.set_delta({ball.get_delta().x, -ball.get_delta().y});
    } else if (horizontal_check) {
        ball.set_delta({-ball.get_delta().x, ball.get_delta().y});
    }
}

void Game::hit_colliding_paddle(Ball& ball) {
    Point centre_ball = ball.get_circle().center;
    Point centre_paddle = paddle.get_circle().center;
    Point delta_ball = ball.get_delta();
    Point delta_paddle = paddle.get_last_delta();

    Point n = { centre_paddle.x - centre_ball.x,
                centre_paddle.y - centre_ball.y };
    double n_norm = norm(n);
    if (n_norm < epsil_zero) return;
    n.x /= n_norm;
    n.y /= n_norm;

    double v_n        = dot_product(delta_ball,   n);
    double v_paddle_n = dot_product(delta_paddle, n);
    double impulsion  = 2 * (-v_n + v_paddle_n);

    Point new_delta = { delta_ball.x + impulsion * n.x,
                        delta_ball.y + impulsion * n.y };

    double new_norm = norm(new_delta);
    if (new_norm > delta_norm_max) {
        double factor = delta_norm_max / new_norm;
        new_delta.x *= factor;
        new_delta.y *= factor;
    }
    ball.set_delta(new_delta);

    // Ejection : pousser la balle au-delà du cercle paddle + epsil_zero
    // (sinon circles_intersect re-détecte une collision via sa tolérance)
    double sum_r = paddle.get_circle().radius + ball.get_circle().radius;
    if (n_norm < sum_r) {
        double depth = sum_r - n_norm + epsil_zero;
        ball.set_center({ centre_ball.x - depth * n.x,
                          centre_ball.y - depth * n.y });
    }
}


void Game::update_entities() {
    auto i = 0;
    while (i < (int)balls.size()) {
        if (!balls[i].is_living()) {
            balls[i] = std::move(balls.back());
            balls.pop_back();
        } else {
            i++;
        }
    }

    auto j = 0;
    while (j < (int)bricks.size()) {
        if (!bricks[j]->is_living()) {
            bricks[j] = std::move(bricks.back());
            bricks.pop_back();
        } else {
            j++;
        }
    }
}


void Game::update_status() {
    if (bricks.empty()) {
        win();
    } else if (nb_lives <= 0 && balls.empty()) {
        lost();
    } else {
        status = ONGOING;
    }
}


void Game::call_behavior(Brick& brick, const Ball& ball) {
    total_score += score_per_hit;
    brick.hit();
    if (brick.get_type() == BALL_BRICK) {
        new_ball(brick.get_ball_in_brick().center.x,brick.get_ball_in_brick().center.y, 
                brick.get_ball_in_brick().radius,
                ball.get_delta().x, ball.get_delta().y);
    }
    else if (brick.get_type() == SPLIT_BRICK) {
        auto children = brick.get_children();
        for (auto& child : children) {
            bricks.push_back(std::move(child));
        }
    }
}


void Game::lost() {
    status = STOPPED;
    message::lost();
}

void Game::win() {
    status = STOPPED;
    total_score += score_per_life * nb_lives;
    message::won();
}