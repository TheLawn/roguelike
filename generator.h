#ifndef GENERATOR_H
#define GENERATOR_H
#include<windows.h>
#include<iostream>
#include <string>
#include<vector>
#include<cmath>
#include <ctime>
#include"constants.h"
#include<iterator>
using namespace std;
enum ConsoleColor
{
        Black         = 0,
        Blue          = 1,
        Green         = 2,
        Cyan          = 3,
        Red           = 4,
        Magenta       = 5,
        Brown         = 6,
        LightGray     = 7,
        DarkGray      = 8,
        LightBlue     = 9,
        LightGreen    = 10,
        LightCyan     = 11,
        LightRed      = 12,
        LightMagenta  = 13,
        Yellow        = 14,
        White         = 15
};
#define DOOR 2
#define VOID 0
#define WALL 3
#define FLOOR 1
#define FALSEWALL 4

struct Point{
    int x,y;
    void print(){
        cout<<"{"<<x<<","<<y<<"}"<<endl;
    }
    Point(int x_or=0,int y_or=0):
        x(x_or),y(y_or){}
};

void set_color(ConsoleColor text, ConsoleColor background)
{
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, (WORD)((background << 4) | text));
}

struct Room{
    int x,y,w,h;
    bool is_hidden;
    void print(){
        cout<<"{"<<x<<","<<y<<"}"<<"{"<<w<<","<<h<<"}"<<endl;
    }
    Room(int x_or,int y_or,int width,int height):
        x(x_or),y(y_or),w(width),h(height){
        int a=rand()%10;
        is_hidden=a<2;
    }
    vector<Room> reduce(){
        vector<Room> answ;
        int x1,y1,w1,h1;
        w1=w-rand()%(w/2)-1;
        h1=h-rand()%(h/2)-1;
        x1=x+rand()%(w-w1);
        y1=y+rand()%(h-h1);
        answ.emplace_back(x1,y1,w1,h1);
        return answ;
    }
    vector<Room> division(){
            vector<Room> answ;
            if(w<w_min*2&&h<h_min*2){
                answ.emplace_back(x,y,w,h);
                return answ;
            }
            if(w<w_min*2){
                int h_new=h_min+rand()%(h-h_min*2+1);
                //todo split room
                answ.emplace_back(x,y,w,h_new-1);
                answ.emplace_back(x,y+h_new,w,h-h_new);
                return answ;
            }
            if(h<h_min*2){
                int w_new=w_min+rand()%(w-w_min*2+1);
                //todo split room
                answ.emplace_back(x,y,w_new-1,h);
                answ.emplace_back(x+w_new,y,w-w_new,h);
                return answ;
            }
            if(w>h){
                int w_new=w_min+rand()%(w-w_min*2+1);
                //todo split room
                answ.emplace_back(x,y,w_new-1,h);
                answ.emplace_back(x+w_new,y,w-w_new,h);
                return answ;
            }
            if(w<h){
                int h_new=h_min+rand()%(h-h_min*2+1);
                //todo split room
                answ.emplace_back(x,y,w,h_new-1);
                answ.emplace_back(x,y+h_new,w,h-h_new);
                return answ;
            }
            int coin=rand()%2;
            if(coin<1){
                int h_new=h_min+rand()%(h-h_min*2+1);
                //todo split room
                answ.emplace_back(x,y,w,h_new-1);
                answ.emplace_back(x,y+h_new,w,h-h_new);
                return answ;
            }
            int w_new=w_min+rand()%(w-w_min*2+1);
            //todo split room
            answ.emplace_back(x,y,w_new-1,h);
            answ.emplace_back(x+w_new,y,w-w_new,h);
            return answ;

    }
};
int* map_from_vector(vector<Room> rooms){
    int* map=new int[width*height];
    int id=0;
    for(int i=0;i<width*height;i++){
        map[i]=VOID;
    }
    for(auto it=rooms.end();it+1!=rooms.begin();it--){
        id++;
        Room r=*it;
        for(int i=r.x;i<r.x+r.w;i++){
            for(int j=r.y;j<r.y+r.h;j++){
                map[i*height+j]=WALL;
            }
        }
        for(int i=r.x+1;i<r.x+r.w-1;i++){
            for(int j=r.y+1;j<r.y+r.h-1;j++){
                map[i*height+j]=FLOOR;
            }
        }
    }
    return map;
}
int* map_from_halls(vector<vector<Point>> halls){
    int* map=new int[width*height];
    int id=0;
    for(int i=0;i<width*height;i++){
        map[i]=VOID;
    }
    for(auto it=halls.begin();it!=halls.end();it++){
        vector<Point> hall=*it;
        if(hall.size()==2){
            Point p1=hall.back();
            hall.pop_back();
            Point p2=hall.back();
            for(int x=p1.x;x<=p2.x;x++){
                for(int y=p1.y;y<=p2.y;y++){
                    map[x*height+y]=FLOOR;
                }
            }
        }else{

        }
    }
    return map;
}
int* map_summator(int* halls,int* rooms){
    for(int i=0;i<width*height;i++){
        if(rooms[i]==VOID)continue;
        if(rooms[i]==WALL&&halls[i]==FLOOR){
            int a=rand()%10;
            if(a<2)halls[i]=FALSEWALL;
            else halls[i]=DOOR;
        }
        else halls[i]=rooms[i];
    }
    for (int x=1;x<width-1;x++){
        for(int y=1;y<height-1;y++){
            if(halls[x*height+y]==FLOOR||halls[x*height+y]==FALSEWALL||halls[x*height+y]==DOOR){
                if(halls[(x+1)*height+y]==VOID)halls[(x+1)*height+y]=WALL;
                if(halls[(x-1)*height+y]==VOID)halls[(x-1)*height+y]=WALL;
                if(halls[x*height+y+1]==VOID)halls[x*height+y+1]=WALL;
                if(halls[x*height+y-1]==VOID)halls[x*height+y-1]=WALL;
            }
            if(halls[x*height+y]==FALSEWALL){
                if(halls[(x+1)*height+y]==DOOR)halls[(x+1)*height+y]=FALSEWALL;
                if(halls[(x-1)*height+y]==DOOR)halls[(x-1)*height+y]=FALSEWALL;
                if(halls[x*height+y+1]==DOOR)halls[x*height+y+1]=FALSEWALL;
                if(halls[x*height+y-1]==DOOR)halls[x*height+y-1]=FALSEWALL;
            }
        }
    }
    return halls;
}
char* char_map_from_int_map(int* src){
    char* map=new char[width*height];
    for(int i=0;i<width*height;i++){
        switch(src[i]){
        case VOID:
            map[i]=' ';
            break;
        case FLOOR:
            map[i]='.';
            break;
        case WALL:
            map[i]='#';
            break;
        case DOOR:
            map[i]='/';
            break;
        case FALSEWALL:
            map[i]='@';
            break;
        }
    }
    return map;
}
char* generate(){
    srand(time(NULL));
    vector<Room>rooms;
    rooms.emplace_back(0,0,width,height);
    //splitting rooms
    for(int i=0;i<depth;i++){
        vector<Room> rooms_new;
        for(auto it=rooms.begin();it!=rooms.end();it++){
            Room r=*it;
            vector<Room> rs=r.division();
            rooms_new.insert(rooms_new.end(),rs.begin(),rs.end());

        }
        rooms.swap(rooms_new);
    }
    vector<Room> rooms_new;
    //making rooms smaller
    for(auto it=rooms.begin();it!=rooms.end();it++){
        Room r=*it;
        vector<Room> rs=r.reduce();
        rooms_new.insert(rooms_new.end(),rs.begin(),rs.end());
    }
    rooms.swap(rooms_new);
    int room_count=rooms.size();
    bool* halls=new bool[room_count*room_count];
    //creating room hall matrix
    for(int i=0;i<room_count;i++){
        for(int j=0;j<room_count;j++){
            if(i==j){
                halls[i*room_count+j]=false;
                continue;
            }
            int a=rand()%100;
            if(halls[i*room_count+j]){
                halls[i*room_count+j]=a<65;
                halls[j*room_count+i]=a<65;
            }else{
                halls[i*room_count+j]=a<5;
                halls[j*room_count+i]=a<5;
            }
        }
    }
    for(int i=0;i<room_count;i++){
        int count=0;
        for(int j=0;j<room_count;j++){
            if(halls[i*room_count+j])count++;
        }
        if(count<=3){
            int j=rand()%room_count;
            if(j==i)j--;
            halls[i*room_count+j]=true;
            halls[j*room_count+i]=true;
            j=rand()%room_count;
            if(j==i)j--;
            halls[i*room_count+j]=true;
            halls[j*room_count+i]=true;
        }
    }
    //creating halls from matrix
    vector<vector<Point>> halls_vector;
    for(int i=0;i<room_count;i++){
        for(int j=i;j<room_count;j++){
            if(halls[i*room_count+j]){
                vector<Point> hall;
                Room r1=rooms.at(i);
                Room r2=rooms.at(j);
                int x,y,x1,y1;
                x=r1.x;
                y=r1.y;
                x1=r2.x;
                y1=r2.y;
                int flag=0;
                if(r1.y<(r2.y+1)&&((r1.y+r1.h-2)>(r2.y+1))){
                    y=r2.y+1+rand()%(r1.y+r1.h-r2.y-3);
                    if(y>(r2.y+r2.h-2))y=r2.y+1+rand()%(r2.h-2);
                    y1=y;
                    if(x<x1)x+=r1.w-1;
                    else x1+=r2.w-1;
                    if(x<x1){
                        hall.emplace_back(x1,y);
                        hall.emplace_back(x,y);
                    }
                    else {
                        hall.emplace_back(x,y);
                        hall.emplace_back(x1,y);
                    }
                }else if((r2.y<(r1.y+1)&&((r2.y+r2.h-2)>(r1.y+1)))){
                    y=r1.y+1+rand()%(r2.y+r2.h-r1.y-3);
                    if(y>(r1.y+r1.h-2))y=r1.y+1+rand()%(r1.h-2);
                    y1=y;
                    if(x<x1)x+=r1.w-1;
                    else x1+=r2.w-1;
                    if(x<x1){
                        hall.emplace_back(x1,y);
                        hall.emplace_back(x,y);
                    }
                    else {
                        hall.emplace_back(x,y);
                        hall.emplace_back(x1,y);
                    }
                }else if((r1.x<(r2.x+1)&&((r1.x+r1.w-2)>(r2.x+1)))){
                    x=r2.x+1+rand()%(r1.x+r1.w-r2.x-3);
                    if(x>(r2.x+r2.w-2))x=r2.x+1+rand()%(r2.w-2);
                    x1=x;
                    if(y<y1)y+=r1.h-1;
                    else y1+=r2.h-1;
                    if(y<y1){
                        hall.emplace_back(x,y1);
                        hall.emplace_back(x,y);
                    }
                    else {
                        hall.emplace_back(x,y);
                        hall.emplace_back(x,y1);
                    }
                }else if((r2.x<(r1.x+1)&&((r2.x+r2.w-2)>(r1.y+1)))){
                    x=r1.x+1+rand()%(r2.x+r2.w-r1.x-3);
                    if(x>(r1.x+r1.w-2))x=r1.x+1+rand()%(r1.w-2);
                    x1=x;
                    if(y<y1)y+=r1.h-1;
                    else y1+=r2.h-1;
                    if(y<y1){
                        hall.emplace_back(x,y1);
                        hall.emplace_back(x,y);
                    }
                    else {
                        hall.emplace_back(x,y);
                        hall.emplace_back(x,y1);
                    }
                }
                halls_vector.push_back(hall);
            }
        }
    }
    int* temp_map1=map_from_halls(halls_vector);
    int* temp_map2=map_from_vector(rooms);
  //  cin.get();
    return char_map_from_int_map(map_summator(temp_map1,temp_map2));
}
#endif // GENERATOR_H
