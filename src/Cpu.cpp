#include<cstdint>
#include <memory>

#include "Cpu.hpp"
#include"Memory.hpp"


void Cpu::execute(){

    uint8_t opcode = fetch();

    switch (opcode & 0xFF)
    {
    case 0x00:
        break;
    case 0x01:
        C = fetch();
        B = fetch();
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
        B = fetch();
        break;
    case 0x07:
    {
        uint8_t bit = A >> 7;
        setC(bit);
        setZ(0);
        setN(0);
        setH(0);
        A = (A << 1) | bit;
        break;
    }
    case 0x08:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
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
        reg16 BC = setTo16(B,C);
        reg16 HL = setTo16(H,L);
        HL = addR16(BC,HL);
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        
        tick();
        break;
    }
    case 0x0A:
        A = memory.read(setTo16(B,C));
        break;
    case 0x0B:
    {
        uint16_t BC = setTo16(B,C);
        BC--;
        B = getFirstByte(BC);
        C = getSecondByte(BC);
        tick();
        break;
    }
    case 0x0C:
        C = incrementR8(C);
        break;
    case 0x0D:
        C = decrementR8(C);
        break;
    case 0x0E:
        C = fetch();
        break;
    case 0x0F:
    {
        uint8_t bit = (A & 1);
        setC(bit);
        setZ(0);
        setN(0);
        setH(0);
        A = (A >> 1) | (bit << 7);
        break;
    }
    case 0x10:
        //STOP INSTRUCTION (No need to be implemented, no games use it and its behavior in the original GB is not like it should be)
        break;
    case 0x11:
        E = fetch();
        D = fetch();
        break;
    case 0x12:
    {
        uint16_t DE = setTo16(B,C);
        memory.write(DE,A);
        break;
    }
    case 0x13:
    {
        uint16_t DE = setTo16(D,E);
        DE++;
        D = getFirstByte(DE);
        E = getSecondByte(DE);
        tick();
        break;
    }
    case 0x14:
        incrementR8(D);
        break;
    case 0x15:
        decrementR8(D);
        break;
    case 0x16:
        D = fetch();
        break;
    case 0x17:
    {
        uint8_t bit = (A >> 7);
        setZ(0);
        setN(0);
        setH(0);
        uint8_t new_bit = getC();
        A = (A << 1) | new_bit;
        setC(bit);
        break;
    }
    case 0x18:
    {
        int8_t e = fetch();
        PC += e;
        tick();
        break;
    }
    case 0x19:
    {
        reg16 DE = setTo16(D,E);
        reg16 HL = setTo16(H,L);
        HL = addR16(DE, HL);
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        tick();
        break;
    }
    case 0x1A:
        A = memory.read(setTo16(D,E));
        break;
    case 0x1B:
    {
        uint16_t DE = setTo16(D,E);
        DE--;
        D = getFirstByte(DE);
        E = getSecondByte(DE);
        tick();
        break;
    }
    case 0x1C:
        E = incrementR8(E);
        break;
    case 0x1D:
        E = decrementR8(E);
        break;
    case 0x1E:
        E = fetch();
        break;
    case 0x1F:
    {
        uint8_t bit = A & 1;
        setZ(0);
        setN(0);
        setH(0);
        uint8_t new_bit = getC();
        A = (A >> 1) | (new_bit << 7);
        setC(bit);
        break;
    }
    case 0x20:
    {
        int8_t e = fetch();
        if(!getZ()){
            PC += e;
            tick();
        }
        break;
    }
    case 0x21:
        L = fetch();
        H = fetch();
        break;
    case 0x22:
    {
        reg16 HL = setTo16(H,L);
        memory.write(HL,A);
        HL++;
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        break;
    }
    case 0x23:
    {
        reg16 HL = setTo16(H,L);
        HL++;
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        tick();
        break;
    }
    case 0x24:
        H = incrementR8(H);
        break;
    case 0x25:
        H = decrementR8(H);
        break;
    case 0x26:
        H = fetch();
        break;
    case 0x27:
    {
        uint8_t adj;

        if(getN()){
            adj = 0;
            if(getH()){
                adj += 0x6;
            }
            if(getC()){
                adj += 0x60;
            }
            A -= adj;
        }else{
            adj = 0;
            if(getH() || (A & 0xF) > 0x9){
                adj += 0x6;
            }
            if(getC() || A > 0x99){
                adj += 0x60;
                setC(1);
            }
            
            A += adj;
        }
        
        if(A == 0){
            setZ(1);
        }
        setH(0);
        break;
    }
    case 0x28:
    {
        int8_t e = fetch();
        if(getZ()){
            PC += e;
            tick();
        }
        
        break;
    }
    case 0x29:
    {
        reg16 HL = setTo16(H,L);
        HL = addR16(HL,HL);
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        tick();
        break;
    }
    case 0x2A:
    {
        reg16 HL = setTo16(H,L);
        A = memory.read(HL);
        HL++;
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        break;
    }
    case 0x2B:
    {
        reg16 HL = setTo16(H,L);
        HL--;
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        tick();
        break;
    }
    case 0x2C:
        L = incrementR8(L);
        break;
    case 0x2D:
        L = decrementR8(L);
        break;
    case 0x2E:
        L = fetch();
        break;
    case 0x2F:
        A = ~A;
        setN(1);
        setH(1);
        break;
    default:
        break;
    }


}

Cpu::Cpu(Memory &mem) : memory(mem){
   
}

uint8_t Cpu::fetch(){
    
    uint8_t data = memory.read(PC);
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

uint8_t Cpu::getZ(){
    return F >> 7;
}
uint8_t Cpu::getN(){
    return (F & 0x40) >> 6;
}
uint8_t Cpu::getH(){
    return (F & 0x20) >> 5;
}
uint8_t Cpu::getC(){
    return (F & 0x10) >> 7;
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
    if((B & 0xF) > 1){
        setH(1);
    }

    return reg--;
}

reg16 Cpu::addR16(reg16 A, reg16 B){
    uint32_t result = A + B;

        setN(0);
        if(((A & 0xFFF) + (B & 0xFFF)) > 0xFFF){
            setH(1);
        }
        if(result > 0xFFFF){
            setC(1);
        }

        return A + B;
}

void Cpu::tick(){

}

