#ifndef CELL_HPP
#define CELL_HPP

#include <cstdint>

class Cell{
    public:
        enum State: uint8_t{
            Empty, Full, Piece
        };

        static const int COLOR_COMPONENTS = 3;
        
        Cell();

        uint8_t* getColors();
        State getState();
        void setColors(uint8_t r, uint8_t g, uint8_t b);
        void setState(State state);
    
    private:
        uint8_t colors[COLOR_COMPONENTS];
        State state;
};

#endif
