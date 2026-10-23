#pragma once
#include<cstdint>
#include "Memory.hpp"

using reg8 = uint8_t;
using reg16 = uint16_t;

class Cpu{

public:
    reg8 A = 0x01;
    reg8 F = 0b0000;
    reg8 B = 0xFF;
    reg8 C = 0x13;
    reg8 D = 0x00;
    reg8 E = 0xC1;
    reg8 H = 0x84;
    reg8 L = 0x03;
    reg16 PC = 0x100;
    reg16 SP = 0xFFFE;
    uint8_t ime_flag = 0;
    uint8_t ime_next = 0;
    Memory *_memory;

    Cpu(Memory *mem);
    void step();
    void tick();
    uint8_t fetch();
    uint16_t setTo16(uint8_t hi,uint8_t lo);
    uint8_t getFirstByte(reg16 reg);
    uint8_t getSecondByte(reg16 reg);
    void setZ(uint8_t bit);
    void setN(uint8_t bit);
    void setH(uint8_t bit);
    void setC(uint8_t bit);
    reg8 incrementR8(reg8 reg);
    reg8 decrementR8(reg8 reg);
    uint8_t getZ();
    uint8_t getN();
    uint8_t getH();
    uint8_t getC();
    reg16 addR16(reg16 regA, reg16 regB);
    reg8 addR8(reg8 regA, reg8 regB);
    reg8 adcR8(reg8 regA, reg8 regB);
    reg8 subR8(reg8 regA, reg8 regB);
    reg8 sbcR8(reg8 regA, reg8 regB);
    reg8 andR8(reg8 regA, reg8 regB);
    reg8 xorR8(reg8 regA, reg8 regB);
    reg8 orR8(reg8 regA, reg8 regB);
    reg16 popR16();
    void pushR16(reg8 hi, reg8 lo);
    void rst(uint16_t vec);
    reg8 rlcR8(reg8 reg);
    reg8 rrcR8(reg8 reg);
    reg8 rlR8(reg8 reg);
    reg8 rrR8(reg8 reg);
    reg8 slaR8(reg8 reg);
    reg8 sraR8(reg8 reg);
    reg8 swapR8(reg8 reg);
    reg8 slrR8(reg8 reg);
    void bitU3(uint8_t bit, reg8 reg);
    reg8 resU3(uint8_t bit, reg8 reg);
    reg8 setU3(uint8_t bit, reg8 reg);
    void reset();
};