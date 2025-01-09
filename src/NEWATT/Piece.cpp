#include "../include/NEWATT/PieceCoordinates.hpp"

char Pieces::getPieceLetter(Pieces::Piece::Type type){
    switch (type){
        case Piece::Type::I:
            return 'I';

        case Piece::Type::O:
            return 'O';

        case Piece::Type::T:
            return 'T';

        case Piece::Type::L:
            return 'L';

        case Piece::Type::J:
            return 'J';
        
        case Piece::Type::S:
            return 'S';
        
        case Piece::Type::Z:
            return 'Z';
        
        default:
            return 0;
    }
}

using namespace Pieces;

Piece::Piece(): type{I} {}

Piece::Piece(Type type):
    type{type}, orientation_index{}
{
    switch (this->type){
        case Type::I:
            this->orientation_coordinates = (int*)I_ORIENTATION_COORDINATES;
            this->leftmost_coordinate_indices = (int*)I_LEFTMOST_COLLISION_COORDINATE_INDICES;
            this->rightmost_coordinate_indices = (int*)I_RIGHTMOST_COLLISION_COORDINATE_INDICES;
            this->downmost_coordinate_indices = (int*)I_DOWNMOST_COLLISION_COORDINATE_INDICES;
            this->wall_kick_offsets = (int*)I_WALL_KICK_OFFSETS;
            break;

        case Type::O:
            this->orientation_coordinates = (int*)O_ORIENTATION_COORDINATES;
            this->leftmost_coordinate_indices = (int*)O_LEFTMOST_COLLISION_COORDINATE_INDICES;
            this->rightmost_coordinate_indices = (int*)O_RIGHTMOST_COLLISION_COORDINATE_INDICES;
            this->downmost_coordinate_indices = (int*)O_DOWNMOST_COLLISION_COORDINATE_INDICES;
            this->wall_kick_offsets = nullptr;
            break;

        case Type::T:
            this->orientation_coordinates = (int*)T_ORIENTATION_COORDINATES;
            this->leftmost_coordinate_indices = (int*)T_LEFTMOST_COLLISION_COORDINATE_INDICES;
            this->rightmost_coordinate_indices = (int*)T_RIGHTMOST_COLLISION_COORDINATE_INDICES;
            this->downmost_coordinate_indices = (int*)T_DOWNMOST_COLLISION_COORDINATE_INDICES;
            this->wall_kick_offsets = (int*)TLJSZ_WALL_KICK_OFFSETS;
            break;

        case Type::L:
            this->orientation_coordinates = (int*)L_ORIENTATION_COORDINATES;
            this->leftmost_coordinate_indices = (int*)L_LEFTMOST_COLLISION_COORDINATE_INDICES;
            this->rightmost_coordinate_indices = (int*)L_RIGHTMOST_COLLISION_COORDINATE_INDICES;
            this->downmost_coordinate_indices = (int*)L_DOWNMOST_COLLISION_COORDINATE_INDICES;
            this->wall_kick_offsets = (int*)TLJSZ_WALL_KICK_OFFSETS;
            break;

        case Type::J:
            this->orientation_coordinates = (int*)J_ORIENTATION_COORDINATES;
            this->leftmost_coordinate_indices = (int*)J_LEFTMOST_COLLISION_COORDINATE_INDICES;
            this->rightmost_coordinate_indices = (int*)J_RIGHTMOST_COLLISION_COORDINATE_INDICES;
            this->downmost_coordinate_indices = (int*)J_DOWNMOST_COLLISION_COORDINATE_INDICES;
            this->wall_kick_offsets = (int*)TLJSZ_WALL_KICK_OFFSETS;
            break;

        case Type::S:
            this->orientation_coordinates = (int*)S_ORIENTATION_COORDINATES;
            this->leftmost_coordinate_indices = (int*)S_LEFTMOST_COLLISION_COORDINATE_INDICES;
            this->rightmost_coordinate_indices = (int*)S_RIGHTMOST_COLLISION_COORDINATE_INDICES;
            this->downmost_coordinate_indices = (int*)S_DOWNMOST_COLLISION_COORDINATE_INDICES;
            this->wall_kick_offsets = (int*)TLJSZ_WALL_KICK_OFFSETS;
            break;

        case Type::Z:
            this->orientation_coordinates = (int*)Z_ORIENTATION_COORDINATES;
            this->leftmost_coordinate_indices = (int*)Z_LEFTMOST_COLLISION_COORDINATE_INDICES;
            this->rightmost_coordinate_indices = (int*)Z_RIGHTMOST_COLLISION_COORDINATE_INDICES;
            this->downmost_coordinate_indices = (int*)Z_DOWNMOST_COLLISION_COORDINATE_INDICES;
            this->wall_kick_offsets = (int*)TLJSZ_WALL_KICK_OFFSETS;
            break;
    }

    for (int i = 0; i < COORDINATES; i++){
        int coordinate = this->orientation_coordinates[i];
        this->coordinates[2 * i] = coordinate / 4;
        this->coordinates[2 * i + 1] = coordinate % 4 + 3;
    }
}

Piece::Type Piece::getType(){
    return this->type;
}

int Piece::getOrientationIndex(){
    return this->orientation_index;
}

void Piece::increaseOrientationIndex(){
    this->orientation_index = this->orientation_index + 1;
    if (this->orientation_index >= ORIENTATIONS) this->orientation_index -= ORIENTATIONS;
}

void Piece::decreaseOrientationIndex(){
    this->orientation_index = this->orientation_index - 1;
    if (this->orientation_index < 0) this->orientation_index += ORIENTATIONS;
}

int* Piece::getCoordinates(){
    return (int*)(this->coordinates);
}

void Piece::printCoordinates(){
    for (int i = 0; i < COORDINATES; i++)
        std::cout << "(" << this->coordinates[2 * i] << "," << this->coordinates[2 * i + 1] << ") ";
    
    std::cout << std::endl;
}

const int* Piece::getLeftmostCoordinateIndices(){
    return this->leftmost_coordinate_indices;
}

const int* Piece::getRightmostCoordinateIndices(){
    return this->rightmost_coordinate_indices;
}

const int* Piece::getDownmostCoordinateIndices(){
    return this->downmost_coordinate_indices;
}

const int* Piece::getOrientationCoordinates(){
    return this->orientation_coordinates;
}

const int* Piece::getWallKickOffsets(){
    return this->wall_kick_offsets;
}

void Piece::printOrientationCoordinates(){
    for (int i = 0; i < ORIENTATIONS; i++){
        for (int j = 0; j < COORDINATES; j++)
            std::cout << (int)(this->orientation_coordinates[i * COORDINATES + j]) << " ";

        std::cout << std::endl;
    }
}
