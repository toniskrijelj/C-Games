#include <stdio.h>
#include <stdlib.h>
#include "../gotoxy.h"
#include "../setcolor.h"
#include "../scrollconsole.h"
#include <conio.h>
#include <time.h>
#include <math.h>

#define LEVO -1
#define DESNO 1
#define GORE -1
#define DOLE 1

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define YELLOW 14
#define LIGHT_GRAY 7

typedef struct
{
    int x;
    double y;
    int x_add,y_add;
    int ugao;
    double speed;
}Loptica;

typedef struct
{
    int x,y;
    int pomeraj;
}Brod;

void iscrtaj_brod(int x, int y)
{
    setcolor(LIGHT_GRAY);
    for(int i = -1; i < 2; i+=1)
    {
        gotoxy(x,y+i)
        printf("%c",219);
    }
}

void move_down(Brod* brod)
{
    if(brod->y<23)
    {
        setcolor(LIGHT_GRAY);
        gotoxy(brod->x,brod->y-1)
        printf(" ");
        brod->y+=1;
        gotoxy(brod->x,brod->y+1);
        printf("%c",219);
        brod->pomeraj = -1;
    }
}

void move_up(Brod* brod)
{
    if(brod->y>1)
    {
        setcolor(LIGHT_GRAY);
        gotoxy(brod->x,brod->y+1)
        printf(" ");
        brod->y-=1;
        gotoxy(brod->x,brod->y-1);
        printf("%c",219);
        brod->pomeraj = 1;
    }
}

int update_loptica(Loptica* loptica,Brod igrac_1,Brod igrac_2)
{
    gotoxy(loptica->x,loptica->y)
    printf(" ");
    if(loptica->x == 1)
    {
        return 2;
    }
    if(loptica->x == 78)
    {
        return 1;
    }
    if(((int)(loptica->y)) == 0)
    {
        loptica->y_add *= -1;
    }
    else if(((int)(loptica->y)) == 24)
    {
        loptica->y_add *= -1;
    }
    if(loptica->x+loptica->x_add == 1 && ((!((int)(loptica->y) < igrac_1.y-1)) && (!((int)(loptica->y) > igrac_1.y+1))))
    {
        loptica->x_add *= -1;
        if(igrac_1.pomeraj == 1)
        {
            loptica->ugao -= 7.5*loptica->y_add;
        }
        else if(igrac_1.pomeraj == -1)
        {

            loptica->ugao += 7.5*loptica->y_add;
        }
        loptica->speed-=1;
    }
    else if(loptica->x+loptica->x_add == 78 && ((!((int)(loptica->y) < igrac_2.y-1)) && (!((int)(loptica->y) > igrac_2.y+1))) && ((!((int)(loptica->y+atan(loptica->ugao/(180/M_PI))*loptica->y_add) < igrac_2.y-1))
                                                                                                                                   && (!((int)(loptica->y+ atan(loptica->ugao/(180/M_PI))*loptica->y_add) > igrac_2.y+1))))
    {
        loptica->x_add *= -1;
        if(igrac_2.pomeraj == 1)
        {
            loptica->ugao -= 7.5*loptica->y_add;
        }
        else if(igrac_2.pomeraj == -1)
        {
            loptica->ugao += 7.5*loptica->y_add;
        }
        loptica->speed-=1;
    }
    setcolor(4);
    loptica->x += loptica->x_add;
    loptica->y += atan(loptica->ugao/(180/M_PI))*loptica->y_add;
    gotoxy(loptica->x,loptica->y)
    printf("%c",219);

    return 0;
}

#define DA 1
#define NE 0

void pobedio(int igrac)
{
    setcolor(LIGHT_GRAY);
    system("cls");
    gotoxy(31,12)
    printf("Pobedio je %s igrac!\n\n\n\n\n",(igrac == 1) ? "levi" : "desni");
    gotoxy(29,13)
    printf("Pritisni ENTER da izadjes..");
    while(getch()!=13);
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

int main()
{
    hidecursor();
    srand(time(NULL));
    int input,update_counter = 0,kraj_igre = NE;
    Loptica loptica = {39,12};

    loptica.x_add = rand() % 2;
    if(loptica.x_add == 0)
        loptica.x_add = -1;

    loptica.y_add = rand() % 2;
    if(loptica.y_add == 0)
        loptica.y_add = -1;

    loptica.ugao = 45;
    loptica.speed = 40;

    Brod igrac_1 = {1,12},igrac_2 = {78,12};

    iscrtaj_brod(igrac_1.x,igrac_1.y);

    iscrtaj_brod(igrac_2.x,igrac_2.y);

    setcolor(YELLOW);
    gotoxy(0,25);
    for(int i = 0; i < 80; i += 1)
        printf("%c",219);
    setcolor(LIGHT_GRAY);
    ScrollConsoleUp(2);
    while(kraj_igre == NE)
    {
        do{
            Sleep(loptica.speed);
            update_counter+=5;
            if(update_counter%10==0)
            {
                switch(update_loptica(&loptica,igrac_1,igrac_2))
                {
                case 1:
                    kraj_igre = DA;
                    pobedio(1);
                    break;
                case 2:
                    kraj_igre = DA;
                    pobedio(2);
                    break;
                }
            }
            if(update_counter%20==0)
            {
                igrac_1.pomeraj = 0;
                igrac_2.pomeraj = 0;
            }
            if(update_counter==100)
                update_counter = 0;
        }while(!kbhit() && kraj_igre == NE);
        if(kraj_igre == DA)
            break;
        input = getch();
        if(input == 224)
            input = getch();

        switch(input)
        {
        case UP: move_up(&igrac_2);
            break;
        case DOWN: move_down(&igrac_2);
            break;
        case 'w': move_up(&igrac_1);
            break;
        case 'W': move_up(&igrac_1);
            break;
        case 's': move_down(&igrac_1);
            break;
        case 'S': move_down(&igrac_1);
            break;
        }
    }
    return 0;
}
