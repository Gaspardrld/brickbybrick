// game.h
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "message.h"


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
    enum Status { ONGOING, WON, LOST };
    bool decode_line(const std::string& line);
    bool verif_score(std::istringstream& iss);
    bool verif_lives(std::istringstream& iss);
    bool verif_paddle(std::istringstream& iss);
    bool verif_nb_bricks(std::istringstream& iss);
    bool verif_brick(std::istringstream& iss);
    std::unique_ptr<Brick> create_brick(int type, double x, double y,
                                        double side, std::istringstream& iss);
    bool verif_nb_balls(std::istringstream& iss);
    bool verif_balls(std::istringstream& iss);

    int total_score;
    int nb_lives;
    int nb_bricks;
    int nb_balls;

    Status status = ONGOING;
    Paddle paddle;
    std::vector<Ball> balls;
    std::vector<std::unique_ptr<Brick>> bricks;
    // polymorphisme pour éviter les problèmes de slicing

    int nb_bricks_read;
    int nb_balls_read;
    State current_state;

    std::string last_file;

public:
    enum ReadResult { OK, FILE_NOT_FOUND, INVALID_CONTENT };

    Game();
    ~Game();
    void reset();
    ReadResult read(const char* file_name);
    bool save(const std::string&);
    int get_score() const;
    int get_nb_lives() const;
    int get_nb_bricks() const;
    int get_nb_balls() const;
    Status get_status() const { return status; }
    void step();
    bool restart();

    const std::vector<std::unique_ptr<Brick>>& get_bricks() const;
    const Paddle& get_paddle() const;
    const std::vector<Ball>& get_balls() const;

    void new_ball();
    void new_ball(double x, double y, double radius, double delta_x, double delta_y);
    void hit_colliding_brick(Ball& ball);
    void hit_colliding_ball(Ball& ball, Ball* other_ball);
    void hit_colliding_paddle(Ball& ball);
    void hit_collisions_wall(Ball& ball);
    void move_paddle();
    void set_target_paddle(double x);
    bool has_collision(const Ball& ball) const;
    void update_entities();
    void update_status();

    void lost();
    void win();
};

#endif