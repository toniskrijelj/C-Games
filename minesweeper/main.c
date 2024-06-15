#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>

unsigned mapSizeX = 30;
unsigned mapSizeY = 20;

#define SPACE 32
#define ENTER 13

unsigned bombCount = 100;

#define MOUSE 0
#define KEYBOARD 1

int gamemode = MOUSE;

void showcursor(bool b)
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = b;
   SetConsoleCursorInfo(consoleHandle, &info);
}

 void setcolor(int ForgC)
 {
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }

typedef struct blockStruct{
    bool isBomb;
    unsigned bombsAround;
    bool revealed;
    bool hasFlag;
    unsigned flagsAround;
    struct blockStruct* next;
} block;

unsigned maxFlags;
unsigned flagsUsed = 0;

void gotoxy(int x, int y)
{
    SHORT newX = x * 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) {newX, (SHORT)y});
}

block* CreateNewBlock()
{
    block* newBlock = (block*) malloc(sizeof(block));
    newBlock->bombsAround = 0;
    newBlock->flagsAround = 0;
    newBlock->hasFlag = false;
    newBlock->isBomb = false;
    newBlock->next = NULL;
    newBlock->revealed = false;
    return newBlock;
}

block* GetBlockByIndex(int y, int x, block* head)
{
    int index = y * mapSizeX + x;
    for(int i = 0; i < index; i++)
    {
        head = head->next;
    }
    return head;
}

void FreeBlocks(block** head)
{
    if(*head == NULL)
    {
        return;
    }
    block* next = *head;
    while(next != NULL)
    {
        block* current = next;
        next = next->next;
        free(current);
        current = NULL;
    }
}

void Initialize(block** head)
{
    block* current = NULL;

    for(int i = 0; i < mapSizeY; i++)
    {
        for(int j = 0; j < mapSizeX; j++)
        {
            if((*head) == NULL)
            {
                *head = CreateNewBlock();
                current = *head;
            }
            else
            {
                current->next = CreateNewBlock();
                current = current->next;
            }
        }
    }
    gotoxy(0, mapSizeY);
    for(int i = 0; i <= mapSizeX; i++)
    {
        printf("O ");
    }
    for(int i = 0; i < mapSizeY; i++)
    {
        gotoxy(mapSizeX, i);
        printf("O");
    }
}

void SetBombs(COORD startingPosition, block* head)
{
    for(int b = 0; b < bombCount; b++)
    {
        int randX = rand() % mapSizeX, randY = rand() % mapSizeY;
        block* randomBlock = GetBlockByIndex(randY, randX, head);
        if(randomBlock->isBomb || (randX  >= startingPosition.X - 1 && randX <= startingPosition.X + 1 && randY >= startingPosition.Y - 1 && randY <= startingPosition.Y + 1))
        {
            b--;
            continue;
        }
        randomBlock->isBomb = true;
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                int currentX = randX + j;
                int currentY = randY + i;
                if(currentX < 0 || currentY < 0 || currentX >= mapSizeX || currentY >= mapSizeY)
                {
                    continue;
                }
                block* current = GetBlockByIndex(currentY, currentX, head);
                if(current->isBomb)
                {
                    continue;
                }
                current->bombsAround++;
            }
        }
    }
}


bool Reveal(COORD position, block* head)
{
    if(position.X >= mapSizeX || position.Y >= mapSizeY || position.X < 0 || position.Y < 0) return false;
    block* current = GetBlockByIndex(position.Y, position.X, head);
    if(current->hasFlag) return false;
    if(current->revealed) return false;
    if(current->isBomb) return true; /// dodati lose animaciju neku
    current->revealed = true;
    if(current->bombsAround > 0)
    {
        gotoxy(position.X,position.Y);
        setcolor(current->bombsAround + 1);
        printf("%d",current->bombsAround);
        return false;
    }
    gotoxy(position.X,position.Y);
    setcolor(7);
    printf("x");
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            SHORT newX = position.X + j, newY = position.Y + i;
            Reveal((COORD){newX, newY}, head);
        }
    }
    return false;
}

bool firstClick = true;

