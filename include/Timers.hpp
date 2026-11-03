#pragma once
#include <cstdint>
#include "Memory.hpp"

class Timers{



    public:
        uint8_t overflow = 0;
        uint16_t sys_clk = 0xAB00;
        uint16_t rates = 1024;
        uint16_t v = 1;
        uint8_t previous_and_result;
        uint8_t and_result;
        Memory *_memory;
        Timers(Memory *mem);
        void tick();
        void incrementTimers();
        void incrementTIMA();
        uint8_t fallingEdge();

    
};