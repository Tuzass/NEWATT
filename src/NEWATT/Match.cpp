#include "../include/NEWATT/Match.hpp"

Match::Match():
    state{}, grid{}, match_time{}, match_score{}, current_speed{0.25f}, lines_cleared{}, pieces_dropped{},
    highest_non_empty_row{ROWS}, last_drop_time{}, piece_list_index{}, piece{}, held_piece{-1}, has_switched{false}
{
    for (int i = 0; i < COORDINATES * DIMENSIONS; i++)
            this->ghost_coordinates[i] = 0;
}

Match::Match(Cell grid[ROWS * COLUMNS]):
    state{}, match_time{}, match_score{}, current_speed{0.25f}, lines_cleared{}, pieces_dropped{},
    highest_non_empty_row{}, last_drop_time{}, piece_list_index{}, piece{}, held_piece{-1}, has_switched{false}
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
    srand(time(NULL));
    int sequence[PIECES]{};
    this->generateRandomPieceSequence(sequence);

    for (int i = 0; i < PIECES; i++)
        this->piece_list[i] = sequence[i];
    
    this->generateRandomPieceSequence(sequence);

    for (int i = PIECES; i < 2 * PIECES; i++)
        this->piece_list[i] = sequence[i - PIECES];

    this->spawnNewPiece();
    this->state = State::Ongoing;
}

void Match::printColorGrid(){
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

void Match::printStateGrid(){
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
    for (int i = 0; i < COORDINATES; i++)
        std::cout << "(" << this->ghost_coordinates[2 * i] << "," << this->ghost_coordinates[2 * i + 1] << ") ";
    
    std::cout << std::endl;
}

void Match::printNextPieces(){
    std::cout << "Next " << NEXT_PIECES << " pieces: ";
    for (int i = 0; i < NEXT_PIECES; i++){
        Pieces::Piece::Type piece_type = (Pieces::Piece::Type)this->piece_list[this->piece_list_index + i];
        std::cout << Pieces::getPieceLetter(piece_type) << " ";
    }
    std::cout << std::endl;
}

void Match::printHeldPiece(){
    std::cout << "Held piece: ";

    if (this->held_piece == -1) std::cout << "None";

    else {
        Pieces::Piece::Type piece_type = (Pieces::Piece::Type)this->held_piece;
        std::cout << Pieces::getPieceLetter(piece_type);
    }

    std::cout << std::endl;
}

void Match::generateRandomPieceSequence(int sequence[PIECES]){
    using Pieces::Piece;

    int j, tmp;
    int base_sequence[PIECES] {0, 1, 2, 3, 4, 5, 6};

    for (int i = PIECES - 1; i > 0; i--){
        j = rand() % (i + 1);
        tmp = base_sequence[j];
        base_sequence[j] = base_sequence[i];
        base_sequence[i] = tmp;
    }

    for (int i = 0; i < PIECES; i++)
        sequence[i] = base_sequence[i];
}

void Match::spawnNewPiece(int piece_type){
    using Pieces::Piece;

    if (piece_type == -1){
        piece_type = this->piece_list[this->piece_list_index];
        this->piece_list_index++;
    }

    Piece new_piece = Piece {(Piece::Type)piece_type};
    this->piece = new_piece;
    int* coordinates = this->piece.getCoordinates();

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];

        if (this->grid[row * COLUMNS + column].getState() != Cell::State::Empty){
            this->state = State::Finished;
            return;
        }
    }
    
    if (this->piece_list_index == PIECES){
        int new_sequence[PIECES]{};
        this->generateRandomPieceSequence(new_sequence);

        for (int i = 0; i < PIECES; i++){
            this->piece_list[i] = this->piece_list[i + PIECES];
            this->piece_list[i + PIECES] = new_sequence[i];
        }

        this->piece_list_index = 0;
    }

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];

        this->grid[row * COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * COLUMNS + column].setState(Cell::State::Piece);
    }

    this->state = State::Ongoing;
    this->calculateGhostCoordinates();
}

