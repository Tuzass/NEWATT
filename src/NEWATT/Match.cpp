#include "../include/NEWATT/Match.hpp"

Match::Match():
    state{}, grid{}, match_time{}, match_score{}, current_speed{0.25f},
    lines_cleared{}, pieces_dropped{}, last_drop_time{}, piece_list_index{}, piece{}
{
    for (int i = 0; i < COORDINATES * DIMENSIONS; i++)
            this->ghost_coordinates[i] = 0;
}

Match::Match(Cell grid[ROWS * COLUMNS]):
    state{}, match_time{}, match_score{}, current_speed{0.25f}, lines_cleared{},
    pieces_dropped{}, last_drop_time{}, piece_list_index{}, piece{}
{
    for (int i = 0; i < ROWS * COLUMNS; i++)
        this->grid[i] = grid[i];
    
    for (int i = 0; i < COORDINATES * DIMENSIONS; i++)
        this->ghost_coordinates[i] = 0;
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

int* Match::getGhostCoordinates(){
    return (int*)(this->ghost_coordinates);
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

void Match::printGhostCoordinates(){
    for (int i = 0; i < this->COORDINATES; i++)
        std::cout << "(" << this->ghost_coordinates[2 * i] << "," << this->ghost_coordinates[2 * i + 1] << ") ";
    
    std::cout << std::endl;
}

void Match::generateRandomPieceSequence(){
    using Pieces::Piece;
    srand(time(NULL));

    int j, tmp;
    int sequence[this->PIECES] {0, 1, 2, 3, 4, 5, 6};

    for (int i = this->PIECES - 1; i > 0; i--){
        j = rand() % (i + 1);
        tmp = sequence[j];
        sequence[j] = sequence[i];
        sequence[i] = tmp;
    }

    for (int i = 0; i < this->PIECES; i++){
        this->piece_list[i] = sequence[i];
    }
}

void Match::spawnNewPiece(){
    using Pieces::Piece;

    Piece::Type piece_type = (Piece::Type)(this->piece_list[this->piece_list_index]);
    Piece new_piece = Piece {piece_type};
    this->piece = new_piece;
    int* coordinates = this->piece.getCoordinates();
    this->piece_list_index++;

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];

        if (this->grid[row * this->COLUMNS + column].getState() != Cell::State::Empty){
            this->state = State::Finished;
            return;
        }
    }
    
    if (this->piece_list_index == this->PIECES){
        this->generateRandomPieceSequence();
        this->piece_list_index = 0;
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];

        this->grid[row * this->COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Piece);
    }

    this->state = State::Ongoing;
    this->calculateGhostCoordinates();
}

void Match::clearLines(){
    
}

void Match::calculateGhostCoordinates(){
    int* coordinates = this->piece.getCoordinates();
    const int* downmost_coordinate_indices = this->piece.getDownmostCoordinateIndices();
    int orientation_offset = this->piece.getOrientationIndex() * Pieces::Piece::ORIENTATIONS;

    int i = 0;
    bool found_bottom_collision{false};
    while (!found_bottom_collision){
        for (int j = 0; j < this->COORDINATES; j++){
            int index = downmost_coordinate_indices[j];
            if (index == -1) break;
            
            int row = coordinates[2 * index] + i;
            int column = coordinates[2 * index + 1];

            if (row == this->ROWS - 1){
                found_bottom_collision = true;
                break;
            }

            Cell down_cell = this->grid[row * this->COLUMNS + column];

            if (down_cell.getState() == Cell::State::Full){
                found_bottom_collision = true;
                i--;
                break;
            }
        }

        if (!found_bottom_collision) i++;
    }

    for (int j = 0; j < this->COORDINATES; j++){
        this->ghost_coordinates[2 * j] = coordinates[2 * j] + i;
        this->ghost_coordinates[2 * j + 1] = coordinates[2 * j + 1];
    }
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
        this->grid[row * this->COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Piece);
    }

    this->calculateGhostCoordinates();
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
        this->grid[row * this->COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Piece);
    }

    this->calculateGhostCoordinates();
}

void Match::rotateCW(){
    using Pieces::Piece;

    if (this->piece.getType() == Piece::Type::O) return;

    const int* orientation_coordinates = this->piece.getOrientationCoordinates();
    int old_orientation_index = this->piece.getOrientationIndex();
    int orientation_coordinate = orientation_coordinates[old_orientation_index * this->COORDINATES];
    int orientation_coordinate_row = orientation_coordinate / this->COORDINATES;
    int orientation_coordinate_column = orientation_coordinate % this->COORDINATES;

    int* coordinates = this->piece.getCoordinates();
    int coordinate_row = coordinates[0];
    int coordinate_column = coordinates[1];

    int bounding_box_row_offset = coordinate_row - orientation_coordinate_row;
    int bounding_box_column_offset = coordinate_column - orientation_coordinate_column;
    
    int new_orientation_index = old_orientation_index + 1;
    if (new_orientation_index >= Piece::ORIENTATIONS) new_orientation_index -= Piece::ORIENTATIONS;

    const int* wall_kick_offsets = this->piece.getWallKickOffsets();
    for (int i = 0; i < Piece::OFFSETS; i++){

        int wall_kick_row_offset = wall_kick_offsets[old_orientation_index * Piece::OFFSETS * this->DIMENSIONS + i * this->DIMENSIONS];
        int wall_kick_column_offset = wall_kick_offsets[old_orientation_index * Piece::OFFSETS * this->DIMENSIONS + i * this->DIMENSIONS + 1];

        bool test_result{true};
        for (int j = 0; j < this->COORDINATES; j++){

            int coordinate = orientation_coordinates[new_orientation_index * this->COORDINATES + j];
            int row = coordinate / this->COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int column = coordinate % this->COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            if (row < 0 || row >= this->ROWS || column < 0 || column >= this->COLUMNS){
                test_result = false;
                break;
            }

            Cell offset_cell = this->grid[row * this->COLUMNS + column];
            if (offset_cell.getState() == Cell::State::Full){
                test_result = false;
                break;
            }
        }

        if (!test_result) continue;

        for (int j = 0; j < this->COORDINATES; j++){
            int current_row = coordinates[2 * j];
            int current_column = coordinates[2 * j + 1];
            this->grid[current_row * this->COLUMNS + current_column].setState(Cell::State::Empty);
            this->grid[current_row * this->COLUMNS + current_column].setColors(0, 0, 0);
        }

        for (int j = 0; j < this->COORDINATES; j++){
            int new_coordinate = orientation_coordinates[new_orientation_index * this->COORDINATES + j];
            int new_row = new_coordinate / this->COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int new_column = new_coordinate % this->COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            coordinates[2 * j] = new_row;
            coordinates[2 * j + 1] = new_column;
            this->grid[new_row * this->COLUMNS + new_column].setState(Cell::State::Piece);
            this->grid[new_row * this->COLUMNS + new_column].setColors(255, 255, 255);
        }

        this->piece.increaseOrientationIndex();
        this->calculateGhostCoordinates();
        break;
    }
}

