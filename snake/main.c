#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
void setcolor(int ForgC){
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
}}
void gotoxy(int x, int y) {SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { (x+9),(y+3)});}
#define colprintxy(color,x,y,format,...) {setcolor(color);gotoxy(x,y);printf(format,__VA_ARGS__);}
typedef struct{ int head_x,head_y,tail_x,tail_y,add_x,add_y,speed;  }Zmija;
struct{ int objekat,add_x,add_y; }at[19][62];
void postavi_mapu_i_zmiju(Zmija zmija){
    for(int i = 0; i < 19; i += 1){
        gotoxy(0,i);
        for(int j = 0;j < 62; j += 1){
            if(j == 0 || i == 0 || i == 18 || j == 61){
                at[i][j].objekat = 1;
                colprintxy(7,j,i,"%c",219);
            }else if(zmija.head_y == i && (zmija.head_x-3 == j || zmija.head_x-1 == j || zmija.head_x-2 == j)){
                at[i][j].objekat = 1;
                at[i][j].add_x = 1;
                at[i][j].add_y = 0;
                colprintxy(10,j,i,"%c",219);
            }else{
            at[i][j].objekat = 0;
            printf(" ");
            }
        }
    }
    colprintxy(2,zmija.head_x,zmija.head_y,"%c",219);
}
void food(){
    int x,y;
    do{
        x = rand()% 60 + 1;
        y = rand()% 17 + 1;
    }while(at[y][x].objekat != 0);
    at[y][x].objekat = 2;
    colprintxy(14,x,y,"%c",219);
}
int update_head(Zmija* zmija){
    if(at[(zmija->head_y+zmija->add_y)][(zmija->head_x+zmija->add_x)].objekat == 1){
        Sleep(1000);
        exit(0);
    }
    at[zmija->head_y][zmija->head_x].add_x = zmija->add_x;
    at[zmija->head_y][zmija->head_x].add_y = zmija->add_y;
    at[zmija->head_y][zmija->head_x].objekat = 1;
    colprintxy(10,zmija->head_x,zmija->head_y,"%c",219);
    zmija->head_x+=zmija->add_x;
    zmija->head_y+=zmija->add_y;
    colprintxy(2,zmija->head_x,zmija->head_y,"%c",219);
    if(at[zmija->head_y][zmija->head_x].objekat == 2){
        zmija->speed-=2;
        return 1;
    }
    else return 0;
}
void update_tail(Zmija* zmija){
    at[zmija->tail_y][zmija->tail_x].objekat = 0;
    gotoxy(zmija->tail_x,zmija->tail_y);
    printf(" ");
    int y_pomoc = zmija->tail_y, x_pomoc = zmija->tail_x;
    zmija->tail_y += at[y_pomoc][x_pomoc].add_y;
    zmija->tail_x += at[y_pomoc][x_pomoc].add_x;
}
void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}
int main(){
    srand(time(NULL));
    hidecursor();
    Zmija zmija = {5,3,2,3,1,0,150};
    int input;
    postavi_mapu_i_zmiju(zmija);
    food();
    while(1){
        do{
            Sleep(zmija.speed);
            if(update_head(&zmija)==0) update_tail(&zmija);
            else food();
        }while(!kbhit());
        input = getch();
        if(input == 224) input = getch();
        switch(input){
            case 77:
                if(zmija.add_x != -1)zmija.add_x = 1;
                zmija.add_y = 0; break;
            case 75:
                if(zmija.add_x != 1)zmija.add_x = -1;
                zmija.add_y = 0; break;
            case 72:
                if(zmija.add_y != 1)zmija.add_y = -1;
                zmija.add_x = 0; break;
            case 80:
                if(zmija.add_y != -1)zmija.add_y = 1;
                zmija.add_x = 0; break; }}}
