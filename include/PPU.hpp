#pragma once
#include<cstdint>
#include<array>
#include<queue>
#include<Memory.hpp>


struct fifoPixel{
    uint8_t color_id;
    uint8_t palette;
    uint8_t bg_priority;    
};

class PPU{
    private:
        uint8_t x_pos_counter;
        std::queue<fifoPixel> sprite_fifo;
        std::queue<fifoPixel> bg_fifo;
        Memory *_memory;
        uint16_t getColor(uint8_t palette,uint8_t color_id);
        int dots = 0;
        void oam_scan();
        std::vector<uint32_t> sprite_buffer;
        uint8_t getByte0(uint32_t data);
        uint8_t getByte1(uint32_t data);
        uint8_t getByte2(uint32_t data);
        uint8_t getByte3(uint32_t data);
    public:
        std::array<std::array<uint16_t,144>,160> displayArray;
        PPU(Memory *mem); 
        void pushPixel();
        void step();
};