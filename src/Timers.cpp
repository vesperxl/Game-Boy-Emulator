#include "Timers.hpp"
#include "Memory.hpp"


Timers::Timers(Memory* mem) : _memory(mem){
    
}


void Timers::tick(){
    incrementTimers();
}

void Timers::incrementTimers(){
    sys_clk+= 4;
    _memory->setDIV(sys_clk >> 8);
    incrementTIMA();

}

void Timers::incrementTIMA(){
    if(overflow == 0){
        if(fallingEdge()){
            if(_memory->TIMA() == 0xFF){
            _memory->setTIMA(0);
            overflow = 1;
            }else{
                _memory->setTIMA(_memory->TIMA() + 1);
                overflow = 0;
            }
        }

    }else{
        _memory->setTIMA(_memory->TMA());
        _memory->write(0xFF0F,(_memory->IF() | 2));
        overflow = 0;
    }
    
}

uint8_t Timers::fallingEdge(){
    uint8_t tac = _memory->TAC() & 0b11;
    uint8_t bit_pos = tac == 0b00 ? 9 : tac == 0b01 ? 3 : tac == 0b10 ? 5 : 7;
    uint8_t clk_bit = (sys_clk >> bit_pos) & 1; 
    uint8_t te_bit = (_memory->TAC() & 0b100 ) >> 2;
    and_result = clk_bit & te_bit;

    if((previous_and_result == 1) && (and_result == 0)){
        previous_and_result = and_result;
        return 1;
    }

    previous_and_result = and_result;
    return 0;
    
}
