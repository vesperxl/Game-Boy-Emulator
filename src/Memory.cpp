#include"Memory.hpp"

uint8_t Memory::read(uint16_t adress){
    return 1;
}

void Memory::write(uint16_t adress, uint8_t data){

}

uint8_t Memory::IF(){
    return _memory[0xFF0F];
}
uint8_t Memory::IE(){
    return _memory[0xFFFF];
}