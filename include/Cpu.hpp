#pragma once
#include<cstdint>
#include "Memory.hpp"

typedef uint8_t reg8;
typedef uint16_t reg16;

class Cpu{

private:
    reg8 A = 0x01;
    reg8 F = 0000;
    reg8 B = 0xFF;
    reg8 C = 0x13;
    reg8 D = 0x00;
    reg8 E = 0xC1;
    reg8 H = 0x84;
    reg8 L = 0x03;
    reg16 PC = 0x100;
    reg16 SP = 0xFFFE;
    Memory &memory;


    public:
    Cpu(Memory &mem);
    void execute();
    void tick();
    uint8_t fetch(uint16_t adress);
    uint16_t setTo16(uint8_t hi,uint8_t lo);
    uint8_t getFirstByte(reg16 reg);
    uint8_t getSecondByte(reg16 reg);
    void setZ(uint8_t bit);
    void setN(uint8_t bit);
    void setH(uint8_t bit);
    void setC(uint8_t bit);
    reg8 incrementR8(reg8 reg);
    reg8 decrementR8(reg8 reg);
    
};