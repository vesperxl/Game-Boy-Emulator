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