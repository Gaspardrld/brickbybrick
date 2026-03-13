#ifndef GAME_H
#define GAME_H

#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "message.h"
#include <vector>
#include <string>

using namespace std;

class game {
    private:
        bool decode_line(const std::string& line);
        double total_score;
        int nb_lives;
        int nb_bricks;
        int nb_balls;
        Paddle paddle;        
        vector<Ball> balls;     
        vector<Brick*> bricks; 

        enum State {
            EXPECT_SCORE,
            EXPECT_LIVES,
            EXPECT_PADDLE,
            EXPECT_NB_BRICKS,
            EXPECT_BRICKS,
            EXPECT_NB_BALLS,
            EXPECT_BALLS,
            FINISH,
        };
        int nb_bricks_read = 0;
        int nb_balls_read = 0;
        State current_state;

    public:
        void reset();
        bool read(const char* file_name);
};

#endif