bool OnLeftCommand(COORD position, block* head)
{
    if(position.X >= mapSizeX || position.Y >= mapSizeY || position.X < 0 || position.Y < 0) return false;
    if(firstClick)
    {
        SetBombs(position, head);
        block* current = head;
        for(int i = 0; i < mapSizeY; i++)
        {
            for(int j = 0; j < mapSizeX; j++)
            {
                if(current->hasFlag)
                {
                    flagsUsed--;
                    current->hasFlag = false;
                    gotoxy(j, i);
                    printf(" ");
                }
                current = current->next;
            }
        }
        firstClick = false;
    }
    block* current = GetBlockByIndex(position.Y,position.X, head);
    if(current->revealed)
    {
        if(current->flagsAround >= current->bombsAround)
        {
            for(int i = -1; i <= 1; i++)
            {
                for(int j = -1; j <= 1; j++)
                {
                    SHORT newX = position.X + j, newY = position.Y + i;
                    if(Reveal((COORD){newX, newY}, head))
                    {
                        gotoxy(mapSizeX + 1, 12);
                        setcolor(12);
                        printf("IZGUBLJENO!");
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        if(Reveal(position, head))
        {
            gotoxy(mapSizeX + 1, 12);
            setcolor(12);
            printf("IZGUBLJENO!");
            return true;
        }
    }
    int i;
    current = head;
    for(i = 0; i < mapSizeY; i++)
    {
        for(int j = 0; j < mapSizeX; j++)
        {
            if(current->isBomb && !current->hasFlag)
            {
                i = mapSizeY + 1;
                break;
            }
            if(!current->isBomb && !current->revealed)
            {
                i = mapSizeY + 1;
                break;
            }
            current = current->next;
        }
    }
    if(i == mapSizeY)
    {
        gotoxy(mapSizeX + 1, 12);
        setcolor(14);
        printf("POBEDA!");
        return true;
    }
    return false;
}

void ChangeFlagCount(COORD center, int add, block* head)
{
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int currentX = center.X + j;
            int currentY = center.Y+ i;
            if(currentX < 0 || currentY < 0 || currentX >= mapSizeX || currentY >= mapSizeY)
            {
                continue;
            }
            GetBlockByIndex(currentY, currentX, head)->flagsAround += add;
        }
    }
}

bool OnRightCommand(COORD position, block* head)
{
    if(position.X >= mapSizeX || position.Y >= mapSizeY || position.X < 0 || position.Y < 0) return false;
    block* current = GetBlockByIndex(position.Y, position.X, head);
    if(current->revealed) return false;
    if(current->hasFlag)
    {
        flagsUsed--;
        current->hasFlag = false;
        gotoxy(position.X,position.Y);
        printf(" ");
        ChangeFlagCount(position, -1, head);
    }
    else
    {
        if(flagsUsed == maxFlags) return false;
        flagsUsed++;
        current->hasFlag = true;
        gotoxy(position.X,position.Y);
        setcolor(14);
        printf("X");
        ChangeFlagCount(position, 1, head);
    }
    if(!firstClick)
    {
        int i;
        block* current = head;
        for(i = 0; i < mapSizeY; i++)
        {
            for(int j = 0; j < mapSizeX; j++)
            {
                if(current->isBomb && !current->hasFlag)
                {
                    i = mapSizeY + 1;
                    break;
                }
                if(!current->isBomb && !current->revealed)
                {
                    i = mapSizeY + 1;
                    break;
                }
                current = current->next;
            }
        }
        if(i == mapSizeY)
        {
            gotoxy(mapSizeX + 1, 12);
            setcolor(14);
            printf("POBEDA!");
            return true;
        }
    }
    return false;
}

void clrscr()
{
    gotoxy(0, 0);
    for(int i = 0; i < 1000; i++)
    {
        printf("             ");
    }
    gotoxy(0,0);
    printf(" ");
    gotoxy(0, 0);
}

void SetPlayingDevice()
{
    printf("(0) Mouse\n(1) Keyboard\nEnter mode:");
    do{
        scanf("%d",&gamemode);
        fflush(stdin);
    }while(gamemode != MOUSE && gamemode != KEYBOARD);
    clrscr();
}

void PlayWithMouse(block* head)
{
    showcursor(false);

    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    while (true)
    {
        ReadConsoleInput(
                hStdin,
                irInBuf,
                128,
                &cNumRead);

        for (i = 0; i < cNumRead; i++)
        {
            if(irInBuf[i].EventType == MOUSE_EVENT)
            {
                if(irInBuf[i].Event.MouseEvent.dwEventFlags == 0)
                {
                    if(irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    {
                        if(irInBuf[i].Event.MouseEvent.dwMousePosition.X % 2 == 0)
                        {
                            COORD position = irInBuf[i].Event.MouseEvent.dwMousePosition;
                            position.X /= 2;
                            if(OnLeftCommand(position, head))
                            {
                                return;
                            }
                        }
                    }
                    else if(irInBuf[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
                    {
                        if(irInBuf[i].Event.MouseEvent.dwMousePosition.X % 2 == 0)
                        {
                            COORD position = irInBuf[i].Event.MouseEvent.dwMousePosition;
                            position.X /= 2;
                            if(OnRightCommand(position, head))
                            {
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void PlayWithKeyboard(block* head)
{
    COORD position = {0,0};
    while(true)
    {
        gotoxy(position.X,position.Y);
        int input = getch();
        if(input == 'w' || input == 'W')
        {
            if(position.Y - 1 >= 0)
            {
                position.Y--;
                gotoxy(position.X,position.Y);
            }
        }else if(input == 's' || input == 'S')
        {
            if(position.Y + 1 < mapSizeY)
            {
                position.Y++;
                gotoxy(position.X,position.Y);
            }
        }else if(input == 'a' || input == 'A')
        {
            if(position.X - 1 >= 0)
            {
                position.X--;
                gotoxy(position.X,position.Y);
            }
        }else if(input == 'd' || input == 'D')
        {
            if(position.X + 1 < mapSizeX)
            {
                position.X++;
                gotoxy(position.X,position.Y);
            }
        }else if(input == SPACE)
        {
            if(OnLeftCommand(position, head))
            {
                return;
            }
        }
        else if(input == ENTER)
        {
            if(OnRightCommand(position, head))
            {
                return;
            }
        }
    }
}

void SetMapSizeAndBombCount()
{
    clrscr();
    printf("Enter map size X:");
    do{
        scanf("%d",&mapSizeX);
        fflush(stdin);
    }while(mapSizeX <= 0 || mapSizeX >= 79 / 2);
    printf("\nEnter map size Y:");
    do{
        scanf("%d",&mapSizeY);
        fflush(stdin);
    }while(mapSizeY <= 0 || mapSizeY >= 79);
    printf("\nEnter bomb count:");
    do{
        scanf("%d",&bombCount);
        fflush(stdin);
    }while(bombCount <= 0 || bombCount > mapSizeX * mapSizeY);
    clrscr();
}

int main()
{
    srand(time(NULL));

    SetPlayingDevice();
    SetMapSizeAndBombCount();

    maxFlags = bombCount;

    block* head = NULL;

    Initialize(&head);

    if(gamemode == MOUSE)
    {
        PlayWithMouse(head);
    }
    else
    {
        PlayWithKeyboard(head);
    }
    FreeBlocks(&head);
    return 0;
}
