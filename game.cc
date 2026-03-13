#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "game.h"

using namespace std;

bool game :: decode_line(const string& line) {

    istringstream iss(line);
    
    switch (current_state) {

        case EXPECT_SCORE: {
            current_state = EXPECT_LIVES;
            if (!(iss >> total_score)) { 
                return false;
            }
            if (total_score < 0) {
                cout << message::invalid_score(total_score);
                return false;
            }
            break;
        }
        

        case EXPECT_LIVES: {
            current_state = EXPECT_PADDLE;
            if (!(iss >> nb_lives)) {
                return false;
            }
            if (nb_lives < 0) {
                cout << message::invalid_lives(nb_lives);
                return false;
            }
            break;
        }


        case EXPECT_PADDLE: {
            current_state = EXPECT_NB_BRICKS;
            double x, y, radius;
            if (!(iss >> x >> y >> radius)) {
                return false;
            }
            paddle = Paddle(x, y, radius);
            
            if (!paddle.validate_paddle()) return false;
            break;
        }


        case EXPECT_NB_BRICKS: {
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
            break;
        }


        case EXPECT_BRICKS: {
            int type;
            double x, y, side;
            if (!(iss >> type >> x >> y >> side)) {
                return false;
            }
            Brick* new_brick = nullptr;
            if (type == 0) {
                int hit_points;
                if (!(iss >> hit_points)) {
                return false;
            }
                new_brick = new Rainbow_Brick(x, y, side, hit_points);
            } else if (type == 1) {
                new_brick = new Ball_Brick(x, y, side);
            } else if (type == 2) {
                new_brick = new Split_Brick(x, y, side);
            } else {
                cout << message::invalid_brick_type(type);
                return false;
            } 

            if (!new_brick->valid_Brick()){
                delete new_brick;
                return false;
            }

            for (size_t i = 0; i <bricks.size(); ++i) {
                if (squares_intersect(new_brick->get_form(), bricks[i]->get_form(), false)) {
                    cout << message::collision_bricks(i, nb_bricks_read);
                    delete new_brick;
                    return false;
                }
            }           

                if (circle_square_intersect(paddle.get_circle(), new_brick->get_form())) {
                cout << message::collision_paddle_brick(nb_bricks_read);
                delete new_brick;
                return false;
            }
            
            bricks.push_back(new_brick);
            nb_bricks_read++; //incrémentation du nombre de briques lues

            if (nb_bricks_read == nb_bricks) {
                current_state = EXPECT_NB_BALLS;
            }
            break;
        }


        case EXPECT_NB_BALLS: {
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
            break;
        }


        case EXPECT_BALLS: {
            double x, y, radius, delta_x, delta_y;
            if (!(iss >> x >> y >> radius >> delta_x >> delta_y)) {
                return false;
            }
            Ball ball(x, y, radius, delta_x, delta_y);
            if (!ball.valid_Ball()) {
                return false;
            }

            for (size_t i = 0; i < bricks.size(); ++i) {
                if (circle_square_intersect(ball.get_circle(), bricks[i]->get_form())) { // vérification de la collision avec les briques déjà lues
                    cout << message::collision_ball_brick(nb_balls_read, i);
                    return false;
                }
            }
            
            for (size_t i = 0; i < balls.size(); ++i) {
                if (circles_intersect(ball.get_circle(), balls[i].get_circle())) { // vérification de la collision avec les autres balles déjà lues
                    cout << message::collision_balls(nb_balls_read, i);
                    return false;
                }
            }
            
            if (circles_intersect(paddle.get_circle(), ball.get_circle())) { // vérification de la collision avec la raquette
                cout << message::collision_paddle_ball(nb_balls_read);
                return false;
            }

            balls.push_back(ball);
            nb_balls_read++;
            if (nb_balls_read == nb_balls) {
                current_state = FINISH;
            }
            break;
        }

        case FINISH: {
            return true; // si on reçoit une ligne alors que le fichier est déjà complet, c'est une erreur
        }
    }
    return true;
}



void game :: reset() {
    total_score = 0;
    nb_lives = 0;
    current_state = EXPECT_SCORE;
    nb_bricks_read = 0;
    nb_balls_read = 0;

    for (auto brick : bricks) { 
        delete brick;
    }

    bricks.clear();
    balls.clear();   
}

bool game :: read(const char* file_name) {
    reset(); 
    ifstream file(file_name);
    if (file.fail()) {
        return false;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string first_word;

        if (!(iss >> first_word) || first_word[0] == '#') { //ignore les lignes vides ou les commentaires
            continue;
        }
        if (!decode_line(line)) { // si il y a une erreur de lecture ou de validation, on affiche le message d'erreur correspondant et on reset le jeu
            reset();
            return false;
        }
    }
    file.close();
    if (current_state != FINISH) { //vérification que le fichier est complet et que tous les éléments attendus ont été lus
        reset();
        return false;
    }  
    cout << message::success();  
    return true;
}
