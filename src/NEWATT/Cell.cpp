#include "../include/NEWATT/Cell.hpp"

Cell::Cell(): color{}, state{Empty} {}

uint8_t* Cell::getColor(){
    return (uint8_t*)(this->color);
}

Cell::State Cell::getState(){
    return this->state;
}

void Cell::setColor(uint8_t r, uint8_t g, uint8_t b){
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
}

void Cell::setState(State state){
    this->state = state;
}
