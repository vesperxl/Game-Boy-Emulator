#include"Memory.hpp"

uint8_t Memory::read(uint16_t adress){
    return _memoryBank[adress];
}

void Memory::write(uint16_t adress, uint8_t data){
    _memoryBank[adress] = data;
}

uint8_t Memory::IF(){
    return _memoryBank[0xFF0F];
}
uint8_t Memory::IE(){
    return _memoryBank[0xFFFF];
}

void Memory::clearMemory(){
    _memoryBank.fill(0);
}

uint8_t Memory::DIV(){
    return _memoryBank[0xFF04];
}

uint8_t Memory::TIMA(){
    return _memoryBank[0xFF05];
}

uint8_t Memory::TMA(){
    return _memoryBank[0xFF06];
}

uint8_t Memory::TAC(){
    return _memoryBank[0xFF07];
}

void Memory::setDIV(uint8_t value){
    _memoryBank[0xFF04] = value;
}
void Memory::setTIMA(uint8_t value){
    _memoryBank[0xFF05] = value;
}
void Memory::setTMA(uint8_t value){
    _memoryBank[0xFF06] = value;
}
