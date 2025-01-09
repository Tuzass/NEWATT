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

            Piece();
            Piece(Type type);

            Type getType();
            int getOrientationIndex();
            void increaseOrientationIndex();
            void decreaseOrientationIndex();
            int* getCoordinates();
            void printCoordinates();
            void printOrientationCoordinates();
            const int* getLeftmostCoordinateIndices();
            const int* getRightmostCoordinateIndices();
            const int* getDownmostCoordinateIndices();
            const int* getOrientationCoordinates();
            const int* getWallKickOffsets();

        private:
            Type type;
            int orientation_index;
            int coordinates[COORDINATES * DIMENSIONS];
            int* leftmost_coordinate_indices;
            int* rightmost_coordinate_indices;
            int* downmost_coordinate_indices;
            int* orientation_coordinates;
            int* wall_kick_offsets;
    };

    char getPieceLetter(Piece::Type type);
}

#endif
