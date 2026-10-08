#include<cstdint>
#include <memory>

#include "Cpu.hpp"
#include"Memory.hpp"


void Cpu::execute(){

    uint8_t opcode = fetch(PC);

    switch (opcode & 0xFF)
    {
    case 0x00:
        break;
    case 0x01:
        C = fetch(PC);
        B = fetch(PC);
        break;
    case 0x02:
    {
        uint16_t BC = setTo16(B,C);
        memory.write(BC,A);
        break;
    }
    case 0x03:
    {
        uint16_t BC = setTo16(B,C);
        BC++;
        B = getFirstByte(BC);
        C = getSecondByte(BC);
        tick();
        break;
    }
    case 0x04:
        B = incrementR8(B);
        break;
    case 0x05:
        B = decrementR8(B);
        break;
    case 0x06:
        B = fetch(PC);
        break;
    case 0x07:
    {
        uint8_t bit = (A & 128) >> 7;
        setC(bit);
        setZ(0);
        setN(0);
        setH(0);
        A = (A << 1) | bit;
        break;
    }
    case 0x08:
    {
        uint8_t lo = fetch(PC);
        uint8_t hi = fetch(PC);
        uint16_t adress = setTo16(hi,lo);
        uint8_t data = getSecondByte(SP);
        memory.write(adress,data);
        data = getFirstByte(SP);
        adress++;
        memory.write(adress,data);
        break;
    }
    case 0x09:
    {
        uint16_t BC = setTo16(B,C);
        uint16_t HL = setTo16(H,L);
        uint32_t result = BC + HL;

        setN(0);
        if(((BC & 0xFFF) + (HL & 0xFFF)) > 0xFFF){
            setH(1);
        }
        if(result > 0xFFFF){
            setC(1);
        }

        HL += BC;
        break;
    }
    default:
        break;
    }


}

Cpu::Cpu(Memory &mem) : memory(mem){
   
}

uint8_t Cpu::fetch(uint16_t adress){
    
    uint8_t data = memory.read(adress);
    PC++;
    return data;
}
    

uint16_t Cpu::setTo16(uint8_t hi, uint8_t lo){
    return ((hi << 8) | lo);
}

uint8_t Cpu::getFirstByte(reg16 reg){
    return reg >> 8;
}

uint8_t Cpu::getSecondByte(reg16 reg){
    return reg & 0xFF;
}

void Cpu::setZ(uint8_t bit){
    bit = (bit == 0 ? 0: 1);
    F = (F & ~(1<<7)) | (bit<<7);
}

void Cpu::setN(uint8_t bit){
    bit = (bit == 0 ? 0: 1);
    F = (F & ~(1<<6)) | (bit<<6);
}

void Cpu::setH(uint8_t bit){
    bit = (bit == 0 ? 0: 1);
    F = (F & ~(1<<5)) | (bit<<5);
}
void Cpu::setC(uint8_t bit){
    bit = (bit == 0 ? 0: 1);
    F = (F & ~(1<<4)) | (bit<<4);
}




reg8 Cpu::incrementR8(reg8 reg){
    setN(0);

    if((reg + 1)== 0){
        setZ(1);
        }
    if(((reg & 0xF) + 1) > 0xF){//half carry (check for carry on the 3rd bit)
        setH(1);
    } 
        
    return reg++;
}


reg8 Cpu::decrementR8(reg8 reg){
    setN(1);
    if((reg - 1) == 0){
        setZ(1);
    }
    if((B & 0xF) > 1){//half carry
        setH(1);
    }

    return reg--;
}



void Cpu::tick(){

}

