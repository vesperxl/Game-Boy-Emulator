#pragma once
#include "Memory.hpp"
#include "Cpu.hpp"
#include "Timers.hpp"

class Gameboy{
    private: 
        std::unique_ptr<Memory> _memory;
        Cpu _cpu;
        Timers _timers;
    public:
        Gameboy();
        void run();


};