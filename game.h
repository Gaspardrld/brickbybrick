#ifndef GAME_H
#define GAME_H

#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "message.h"
#include <vector>
#include <string>

class game {
    private:
        bool decode_line(const std::string& line);
        double total_score = 0;
        int nb_lives = 0;
        int nb_bricks = 0;
        int nb_balls = 0;
        Paddle paddle;        
        std::vector<Ball> balls;     
        std::vector<Brick*> bricks; 

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
        State current_state = EXPECT_SCORE; // le reset() le fait déjà, mais on le met ici pour être sûr que c'est bien initialisé à la création de l'objet

    public:
        void reset();
        bool read(const char* file_name);
};

#endif