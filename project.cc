#include <iostream>
#include "game.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./project fichier.txt" << endl;
        return 1;
    }
    
    if (game::read(argv[1])) {
        return 0;
    } else {
        return 1;
    }
}
