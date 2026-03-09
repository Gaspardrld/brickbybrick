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
    unsigned nb_lives;

    bool decode_line(const string& line) {
        istringstream iss(line);
        
        switch (current_state) {

            case EXPECT_SCORE:
                int score;
                current_state = EXPECT_LIVES;
                if (score < 0) {
                    cout << invalid_score(score);
                    return false;
                }
                break;

            case EXPECT_LIVES:
                current_state = EXPECT_PADDLE;
                int lives;
                if (lives < 0) {
                    cout << invalid_lives(lives);
                    return false;
                }
                break;

            case EXPECT_PADDLE:
                current_state = EXPECT_NB_BRICKS;
                double x, y, radius;
                if (x < 0 || x > arena_size || y > 0 || y+radius <= 0) {
                    cout << paddle_outside(x, y);
                    return false;
                }
                break;

            case EXPECT_NB_BRICKS:
                current_state = EXPECT_BRICKS;
                break;

            case EXPECT_BRICKS:
                break;

            case EXPECT_NB_BALLS:
                current_state = EXPECT_BALLS;
                break;
            case EXPECT_BALLS:
                break;
        }
        return true;
    }
}

namespace game {

    void reset() {
        total_score = 0;
        nb_lives = 0;
        current_state = EXPECT_SCORE;
    }

    bool read(char* file_name) {
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
                return false;
            }
        }

        file.close();
        return true;
    }
}