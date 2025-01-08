#ifndef MATCH_HPP
#define MATCH_HPP

#include "Cell.hpp"
#include "PieceCoordinates.hpp"
#include <iostream>
#include <ctime>

class Match{
    public:
        static const int ROWS = 22;
        static const int COLUMNS = 10;
        static const int COORDINATES = 4;
        static const int DIMENSIONS = 2;
        static const int PIECES = 7;

        enum State{
            NotStarted, Ongoing, Paused, PieceLocked, Finished
        };

        Match();
        Match(Cell grid[ROWS * COLUMNS]);

        State getState();
        void setState(State state);

        Cell* getGrid();
        int getMatchTime();
        int getMatchScore();
        int getLinesCleared();
        int getPiecesDropped();
        int* getOutlineCoordinates();
        
        void printColors();
        void printStates();
        void printOutlineCoordinates();
    
        void start();
        void generateRandomPieceSequence();
        void spawnNewPiece();
        void checkForClearLines();
        void calculateOutlineCoordinates();

        void moveLeft();
        void moveRight();

        void rotateCW();
        void rotateCounterCW();
        
        void lowerPiece();
        void lockPiece();

        void normalDrop();
        void softDrop();
        void hardDrop();
    
    private:
        State state;
        Cell grid[ROWS * COLUMNS];
        int match_time;
        int match_score;
        float current_speed;
        int lines_cleared;
        int pieces_dropped;
        int outline_coordinates[COORDINATES * DIMENSIONS];

        int last_drop_time;
        int piece_list_index;
        int piece_list[PIECES];
        Pieces::Piece piece;
};

#endif
