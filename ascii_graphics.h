#ifndef ASCII_GRAPHICS_H
#define ASCII_GRAPHICS_H

#include<constants.h>
#include<windows.h>
#include<iostream>
#include <string>
using namespace std;
namespace ascii_graphics {
void render_ascii(char* map){
    system("cls");
    string out="";
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            out+=map[i*height+j];
            //out+=" ";
        }
        out+="|\n";
    }
    cout<<out;
}

}
#endif // ASCII_GRAPHICS_H
