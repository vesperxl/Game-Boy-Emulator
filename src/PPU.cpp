#include <PPU.hpp>
#include<queue>
#include <Memory.hpp>
#include<cstdint>

PPU::PPU(Memory *mem ) : _memory(mem){

}


void PPU::pushPixel(){
    if(!bg_fifo.empty()){
        uint8_t LY = _memory->LY();
        fifoPixel bg_px = bg_fifo.front();
        bg_fifo.pop();
        if(!sprite_fifo.empty()){
            fifoPixel sprite_px = sprite_fifo.front();
            sprite_fifo.pop();

            //mixing
            if(sprite_px.color_id != 0 && (sprite_px.bg_priority != 1 || bg_px.color_id == 0)){

                uint8_t palette;
                if(sprite_px.palette == 0){
                    palette = _memory->read(0xFF48);
                }else{
                    palette = _memory->read(0xFF49);
                }

                uint16_t hex_color = getColor(palette,sprite_px.color_id);

                displayArray[LY][x_pos_counter] = hex_color;
            }else{

                uint8_t palette = _memory->read(0xFF47);
                uint16_t hex_color = getColor(palette,bg_px.color_id);
                displayArray[LY][x_pos_counter] = hex_color;
            }
        }
    }

}


uint16_t PPU::getColor(uint8_t palette, uint8_t color_id){

    uint8_t color = palette >> (color_id * 2);
    switch(color){
        case 0:
            return 0xFFFFFF;
        case 1:
            return 0xA9A9A9;
        case 2:
            return 0x545454;
        case 3:
            return 0;   
    }
}


void PPU::step(){
    dots++;
}

void PPU::oam_scan(){   
    if(dots % 2 != 1){
        uint8_t curr = dots * 2;
        uint8_t adress = 0xFE00 + curr;
        uint32_t sprite = (_memory->read(adress) << 24) | (_memory->read(adress + 1) << 16) | (_memory->read(adress + 2) << 8) | (_memory->read(adress + 3));
        uint8_t spriteSize = (_memory->LCDC() >> 2) & 0xFF == 0 ? 8 : 16;
        if((getByte1(sprite) > 0) && ((_memory->LY() + 16) >= getByte0(sprite)) 
            &&((_memory->LY() + 16) < (getByte0(sprite) + 16)) && (spriteSize && sprite_buffer.size() <= 10)){
                sprite_buffer.push_back(sprite);
        }
    }
}


uint8_t PPU::getByte0(uint32_t data){

    return data >> 24;
}
uint8_t PPU::getByte1(uint32_t data){

    return (data >> 16) & 0xFF;
}
uint8_t PPU::getByte0(uint32_t data){

    return (data >> 8) & 0xFF;
}
uint8_t PPU::getByte0(uint32_t data){

    return data & 0xFF;
}

