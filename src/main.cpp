#include "../include/NEWATT/Match.hpp"

int main(void){
    Match match{};

    match.start();
    match.printStates();

    while (match.getState() != Match::State::Finished){
        while (match.getState() == Match::State::Ongoing)
            match.lowerPiece();

        match.spawnNewPiece();
    }

    match.printStates();
    return 0;
}
