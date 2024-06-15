#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
#define ESC 27
#define ENTER 13

#define GAMESPEED 10

#define UNISTI 0
#define POSTAVI 1

#define ON 1
#define OFF 0

#define colprintxy(color,x,y,format) {setcolor(color);gotoxy(x,y);printf("%c%c",219,219);}
void gotoxy(int x, int y) {SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x*2+25,y+2});}

void setcolor(int color){
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

int block_list[7][4][4] =
{
    {
        { 0 , 0 , 0 ,  0 },
        { 0 , 0 , 0 ,  0 },
        { 11, 11, 11, 11 },
        { 0 , 0 , 0 ,  0 }
    },
    {
        { 0 , 0 , 0 , 0 },
        { 0 , 14, 14, 0 },
        { 0 , 14, 14, 0 },
        { 0 , 0 , 0 , 0 }
    },
    {
        { 0 , 0 , 0 , 0 },
        { 5 , 0 , 0 , 0 },
        { 5 , 5 , 5 , 0 },
        { 0 , 0 , 0 , 0 }
    },
    {
        { 0 , 0 , 0 , 0 },
        { 0 , 10, 10, 0 },
        { 10, 10, 0 , 0 },
        { 0 , 0 , 0 , 0 }
    },
    {
        { 0 ,12 , 0 , 0 },
        {12 ,12 ,12 , 0 },
        { 0 , 0 , 0 , 0 },
        { 0 , 0 , 0 , 0 }
    },
    {
        { 0 , 0 , 0 , 0 },
        { 3 , 3 , 0 , 0 },
        { 0 , 3 , 3 , 0 },
        { 0 , 0 , 0 , 0 }
    },
    {
        { 0 , 0 , 0 , 0 },
        { 0 , 0 , 2 , 0 },
        { 2 , 2 , 2 , 0 },
        { 0 , 0 , 0 , 0 }
    }
};

typedef struct
{
    int x,y,block_size;
}Position;

int stage[21][12];
int block[4][4];
int gameover = 0;
int score = 0, counter = 0;
int block_next = -1;
int help = OFF;
void updateScore()
{
    gotoxy(14,10);
    setcolor(7);
    printf("Score: ");
    setcolor(14);
    printf("%6d",score);
}

bool isCollide(int x,int y,int block_size)
{
    for(int i = 0; i < block_size; i += 1)
    {
        for(int j = 0; j < block_size; j += 1)
        {
            if(stage[y + i][x + j] > 0 && block[i][j] > 0)
                return true;
        }
    }
    return false;
}

void updateHelp(Position* position,int mode)
{
    int y = position->y;
    while(!isCollide(position->x, y + 1, position->block_size))
    {
        y += 1;
    }
    setcolor(15);
    for(int i = 0; i < position->block_size; i += 1)
    {
        for(int j = 0; j < position->block_size; j += 1)
        {
            if(block[i][j] > 0)
            {
                gotoxy(j+position->x,i+y);
                printf("%c%c",(mode == UNISTI) ? ' ' : 219,(mode == UNISTI) ? ' ' : 219);
            }
        }
    }
}

int title()
{
    printf("#==============================================================================#\n");
    setcolor(2);
    printf("####### ####### ####### ######    ###    #####\n");
    setcolor(3);
    printf("   #    #          #    #     #    #    #     #\n");
    setcolor(3);
    printf("   #    #          #    #     #    #    #\n");
    setcolor(3);
    printf("   #    #####      #    ######     #     #####\n");
    setcolor(3);
    printf("   #    #          #    #   #      #          #\n");
    setcolor(3);
    printf("   #    #          #    #    #     #    #     #\n");
    setcolor(2);
    printf("   #    #######    #    #     #   ###    #####");
    printf("\n\n\n");
    setcolor(7);
    printf("\t\t\t\t\t\tCommands:\n");
    printf("           <Menu>\n");
    printf("\t1: Start Game");
    printf("\t\tLeft,right and down arrow key -- Move");
    printf("\n\t2: Quit\n\t\t\t\tSpace -- Drop   |   Up arrow key -- Rotate\n\n");

    printf("#==============================================================================#\n");
    printf("Choose >> ");

    int input;
    do{
        input = getch();
    }while(input!= '1' && input != '2');
    return input - '0';
}

void makeBlock(Position* position)
{
    position->x = 4;
    position->y = 0;
    int block_type;
    if(block_next == -1)
    {
        if((block_type = rand() % 8) == 7)
            block_type = rand() % 7;
    }
    else
    {
        block_type = block_next;
    }

    block_next = rand() % 8;
    if(block_next == 7 || block_next == block_type)
        block_next = rand() % 7;

    for(int i = 0; i < 4; i += 1)
    {
        for(int j = 0; j < 4; j += 1)
        {
            if(block_list[block_next][i][j] == 0)
            {
                gotoxy(15 + j, 3 + i);
                printf("  ");
            }
            else
            {
                colprintxy(block_list[block_next][i][j],j + 15, i + 3,"x");
            }
        }
    }

    if(block_type == 0 || block_type == 1)
        position->block_size = 4;
    else
        position->block_size = 3;
    for(int i = 0; i < position->block_size; i += 1)
    {
        for(int j = 0; j < position->block_size; j += 1)
        {
            block[i][j] = block_list[block_type][i][j];
        }
    }

    for(int i = 0; i < position->block_size; i += 1)
    {
        for(int j = 0; j < position->block_size; j += 1)
        {
            if(stage[i][j + position->x] > 0 && block[i][j] > 0)
            {
                gameover = true;
                return;
            }
            else if(block[i][j] > 0)
            {
                colprintxy(block[i][j],j + position->x,i,"x");
            }
        }
    }
    if(help == ON)
        updateHelp(position,POSTAVI);
}

void initGame(Position* position)
{
    system("cls");
    for(int y = 0; y < 21; y += 1)
    {
        for(int x = 0; x < 12; x += 1)
        {
            if(x == 0 || x == 11 || y == 20)
            {
                stage[y][x] = 1;
                colprintxy(7,x,y,"x");
            }
            else
            {
                stage[y][x] = 0;
                gotoxy(x,y);
                printf("  ");
            }
        }
    }
    score = 0;
    gameover = 0;
    block_next = -1;
    updateScore();
    makeBlock(position);
}

void moveBlock(int x, int y,Position* position)
{
    for (int i = 0; i < position->block_size; i+=1)
    {
        for (int j = 0; j < position->block_size; j+=1)
        {
            if(block[i][j] > 0)
            {
                gotoxy(j + position->x,i + position->y);
                printf("  ");
            }
        }
    }

    if(help == ON)
        updateHelp(position,UNISTI);

    position->x = x;
    position->y = y;

    for (int i = 0; i < position->block_size; i++)
    {
        for (int j = 0; j < position->block_size; j++)
        {
            if(block[i][j] > 0)
            {
                colprintxy(block[i][j],position->x + j,position->y + i,"x");
            }
        }
    }
    if(help == ON)
        updateHelp(position,POSTAVI);
}

void rotateBlock(Position* position)
{
    int temp[position->block_size][position->block_size];

    if(help == ON)
        updateHelp(position,UNISTI);

    for(int i = 0; i < position->block_size; i += 1)
        for(int j = 0; j < position->block_size; j += 1)
            temp[i][j] = block[i][j];

    for(int i = 0; i < position->block_size; i += 1)
        for(int j = 0; j < position->block_size; j += 1)
            block[i][j] = temp[position->block_size - 1 - j][i];

    if(isCollide(position->x,position->y,position->block_size))
    {
        for (int i = 0; i < position->block_size; i+=1)
        {
            for (int j = 0; j < position->block_size; j+=1)
            {
                block[i][j] = temp[i][j];
            }
        }
        if(help == ON)
            updateHelp(position,POSTAVI);
        return;
    }
    for (int i = 0; i < position->block_size; i+=1)
    {
        for (int j = 0; j < position->block_size; j+=1)
        {
            if(stage[position->y + i][position->x + j] > 0 && temp[i][j] > 0)
            {
                gotoxy(position->x + j,position->y + i);
                printf("  ");
            }
            else if(stage[position->y + i][position->x + j] == 0 && block[i][j] == 0)
            {
                gotoxy(position->x + j,position->y + i);
                printf("  ");
            }
            else if(block[i][j] > 0)
            {
                colprintxy(block[i][j],position->x + j,position->y + i,"x");
            }
        }
    }
    if(help == ON)
        updateHelp(position,POSTAVI);
}

void dropBlock(Position* position)
{
    int lines = 0;
    while(!isCollide(position->x, position->y + 1, position->block_size))
    {
        lines += 1;
        moveBlock(position->x, position->y + 1, position);
    }
    score += lines;
    updateScore();
    counter = 100;
}

void userInput(Position* position)
{
    int input = getch();

    if(input == 224)
        input = getch();

    switch (input)
    {
    case RIGHT:
        if (!isCollide(position->x + 1, position->y,position->block_size))
        {
            moveBlock(position->x + 1, position->y,position);
        }
        break;
    case LEFT:
        if (!isCollide(position->x - 1, position->y,position->block_size))
        {
            moveBlock(position->x - 1, position->y,position);
        }
        break;
    case DOWN:
        if (!isCollide(position->x, position->y + 1,position->block_size))
        {
            moveBlock(position->x, position->y + 1,position);
        }
        break;
    case UP:
        rotateBlock(position);
        break;
    case SPACE:
        dropBlock(position);
        break;
    case 'j':
    case 'J':
        help = (help == ON) ? OFF : ON;
        updateHelp(position,(help == ON) ? POSTAVI : UNISTI);
        break;
    }
}

void checkPlaced(Position* position)
{
    int q;
    int lines = 0;
    for(int i = position->y; i < 20 && i < (position -> y + position -> block_size); i += 1)
    {
        q = true;
        for(int j = 1; j < 11; j += 1)
        {
            if(stage[i][j] == 0)
            {
                q = false;
                break;
            }
        }
        if(q)
        {
            lines += 1;
            for(int k = i; k > 0; k -= 1)
            {
                for(int j = 1; j < 11; j += 1)
                {
                    stage[k][j] = stage[k - 1][j];
                    if(stage[k][j] > 0)
                    {
                        colprintxy(stage[k][j],j,k,"x");
                    }
                    else
                    {
                        gotoxy(j,k);
                        printf("  ");
                    }
                }
            }
        }
    }
    switch(lines)
    {
    case 1:
        score += 40;
        break;
    case 2:
        score += 100;
        break;
    case 3:
        score += 300;
        break;
    case 4:
        score += 1200;
    }
    updateScore();
}
void gameOver()
{
    system("cls");
    setcolor(12);
    printf(" #####     #    #     # ####### ####### #     # ####### ######\n");
    setcolor(5);
    printf("#     #   # #   ##   ## #       #     # #     # #       #     #\n");
    setcolor(12);
    printf("#        #   #  # # # # #       #     # #     # #       #     #\n");
    setcolor(5);
    printf("#  #### #     # #  #  # #####   #     # #     # #####   ######\n");
    setcolor(12);
    printf("#     # ####### #     # #       #     #  #   #  #       #   #\n");
    setcolor(5);
    printf("#     # #     # #     # #       #     #   # #   #       #    #\n");
    setcolor(12);
    printf(" #####  #     # #     # ####### #######    #    ####### #     #\n");
    setcolor(7);
    printf("\n\nYour score was:");
    setcolor(14);
    printf(" %6d",score);
    setcolor(7);
    printf("\n\nPress ");
    setcolor(10);
    printf("ENTER ");
    setcolor(7);
    printf("to restart game...\n");
    printf("\nPress ");
    setcolor(2);
    printf("ESC");
    setcolor(7);
    printf(" to quit...\n\n");
}

void placeBlock(Position* position)
{
    int q = 0;
    for(int i = 0; i < position->block_size; i += 1)
    {
        for(int j = 0; j < position->block_size; j += 1)
        {
            if(block[i][j] > 0)
            {
                if(q == 0)
                {
                    q = 1;
                    setcolor(block[i][j]);
                }
                gotoxy(j+position->x,i+position->y);
                printf("%c%c",219,219);
                stage[i + position->y][j + position->x] = block[i][j];
            }
        }
    }
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
    if(title()==2)
    {
        printf("\n\n");
        return 0;
    }
    int input;
    srand(time(NULL));
    hidecursor();
    Position position;
    do{
        initGame(&position);
        while(!gameover)
        {
            if(kbhit())
            {
                userInput(&position);
            }
            if(counter == 100)
            {
                if(!isCollide(position.x, position.y + 1, position.block_size))
                {
                    moveBlock(position.x, position.y + 1, &position);
                }
                else
                {
                    placeBlock(&position);
                    checkPlaced(&position);
                    makeBlock(&position);
                }
                counter = 0;
            }
            else
                counter += 1;
            Sleep(GAMESPEED);
        }
        gameOver();
        do{
            input = getch();
        }while(input != ENTER && input != ESC);
    }while(input == ENTER);
}