void Match::rotateCounterCW(){
    using Pieces::Piece;

    if (this->piece.getType() == Piece::Type::O) return;

    const int* orientation_coordinates = this->piece.getOrientationCoordinates();
    int old_orientation_index = this->piece.getOrientationIndex();
    int orientation_coordinate = orientation_coordinates[old_orientation_index * this->COORDINATES];
    int orientation_coordinate_row = orientation_coordinate / this->COORDINATES;
    int orientation_coordinate_column = orientation_coordinate % this->COORDINATES;

    int* coordinates = this->piece.getCoordinates();
    int coordinate_row = coordinates[0];
    int coordinate_column = coordinates[1];

    int bounding_box_row_offset = coordinate_row - orientation_coordinate_row;
    int bounding_box_column_offset = coordinate_column - orientation_coordinate_column;

    int new_orientation_index = old_orientation_index - 1;
    if (new_orientation_index < 0) new_orientation_index += Piece::ORIENTATIONS;

    const int* wall_kick_offsets = this->piece.getWallKickOffsets();
    for (int i = 0; i < Piece::OFFSETS; i++){
        int wall_kick_row_offset = wall_kick_offsets[new_orientation_index * Piece::OFFSETS * this->DIMENSIONS + i * this->DIMENSIONS];
        int wall_kick_column_offset = wall_kick_offsets[new_orientation_index * Piece::OFFSETS * this->DIMENSIONS + i * this->DIMENSIONS + 1];

        bool test_result{true};
        for (int j = 0; j < this->COORDINATES; j++){
            int coordinate = orientation_coordinates[new_orientation_index * this->COORDINATES + j];
            int row = coordinate / this->COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int column = coordinate % this->COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            if (row < 0 || row >= this->ROWS || column < 0 || column >= this->COLUMNS){
                test_result = false;
                break;
            }

            Cell offset_cell = this->grid[row * this->COLUMNS + column];
            if (offset_cell.getState() == Cell::State::Full){
                test_result = false;
                break;
            }
        }

        if (!test_result) continue;

        for (int j = 0; j < this->COORDINATES; j++){
            int current_row = coordinates[2 * j];
            int current_column = coordinates[2 * j + 1];

            this->grid[current_row * this->COLUMNS + current_column].setState(Cell::State::Empty);
            this->grid[current_row * this->COLUMNS + current_column].setColors(0, 0, 0);
        }

        for (int j = 0; j < this->COORDINATES; j++){
            int new_coordinate = orientation_coordinates[new_orientation_index * this->COORDINATES + j];
            int new_row = new_coordinate / this->COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int new_column = new_coordinate % this->COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            coordinates[2 * j] = new_row;
            coordinates[2 * j + 1] = new_column;
            this->grid[new_row * this->COLUMNS + new_column].setState(Cell::State::Piece);
            this->grid[new_row * this->COLUMNS + new_column].setColors(255, 255, 255);
        }

        this->piece.decreaseOrientationIndex();
        this->calculateGhostCoordinates();
        break;
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
        this->grid[row * this->COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Piece);
    }
}

void Match::lockPiece(){
    int* coordinates = this->piece.getCoordinates();
    int lowest_row = -1;

    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        std::cout << "locking new coordinate (" << row << "," << column << ")" << std::endl;
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Full);

        if (row > lowest_row) lowest_row = row;
    }

    this->pieces_dropped++;

    if (lowest_row < 2){
        this->state = State::Finished;
        return;
    }

    this->clearLines();
    this->state = State::PieceLocked;
}

void Match::normalDrop(){
    // maybe toggle a boolean depending on how input works
    this->current_speed /= soft_drop_multiplier;
}

void Match::softDrop(){
    // maybe toggle a boolean depending on how input works
    this->current_speed *= soft_drop_multiplier;
}

void Match::hardDrop(){
    int* coordinates = this->piece.getCoordinates();
    for (int i = 0; i < this->COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        std::cout << "emptying cell (" << row << "," << column << ")" << std::endl;
        this->grid[row * this->COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < this->COORDINATES; i++){
        int row = this->ghost_coordinates[2 * i];
        int column = this->ghost_coordinates[2 * i + 1];
        std::cout << "setting new coordinate (" << row << "," << column << ")" << std::endl;
        coordinates[2 * i] = row;
        coordinates[2 * i + 1] = column;
        this->grid[row * this->COLUMNS + column].setColors(255, 255, 255);
    }

    this->lockPiece();
}
