#include "../include/NEWATT/Match.hpp"

int main(void){
    Match match{};

    match.start();
    match.printStates();

    while (match.getState() != Match::State::Finished){
        std::cout << "\nOutline coordinates for piece " << match.getPiecesDropped() << ": " << std::endl;
        match.printOutlineCoordinates();

        while (match.getState() == Match::State::Ongoing)
            match.lowerPiece();

        match.spawnNewPiece();
        match.printStates();
    }

    return 0;
}
