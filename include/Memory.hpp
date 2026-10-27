#pragma once
#include <cstdint>
#include <vector>
#include<array>

class Memory{
    
private:
    //std::vector<uint8_t> rom;

    std::array<uint8_t, 0xFFFF> _memoryBank = {0};

public:
    uint8_t read(uint16_t adress);
    void write(uint16_t adress, uint8_t data);
    uint8_t IF();
    uint8_t IE();
    void clearMemory();
    uint8_t DIV();
    uint8_t TIMA();
    uint8_t TMA();
    uint8_t TAC();
    void setDIV(uint8_t value);
    void setTIMA(uint8_t value);
    void setTMA(uint8_t value);



    //TODO : INC TIMA IF FALLINGEDGE WHEN WRITING TO TAC / DIV;
    //WRITING TO TIMA -> if(overflow == 1) {overflow = 0}

};
