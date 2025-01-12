#ifndef PIECE_HPP
#define PIECE_HPP

#include <iostream>
#include <cstdint>

namespace Pieces{
    class Piece{
        public:
            enum Type{
                I, O, T, L, J, S, Z
            };

            static const int ORIENTATIONS = 4;
            static const int COORDINATES = 4;
            static const int DIMENSIONS = 2;
            static const int OFFSETS = 5;
            static const int COLOR_COMPONENTS = 3;

            Piece();
            Piece(Type type);

            Type getType();
            int getOrientationIndex();
            void increaseOrientationIndex();
            void decreaseOrientationIndex();
            int* getCoordinates();
            void printCoordinates();
            void printOrientationCoordinates();
            const uint8_t* getColor();
            const int* getLeftmostCoordinateIndices();
            const int* getRightmostCoordinateIndices();
            const int* getDownmostCoordinateIndices();
            const int* getOrientationCoordinates();
            const int* getWallKickOffsets();

        private:
            Type type;
            int orientation_index;
            int coordinates[COORDINATES * DIMENSIONS];
            uint8_t* color;
            int* leftmost_coordinate_indices;
            int* rightmost_coordinate_indices;
            int* downmost_coordinate_indices;
            int* orientation_coordinates;
            int* wall_kick_offsets;
    };

    char getPieceLetter(Piece::Type type);
}

#endif
