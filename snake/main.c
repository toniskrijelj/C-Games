#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#define gotoxy(x, y) { SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){(x), (y)}); }

#define RIGHT 4
#define LEFT 5
#define UP 2
#define DOWN 3
#define FOOD 7
#define HEAD 6
#define AUP 72
#define ADOWN 80
#define ALEFT 75
#define ARIGHT 77
float tezina;
void restart();
void SetColor(int ForgC)
 {
 WORD wColor;

  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
 }
 }
void draw();
float highscore;
void quit();
float points;
void readRecord();
float highlenght;
void die();
float tezina1;
float x=1;
void move();
int collision();
void record();
float eatFood = 0;
int getInput = 0;
float tezina2;
void Food();
int foodx;
int foody;
float eaten;
typedef struct
{
    int x;
    int y;
    int direction;
} Zmija;
Zmija tail = {10,7};
Zmija head = {13,7,4};


int at[25][80]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,4,4,4,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

int original[25][80]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,4,4,4,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

void draw()
{
    for(int j=0;j<24;j+=1)
        {for(int i=0;i<80;i+=1)
        {
            if(at[j][i]==1){gotoxy(i,j);SetColor(8);printf("%c",219);}
            if(at[j][i]==0){gotoxy(i,j);printf(" ");}
            if(at[j][i]==7){gotoxy(i,j);SetColor(14);printf("%c",219);}
            if(at[j][i] > 1 && at[j][i] < 6){gotoxy(i,j);SetColor(2);printf("%c",219);}
            if(at[j][i]==6){gotoxy(i,j);SetColor(12);printf("%c",219);}
        } if(j!=23)printf("\n");}
        SetColor(14);gotoxy(14,23);printf("Score: ");
        SetColor(3);gotoxy(35,23);printf("Speed: %3.0f",150-(eaten/3*2));
        gotoxy(55,23); SetColor(2); printf("Lenght:%3.0f",4+eaten);
        gotoxy(35,1);
        SetColor(15);
        if(tezina==1)
            printf("Slow mode");
        else printf("Fast mode");

}

int collision(int direction)
{
    switch(direction)
    {
        case RIGHT: {if(at[head.y][head.x+1]==LEFT) {head.direction=LEFT;return 3;}
                     if((at[head.y][head.x+1]!=0) && (at[head.y][head.x+1]!= 7)){return 0;}
                     if(at[head.y][head.x+1]==7) return 2; break;
                    }
        case LEFT: {if(at[head.y][head.x-1]==RIGHT) {head.direction=RIGHT;return 3;}
                    if((at[head.y][head.x-1]!=0) && (at[head.y][head.x-1]!=7)){return 0;}
                    if(at[head.y][head.x-1]==7) return 2; break;
                   }
        case UP: {if(at[head.y-1][head.x]==DOWN) {head.direction=DOWN;return 3;}
                  if((at[head.y-1][head.x]!=0) && (at[head.y-1][head.x]!= 7)){return 0;}
                  if(at[head.y-1][head.x]==7) return 2; break;
                 }
        case DOWN: {if(at[head.y+1][head.x]==UP) {head.direction=UP;return 3;}
                    if((at[head.y+1][head.x]!=0) && (at[head.y+1][head.x]!= 7)){return 0;}
                    if(at[head.y+1][head.x]==7) return 2; break;
                   }
    }
    return 1;

}

