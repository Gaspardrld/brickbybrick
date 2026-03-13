#include <iostream>
#include "game.h"

using namespace std;

int main(int argc, char* argv[]) {
    Game game;
    if (argc != 2) {
        return 1;
    }
    
    if (game.read(argv[1])) {
        return 0;
    } else {
        return 1;
    }
}
