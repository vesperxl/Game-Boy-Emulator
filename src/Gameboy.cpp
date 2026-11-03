#include "Memory.hpp"
#include "Cpu.hpp"
#include "Timers.hpp"
#include "Gameboy.hpp"

Gameboy::Gameboy(): _memory(std::make_unique<Memory>()),
                    _timers(_memory.get()),
                    _cpu(_memory.get(),&_timers)
{

};


void Gameboy::run(){

    

    while(1){

            uint8_t opcode = _cpu.fetch();

            _cpu.handleInterrupts();

            _cpu.execute(opcode);

    }
}