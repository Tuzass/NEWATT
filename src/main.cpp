#include "../include/NEWATT/Match.hpp"
#include <cstdlib>
#include <string>

int main(void){
    Match match{};

    match.start();
    match.printStates();

    std::string input = "";
    while (true){
        if (match.getState() == Match::State::Finished){
            std::cout << "you fucking suck" << std::endl;
            break;
        }

        else if (input == "exit"){
            std::cout << "match exited successfully" << std::endl;
            break;
        }

        system("cls");

        if (input == "l")
            match.moveLeft();

        else if (input == "r")
            match.moveRight();

        else if (input == "cw")
            match.rotateCW();

        else if (input == "ccw")
            match.rotateCounterCW();

        else if (input == "h")
            match.hardDrop();
        
        else if (input == "lp")
            match.lowerPiece();

        std::cout << "current state = " << match.getState() << std::endl;
        match.printStates();

        std::cout << "l = move left\nr = move right\ncw = rotate clockwise\nccw = rotate counterclockwise\nh = hard drop\nlp = lower piece" << std::endl;
        std::cout << "enter \"exit\" to end the game" << std::endl;

        if (match.getState() == Match::State::PieceLocked){
            match.spawnNewPiece();
            input = "";
            continue;
        }

        std::cin >> input;
    }

    return 0;
}
