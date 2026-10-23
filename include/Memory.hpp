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

};
