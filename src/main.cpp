#include "../include/NEWATT/Match.hpp"

int main(void){
    Match match{};

    match.generateRandomPieceSequence();
    match.spawnNewPiece();
    match.printStates();
    return 0;
}
