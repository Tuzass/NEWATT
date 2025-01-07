#include "../include/NEWATT/Match.hpp"

Match::Match():
    state{}, grid{}, match_time{}, match_score{}, current_speed{0.25f},
    lines_cleared{}, pieces_dropped{}, last_drop_time{}, piece_list_index{}, piece{}
{
    for (int i = 0; i < COORDINATES * DIMENSIONS; i++)
            this->outline_coordinates[i] = 0;
    
    std::cout << "Match state is NotStarted" << std::endl;
}

Match::Match(Cell grid[ROWS * COLUMNS]):
    state{}, match_time{}, match_score{}, current_speed{0.25f}, lines_cleared{},
    pieces_dropped{}, last_drop_time{}, piece_list_index{}, piece{}
{
    for (int i = 0; i < ROWS * COLUMNS; i++)
        this->grid[i] = grid[i];
    
    for (int i = 0; i < COORDINATES * DIMENSIONS; i++)
        this->outline_coordinates[i] = 0;
}

Match::State Match::getState(){
    return this->state;
}

void Match::setState(State state){
    this->state = state;
}

Cell* Match::getGrid(){
    return (Cell*)(this->grid);
}

int Match::getMatchTime(){
    return this->match_time;
}

int Match::getMatchScore(){
    return this->match_score;
}

int Match::getLinesCleared(){
    return this->lines_cleared;
}

int Match::getPiecesDropped(){
    return this->pieces_dropped;
}

int* Match::getOutlineCoordinates(){
    return (int*)(this->outline_coordinates);
}

void Match::start(){
    this->generateRandomPieceSequence();
    this->spawnNewPiece();
    this->state = State::Ongoing;
}

void Match::printColors(){
    std::cout << std::endl;

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            uint8_t* cell_colors = this->grid[i * COLUMNS + j].getColors();
            std::cout << "(" << (int)cell_colors[0] << "," << (int)cell_colors[1] << "," << (int)cell_colors[2] << ")";

            if (j < COLUMNS - 1)
                std::cout << " | ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void Match::printStates(){
    std::cout << std::endl;

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
            Cell::State cell_state = this->grid[i * COLUMNS + j].getState();
            std::cout << (int)cell_state;

            if (j < COLUMNS - 1)
                std::cout << " | ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void Match::generateRandomPieceSequence(){
    using Pieces::Piece;
    srand(time(NULL));

    int j;
    int tmp;
    int half_length = 0.5 * PIECE_LIST_LENGTH;
    int sequence1[half_length] {0, 1, 2, 3, 4, 5, 6};
    int sequence2[half_length] {0, 1, 2, 3, 4, 5, 6};

    for (int i = half_length - 1; i > 0; i--){
        j = rand() % (i + 1);
        tmp = sequence1[j];
        sequence1[j] = sequence1[i];
        sequence1[i] = tmp;
    }

    for (int i = 0; i < half_length; i++){
        this->piece_list[i] = sequence1[i];
    }

    for (int i = half_length - 1; i > 0; i--){
        j = rand() % (i + 1);
        tmp = sequence2[j];
        sequence2[j] = sequence2[i];
        sequence2[i] = tmp;
    }

    for (int i = half_length; i < PIECE_LIST_LENGTH; i++){
        this->piece_list[i] = sequence2[i % half_length];
    }
}

void Match::spawnNewPiece(){
    using Pieces::Piece;

    Piece::Type piece_type = (Piece::Type)(this->piece_list[this->piece_list_index]);
    Piece new_piece = Piece {piece_type};
    this->piece = new_piece;
    int* coordinates = this->piece.getCoordinates();
    this->piece_list_index++;

    // check for spawning spot availability
    // i hope this is how it's done
    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];

        if (this->grid[row * COLUMNS + column + 3].getState() != Cell::State::Empty){
            std::cout << "Conflict at cell (" << row << "," << column << "): couldn't spawn piece " << (int)piece_type << std::endl;
            std::cout << "Match state is: Finished (you fucking suck)" << std::endl;
            this->state = State::Finished;
            return;
        }
    }
    
    if (this->piece_list_index == 14){
        this->generateRandomPieceSequence();
        this->piece_list_index = 0;
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];

        this->grid[row * COLUMNS + column].setColors(1, 1, 1);
        this->grid[row * COLUMNS + column].setState(Cell::State::Piece);
    }

    this->state = State::Ongoing;
}

void Match::checkForClearLines(){
    
}

void Match::moveLeft(){
    int* coordinates = this->piece.getCoordinates();
    const int* leftmost_coordinate_indices = this->piece.getLeftmostCoordinateIndices();
    int orientation_offset = this->piece.getOrientationIndex() * Pieces::Piece::ORIENTATIONS;

    for (int i = 0; i < this->COORDINATES; i++){
        int index = leftmost_coordinate_indices[orientation_offset + i];
        if (index == -1)
            break;
        
        int row = coordinates[2 * index];
        int column = coordinates[2 * index + 1];
        if (column == 0)
            return;
        
        Cell left_cell = this->grid[row * COLUMNS + column - 1];
        if (left_cell.getState() == Cell::State::Full)
            return;
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        coordinates[2 * i + 1]--;
        this->grid[row * this->COLUMNS + column].setColors(0, 0, 0);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * this->COLUMNS + column].setColors(1, 1, 1);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Piece);
    }
}

void Match::moveRight(){
    int* coordinates = this->piece.getCoordinates();
    const int* rightmost_coordinate_indices = this->piece.getRightmostCoordinateIndices();
    int orientation_offset = this->piece.getOrientationIndex() * Pieces::Piece::ORIENTATIONS;

    for (int i = 0; i < this->COORDINATES; i++){
        int index = rightmost_coordinate_indices[orientation_offset + i];
        if (index == -1)
            break;
        
        int row = coordinates[2 * index];
        int column = coordinates[2 * index + 1];
        if (column == this->COLUMNS - 1)
            return;
        
        Cell right_cell = this->grid[row * COLUMNS + column + 1];
        if (right_cell.getState() == Cell::State::Full)
            return;
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        coordinates[2 * i + 1]++;
        this->grid[row * this->COLUMNS + column].setColors(0, 0, 0);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * this->COLUMNS + column].setColors(1, 1, 1);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Piece);
    }
}

void Match::lowerPiece(){
    int* coordinates = this->piece.getCoordinates();
    const int* downmost_coordinate_indices = this->piece.getDownmostCoordinateIndices();
    int orientation_offset = this->piece.getOrientationIndex() * Pieces::Piece::ORIENTATIONS;

    for (int i = 0; i < this->COORDINATES; i++){
        int index = downmost_coordinate_indices[orientation_offset + i];
        if (index == -1)
            break;
        
        int row = coordinates[2 * index];
        int column = coordinates[2 * index + 1];
        if (row == this->ROWS - 1){
            this->lockPiece();
            return;
        }
        
        Cell down_cell = this->grid[(row + 1) * COLUMNS + column];
        if (down_cell.getState() == Cell::State::Full){
            this->lockPiece();
            return;
        }
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        coordinates[2 * i]++;
        this->grid[row * this->COLUMNS + column].setColors(0, 0, 0);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * this->COLUMNS + column].setColors(1, 1, 1);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Piece);
    }
}

void Match::lockPiece(){
    int* coordinates = this->piece.getCoordinates();

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Full);
    }

    this->checkForClearLines();
    this->state = State::PieceLocked;
}

void Match::normalDrop(){

}

void Match::softDrop(){

}

void Match::hardDrop(){

}