void move(int direction)
{
    switch(collision(direction))
    {

    case 3:
        {
            if(direction==RIGHT) {direction=LEFT;if(at[head.y][head.x-1]!= 7)goto ovde; else goto vamo;}
            if(direction==LEFT) {direction=RIGHT;if(at[head.y][head.x+1]!= 7)goto ovde; else goto vamo;}
            if(direction==UP) {direction=DOWN;if(at[head.y+1][head.x]!= 7)goto ovde; else goto vamo;}
            if(direction==DOWN) {direction=UP;if(at[head.y-1][head.x]!= 7)goto ovde; else goto ovde;}
        }
        vamo:
    case 2:
        {
            eatFood += (0.5*tezina);
            eaten+=(0.5*tezina);
            points+=(1000*tezina2);
            Food();
        }
        ovde:
    case 1:
    {
        if(direction == RIGHT)
        {
            at[head.y][head.x]=RIGHT;
            at[head.y][head.x+1]=HEAD;
            gotoxy(head.x+1,head.y); SetColor(12);printf("%c",219);SetColor(15);
            gotoxy(head.x,head.y); SetColor(2);printf("%c",219);SetColor(15);
            head.x+=1;
            gotoxy(20,23); SetColor(14); printf("%7.0f",points);
        }
        if(direction == LEFT)
        {
            at[head.y][head.x]=LEFT;
            at[head.y][head.x-1]=HEAD;
            gotoxy(head.x-1,head.y); SetColor(12);printf("%c",219);SetColor(15);
            gotoxy(head.x,head.y); SetColor(2);printf("%c",219);SetColor(15);
            head.x-=1;
            gotoxy(20,23); SetColor(14); printf("%7.0f",points);
        }
        if(direction == UP)
        {
            at[head.y][head.x]=UP;
            at[head.y-1][head.x]=HEAD;
            gotoxy(head.x,head.y-1); SetColor(12);printf("%c",219);SetColor(15);
            gotoxy(head.x,head.y); SetColor(2);printf("%c",219);SetColor(15);
            head.y-=1;
            gotoxy(20,23); SetColor(14); printf("%7.0f",points);
        }
        if(direction == DOWN)
        {
            at[head.y][head.x]=DOWN;
            at[head.y+1][head.x]=HEAD;
            gotoxy(head.x,head.y+1); SetColor(12);printf("%c",219);SetColor(15);
            gotoxy(head.x,head.y); SetColor(2);printf("%c",219);SetColor(15);
            head.y+=1;
            gotoxy(20,23); SetColor(14); printf("%7.0f",points);
        }
        if(eatFood < 1) {
        switch(at[tail.y][tail.x])
            {
                case RIGHT:
                    {
                    at[tail.y][tail.x]=0;
                    gotoxy(tail.x,tail.y); printf(" ");
                    tail.x+=1;
                    if(points >= 12 && (eaten/5/tezina < 12))
                       points=points-12+(eaten/5/tezina);
                       else points-=points;
                       gotoxy(20,23); SetColor(14); printf("%7.0f",points); break;
                    }
                case LEFT:
                    {
                    at[tail.y][tail.x]=0;
                    gotoxy(tail.x,tail.y); printf(" ");
                    tail.x-=1;
                    if(points >= 12 && (eaten/5/tezina < 12))
                       points=points-12+(eaten/5/tezina);
                       else points-=points;
                       gotoxy(20,23); SetColor(14); printf("%7.0f",points); break;
                    }
                case UP:
                    {
                    at[tail.y][tail.x]=0;
                    gotoxy(tail.x,tail.y); printf(" ");
                    tail.y-=1;
                    if(points >= 12 && (eaten/5/tezina < 12))
                       points=points-12+(eaten/5/tezina);
                       else points-=points;
                       gotoxy(20,23); SetColor(14); printf("%7.0f",points); break;
                    }
                case DOWN:
                    {
                    at[tail.y][tail.x]=0;
                    gotoxy(tail.x,tail.y); printf(" ");
                    tail.y+=1;
                    if(points >= 12 && (eaten/5/tezina < 12))
                       points=points-12+(eaten/5/tezina);
                       else points-=points;
                       gotoxy(20,23); SetColor(14); printf("%7.0f",points); break;
                    }
            }}
            else {eatFood -= 1;
            gotoxy(62,23);SetColor(2);
            printf("%3.0f",4+eaten);}

            if((eaten/3*tezina1)==(x*0.5*tezina))
            {
            gotoxy(42,23);
            SetColor(3);
            printf("%3.0f",150-(eaten/3*2*tezina1/tezina));
            x+=1;
            }
       break;
    }
    case 0: die();break;
    }
}
void pause()
{
    if(highscore<points)
        highscore=points;
    if(highlenght<(eaten+4))
        highlenght= eaten+4;
    system("cls");
    getInput=0;
    gotoxy(35,7);
    SetColor(12);
    printf("PAUSED");
    gotoxy(26,9);
    SetColor(15);
    printf("Press SPACE to resume...");
    gotoxy(28,11);
    printf("To quit press ESC...");
    gotoxy(24,13);
    printf("To save & quit press ENTER...");
    gotoxy(20,16)
    SetColor(14);
    printf("Highscore: %6.0f",highscore);
    SetColor(2);
    gotoxy(42,16);
    printf("Max lenght: %3.1f",highlenght);
    SetColor(15);
    gotoxy(0,24)
    while((getInput!=32) && (getInput!=27) && getInput!=13)
        getInput=getch();
     if(getInput==27)
         quit();
     if(getInput==32)
         draw();
     else record();

}
void quit()
{
        getInput=0;
        system("cls");
        gotoxy(35,8);
        SetColor(12);
        printf("Quiting...");
        gotoxy(34,10);
        SetColor(14);
        printf("Score:%7.0f",points);
        gotoxy(30,12);
        printf("Highscore:%7.0f",highscore);
        SetColor(2);
        gotoxy(29,14);
        printf("Max lenght:   %3.1f",highlenght);
        gotoxy(33,16);
        printf("Lenght:   %3.1f\n\n\n\n\n",4+eaten);
        SetColor(15);
        exit(1);
}
void die()
{
        if(highscore<points)
            highscore=points;
        if(highlenght<(eaten+4))
            highlenght= eaten+4;
        system("cls");
        gotoxy(36,5);
        SetColor(12);
        printf("YOU DIED!");
        gotoxy(34,8);
        SetColor(14);
        printf("Score:%7.0f",points);
        gotoxy(30,10);
        printf("Highscore:%7.0f",highscore);
        SetColor(2);
        gotoxy(29,12);
        printf("Max lenght:   %3.1f",highlenght);
        gotoxy(33,14);
        printf("Lenght:   %3.1f\n\n\n\n\n",4+eaten);
        SetColor(15);
        gotoxy(29,18);
        printf("To quit press ESC...");
        gotoxy(24,20);
        printf("To save & quit press ENTER...");
        gotoxy(26,22);
        printf("To restart press SPACE...");
        getInput=getch();
        while((getInput!=27 && getInput!=32) && getInput!=13)
            getInput=getch();
        if(getInput==13)
            record();
        if(getInput==27)
        exit(1);
        if(getInput==32)
            restart();

}
void Food()
{
    again:
    foody = rand() % 10 + 5;
    foodx = rand() % 58 + 11;
    switch(at[foody][foodx])
    {
    case 0:
        {
            at[foody][foodx]=7;gotoxy(foodx,foody); SetColor(14);printf("%c",219); break;
        }
    default: goto again;
    }
}


