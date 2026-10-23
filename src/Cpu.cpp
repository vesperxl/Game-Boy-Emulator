#include<cstdint>
#include <memory>
#include <iostream>

#include "Cpu.hpp"
#include"Memory.hpp"


void Cpu::step(){

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
        _memory->write(BC,A);
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
        _memory->write(adress,data);
        data = getFirstByte(SP);
        adress++;
        _memory->write(adress,data);
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
        A = _memory->read(setTo16(B,C));
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
        _memory->write(DE,A);
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
        A = _memory->read(setTo16(D,E));
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
        _memory->write(HL,A);
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
        A = _memory->read(HL);
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
    case 0x30:
    {
        int8_t e = fetch();
        if(!getC()){
            PC += e;
            tick();
        }
        break;
    }
    case 0x31:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();

        uint16_t data = setTo16(hi,lo);
        SP = data;

        break;
    }
    case 0x32:
    {
        reg16 HL = setTo16(H,L);
        _memory->write(HL,A);
        HL--;
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        break;
    }
    case 0x33:
        SP++;
        tick();
        break;
    case 0x34:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        incrementR8(data);
        _memory->write(setTo16(H,L),data);
        break;
    }
    case 0x35:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        decrementR8(data);
        _memory->write(setTo16(H,L),data);
        break;
    }
    case 0x36:
    {
        uint8_t data = fetch();
        _memory->write(setTo16(H,L),data);
        break;
    }
    case 0x37:
        setC(1);
        break;        
    case 0x38:
    {
        uint8_t e = fetch();
        if(getC()){
            PC += e;
            tick();
        }
        break;
    }
    case 0x39:
    {
        reg16 HL = setTo16(H,L);
        HL = addR16(SP,HL);
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        tick();
        break;
    }
    case 0x3A:
    {
        reg16 HL = setTo16(H,L);
        A = _memory->read(HL);
        HL--;
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        break;
    }
    case 0x3B:
        SP--;
        tick();
        break;
    case 0x3C:
        A = incrementR8(A);
        break;
    case 0x3D:
        A = decrementR8(A);
        break;
    case 0x3E:
        A = fetch();
        break;
    case 0x3F:
        setN(0);
        setH(0);
        if(getC()){
            setC(0);
        }else{
            setC(1);
        }
        break;
    case 0x40:
        //B = B;
        break;
    case 0x41:
        B = C;
        break;
    case 0x42:
        B = D;
        break;
    case 0x43:
        B = E;
        break;
    case 0x44:
        B = H;
        break;
    case 0x45:
        B = L;
        break;
    case 0x46:
        B = _memory->read(setTo16(H,L));
        break;
    case 0x47:
        B = A;
        break;
    case 0x48:
        C = B;
        break;
    case 0x49:
        //C = C;
        break;
    case 0x4A: 
        C = D;
        break;
    case 0x4B:
        C = E;
        break;
    case 0x4C:
        C = H;
        break;
    case 0x4D:
        C = L;
        break;
    case 0x4E:
        C = _memory->read(setTo16(H,L));
        break;
    case 0x4F:
        C = A;
        break;
    case 0x50:
        D = B;
        break;
    case 0x51:
        D = C;
        break;
    case 0x52:
        //D = D;
        break;
    case 0x53:
        D = E;
        break;
    case 0x54:
        D = H;
        break;
    case 0x55:
        D = L;
        break;
    case 0x56:
        D = _memory->read(setTo16(H,L));
        break;
    case 0x57:
        D = A;
        break;
    case 0x58:
        E = B;
        break;
    case 0x59:
        E = C;
        break;
    case 0x5A:
        E = D;
        break;
    case 0x5B:
        //E = E;
        break;
    case 0x5C:
        E = H;
        break;
    case 0x5D:
        E = L;
        break;
    case 0x5E:
        E = _memory->read(setTo16(H,L));
        break;
    case 0x5F:
        E = A;
        break;
    case 0x60:
        H = B;
        break;
    case 0x61:
        H = C;
        break;
    case 0x62:
        H = D;
        break;
    case 0x63:
        H = E;
        break;
    case 0x64:
        //H = H;
        break;
    case 0x65:
        H = L;
        break;
    case 0x66:
        H = _memory->read(setTo16(H,L));
        break;
    case 0x67:
        H = A;
        break;
    case 0x68:
        L = B;
        break;
    case 0x69:
        L = C;
        break;
    case 0x6A:
        L = D;
        break;
    case 0x6B:
        L = E;
        break;
    case 0x6C:
        L = H;
        break;
    case 0x6D:
        //L = L;
        break;
    case 0x6E:
        L = _memory->read(setTo16(H,L));
        break;
    case 0x6F:
        L = A;
        break;
    case 0x70:
        _memory->write(setTo16(H,L), B);
        break;
    case 0x71:
        _memory->write(setTo16(H,L), C);
        break;
    case 0x72:
        _memory->write(setTo16(H,L), D);
        break;
    case 0x73:
        _memory->write(setTo16(H,L), E);
        break;
    case 0x74:
        _memory->write(setTo16(H,L), H);
        break;
    case 0x75:
        _memory->write(setTo16(H,L), L);
        break;
    case 0x76:
        //TODO
        //TODO
        //TODO
        //TODO
        //TODO
        //TODO
        //TODO
        //TODO
        //TODO 
        break;
    case 0x77:
        _memory->write(setTo16(H,L), A);
        break;
    case 0x78:  
        A = B;
        break;
    case 0x79:
        A = C;
        break;
    case 0x7A:
        A = D;
        break;
    case 0x7B:
        A = E;
        break;
    case 0x7C:
        A = H;
        break;
    case 0x7D:
        A = L;
        break;
    case 0x7E:
        A = _memory->read(setTo16(H,L));
        break;
    case 0x7F:
        //A = A; 
        break;
    case 0x80:
        A = addR8(A,B);
        break;
    case 0x81:
        A = addR8(A,C);
        break;
    case 0x82:
        A = addR8(A,D);
        break;
    case 0x83:
        A = addR8(A,E);
        break;
    case 0x84:
        A = addR8(A,H);
        break;
    case 0x85:
        A = addR8(A,L);
        break;
    case 0x86:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        A = addR8(A,data);
        break;
    }
    case 0x87:
        A = addR8(A,A);
        break;
    case 0x88:
        A = adcR8(A,B);
        break;
    case 0x89:
        A = adcR8(A,C);
        break;
    case 0x8A:
        A = adcR8(A,D);
        break;
    case 0x8B:
        A = adcR8(A,E);
        break;
    case 0x8C:
        A = adcR8(A,H);
        break;
    case 0x8D:
        A = adcR8(A,L);
        break;
    case 0x8E:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        A = adcR8(A,data);
                break;
    }
    case 0x8F:
        A = adcR8(A,A);
        break;
    case 0x90:
        A = subR8(A,B);
        break;
    case 0x91:
        A = subR8(A,C);
        break;
    case 0x92:
        A = subR8(A,D);
        break;
    case 0x93:
        A = subR8(A,E);
        break;
    case 0x94:
        A = subR8(A,H);
        break;
    case 0x95:  
        A = subR8(A,L);
        break;
    case 0x96:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        A = subR8(A,data);
        break;
    }
    case 0x97:
        A = subR8(A,A);
        break;
    case 0x98:
        A = sbcR8(A,B);
        break;
    case 0x99:
        A = sbcR8(A,C);
        break;
    case 0x9A:
        A = sbcR8(A,D);
        break;
    case 0x9B:
        A = sbcR8(A,E);
        break;
    case 0x9C:
        A = sbcR8(A,H);
        break;
    case 0x9D:
        A = sbcR8(A,L);
        break;
    case 0x9E:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        A = sbcR8(A,data);
        break;
    }
    case 0x9F:
        A = sbcR8(A,A);
        break;
    case 0xA0:
        A = andR8(A,B);
        break;
    case 0xA1:
        A = andR8(A,C);
        break;
    case 0xA2:
        A = andR8(A,D);
        break;
    case 0xA3:
        A = andR8(A,E);
        break;
    case 0xA4:
        A = andR8(A,H);
        break;
    case 0xA5:
        A = andR8(A,L);
        break;
    case 0xA6:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        A = andR8(A,data);
        break;
    }
    case 0xA7:
        A = andR8(A,A);
        break;
    case 0xA8:
        A = xorR8(A,B);
        break;
    case 0xA9:
        A = xorR8(A,C);
        break;
    case 0xAA:
        A = xorR8(A,D);
        break;
    case 0xAB:
        A = xorR8(A,E);
        break;
    case 0xAC:
        A = xorR8(A,H);
        break;
    case 0xAD:
        A = xorR8(A,L);
        break;
    case 0xAE:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        A = xorR8(A,data);
        break;
    }
    case 0xAF:
        A = xorR8(A,A);
        break;
    case 0xB0:
        A = orR8(A,B);
        break;
    case 0xB1:
        A = orR8(A,C);
        break;
    case 0xB2:
        A = orR8(A,D);
        break;
    case 0xB3:
        A = orR8(A,E);
        break;
    case 0xB4:
        A = orR8(A,H);
        break;
    case 0xB5:
        A = orR8(A,L);
        break;
    case 0xB6:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        A = orR8(A,data);
        break;
    }
    case 0xB7:
        A = orR8(A,A);
        break;
    case 0xB8:
        subR8(A,B); //Compare -> basically a SUB operation to set the flags but doesnt save results
        break;
    case 0xB9:
        subR8(A,C);
        break;
    case 0xBA:
        subR8(A,D);
        break;
    case 0xBB:
        subR8(A,E);
        break;
    case 0xBC:
        subR8(A,H);
        break;
    case 0xBD:
        subR8(A,L);
        break;
    case 0xBE:
    {
        uint8_t data = _memory->read(setTo16(H,L));
        subR8(A,data);
        break;
    }
    case 0xBF:
        subR8(A,A);
        break;
    case 0xC0:
    {
        tick();
        if(!getZ()){
            PC = popR16();
            tick();
        }
        break;
    }
    case 0xC1:
    {
        reg16 BC = popR16();
        B = getFirstByte(BC);
        C = getSecondByte(BC);
        break;
    }
    case 0xC2:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if(!getZ()){
            PC = adress;
            tick();
        }
        break;
    }
    case 0xC3:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        PC = adress;
        tick();
        break;
    }
    case 0xC4:
    {
        uint8_t lo = fetch();      
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if (!getZ())
        {
            uint8_t hiPC = getFirstByte(PC);
            uint8_t loPC = getSecondByte(PC);
            pushR16(hiPC,loPC);   
            PC = adress;
            tick();
        }
        break;
    }
    case 0xC5:
        pushR16(B,C);
        tick();
        break;
    case 0xC6:
    {
        uint8_t data = fetch();
        A = addR8(A,data);
        break;
    }
    case 0xC7:
    {
        rst(0x00);
        break;
    }
    case 0xC8:
    {
        tick();
        if(getZ()){

            PC = popR16();
            tick();
        }
        break;
    }
    case 0xC9:
    {
        PC = popR16();
        tick();
        break;
    }
    case 0xCA:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if(getZ()){
            PC = adress;
            tick();
        }
        break;
    }
    case 0xCC:  
    {
        uint8_t lo = fetch();      
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if (getZ())
        {
            uint8_t hiPC = getFirstByte(PC);
            uint8_t loPC = getSecondByte(PC);
            pushR16(hiPC,loPC);   
            PC = adress;
            tick();
        }
        break;
    }
    case 0xCD:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        uint8_t hiPC = getFirstByte(PC);
        uint8_t loPC = getSecondByte(PC);
        pushR16(hiPC,loPC);   
        PC = adress;
        tick();
        break;
    }
    case 0xCE:
    {
        uint8_t data = fetch();
        A = addR8(A,data);
        break;
    }
    case 0xCF:  
        rst(0x08);
        break;
    case 0xD0:
        tick();
        if(!getC()){
            PC = popR16(); 
        }
        break;
    case 0xD1:
    {
        reg16 DE = popR16();
        D = getFirstByte(DE);
        E = getSecondByte(DE);
        break;
    }
    case 0xD2:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if(!getC()){
            PC = adress;
            tick();
        }
        break;
    }
    case 0xD4:
    {
        uint8_t lo = fetch();      
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if (!getC())
        {
            uint8_t hiPC = getFirstByte(PC);
            uint8_t loPC = getSecondByte(PC);
            pushR16(hiPC,loPC);   
            PC = adress;
            tick();
        }
        break;
    }
    case 0xD5:
        pushR16(D,E);
        tick();
        break;
    case 0xD6:
    {
        uint8_t data = fetch();
        A = subR8(A,data);
        break;
    }
    case 0xD7:
        rst(0x10);
        break;
    case 0xD8:
        tick();
        if(getC()){

            PC = popR16();
            tick();
        }
        break;
    case 0xD9:
    {
        ime_flag = 1;
        PC = popR16();
        tick();
        break;
    }       
    case 0xDA:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if(getC()){
            PC = adress;
            tick();
        }
        break;
    }
    case 0xDC:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        if(getC()){
            uint8_t hiPC = getFirstByte(PC);
            uint8_t loPC = getSecondByte(PC);
            pushR16(hiPC,loPC);   
            PC = adress;
            tick();
        }
        break;
    }
    case 0xDE:
    {
        uint8_t data = fetch();
        A = sbcR8(A,data);
        break;
    }
    case 0xDF:
        rst(0x18);
        break;
    case 0xE0:
    {
        uint8_t lo = fetch();
        uint16_t adress = setTo16(0xFF, lo);
        _memory->write(adress,A);
        break;
    }
    case 0xE1:
    {
        reg16 HL = popR16();
        H = getFirstByte(HL);
        L = getSecondByte(HL);
        break;
    }
    case 0xE2:
    {
        uint16_t adress = setTo16(0xFF,C);
        _memory->write(adress,A);
        break;
    }
    case 0xE5:  
        pushR16(H,L);
        tick();
        break;
    case 0xE6:
    {
        uint8_t data = fetch();
        A = andR8(A,data);
        break;
    }
    case 0xE7:
        rst(0x20);
        break;
    case 0xE8:
    {
        int8_t e = fetch();
        setZ(0);
        setN(0);

        if(((SP & 0xF) + (e & 0xF)) > 0xF){
            setH(1);
        }

        if(((SP & 0xFF) + e) > 0xFF){
            setC(1);
        }

        SP += e;
        tick();
        tick();
        break;

    }
    case 0xE9:
        PC = setTo16(H,L);
        break;
    case 0xEA:
    {
        uint8_t lo = fetch();
        uint8_t hi = fetch();
        uint16_t adress = setTo16(hi,lo);
        _memory->write(adress,A);
        break;
    }
    case 0xEE:
    {
        uint8_t data = fetch();
        A = xorR8(A,data);
        break;
    }
    case 0xEF:
        rst(0x28);
        break;
    case 0xF0:
    {
        uint8_t lo = fetch();
        uint16_t adress = setTo16(0xFF,lo);
        A = _memory->read(adress);
        break;
    }
    case 0xF1:
    {
        reg16 AF = popR16();
        A = getFirstByte(AF);
        F = getSecondByte(AF);
        break;
    }
    case 0xF2:
    {
        uint16_t adress = setTo16(0xFF,C);
        A = _memory->read(adress);
        break;
    }
    case 0xF3:
        ime_flag = 0;
        ime_next = 0;
        break;
    case 0xF5:  
    {
        pushR16(A,F);
        tick();
        break;
    }
    case 0xF6:
    {
        uint8_t data = fetch();
        A = orR8(A,data);
        break;
    }
    case 0xF7:
        rst(0x30);
        break;
    case 0xF8:
    {
        int8_t e = fetch();
        setZ(0);
        setN(0);

        if(((SP & 0xF) + (e & 0xF)) > 0xF){
            setH(1);
        }

        if(((SP & 0xFF) + e) > 0xFF){
            setC(1);
        }

        SP += e;
        tick();
        
        H = getFirstByte(SP);
        L = getSecondByte(SP);
        break;
    }
    case 0xF9:
        SP = setTo16(H,L);
        break;
    case 0xFA:
    {
        uint8_t lo = fetch();
        uint8_t hi= fetch();
        uint16_t adress = setTo16(hi,lo);
        A = _memory->read(adress);
        break;
    }
    case 0xFB:
        ime_next = 1;
        break;
    case 0xFE:
    {
        uint8_t data = fetch();
        subR8(A,data);
        break;
    }
    case 0xFF:
        rst(0x38);
        break;
    case 0xCB:
    {
        opcode = fetch();
        switch(opcode){
            case 0x00:
                B = rlcR8(B);
                break;
            case 0x01:
                C = rlcR8(C);
                break;          
            case 0x02:
                D = rlcR8(D);
                break;
            case 0x03:
                E = rlcR8(E);
                break;
            case 0x04:
                H = rlcR8(H);
                break;
            case 0x05:
                L = rlcR8(L);
                break;
            case 0x06:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = rlcR8(data);
                _memory->write(setTo16(H,L),data);
                break;
            }
            case 0x07:
                A = rlcR8(A);
                break;
            case 0x08:
                B = rrcR8(B);
                break;
            case 0x09:
                C = rrcR8(C);
                break;
            case 0x0A:
                D = rrcR8(D);
                break;
            case 0x0B:
                E = rrcR8(E);
                break;
            case 0x0C:
                H = rrcR8(H);
                break;
            case 0x0D:
                L = rrcR8(L);
                break;
            case 0x0E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = rrcR8(data);
                _memory->write(setTo16(H,L), data);
                break;
            }
            case 0x0F:
                A = rrcR8(A);
                break;
            case 0x10:
                B = rlR8(B);
                break;
            case 0x11:
                C = rlR8(C);
                break;
            case 0x12:
                D = rlR8(D);
                break;
            case 0x13:
                E = rlR8(E);
                break;
            case 0x14:
                H = rlR8(H);
                break;
            case 0x15:
                L = rlR8(L);
                break;
            case 0x16:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = rlR8(data);
                _memory->write(setTo16(H,L), data);
                break;
            }
            case 0x17:
                A = rlR8(A);
                break;
            case 0x18:
                B = rrR8(B);
                break;
            case 0x19:
                C = rrR8(C);
                break;
            case 0x1A:
                D = rrR8(D);
                break;
            case 0x1B:  
                E = rrR8(E);
                break;
            case 0x1C:
                H = rrR8(H);
                break;
            case 0x1D:
                L = rrR8(L);
                break;
            case 0x1E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = rrR8(data);
                _memory->write(setTo16(H,L),data);
                break;
            }
            case 0x1F:
                A = rrR8(A);
                break;
            case 0x20:
                B = slaR8(B);
                break;
            case 0x21:
                C = slaR8(C);
                break;
            case 0x22:
                D = slaR8(D);
                break;
            case 0x23:
                E = slaR8(E);
                break;
            case 0x24:  
                H = slaR8(H);
                break;
            case 0x25:
                L = slaR8(L);
                break;
            case 0x26:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = slaR8(data);
                _memory->write(setTo16(H,L),data);
                break;
            }
            case 0x27:  
                A = slaR8(A);
                break;
            case 0x28:
                B = sraR8(B);
                break;
            case 0x29:
                C = sraR8(C);
                break;
            case 0x2A:
                D = sraR8(D);
                break;
            case 0x2B:
                E = sraR8(E);
                break;
            case 0x2C:
                H = sraR8(H);
                break;
            case 0x2D:
                L = sraR8(L);
                break;
            case 0x2E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = sraR8(data);
                _memory->write(setTo16(H,L),data);
                break;
            }
            case 0x2F:
                A= sraR8(A);
                break;
            case 0x30:
                B = swapR8(B);
                break;
            case 0x31:
                C = swapR8(C);
                break;
            case 0x32:
                D = swapR8(D);
                break;
            case 0x33:
                E = swapR8(E);
                break;
            case 0x34:
                H = swapR8(H);
                break;
            case 0x35:
                L = swapR8(L);
                break;
            case 0x36:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = swapR8(data);
                _memory->write(setTo16(H,L),data);
                break;
            }
            case 0x37:
                A = swapR8(A);
                break;
            case 0x38:
                B = slrR8(B);
                break;
            case 0x39:
                C = slrR8(C);
                break;
            case 0x3A:
                D = slrR8(D);
                break;
            case 0x3B:
                E = slrR8(E);
                break;
            case 0x3C:
                H = slrR8(L);
                break;
            case 0x3D:
                L = slrR8(L);
                break;
            case 0x3E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                data = slrR8(data);
                _memory->write(setTo16(H,L),data);
                break;
            }
            case 0x3F:
                A = slrR8(A);
                break;
            case 0x40:
                bitU3(0,B);
                break;
            case 0x41:
                bitU3(0,C);
                break;
            case 0x42:
                bitU3(0,D);
                break;
            case 0x43:
                bitU3(0,E);
                break;
            case 0x44:
                bitU3(0,H);
                break;
            case 0x45:
                bitU3(0,L);
                break;
            case 0x46:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(0,data);
                break;
            }
            case 0x47:
                bitU3(0,A);
                break;
            case 0x48:
                bitU3(1,B);
                break;
            case 0x49:
                bitU3(1,C);
                break;
            case 0x4A:
                bitU3(1,D);
                break;
            case 0x4B:
                bitU3(1,E);
                break;
            case 0x4C:
                bitU3(1,H);
                break;
            case 0x4D:
                bitU3(1,L);
                break;
            case 0x4E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(1,data);
                break;
            }
            case 0x4F:
                bitU3(1,A);
                break;
            case 0x50:
                bitU3(2,B);
                break;
            case 0x51:
                bitU3(2,C);
                break;
            case 0x52:
                bitU3(2,D);
                break;
            case 0x53:
                bitU3(2,E);
                break;
            case 0x54:
                bitU3(2,H);
                break;
            case 0x55:
                bitU3(2,L);
                break;
            case 0x56:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(2,data);
                break;
            }
            case 0x57:
                bitU3(2,A);
                break;
            case 0x58:
                bitU3(3,B);
                break;
            case 0x59:
                bitU3(3,C);
                break;
            case 0x5A:
                bitU3(3,D);
                break;
            case 0x5B:
                bitU3(3,E);
                break;
            case 0x5C:
                bitU3(3,H);
                break;
            case 0x5D:
                bitU3(3,L);
                break;
            case 0x5E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(3,data);
                break;
            }
            case 0x5F:
                bitU3(3,A);
                break;
            case 0x60:
                bitU3(4,B);
                break;
            case 0x61:
                bitU3(4,C);
                break;
            case 0x62:
                bitU3(4,D);
                break;
            case 0x63:
                bitU3(4,E);
                break;
            case 0x64:
                bitU3(4,H);
                break;
            case 0x65:
                bitU3(4,L);
                break;
            case 0x66:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(4,data);
                break;
            }
            case 0x67:
                bitU3(4,A);
                break;
            case 0x68:
                bitU3(5,B);
                break;
            case 0x69:
                bitU3(5,C);
                break;
            case 0x6A:
                bitU3(5,D);
                break;
            case 0x6B:
                bitU3(5,E);
                break;
            case 0x6C:
                bitU3(5,H);
                break;
            case 0x6D:
                bitU3(5,L);
                break;
            case 0x6E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(5,data);
                break;
            }
            case 0x6F:
                bitU3(5,A);
                break;
            case 0x70:
                bitU3(6,B);
                break;
            case 0x71:
                bitU3(6,C);
                break;
            case 0x72:
                bitU3(6,D);
                break;
            case 0x73:
                bitU3(6,E);
                break;
            case 0x74:
                bitU3(6,H);
                break;
            case 0x75:
                bitU3(6,L);
                break;
            case 0x76:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(6,data);
                break;
            }
            case 0x77:
                bitU3(6,A);
                break;
            case 0x78:
                bitU3(7,B);
                break;
            case 0x79:
                bitU3(7,C);
                break;
            case 0x7A:
                bitU3(7,D);
                break;
            case 0x7B:
                bitU3(7,E);
                break;
            case 0x7C:
                bitU3(7,H);
                break;
            case 0x7D:
                bitU3(7,L);
                break;
            case 0x7E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                bitU3(7,data);
                break;
            }
            case 0x7F:
                bitU3(7,A);
                break;
            case 0x80:
                resU3(0,B);
                break;
            case 0x81:
                resU3(0,C);
                break;
            case 0x82:
                resU3(0,D);
                break;
            case 0x83:
                resU3(0,E);
                break;
            case 0x84:
                resU3(0,H);
                break;
            case 0x85:
                resU3(0,L);
                break;
            case 0x86:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(0,data);
                break;
            }
            case 0x87:
                resU3(0,A);
                break;
            case 0x88:
                resU3(1,B);
                break;
            case 0x89:
                resU3(1,C);
                break;
            case 0x8A:
                resU3(1,D);
                break;
            case 0x8B:
                resU3(1,E);
                break;
            case 0x8C:
                resU3(1,H);
                break;
            case 0x8D:
                resU3(1,L);
                break;
            case 0x8E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(1,data);
                break;
            }
            case 0x8F:
                resU3(1,A);
                break;
            case 0x90:
                resU3(2,B);
                break;
            case 0x91:
                resU3(2,C);
                break;
            case 0x92:
                resU3(2,D);
                break;
            case 0x93:
                resU3(2,E);
                break;
            case 0x94:
                resU3(2,H);
                break;
            case 0x95:
                resU3(2,L);
                break;
            case 0x96:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(2,data);
                break;
            }
            case 0x97:
                resU3(2,A);
                break;
            case 0x98:
                resU3(3,B);
                break;
            case 0x99:
                resU3(3,C);
                break;
            case 0x9A:
                resU3(3,D);
                break;
            case 0x9B:
                resU3(3,E);
                break;
            case 0x9C:
                resU3(3,H);
                break;
            case 0x9D:
                resU3(3,L);
                break;
            case 0x9E:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(3,data);
                break;
            }
            case 0x9F:
                resU3(3,A);
                break;
            case 0xA0:
                resU3(4,B);
                break;
            case 0xA1:
                resU3(4,C);
                break;
            case 0xA2:
                resU3(4,D);
                break;
            case 0xA3:
                resU3(4,E);
                break;
            case 0xA4:
                resU3(4,H);
                break;
            case 0xA5:
                resU3(4,L);
                break;
            case 0xA6:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(4,data);
                break;
            }
            case 0xA7:
                resU3(4,A);
                break;
            case 0xA8:
                resU3(5,B);
                break;
            case 0xA9:
                resU3(5,C);
                break;
            case 0xAA:
                resU3(5,D);
                break;
            case 0xAB:
                resU3(5,E);
                break;
            case 0xAC:
                resU3(5,H);
                break;
            case 0xAD:
                resU3(5,L);
                break;
            case 0xAE:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(5,data);
                break;
            }
            case 0xAF:
                resU3(5,A);
                break;
            case 0xB0:
                resU3(6,B);
                break;
            case 0xB1:
                resU3(6,C);
                break;
            case 0xB2:
                resU3(6,D);
                break;
            case 0xB3:
                resU3(6,E);
                break;
            case 0xB4:
                resU3(6,H);
                break;
            case 0xB5:
                resU3(6,L);
                break;
            case 0xB6:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(6,data);
                break;
            }
            case 0xB7:
                resU3(6,A);
                break;
            case 0xB8:
                resU3(7,B);
                break;
            case 0xB9:
                resU3(7,C);
                break;
            case 0xBA:
                resU3(7,D);
                break;
            case 0xBB:
                resU3(7,E);
                break;
            case 0xBC:
                resU3(7,H);
                break;
            case 0xBD:
                resU3(7,L);
                break;
            case 0xBE:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                resU3(7,data);
                break;
            }
            case 0xBF:
                resU3(7,A);
                break;
            case 0xC0:
                setU3(0,B);
                break;
            case 0xC1:
                setU3(0,C);
                break;
            case 0xC2:
                setU3(0,D);
                break;
            case 0xC3:
                setU3(0,E);
                break;
            case 0xC4:
                setU3(0,H);
                break;
            case 0xC5:
                setU3(0,L);
                break;
            case 0xC6:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(0,data);
                break;
            }
            case 0xC7:
                setU3(0,A);
                break;
            case 0xC8:
                setU3(1,B);
                break;
            case 0xC9:
                setU3(1,C);
                break;
            case 0xCA:
                setU3(1,D);
                break;
            case 0xCB:
                setU3(1,E);
                break;
            case 0xCC:
                setU3(1,H);
                break;
            case 0xCD:
                setU3(1,L);
                break;
            case 0xCE:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(1,data);
                break;
            }
            case 0xCF:
                setU3(1,A);
                break;
            case 0xD0:
                setU3(2,B);
                break;
            case 0xD1:
                setU3(2,C);
                break;
            case 0xD2:
                setU3(2,D);
                break;
            case 0xD3:
                setU3(2,E);
                break;
            case 0xD4:
                setU3(2,H);
                break;
            case 0xD5:
                setU3(2,L);
                break;
            case 0xD6:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(2,data);
                break;
            }
            case 0xD7:
                setU3(2,A);
                break;
            case 0xD8:
                setU3(3,B);
                break;
            case 0xD9:
                setU3(3,C);
                break;
            case 0xDA:
                setU3(3,D);
                break;
            case 0xDB:
                setU3(3,E);
                break;
            case 0xDC:
                setU3(3,H);
                break;
            case 0xDD:
                setU3(3,L);
                break;
            case 0xDE:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(3,data);
                break;
            }
            case 0xDF:
                setU3(3,A);
                break;
            case 0xE0:
                setU3(4,B);
                break;
            case 0xE1:
                setU3(4,C);
                break;
            case 0xE2:
                setU3(4,D);
                break;
            case 0xE3:
                setU3(4,E);
                break;
            case 0xE4:
                setU3(4,H);
                break;
            case 0xE5:
                setU3(4,L);
                break;
            case 0xE6:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(4,data);
                break;
            }
            case 0xE7:
                setU3(4,A);
                break;
            case 0xE8:
                setU3(5,B);
                break;
            case 0xE9:
                setU3(5,C);
                break;
            case 0xEA:
                setU3(5,D);
                break;
            case 0xEB:
                setU3(5,E);
                break;
            case 0xEC:
                setU3(5,H);
                break;
            case 0xED:
                setU3(5,L);
                break;
            case 0xEE:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(5,data);
                break;
            }
            case 0xEF:
                setU3(5,A);
                break;
            case 0xF0:
                setU3(6,B);
                break;
            case 0xF1:
                setU3(6,C);
                break;
            case 0xF2:
                setU3(6,D);
                break;
            case 0xF3:
                setU3(6,E);
                break;
            case 0xF4:
                setU3(6,H);
                break;
            case 0xF5:
                setU3(6,L);
                break;
            case 0xF6:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(6,data);
                break;
            }
            case 0xF7:
                setU3(6,A);
                break;
            case 0xF8:
                setU3(7,B);
                break;
            case 0xF9:
                setU3(7,C);
                break;
            case 0xFA:
                setU3(7,D);
                break;
            case 0xFB:
                setU3(7,E);
                break;
            case 0xFC:
                setU3(7,H);
                break;
            case 0xFD:
                setU3(7,L);
                break;
            case 0xFE:
            {
                uint8_t data = _memory->read(setTo16(H,L));
                setU3(7,data);
                break;
            }
            case 0xFF:
                setU3(7,A);
                break;
        }
        

    }
    default:
        break;
    }


}

