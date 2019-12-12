#ifndef GRAPHICS_H
#define GRAPHICS_H

#include<ascii_graphics.h>
#include<qt_graphics.h>
namespace graphics{
bool is_ascii=true;
void render(char* map){
    if(is_ascii){
        ascii_graphics::render_ascii(map);
    }else{
        qt_graphics::render_ascii(map);
    }
}
}
#endif // GRAPHICS_H
