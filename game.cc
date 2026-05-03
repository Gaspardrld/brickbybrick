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
    nb_bricks_read = 0;
    nb_balls_read = 0;

    bricks.clear();
    balls.clear(); 
    paddle = Paddle();
}

Game::ReadResult Game::read(const char* file_name) {
    std::string full_name = std::string("tests/") + file_name;
    last_file = file_name;
    reset();
    ifstream file(full_name);
    if (file.fail()) {
        return FILE_NOT_FOUND;
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
        if (brick->get_type() == 0){
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
    for (auto& ball : balls){
        ball.move();
        if (ball.get_circle().center.y < 0) {
            ball = balls.back();
            balls.pop_back();
            nb_lives--;
        }
        int nb_rebonds = 0;
        while (has_collision(ball)) {
            //ANNULATION DU DEPLACEMENT !!!!!!!!!!!
            if (nb_rebonds < nb_bounce_max) {
                // DEPLACEMENT AVEC REBOND !!!!!!!!!!!
                nb_rebonds++;
                hit_colliding_brick(ball);
                hit_colliding_ball(ball);
                hit_colliding_paddle(ball);
            } 
        }
    }
    move_paddle();    
    for (auto& ball : ball) {
        if (circles_intersect(ball.get_circle(), paddle.get_circle())){
            //DEPLACEMENT AVEC REBOND PADDLE -- pas compté dans nb_rebondd
        }
        while (has_collision(ball)) {
            // ANNULATION DU DEPLACEMENT !!!!!!!!!!!
            if (nb_rebonds < nb_bounce_max) {
                // DEPLACEMENT AVEC REBOND !!!!!!!!!!!
                nb_rebonds++;
                hit_colliding_brick(ball);
                hit_colliding_ball(ball);
                hit_colliding_paddle(ball);
            }
        }
    }
    update_entities();
    update_status();
}


void Game::move_paddle() {
    double x_previous= paddle.get_circle().center.x;
    paddle.move();
    
    // collision brique
    for (auto &brick : get_bricks()) {
        if (circle_square_intersect(paddle.get_circle(),
                                    brick->get_form())) {
            paddle.get_circle().center.x = x_previous;
            return;
        }
    }

    double r = paddle.get_circle().radius;
    double half_width = r * cos(paddle.get_theta());
    
    // clamper aux bords
    paddle.get_circle().center.x = max(half_width+epsil_zero, 
                    min(arena_size - half_width-epsil_zero, 
                        paddle.get_circle().center.x));
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
        balls.push_back(new_b);
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


bool Game::has_collision(coBall& ball) const {
    if (!circle_in_square(ball.get_circle(), arena, true, true)) return true;
    for (const auto& brick : bricks)
        if (circle_square_intersect(ball.get_circle(), brick->get_form())) {
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


void Game::hit_colliding_brick(Ball& ball) {
    for (auto& brick : bricks) {
        if (circle_square_intersect(ball.get_circle(), brick->get_form())) {
            brick->hit();
            if (brick->get_type() == 1) {
                new_ball(brick->get_ball_in_brick().center.x, 
                brick->get_ball_in_brick().center.y + ball_spawn_gap, 
                new_ball_radius, ball->get_delta().x, 
                ball->get_delta().y);
            }
            if (brick->get_type() == 2) {
                for (auto& child : brick->get_children()) {
                    bricks.push_back(std::move(child));
                }
            }
            total_score += score_per_hit;
            return;
        }
    }
}




void Game::update_entities() {
    auto i = 0;
    while (i < (int)balls.size()) {
        if (!balls[i].is_living()) {
            balls[i] = balls.back();
            balls.pop_back();
            nb_balls--;
        } else {
            i++;
        }
    }

    auto j = 0;
    while (j < (int)bricks.size()) {
        if (!bricks[j]->is_living()) {
            bricks[j] = std::move(bricks.back());
            bricks.pop_back();
            nb_bricks--;
        } else {
            j++;
        }
    }
}

void Game::update_status() {
    if (nb_lives <= 0 && nb_balls == 0) {
        status = LOST;
    } else if (nb_bricks == 0) {
        status = WON;
    } else {
        status = ONGOING;
    }
}