Cpu::Cpu(Memory *mem) : _memory(mem){
   
}

uint8_t Cpu::fetch(){
    
    uint8_t data = _memory->read(PC);
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

    if((uint8_t)(reg + 1) == 0){
        setZ(1);
        }else{
            setZ(0);
        }
    if(((reg & 0xF) + 1) > 0xF){//half carry (check for carry on the 3rd bit)
        setH(1);
    } else{
        setH(0);
    }
    
    return ++reg;
}


reg8 Cpu::decrementR8(reg8 reg){
    setN(1);
    if((uint8_t)(reg - 1) == 0){
        setZ(1);
    }else{
        setZ(0);
    }
    if((reg & 0xF) < 1){
        setH(1);
    }else{
        setH(0);
    }

    return --reg;
}

reg16 Cpu::addR16(reg16 regA, reg16 regB){
    uint32_t result = regA + regB;

        setN(0);
        if(((regA & 0xFFF) + (regB & 0xFFF)) > 0xFFF){
            setH(1);
        }else{
            setH(0);
        }
        if(result > 0xFFFF){
            setC(1);
        }else{
            setC(0);
        }

        return regA + regB;
}

reg8 Cpu::addR8(reg8 regA, reg8 regB){

    setN(0);

    if((uint8_t)(regA + regB) == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    if(((regA & 0xF) + (regB & 0xF)) > 0xF){
        setH(1);
    } else{
        setH(0);
    }

    if(regA + regB > 0xFF){
        setC(1);
    }else{
        setC(0);
    }

    return regA + regB;

}

reg8 Cpu::adcR8(reg8 regA, reg8 regB){

    setN(0);

    if((uint8_t)(regA + regB + getC()) == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    if(((regA & 0xF) + (regB & 0xF)) + getC() > 0xF){
        setH(1);
    } else{
        setH(0);
    }

    if(regA + regB + getC() > 0xFF){
        setC(1);
    }else{
        setC(0);
    }

    return regA + regB + getC();

}

reg8 Cpu::subR8(reg8 regA, reg8 regB){

    setN(1);

    if((uint8_t)(regA - regB) == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    if((regB & 0xF) > (regA & 0xF)){
        setH(1);
    } else{
        setH(0);
    }

    if(regB > regA){
        setC(1);
    }else{
        setC(0);
    }

    return regA - regB;
}

reg8 Cpu::sbcR8(reg8 regA, reg8 regB){

    setN(1);

    if((uint8_t)(regA - regB - getC())== 0){
        setZ(1);
    }else{
        setZ(0);
    }

    if(((regB & 0xF) + getC()) > (regA & 0xF)){
        setH(1);
    } else{
        setH(0);
    }

    if((regB + getC()) > regA){
        setC(1);
    }else{
        setC(0);
    }

    return regA - regB - getC();
}

reg8 Cpu::andR8(reg8 regA, reg8 regB){
    setN(0);
    setH(1);
    setC(0);

    if((regA & regB) == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    return (regA & regB);
}

reg8 Cpu::xorR8(reg8 regA, reg8 regB){
    setN(0);
    setH(0);
    setC(0);

    if((regA ^ regB) == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    return (regA ^ regB);
}

reg8 Cpu::orR8(reg8 regA, reg8 regB){
    setN(0);
    setH(0);
    setC(0);

    if((regA | regB) == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    return (regA | regB);
}

reg16 Cpu::popR16(){
    uint8_t lo = _memory->read(SP);
    SP++;
    uint8_t hi = _memory->read(SP);
    SP++;

    return setTo16(hi,lo);
}

void Cpu::pushR16(reg8 hi, reg8 lo){
    SP--;
    _memory->write(SP,hi);
    SP--;
    _memory->write(SP,lo);
}

void Cpu::rst(uint16_t vector){

    uint8_t hiPC = getFirstByte(PC);
    uint8_t loPC = getSecondByte(PC);
    pushR16(hiPC,loPC);   
    PC = vector;
    tick();
}

reg8 Cpu::rlcR8(reg8 reg){
 
    uint8_t bit = reg >> 7;
    setC(bit);
    setN(0);
    setH(0);

    reg = (reg << 1) | bit;
    if(reg == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    return reg;
}

reg8 Cpu::rrcR8(reg8 reg){
    uint8_t bit = (reg & 1);
    setC(bit);
    setN(0);
    setH(0);
    reg = (reg >> 1) | (bit << 7);
    if(reg == 0){
        setZ(1);
    }else{setZ(0);}
    return reg;
}

reg8 Cpu::rlR8(reg8 reg){
    uint8_t bit = (reg >> 7);
    setN(0);
    setH(0);
    uint8_t new_bit = getC();
    reg = (reg << 1) | new_bit;
    if(reg == 0){
        setZ(1);
    }else{
        setZ(0);
    }
    
    setC(bit);

    return reg;
}

reg8 Cpu::rrR8(reg8 reg)
{
    uint8_t bit = reg & 1;
    setN(0);
    setH(0);
    uint8_t new_bit = getC();
    reg = (reg >> 1) | (new_bit << 7);
    if(reg == 0){
        setZ(1);
    }else{
        setZ(0);
    }
    setC(bit);

    return reg;
}

reg8 Cpu::slaR8(reg8 reg)
{
    setN(0);
    setH(0);
    uint8_t bit = reg >> 7;
    setC(bit);
    reg = reg << 1;
    if(reg == 0){
        setZ(1);
    }else{
        setZ(0);
    }
    return reg;
}

reg8 Cpu::sraR8(reg8 reg)
{
    setN(0);
    setH(0);
    uint8_t bit = reg & 1;
    setC(bit);
    bit = reg >> 7;
    reg = (bit << 7) | (reg >> 1);
    if(reg == 0){
        setZ(1);
    }else{
        setZ(0);
    }
    return reg;
}

reg8 Cpu::swapR8(reg8 reg){
    setN(0);
    setH(0);
    setC(0);
    
    uint8_t upper = (reg >> 4);
    uint8_t lower = (reg & 0xF);
    reg = (lower << 4) | upper;

    if(reg == 0){
        setZ(1);
    }else{
        setZ(0);
    }
    return reg;
}

reg8 Cpu::slrR8(reg8 reg){
    setN(0);
    setH(0);
    uint8_t bit = reg & 1;
    setC(bit);

    reg = reg >> 1;
    if(reg == 0){
        setZ(1);
    }else{
        setZ(0);
    }

    return reg;
}

void Cpu::bitU3(uint8_t bit, reg8 reg){
    setN(0);
    setH(1);

    if(((reg >> bit) & 1) == 0){
        setZ(1);
    }else{
        setZ(0);
    }
}

reg8 Cpu::resU3(uint8_t bit, reg8 reg){
    reg = (reg & ~(1<<bit));
    return reg; 
}

reg8 Cpu::setU3(uint8_t bit, reg8 reg){
    reg = (reg | (1<<bit));
    return reg; 
}
void Cpu::tick(){

}

void Cpu::reset(){
    A = 0x01;
    F = 0b0000;
    B = 0xFF;
    C = 0x13;
    D = 0x00;
    E = 0xC1;
    H = 0x84;
    L = 0x03;
    PC = 0x100;
    SP = 0xFFFE;
    ime_flag = 0;
    ime_next = 0;
}