void Match::checkForClearRows(int rows[COORDINATES]){
    for (int i = 0; i < COORDINATES; i++){
        int row = rows[i];
        if (row == -1) break;

        bool should_clear_row{true};
        for (int j = 0; j < COLUMNS; j++){
            Cell cell = this->grid[row * COLUMNS + j];

            if (cell.getState() == Cell::State::Empty){
                should_clear_row = false;
                break;
            }
        }

        if (!should_clear_row) continue;
        
        for (int j = row - 1; j >= this->highest_non_empty_row; j--){
            for (int k = 0; k < COLUMNS; k++){
                Cell::State state = this->grid[j * COLUMNS + k].getState();
                uint8_t* colors = this->grid[j * COLUMNS + k].getColors();

                this->grid[j * COLUMNS + k].setState(Cell::State::Empty);
                this->grid[j * COLUMNS + k].setColors(0, 0, 0);

                this->grid[(j + 1) * COLUMNS + k].setState(state);
                this->grid[(j + 1) * COLUMNS + k].setColors(colors[0], colors[1], colors[2]);
            }
        }

        this->lines_cleared++;
        this->highest_non_empty_row--;
    }
}

void Match::calculateGhostCoordinates(){
    int* coordinates = this->piece.getCoordinates();
    const int* downmost_coordinate_indices = this->piece.getDownmostCoordinateIndices();
    int orientation_offset = this->piece.getOrientationIndex() * Pieces::Piece::ORIENTATIONS;

    int i = 0;
    bool found_bottom_collision{false};
    while (!found_bottom_collision){
        for (int j = 0; j < COORDINATES; j++){
            int index = downmost_coordinate_indices[j];
            if (index == -1) break;
            
            int row = coordinates[2 * index] + i;
            int column = coordinates[2 * index + 1];

            if (row == ROWS){
                found_bottom_collision = true;
                i--;
                break;
            }

            Cell down_cell = this->grid[row * COLUMNS + column];

            if (down_cell.getState() == Cell::State::Full){
                found_bottom_collision = true;
                i--;
                break;
            }
        }

        if (!found_bottom_collision) i++;
    }

    for (int j = 0; j < COORDINATES; j++){
        this->ghost_coordinates[2 * j] = coordinates[2 * j] + i;
        this->ghost_coordinates[2 * j + 1] = coordinates[2 * j + 1];
    }
}

void Match::moveLeft(){
    int* coordinates = this->piece.getCoordinates();
    const int* leftmost_coordinate_indices = this->piece.getLeftmostCoordinateIndices();
    int orientation_offset = this->piece.getOrientationIndex() * Pieces::Piece::ORIENTATIONS;

    for (int i = 0; i < COORDINATES; i++){
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

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        coordinates[2 * i + 1]--;
        this->grid[row * COLUMNS + column].setColors(0, 0, 0);
        this->grid[row * COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * COLUMNS + column].setState(Cell::State::Piece);
    }

    this->calculateGhostCoordinates();
}

void Match::moveRight(){
    int* coordinates = this->piece.getCoordinates();
    const int* rightmost_coordinate_indices = this->piece.getRightmostCoordinateIndices();
    int orientation_offset = this->piece.getOrientationIndex() * Pieces::Piece::ORIENTATIONS;

    for (int i = 0; i < COORDINATES; i++){
        int index = rightmost_coordinate_indices[orientation_offset + i];
        if (index == -1)
            break;
        
        int row = coordinates[2 * index];
        int column = coordinates[2 * index + 1];
        if (column == COLUMNS - 1)
            return;
        
        Cell right_cell = this->grid[row * COLUMNS + column + 1];
        if (right_cell.getState() == Cell::State::Full)
            return;
    }

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        coordinates[2 * i + 1]++;
        this->grid[row * COLUMNS + column].setColors(0, 0, 0);
        this->grid[row * COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * COLUMNS + column].setState(Cell::State::Piece);
    }

    this->calculateGhostCoordinates();
}

