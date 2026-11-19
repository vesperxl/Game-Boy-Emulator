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
            sprite_fifo.pop_front();  

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
                x_pos_counter++;
            }
        }
        else
        {
            uint8_t palette = _memory->read(0xFF47);
            uint16_t hex_color = getColor(palette,bg_px.color_id);
            displayArray[LY][x_pos_counter] = hex_color;
            x_pos_counter++;
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




void PPU::oam_scan(){   
    if(dots % 2 != 1){
        uint8_t curr = dots * 2;
        uint8_t adress = 0xFE00 + curr;
        uint32_t sprite = (_memory->read(adress) << 24) | (_memory->read(adress + 1) << 16) | (_memory->read(adress + 2) << 8) | (_memory->read(adress + 3));
        uint8_t spriteSize = (_memory->LCDC() >> 2) & 0xFF == 0 ? 8 : 16;
        if((getByte1(sprite) > 0) && ((_memory->LY() + 16) >= getByte0(sprite)) 
            &&((_memory->LY() + 16) < (getByte0(sprite) + 16)) && (spriteSize && sprite_buffer.size() < 10)){
                sprite_buffer.push_back(sprite);
        }
    }
}



void PPU::bgPixelFetch(){

    v = 1;
    if(bg_dots_counter == 1){//STEP 1
        if(!window_rendering){
            if((_memory->LCDC() >> 3) & 0x1){
                tilemap = 0x9C00;
            }else{
                tilemap = 0x9800;
            }
            offset = x_pos_fetcher_counter + (_memory->SCX() / 8);
            offset = offset & 0x1F;
            offset += 32 * (((_memory->LY() + _memory->SCY()) & 0xFF) / 8);
        }else{
            if((_memory->LCDC() >> 6) & 0x1){
                tilemap = 0x9C00;
            }else{
                tilemap =0x9800;
            }
            offset = x_pos_fetcher_counter + (32 * (window_line_counter / 8));
            if(!window_in_scanline){
                window_in_scanline = 1;
                window_line_counter++;
            }
        }
        tile_num = _memory->read(tilemap + offset);

    }else if (bg_dots_counter == 3){//STEP 2
        if(!window_rendering){
            tile_adress = 2 * ((_memory->LY() + _memory->SCY()) % 8);
        }else{
            tile_adress = 2 * (window_line_counter % 8);
        }

        if((_memory->LCDC() >> 4) & 1){
            tile_adress += 0x8000 + (tile_num * 16);
        }else{
            tile_adress += 0x8800 + ((int8_t)tile_num * 16);
        }

        low_tile_data = _memory->read(tile_adress);
    }else if(bg_dots_counter == 5){
        high_tile_data = _memory->read(tile_adress + 1);
    }else if(bg_dots_counter == 6){
        if(first_time){
            bg_dots_counter = 0;
            first_time = 0;
            v = 0;
            if(window_in_scanline){
                window_in_scanline = 0;
                window_line_counter--;
            }
        }
    }else if(bg_dots_counter > 6){
        if(bg_fifo.empty()){
            for(int i = 7; i >= 0; i--){
                fifoPixel px;
                decodeTileData(high_tile_data,low_tile_data,i,&px);
                bg_fifo.push(px);
            }
            bg_dots_counter = 0;
            v =0;
        }
    }
    

    bg_dots_counter += v;
    
   
}


void PPU::spriteFetch(){
    if(sprite_dots_counter == 1){//step 1
        tile_num = getByte2(curr_sprite);
    }else if(sprite_dots_counter == 3){//step 2

        if((getByte3(curr_sprite) >> 6) & 1){//vertical flip 
            vertical_flip = 1;
        }
        if((_memory->LCDC() >> 2) & 1){//1x2 
            if(_memory->LY() < getByte0(curr_sprite) - 8){//upper tile
            
                tile_num = vertical_flip ? tile_num | 1 : tile_num & 0xFE;
            }else{
                tile_num = vertical_flip ? tile_num & 0xFE : tile_num | 1;//lower tile
            }
        }
        
        if(vertical_flip){
            tile_adress = (0x8000 + tile_num * 16) + 2 * (7 -(_memory->LY() - (getByte0(curr_sprite) - 16)));
        }else{
            tile_adress = (0x8000 + tile_num * 16) + 2 * (_memory->LY() - (getByte0(curr_sprite) - 16));
        }
        

        low_tile_data = _memory->read(tile_adress);
    }else if(sprite_dots_counter == 5){//step 3
        high_tile_data = _memory->read(tile_adress + 1); 
    }else if (sprite_dots_counter == 6){//step 4
        uint8_t bg_priority = getByte3(curr_sprite) >> 7;
        uint8_t b1 = getByte1(curr_sprite);
        uint8_t sprite_x_pos = b1 > 8 ? 7 : b1 == 0 ? 0 : b1 - 1;

        for(uint8_t i = sprite_x_pos; i >= 0;i--){//load to fifo 
            fifoPixel px;
            decodeTileData(high_tile_data,low_tile_data,i,&px);
            px.bg_priority = bg_priority;


            uint8_t n = sprite_x_pos - i;
            if(n > sprite_fifo.size()){//can insert 

                sprite_fifo.push_back(px);
            }else{
                if(sprite_fifo[n].color_id == 0){//transparent
                    
                    sprite_fifo[n] = px;
                }
            }
            
        }       
    }

    if(sprite_dots_counter < 6){
        sprite_dots_counter++;
    }else{
        sprite_dots_counter = 0;
        sprite_in_progress = 0;
    }
}




void PPU::step(){
     if(new_scanline){//COINCIDENCE FLAG
            if(_memory->LY() == _memory->read(0xFF45)){
                _memory->write(0xFF41,_memory->read(0xFF41) | 1);
            }else{
                _memory->write(0xFF41,_memory->read(0xFF41) & 0);
            }
            new_scanline = 0;
     }


    if(_memory->LY() < 144){
         if(dots <= 80){// MODE 2 : OAM SCAN
           
                if(first_oam)
                {
                    if(((_memory->read(0xFF41) >> 5) & 1) == 1 && !ly_lyc_condition && !mode_condition){//stat interrupt 
                        _memory->write(0xFF0F,_memory->IF() | 2);
                         mode_condition = 1;
                    }

                    uint8_t stat = _memory->STAT() & 0xFC;
                    stat = stat | 2;
                    _memory->write(0xFF41,stat);//update stat on MODE 2
                } else{
                    mode_condition = 0;
                }
             
            oam_scan();
        }else{
            if(x_pos_counter < 160 ){//drawing mode : MODE 3

                uint8_t stat = _memory->STAT() & 0xFC;
                stat = stat | 3;
                _memory->write(0xFF41,stat);//update stat on MODE 3
                if(!sprite_in_progress){
                    bgPixelFetch();
                    verifSprite();
                }else{
                    spriteFetch();
                }
                    
                if(!sprite_in_progress){
                    verifSprite;
                }

                if(!sprite_in_progress){
                    pushPixel();
                    if((_memory->LCDC() >> 5) & 1 == 1 && wy_ly == 1 && x_pos_counter >= _memory->read(0xFF4B) - 7){
                        window_rendering = 1;
                    }else{
                        window_rendering = 0;

                    }
                }


            }else{//mode 0 h blank
                if(first_hblank){
                    if(((_memory->read(0xFF41) >> 3) & 1) == 1 && !ly_lyc_condition && !mode_condition){//stat interrupt 
                        _memory->write(0xFF0F,_memory->IF() | 2);
                        mode_condition = 1;
                    }


                    uint8_t stat = _memory->STAT() & 0xFC;
                    stat = stat | 0;
                    _memory->write(0xFF41,stat);//update stat on MODE 0

                    first_hblank = 0;
                }else{
                    mode_condition = 0;

                }

                if(dots >= 456){ //End of scanline
                    _memory->write(0xFF44,_memory->LY() + 1);
                    dots = 0;
                    window_in_scanline = 0;
                    first_time =0;
                    x_pos_counter = 0;
                    new_scanline = 1;
                }
            }

        }

    }else{// V - BLANK
        if(first_vblank){
            if(((_memory->read(0xFF41) >> 4) & 1) == 1 && !ly_lyc_condition && !mode_condition){//stat interrupt 
            _memory->write(0xFF0F,_memory->IF() | 2);
            first_vblank = 0;
            }
        }else{
            mode_condition = 0;
        }

        if(_memory->LY() == 144){
            _memory->write(0xFF0F,_memory->IF() | 1);
        }

        uint8_t stat = _memory->STAT() & 0xFC;
        stat = stat | 1;
        _memory->write(0xFF41,stat);//update stat on MODE 1

        _memory->write(0xFF44,_memory->LY() + 1);

        window_line_counter = 0;
        wy_ly = 0;
        if (dots >= 456){
        _memory->write(0xFF44,_memory->LY() + 1);
            dots = 0;
            window_in_scanline = 0;
            first_time =0;
            x_pos_counter = 0;
            new_scanline = 1;
        }
    }
   

    if(((_memory->read(0xFF41) >> 6) & 1) == 1 && _memory->LY() == _memory->read(0xFF45) && !ly_lyc_condition && !mode_condition){//LY = LYC 
        _memory->write(0xFF0F,_memory->IF() | 2);
        ly_lyc_condition = 1;
    }else{
        ly_lyc_condition = 0;
    }

    dots++;
}




void PPU::verifSprite(){

    if((_memory->LCDC() >> 1) & 1 == 1){
            for(uint8_t i = 0; i < sprite_buffer.size(); i++){
                if(getByte1(sprite_buffer[i]) <= x_pos_counter + 8){
                    sprite_in_progress = 1;
                    curr_sprite = sprite_buffer[i];
                    sprite_buffer.erase(sprite_buffer.begin() + i);
                    bg_dots_counter = 0;
                    break;
                }
            }
        }
}
fifoPixel PPU::decodeTileData(uint8_t hi, uint8_t lo, uint8_t i,fifoPixel *px){
    uint8_t color = ((hi >> i) & 1) << 1 | (lo >> i) & 1 ;
    px->color_id = color;
    px->bg_priority = 0;
    px->palette = 0;
}



uint8_t PPU::getByte0(uint32_t data){

    return data >> 24;
}
uint8_t PPU::getByte1(uint32_t data){

    return (data >> 16) & 0xFF;
}
uint8_t PPU::getByte2(uint32_t data){

    return (data >> 8) & 0xFF;
}
uint8_t PPU::getByte3(uint32_t data){

    return data & 0xFF;
}