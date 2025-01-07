#include "../include/NEWATT/PieceCoordinates.hpp"

using namespace Pieces;

Piece::Piece(): type{I} {}

Piece::Piece(Type type):
    type{type}, orientation_index{}
{
    switch (this->type){
        case Type::I:
            this->orientation_coordinates = (int*)(I_ORIENTATION_COORDINATES);
            this->leftmost_coordinate_indices = (int*)(I_LEFTMOST_COLLISION_COORDINATE_INDICES);
            this->rightmost_coordinate_indices = (int*)(I_RIGHTMOST_COLLISION_COORDINATE_INDICES);
            this->downmost_coordinate_indices = (int*)(I_DOWNMOST_COLLISION_COORDINATE_INDICES);
            break;
        case Type::O:
            this->orientation_coordinates = (int*)(O_ORIENTATION_COORDINATES);
            this->leftmost_coordinate_indices = (int*)(O_LEFTMOST_COLLISION_COORDINATE_INDICES);
            this->rightmost_coordinate_indices = (int*)(O_RIGHTMOST_COLLISION_COORDINATE_INDICES);
            this->downmost_coordinate_indices = (int*)(O_DOWNMOST_COLLISION_COORDINATE_INDICES);
            break;
        case Type::T:
            this->orientation_coordinates = (int*)(T_ORIENTATION_COORDINATES);
            this->leftmost_coordinate_indices = (int*)(T_LEFTMOST_COLLISION_COORDINATE_INDICES);
            this->rightmost_coordinate_indices = (int*)(T_RIGHTMOST_COLLISION_COORDINATE_INDICES);
            this->downmost_coordinate_indices = (int*)(T_DOWNMOST_COLLISION_COORDINATE_INDICES);
            break;
        case Type::L:
            this->orientation_coordinates = (int*)(L_ORIENTATION_COORDINATES);
            this->leftmost_coordinate_indices = (int*)(L_LEFTMOST_COLLISION_COORDINATE_INDICES);
            this->rightmost_coordinate_indices = (int*)(L_RIGHTMOST_COLLISION_COORDINATE_INDICES);
            this->downmost_coordinate_indices = (int*)(L_DOWNMOST_COLLISION_COORDINATE_INDICES);
            break;
        case Type::J:
            this->orientation_coordinates = (int*)(J_ORIENTATION_COORDINATES);
            this->leftmost_coordinate_indices = (int*)(J_LEFTMOST_COLLISION_COORDINATE_INDICES);
            this->rightmost_coordinate_indices = (int*)(J_RIGHTMOST_COLLISION_COORDINATE_INDICES);
            this->downmost_coordinate_indices = (int*)(J_DOWNMOST_COLLISION_COORDINATE_INDICES);
            break;
        case Type::S:
            this->orientation_coordinates = (int*)(S_ORIENTATION_COORDINATES);
            this->leftmost_coordinate_indices = (int*)(S_LEFTMOST_COLLISION_COORDINATE_INDICES);
            this->rightmost_coordinate_indices = (int*)(S_RIGHTMOST_COLLISION_COORDINATE_INDICES);
            this->downmost_coordinate_indices = (int*)(S_DOWNMOST_COLLISION_COORDINATE_INDICES);
            break;
        case Type::Z:
            this->orientation_coordinates = (int*)(Z_ORIENTATION_COORDINATES);
            this->leftmost_coordinate_indices = (int*)(Z_LEFTMOST_COLLISION_COORDINATE_INDICES);
            this->rightmost_coordinate_indices = (int*)(Z_RIGHTMOST_COLLISION_COORDINATE_INDICES);
            this->downmost_coordinate_indices = (int*)(Z_DOWNMOST_COLLISION_COORDINATE_INDICES);
            break;
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int coordinate = this->orientation_coordinates[i];
        this->coordinates[2 * i] = coordinate / 4;
        this->coordinates[2 * i + 1] = coordinate % 4 + 3;
    }
}

int Piece::getOrientationIndex(){
    return this->orientation_index;
}

int* Piece::getCoordinates(){
    return (int*)(this->coordinates);
}

void Piece::printCoordinates(){
    for (int i = 0; i < this->COORDINATES; i++)
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

void Piece::printOrientationCoordinates(){
    for (int i = 0; i < this->ORIENTATIONS; i++){
        for (int j = 0; j < this->COORDINATES; j++)
            std::cout << (int)(this->orientation_coordinates[i * this->COORDINATES + j]) << " ";

        std::cout << std::endl;
    }
}