void dobrodoslica()
{
    xD:
    system("cls");
    gotoxy(27,4);
    printf("Welcome to the");
    gotoxy(42,4);
    SetColor(2);
    printf("Snake Game!");
    SetColor(14);
    gotoxy(32,6);
    printf("Choose mode:");
    gotoxy(16,10);
    SetColor(4);
    printf("(1)");
    gotoxy(13,12);
    printf("Slow mode");
    SetColor(15);
    gotoxy(5,14);
    printf("Every 2 foods -> bigger snake");
    gotoxy(5,16);
    printf("Every 3 foods -> faster snake");
    gotoxy(58,10);
    SetColor(12);
    printf("(2)");
    gotoxy(55,12);
    printf("Fast mode");
    SetColor(15);
    gotoxy(47,14);
    printf("Every 1 food -> bigger snake");
    gotoxy(47,16);
    printf("Every 1 food -> faster snake\n\n\n\n\n\n");
    gotoxy(25,21);
    printf("To see records press ENTER...");
    aj:
    switch(getInput=getch())
    {case '2': tezina=2;tezina1=3;tezina2=3; break;
    case '1': tezina=1;tezina1=1;tezina2=1.5; break;
    case 13: system("cls");readRecord();goto xD;
    default: goto aj;}
    system("cls");
    gotoxy(36,14);
    printf("loading...");
    gotoxy(30,15);
    for(int r=0;r<20;r++){
    Sleep(25);
    printf("%c",177);}

}

void restart()
{
    int j,i;
    for(j=4;j<21;j+=1)
    {for(i=10;i<70;i+=1)
     if(at[j][i]==2 || (at[j][i]==3 || (at[j][i]==4 || (at[j][i]==5 || (at[j][i]==6 || at[j][i]==7)))))
        at[j][i]=0;
    }
    at[7][10]=4;
    at[7][11]=4;
    at[7][12]=4;
    at[7][13]=6;
    head.x=13;
    head.y=7;
    tail.x=10;
    tail.y=7;
    head.direction=RIGHT;
    points= 0;
    eaten=0;
    eatFood=0;
    Food();
    x=1;
    draw();
}
void readRecord()
{
    FILE *open = fopen("record.txt","r");
    char c;
    do{
        putchar(c=getc(open));
    }while(c!=EOF);
    fclose(open);
    printf("\nTo return press any key...");
    getch();
}

void record()
{
    system("cls");
    char name[255];
    FILE *open;
    open = fopen("record.txt","a+");
    gotoxy(30,10);
    printf("Type in your name: ");
    gotoxy(33,12);
    scanf("%[^\n]",name);
    fprintf(open,"Name: %s\n",name);
    if(tezina==1)
    fprintf(open,"Mode: Slow\n");
    if(tezina==2)
    fprintf(open,"Mode: Fast\n");
    fprintf(open,"Highscore: %.0f\n",highscore);
    fprintf(open,"Lenght: %.1f\n",highlenght);
    time_t mytime;
    mytime = time(NULL);
    fprintf(open,"Played Date: %s",ctime(&mytime));
    fprintf(open,"--------------------------------------------------\n");
    system("cls");
    gotoxy(34,10);
    printf("Saved!\n\n\n\n\n\n\n\n\n\n\n");
    fclose(open);
    free(open);
    exit(1);

}
void hidecursor()
{
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
int main()
{
    hidecursor();
    srand(time(NULL));
    fflush(stdin);
    dobrodoslica();
    Food();
    draw();
    nazad:
    do{
        move(head.direction);
        Sleep(150-(eaten/3*2*tezina1/tezina));

    }while(!kbhit());

    getInput = getch();
    if(getInput==224)
        getInput=getch();
    switch(getInput)
    {
        case 27: pause();break;
        case 'W': case 'w': case AUP: head.direction=UP;break;
        case 'A': case 'a': case ALEFT: head.direction=LEFT;break;
        case 'S': case 's': case ADOWN: head.direction=DOWN;break;
        case 'D': case 'd': case ARIGHT: head.direction=RIGHT;break;
    }
    goto nazad;
}
