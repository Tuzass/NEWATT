#include "../include/NEWATT/Cell.hpp"

Cell::Cell(): colors{}, state{Empty} {}

uint8_t* Cell::getColors(){
    return (uint8_t*)(this->colors);
}

Cell::State Cell::getState(){
    return this->state;
}

void Cell::setColors(uint8_t r, uint8_t g, uint8_t b){
    this->colors[0] = r;
    this->colors[1] = g;
    this->colors[2] = b;
}

void Cell::setState(State state){
    this->state = state;
}
