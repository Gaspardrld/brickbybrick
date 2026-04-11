#ifndef GAME_H
#define GAME_H

#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "message.h"
#include <vector>
#include <string>
#include <sstream>
#include <memory>


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
        bool verif_score(std::istringstream& iss);
        bool verif_lives(std::istringstream& iss);
        bool verif_paddle(std::istringstream& iss);
        bool verif_nb_bricks(std::istringstream& iss);
        bool verif_brick(std::istringstream& iss);
        bool verif_nb_balls(std::istringstream& iss);
        bool verif_balls(std::istringstream& iss);

        int total_score;
        int nb_lives;
        int nb_bricks;
        int nb_balls;

        Paddle paddle;        
        std::vector<Ball> balls;     
        std::vector<std::unique_ptr<Brick>> bricks;
        //polymorphisme pour éviter les problèmes de slicing

        int nb_bricks_read;
        int nb_balls_read;
        State current_state;


    public:
        Game();
        ~Game(); // pour éviter les problèmes de memory leak avec 
                // les pointeurs de type Brick* dans Game
        void reset();
        bool read(const char* file_name);
        int get_score() const;
        int get_nb_lives() const;
        int get_nb_bricks() const;
        int get_nb_balls() const;
        const std::vector<std::unique_ptr<Brick>>& get_bricks() const;
        const Paddle& get_paddle() const;
        const std::vector<Ball>& get_balls() const;
        void move_paddle(double x) { paddle.move(x); }
};

#endif