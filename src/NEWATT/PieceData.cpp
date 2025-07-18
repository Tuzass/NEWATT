#include "../include/NEWATT/PieceData.hpp"

// binary representations of orientations if needed
// const uint64_t I_ORIENTATIONS = 0x0f00222200f04444;
// const uint64_t O_ORIENTATIONS = 0x0660066006600660;
// const uint64_t T_ORIENTATIONS = 0x4e0046400e404c40;
// const uint64_t L_ORIENTATIONS = 0x2e0044600e80c440;
// const uint64_t J_ORIENTATIONS = 0x8e0064400e2044c0;
// const uint64_t S_ORIENTATIONS = 0x6c00462006c08c40;
// const uint64_t Z_ORIENTATIONS = 0xc60026400c604c80;

int Pieces::I_ORIENTATION_COORDINATES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    4, 5, 6, 7,
    2, 6, 10, 14,
    8, 9, 10, 11,
    1, 5, 9, 13
};

int Pieces::I_LEFTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, -1, -1, -1,
    0, 1, 2, 3,
    0, -1, -1, -1,
    0, 1, 2, 3
};

int Pieces::I_RIGHTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    3, -1, -1, -1,
    0, 1, 2, 3,
    3, -1, -1, -1,
    0, 1, 2, 3
};

int Pieces::I_DOWNMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 1, 2, 3,
    3, -1, -1, -1,
    0, 1, 2, 3,
    3, -1, -1, -1
};

uint8_t Pieces::I_COLOR[Piece::COLOR_COMPONENTS] {128, 238, 221};



int Pieces::O_ORIENTATION_COORDINATES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 2, 5, 6,
    1, 2, 5, 6,
    1, 2, 5, 6,
    1, 2, 5, 6
};

int Pieces::O_LEFTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 2, -1, -1,
    0, 2, -1, -1,
    0, 2, -1, -1,
    0, 2, -1, -1
};

int Pieces::O_RIGHTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 3, -1, -1,
    1, 3, -1, -1,
    1, 3, -1, -1,
    1, 3, -1, -1
};

int Pieces::O_DOWNMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    2, 3, -1, -1,
    2, 3, -1, -1,
    2, 3, -1, -1,
    2, 3, -1, -1
};

uint8_t Pieces::O_COLOR[Piece::COLOR_COMPONENTS] {235, 240, 74};



int Pieces::T_ORIENTATION_COORDINATES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 4, 5, 6,
    1, 5, 6, 9,
    4, 5, 6, 9,
    1, 4, 5, 9
};

int Pieces::T_LEFTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 1, -1, -1,
    0, 1, 3, -1,
    0, 3, -1, -1,
    0, 1, 3, -1
};

int Pieces::T_RIGHTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 3, -1, -1,
    0, 2, 3, -1,
    2, 3, -1, -1,
    0, 2, 3, -1
};

int Pieces::T_DOWNMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 2, 3, -1,
    2, 3, -1, -1,
    0, 2, 3, -1,
    1, 3, -1, -1
};

uint8_t Pieces::T_COLOR[Piece::COLOR_COMPONENTS] {230, 102, 238};



int Pieces::L_ORIENTATION_COORDINATES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    2, 4, 5, 6,
    1, 5, 9, 10,
    4, 5, 6, 8,
    0, 1, 5, 9
};

int Pieces::L_LEFTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 1, -1, -1,
    0, 1, 2, -1,
    0, 3, -1, -1,
    0, 2, 3, -1
};

int Pieces::L_RIGHTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 3, -1, -1,
    0, 2, 3, -1,
    2, 3, -1, -1,
    1, 2, 3, -1
};

int Pieces::L_DOWNMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 2, 3, -1,
    2, 3, -1, -1,
    1, 2, 3, -1,
    0, 3, -1, -1
};

uint8_t Pieces::L_COLOR[Piece::COLOR_COMPONENTS] {240, 160, 70};



int Pieces::J_ORIENTATION_COORDINATES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 4, 5, 6,
    1, 2, 5, 9,
    4, 5, 6, 10,
    1, 5, 8, 9
};

int Pieces::J_LEFTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 1, -1, -1,
    0, 2, 3, -1,
    0, 3, -1, -1,
    0, 1, 2, -1
};

int Pieces::J_RIGHTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 3, -1, -1,
    1, 2, 3, -1,
    0, 3, -1, -1,
    0, 1, 2, -1
};

int Pieces::J_DOWNMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 2, 3, -1,
    1, 3, -1, -1,
    0, 1, 3, -1,
    2, 3, -1, -1
};

uint8_t Pieces::J_COLOR[Piece::COLOR_COMPONENTS] {110, 114, 240};



int Pieces::S_ORIENTATION_COORDINATES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 2, 4, 5,
    1, 5, 6, 10,
    5, 6, 8, 9,
    0, 4, 5, 9
};

int Pieces::S_LEFTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 2, -1, -1,
    0, 1, 3, -1,
    0, 2, -1, -1,
    0, 1, 3, -1
};

int Pieces::S_RIGHTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 3, -1, -1,
    0, 2, 3, -1,
    1, 3, -1, -1,
    0, 2, 3, -1
};

int Pieces::S_DOWNMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 2, 3, -1,
    1, 3, -1, -1,
    1, 2, 3, -1,
    1, 3, -1, -1
};

uint8_t Pieces::S_COLOR[Piece::COLOR_COMPONENTS] {85, 184, 77};



int Pieces::Z_ORIENTATION_COORDINATES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 1, 5, 6,
    2, 5, 6, 9,
    4, 5, 9, 10,
    1, 4, 5, 8
};

int Pieces::Z_LEFTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 2, -1, -1,
    0, 1, 3, -1,
    0, 2, -1, -1,
    0, 1, 3, -1
};

int Pieces::Z_RIGHTMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    1, 3, -1, -1,
    0, 2, 3, -1,
    1, 3, -1, -1,
    0, 2, 3, -1
};

int Pieces::Z_DOWNMOST_COLLISION_COORDINATE_INDICES[Piece::ORIENTATIONS * Piece::COORDINATES] {
    0, 2, 3, -1,
    2, 3, -1, -1,
    0, 2, 3, -1,
    2, 3, -1, -1
};

uint8_t Pieces::Z_COLOR[Piece::COLOR_COMPONENTS] {229, 88, 88};



int Pieces::I_WALL_KICK_OFFSETS[Piece::ORIENTATIONS * Piece::DIMENSIONS * Piece::OFFSETS] {
    // rotating orientation 0->1
     0,  0,
     0, -2,
     0,  1,
     1, -2, 
    -2,  1,

    // rotating orientation 1->2
     0,  0,
     0, -1,
     0,  2,
    -2, -1, 
     1,  2,

    // rotating orientation 2->3
     0,  0,
     0,  2,
     0, -1,
    -1,  2, 
     2, -1,

    // rotating orientation 3->0
     0,  0,
     0,  1,
     0, -2,
     2,  1, 
    -1, -2
};

int Pieces::TLJSZ_WALL_KICK_OFFSETS[Piece::ORIENTATIONS * Piece::DIMENSIONS * Piece::OFFSETS] {
    // rotating orientation 0->1
     0,  0,
     0, -1,
    -1, -1,
     2,  0,
     2, -1,

    // rotating orientation 1->2
     0,  0,
     0,  1,
     1,  1,
    -2,  0,
    -2,  1,

    // rotating orientation 2->3
     0,  0,
     0,  1,
    -1,  1,
     2,  0,
     2,  1,

    // rotating orientation 3->0
     0,  0,
     0, -1,
     1, -1,
    -2,  0,
    -2, -1
};
