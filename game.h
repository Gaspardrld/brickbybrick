#ifndef GAME_H
#define GAME_H

#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "message.h"
#include <vector>
#include <string>


class Game {
    private:
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
        bool decode_line(const std::string& line);
        bool verif_score(istringstream& iss);
        bool verif_lives(istringstream& iss);
        bool verif_paddle(istringstream& iss);
        bool verif_nb_bricks(istringstream& iss);
        bool verif_brick(istringstream& iss);
        bool verif_nb_balls(istringstream& iss);
        bool verif_balls(istringstream& iss);

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