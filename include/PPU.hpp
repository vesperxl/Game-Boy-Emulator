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
        uint8_t x_pos_fetcher_counter;
        uint8_t window_rendering = 0;
        uint16_t offset;
        uint16_t tilemap;
        uint8_t window_line_counter;
        uint8_t window_in_scanline;
        uint8_t low_tile_data;
        uint8_t high_tile_data;
        uint8_t tile_num;
        uint16_t tile_adress;
        uint32_t curr_sprite;
        uint8_t v = 1;
        uint8_t new_scanline = 1;
        uint8_t mode_condition = 0;
        uint8_t ly_lyc_condition = 0;
        uint8_t first_vblank = 1;
        uint8_t first_oam = 1;
        uint8_t first_hblank = 1;
        uint8_t sprite_in_progress = 0;
        std::deque<fifoPixel> sprite_fifo;
        std::queue<fifoPixel> bg_fifo;
        Memory *_memory;
        uint16_t getColor(uint8_t palette,uint8_t color_id);
        int dots = 1;
        uint8_t wy_ly = 0;
        void oam_scan();
        std::vector<uint32_t> sprite_buffer;
        uint8_t getByte0(uint32_t data);
        uint8_t getByte1(uint32_t data);
        uint8_t getByte2(uint32_t data);
        uint8_t getByte3(uint32_t data);
        void verifSprite();
        uint16_t bg_dots_counter = 0;
        uint16_t sprite_dots_counter = 0;
        uint8_t first_time = 1;
        void bgPixelFetch();
        void spriteFetch();
        uint8_t vertical_flip = 0;
        fifoPixel decodeTileData(uint8_t hi, uint8_t lo, uint8_t i, fifoPixel *px);
    public:
        std::array<std::array<uint16_t,144>,160> displayArray;
        PPU(Memory *mem); 
        void pushPixel();
        void step();
};