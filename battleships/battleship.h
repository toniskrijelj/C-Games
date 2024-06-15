#ifndef BATTLESHIP_H_INCLUDED
#define BATTLESHIP_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define DOWN 80
#define UP 72
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13

#define NOSAC_AVIONA 4
#define KRSTARICA 3
#define RAZARAC 2
#define PODMORNICA 1

#define horizontal 0
#define vertical 1

#define RED 4
#define LIGHT_GRAY 7
#define CYAN 3
#define MAGENTA 5
#define WHITE 15
#define LIGHT_GREEN 10
#define GREEN 2
#define YELLOW 14

#define SUMMON 1
#define DESTROY 0

#define SKROZ 2
#define DA 1
#define NE 0

#define COVEK 1
#define KOMP 2

#define PLAY 1
#define QUIT 0

#define gotoxy(x,y) { SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { (x),(y)});}

typedef struct
{
    int Horizontal; /// ako je horizontalno 1 ako nije 0
    int Vertical; /// ako je vertikalno 1 ako nije 0
} Direction;

typedef struct
{
    int object;
    int brod_je_tu;
    int pogodjeno;
    Direction direction;
    int brod_len;
} tabla;
struct{
    int x,y;
}bot_bomb;
typedef struct
{
    int len;
    int x,y;            /// brod koji se trenutno postavlja na mapu
    Direction direction;
} Brod;
struct
{
    char symbol;
    int boja;
} at[25][80];
void setcolor(int ForgC);
void draw(tabla** tabla);
void inicijalizacija(tabla** tabla1, tabla** tabla2,tabla** pogodjena_tabla_1,tabla** pogodjena_tabla_2);
void getInput(int* variable); /// funkcija za unos
void colprintxy(int color,const char* text,int x,int y);
void update_collision(tabla** tabla,int x, int y,int type);
int check_error(tabla** tabla, int x, int y,Direction direction);   /// funkcija koja proverava da li se brod sudara
void postavi_brod(tabla** tabla, int naziv_broda,int IGRAC);
void popuni_tabelu(tabla** tabla,int IGRAC);
void ucitaj_sliku(const char* name);
int welcome_screen();
int update_bomba(tabla** tabla,int x_pomeraj,int y_pomeraj,int x_bomba, int y_bomba);
int vec_bacena_bomba(tabla** tabla,int x,int y);
void unisti_ceo_brod(tabla** _tabla,tabla** pogadjanja,int x,int y);
int proveri_ceo_brod(tabla** _tabla,tabla** pogadjanja,int x, int y);
int pogodjeno(tabla** sa_brodovima,tabla** pogadjanja,int x,int y,int IGRAC);
int check_win(tabla** sa_brodovima,tabla** pogadjanja);
int proveri_pored(tabla** _tabla,int x,int y);
int pronadji_polje_za_bota(tabla** _tabla);
int pogadjaj(tabla** sa_brodovima,tabla** pogadjanja,int IGRAC);
void pobedio(int igrac,int jel_komp);
#endif // BATTLESHIP_H_INCLUDED
