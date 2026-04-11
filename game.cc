#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "game.h"

using namespace std;

namespace {
    enum BrickType { RAINBOW = 0, BALL_BRICK = 1, SPLIT_BRICK = 2 };
}

Game :: Game() : 
    total_score(0), 
    nb_lives(0), 
    nb_bricks(0), 
    nb_balls(0), 
    nb_bricks_read(0), 
    nb_balls_read(0), 
    current_state(EXPECT_SCORE)
{    
}

Game :: ~Game() {
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



void Game :: reset() {
    total_score = 0;
    nb_lives = 0;
    current_state = EXPECT_SCORE;
    nb_bricks_read = 0;
    nb_balls_read = 0;

    bricks.clear();
    balls.clear();   
}

bool Game :: read(const char* file_name) {
    reset(); 
    ifstream file(file_name);
    if (file.fail()) {
        return false;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string first_word;

        if (!(iss >> first_word) || first_word[0] == '#') { //ignore vides/commentaires
            continue;
        }
        if (!decode_line(line)) { //  affiche le message d'erreur correspondant + reset
            reset();
            return false;
        }
    }
    file.close();
    if (current_state != FINISH) { //vérification fichier complet et éléments lus      
        reset();
        return false;
    }  
    cout << message::success();  
    return true;
}

int Game :: get_score() const {
    return total_score;
}

int Game :: get_nb_lives() const {
    return nb_lives;
}

int Game :: get_nb_bricks() const {
    return nb_bricks;
}

int Game :: get_nb_balls() const {
    return nb_balls;
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

bool Game :: verif_score(istringstream& iss) {
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


bool Game::verif_brick(istringstream& iss) {
    int type;
    double x, y, side;
    if (!(iss >> type >> x >> y >> side)) {return false;}

    std::unique_ptr<Brick> new_brick;
    switch (type) {
        case RAINBOW: {
            int hit_points;
            if (!(iss >> hit_points)) return false;
            if (hit_points < 1 || hit_points > 7) {
                cout << message::invalid_hit_points(hit_points);
                return false;
            }
            new_brick = std::make_unique<Rainbow_Brick>(x, y, side, hit_points);
            break;
        }
        case BALL_BRICK:
            new_brick = std::make_unique<Ball_Brick>(x, y, side);
            break;
        case SPLIT_BRICK:
            new_brick = std::make_unique<Split_Brick>(x, y, side);
            break;
        default:
            cout << message::invalid_brick_type(type);
            return false;
    }
    if (!new_brick->valid_brick()){
        return false;
    }
    for (size_t i = 0; i <bricks.size(); ++i) {
        if (squares_intersect(new_brick->get_form(),
                                bricks[i]->get_form(), false)) {
            cout << message::collision_bricks(i, nb_bricks_read);
            return false;
        }
    }           
    if (circle_square_intersect(paddle.get_circle(),
                                    new_brick->get_form())) {
        cout << message::collision_paddle_brick(nb_bricks_read);
        return false;
    }
    bricks.push_back(std::move(new_brick));
    nb_bricks_read++; //incrémentation du nombre de briques lues
    if (nb_bricks_read == nb_bricks) {
        current_state = EXPECT_NB_BALLS;
    }
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

    // vérification de la collision avec les briques déjà lues
    for (size_t i = 0; i < bricks.size(); ++i) {
        if (circle_square_intersect(ball.get_circle(),
                                    bricks[i]->get_form())) { 
            cout << message::collision_ball_brick(nb_balls_read, i);
            return false;
        }
    }

    // vérification de la collision avec les autres balles déjà lues
    for (size_t i = 0; i < balls.size(); ++i) {
        if (circles_intersect(ball.get_circle(), balls[i].get_circle())) { 
            cout << message::collision_balls(nb_balls_read, i);
            return false;
        }
    }

    // vérification de la collision avec la raquette
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