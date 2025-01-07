#ifndef MATCH_HPP
#define MATCH_HPP

#include "Cell.hpp"
#include "PieceCoordinates.hpp"
#include <iostream>
#include <ctime>

class Match{
    public:
        static const int ROWS = 20;
        static const int COLUMNS = 10;
        static const int COORDINATES = 4;
        static const int DIMENSIONS = 2;
        static const int PIECE_LIST_LENGTH = 14;

        Match();
        Match(Cell grid[ROWS * COLUMNS]);

        Cell* getGrid();
        int getMatchTime();
        int getMatchScore();
        int getLinesCleared();
        int getPiecesDropped();
        int* getOutlineCoordinates();
        
        void printColors();
        void printStates();
    
        void generateRandomPieceSequence();
        void spawnNewPiece();
        void checkForClearLines();

        void moveLeft();
        void moveRight();
        
        void lowerPiece();
        void dropPiece();
        void normalDrop();
        void softDrop();
        void hardDrop();
    
    private:
        Cell grid[ROWS * COLUMNS];
        int match_time;
        int match_score;
        float current_speed;
        int lines_cleared;
        int pieces_dropped;
        int outline_coordinates[COORDINATES * DIMENSIONS];

        int last_drop_time;
        int piece_list_index;
        int piece_list[PIECE_LIST_LENGTH];
        Pieces::Piece piece;
};

#endif
