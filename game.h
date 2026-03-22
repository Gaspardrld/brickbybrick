#ifndef GAME_H
#define GAME_H

#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "message.h"
#include <vector>
#include <string>


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

class Game {
    private:
        bool decode_line(const std::string& line);
        double total_score;
        int nb_lives;
        int nb_bricks;
        int nb_balls;
        Paddle paddle;        
        std::vector<Ball> balls;     
        std::vector<Brick*> bricks;//polymorphisme pour éviter les problèmes de slicing

        int nb_bricks_read;
        int nb_balls_read;
        State current_state;

    public:
        Game();
        void reset();
        bool read(const char* file_name);
};

#endif