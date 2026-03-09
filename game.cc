#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "game.h"
#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "message.h"

using namespace std;

namespace {
    enum State {
        EXPECT_SCORE,
        EXPECT_LIVES,
        EXPECT_PADDLE,
        EXPECT_NB_BRICKS,
        EXPECT_BRICKS,
        EXPECT_NB_BALLS,
        EXPECT_BALLS
    };

    State current_state;
    double total_score;
    int nb_lives;
    int nb_bricks;
    int nb_balls;
    int nb_bricks_read = 0;
    int nb_balls_read = 0;

    Paddle paddle;        
    vector<Ball> balls;     
    vector<Brick*> bricks;  

    bool decode_line(const string& line) {

        istringstream iss(line);
        
        switch (current_state) {

            case EXPECT_SCORE: {
                current_state = EXPECT_LIVES;
                iss >> total_score;

                if (total_score < 0) {
                    cout << invalid_score(total_score);
                    return false;
                }
                break;
            }
            
            case EXPECT_LIVES: {
                current_state = EXPECT_PADDLE;
                iss >> nb_lives;

                if (nb_lives < 0) {
                    cout << invalid_lives(nb_lives);
                    return false;
                }
                break;
            }

            case EXPECT_PADDLE: {
                current_state = EXPECT_NB_BRICKS;
                double x, y, radius;
                iss >> x >> y >> radius;
                paddle = Paddle(x, y, radius);
                
                if (!paddle.validate_paddle()) return false;
                break;
            }

            case EXPECT_NB_BRICKS: {
                iss >> nb_bricks;
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
                iss >> type >> x >> y >> side;

                Brick* new_brick = nullptr;

                if (type == 0) {
                    int hit_points;
                    iss >> hit_points;
                    new_brick = new Rainbow_Brick(x, y, side, hit_points);
                } else if (type == 1) {
                    new_brick = new Ball_Brick(x, y, side);
                } else if (type == 2) {
                    new_brick = new Split_Brick(x, y, side);
                } else {
                    cout << invalid_brick_type(type);
                    return false;
                } 

                if (!new_brick->valid_Brick()){
                    delete new_brick;
                    return false;
                }
                
                bricks.push_back(new_brick);
                nb_bricks_read++;

                if (nb_bricks_read == nb_bricks) {
                    current_state = EXPECT_NB_BALLS;
                }
                break;
            }

            case EXPECT_NB_BALLS: {
                iss >> nb_balls;
                if (nb_balls < 0) {
                    return false;
                }
                
                nb_balls_read = 0;
                
                if (nb_balls > 0) {
                    current_state = EXPECT_BALLS;
                } else {
                    current_state = EXPECT_SCORE;
                }
                break;
            }

            case EXPECT_BALLS: {
                double x, y, radius, delta_x, delta_y;
                iss >> x >> y >> radius >> delta_x >> delta_y;
                Ball ball(x, y, radius, delta_x, delta_y);
                
                if (!ball.validate_ball()) {
                    return false;
                }
                
                balls.push_back(ball);
                nb_balls_read++;
                
                if (nb_balls_read == nb_balls) {
                    current_state = EXPECT_SCORE;
                }
                break;
            }
        }
        return true;
    }
}

namespace game {

    void reset() {
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

    bool read(const char* file_name) {
        reset(); 

        ifstream file(file_name);
        if (file.fail()) {
            return false;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string first_word;

            if (!(iss >> first_word) || first_word[0] == '#') {
                continue;
            }

            if (!decode_line(line)) {
                reset();
                return false;
            }
        }

        file.close();
        return true;
    }
}