void Match::rotateCW(){
    using Pieces::Piece;

    if (this->piece.getType() == Piece::Type::O) return;

    const int* orientation_coordinates = this->piece.getOrientationCoordinates();
    int old_orientation_index = this->piece.getOrientationIndex();
    int orientation_coordinate = orientation_coordinates[old_orientation_index * COORDINATES];
    int orientation_coordinate_row = orientation_coordinate / COORDINATES;
    int orientation_coordinate_column = orientation_coordinate % COORDINATES;

    int* coordinates = this->piece.getCoordinates();
    int coordinate_row = coordinates[0];
    int coordinate_column = coordinates[1];

    int bounding_box_row_offset = coordinate_row - orientation_coordinate_row;
    int bounding_box_column_offset = coordinate_column - orientation_coordinate_column;
    
    int new_orientation_index = old_orientation_index + 1;
    if (new_orientation_index >= Piece::ORIENTATIONS) new_orientation_index -= Piece::ORIENTATIONS;

    const int* wall_kick_offsets = this->piece.getWallKickOffsets();
    for (int i = 0; i < Piece::OFFSETS; i++){
        int wall_kick_row_offset = wall_kick_offsets[old_orientation_index * Piece::OFFSETS * DIMENSIONS + i * DIMENSIONS];
        int wall_kick_column_offset = wall_kick_offsets[old_orientation_index * Piece::OFFSETS * DIMENSIONS + i * DIMENSIONS + 1];

        bool test_result{true};
        for (int j = 0; j < COORDINATES; j++){
            int coordinate = orientation_coordinates[new_orientation_index * COORDINATES + j];
            int row = coordinate / COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int column = coordinate % COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS){
                test_result = false;
                break;
            }

            Cell offset_cell = this->grid[row * COLUMNS + column];
            if (offset_cell.getState() == Cell::State::Full){
                test_result = false;
                break;
            }
        }

        if (!test_result) continue;

        for (int j = 0; j < COORDINATES; j++){
            int current_row = coordinates[2 * j];
            int current_column = coordinates[2 * j + 1];

            this->grid[current_row * COLUMNS + current_column].setState(Cell::State::Empty);
            this->grid[current_row * COLUMNS + current_column].setColors(0, 0, 0);
        }

        for (int j = 0; j < COORDINATES; j++){
            int new_coordinate = orientation_coordinates[new_orientation_index * COORDINATES + j];
            int new_row = new_coordinate / COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int new_column = new_coordinate % COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            coordinates[2 * j] = new_row;
            coordinates[2 * j + 1] = new_column;
            this->grid[new_row * COLUMNS + new_column].setState(Cell::State::Piece);
            this->grid[new_row * COLUMNS + new_column].setColors(255, 255, 255);
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
    int orientation_coordinate = orientation_coordinates[old_orientation_index * COORDINATES];
    int orientation_coordinate_row = orientation_coordinate / COORDINATES;
    int orientation_coordinate_column = orientation_coordinate % COORDINATES;

    int* coordinates = this->piece.getCoordinates();
    int coordinate_row = coordinates[0];
    int coordinate_column = coordinates[1];

    int bounding_box_row_offset = coordinate_row - orientation_coordinate_row;
    int bounding_box_column_offset = coordinate_column - orientation_coordinate_column;

    int new_orientation_index = old_orientation_index - 1;
    if (new_orientation_index < 0) new_orientation_index += Piece::ORIENTATIONS;

    const int* wall_kick_offsets = this->piece.getWallKickOffsets();
    for (int i = 0; i < Piece::OFFSETS; i++){
        int wall_kick_row_offset = wall_kick_offsets[new_orientation_index * Piece::OFFSETS * DIMENSIONS + i * DIMENSIONS];
        int wall_kick_column_offset = wall_kick_offsets[new_orientation_index * Piece::OFFSETS * DIMENSIONS + i * DIMENSIONS + 1];

        bool test_result{true};
        for (int j = 0; j < COORDINATES; j++){
            int coordinate = orientation_coordinates[new_orientation_index * COORDINATES + j];
            int row = coordinate / COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int column = coordinate % COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS){
                test_result = false;
                break;
            }

            Cell offset_cell = this->grid[row * COLUMNS + column];
            if (offset_cell.getState() == Cell::State::Full){
                test_result = false;
                break;
            }
        }

        if (!test_result) continue;

        for (int j = 0; j < COORDINATES; j++){
            int current_row = coordinates[2 * j];
            int current_column = coordinates[2 * j + 1];

            this->grid[current_row * COLUMNS + current_column].setState(Cell::State::Empty);
            this->grid[current_row * COLUMNS + current_column].setColors(0, 0, 0);
        }

        for (int j = 0; j < COORDINATES; j++){
            int new_coordinate = orientation_coordinates[new_orientation_index * COORDINATES + j];
            int new_row = new_coordinate / COORDINATES + bounding_box_row_offset + wall_kick_row_offset;
            int new_column = new_coordinate % COORDINATES + bounding_box_column_offset + wall_kick_column_offset;

            coordinates[2 * j] = new_row;
            coordinates[2 * j + 1] = new_column;
            this->grid[new_row * COLUMNS + new_column].setState(Cell::State::Piece);
            this->grid[new_row * COLUMNS + new_column].setColors(255, 255, 255);
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

    for (int i = 0; i < COORDINATES; i++){
        int index = downmost_coordinate_indices[orientation_offset + i];
        if (index == -1)
            break;
        
        int row = coordinates[2 * index];
        int column = coordinates[2 * index + 1];
        if (row == ROWS - 1){
            this->lockPiece();
            return;
        }
        
        Cell down_cell = this->grid[(row + 1) * COLUMNS + column];
        if (down_cell.getState() == Cell::State::Full){
            this->lockPiece();
            return;
        }
    }

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        coordinates[2 * i]++;
        this->grid[row * COLUMNS + column].setColors(0, 0, 0);
        this->grid[row * COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * COLUMNS + column].setColors(255, 255, 255);
        this->grid[row * COLUMNS + column].setState(Cell::State::Piece);
    }
}

void Match::lockPiece(){
    int* coordinates = this->piece.getCoordinates();
    int highest_row = -1;
    int rows_changed[COORDINATES]{-1, -1, -1, -1};
    int rows_changed_index = 0;

    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * COLUMNS + column].setState(Cell::State::Full);
        
        bool should_write_row{true};
        for (int j = 0; j < rows_changed_index; j++){
            if (rows_changed[j] == -1 || rows_changed[j] == row){
                should_write_row = false;
            }
        }

        if (!should_write_row) continue;

        rows_changed[rows_changed_index] = row;
        rows_changed_index++;

        if (row > highest_row) highest_row = row;
        if (row < this->highest_non_empty_row) this->highest_non_empty_row = row;
    }

    if (highest_row < ROWS - 20){
        this->state = State::Finished;
        return;
    }

    this->pieces_dropped++;
    this->checkForClearRows(rows_changed);
    this->state = State::PieceLocked;
    this->has_switched = false;
}

void Match::holdPiece(){
    if (this->has_switched){
        std::cout << "can't switch again dude..." << std::endl;
        return;
    }

    int* coordinates = this->piece.getCoordinates();
    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * COLUMNS + column].setState(Cell::State::Empty);
        this->grid[row * COLUMNS + column].setColors(0, 0, 0);
    }

    if (this->held_piece == -1){
        this->held_piece = (int)this->piece.getType();
        this->spawnNewPiece();
        this->has_switched = true;
        return;
    }

    int tmp = this->held_piece;
    this->held_piece = (int)this->piece.getType();
    this->spawnNewPiece(tmp);
    this->has_switched = true;
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
    for (int i = 0; i < COORDINATES; i++){
        int row = coordinates[2 * i];
        int column = coordinates[2 * i + 1];
        this->grid[row * COLUMNS + column].setState(Cell::State::Empty);
    }

    for (int i = 0; i < COORDINATES; i++){
        int row = this->ghost_coordinates[2 * i];
        int column = this->ghost_coordinates[2 * i + 1];
        coordinates[2 * i] = row;
        coordinates[2 * i + 1] = column;
        this->grid[row * COLUMNS + column].setColors(255, 255, 255);
    }

    this->lockPiece();
}
