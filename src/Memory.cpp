#include"Memory.hpp"

uint8_t Memory::read(uint16_t adress){
    return _memoryBank[adress];
    //tick todo
    //create same method in cpu to tick  
}

void Memory::write(uint16_t adress, uint8_t data){
    _memoryBank[adress] = data;
    //tick todo
    //create same method in cpu to tick  

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