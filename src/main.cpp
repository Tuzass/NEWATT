#include "../include/NEWATT/Match.hpp"

int main(void){
    Match match{};

    match.start();
    match.printStates();

    match.rotateCW();
    match.printStates();

    match.rotateCounterCW();
    match.printStates();

    return 0;
}
