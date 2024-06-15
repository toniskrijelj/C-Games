#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

void gotoxy(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) {(SHORT)(x), (SHORT)(y)});
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
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

class Time
{
private:
    static struct timeval start;
    static long double deltaTime;
public:

    static void Start()
    {
        gettimeofday(&start, NULL);
        deltaTime = 0;
    }

    static long double GetTime()
    {
        struct timeval now;
        gettimeofday(&now, NULL);
        long s = (now.tv_sec - start.tv_sec);
        long m = ((s * 1000000) + now.tv_usec) - (start.tv_usec);
        return ((long double)(m) / 1000000);
    }

    static void SetDeltaTime(long double newDeltaTime)
    {
        deltaTime = newDeltaTime;
    }

    static long double DeltaTime()
    {
        return deltaTime;
    }
};
struct timeval Time::start;
long double Time::deltaTime = 0;

static int score = 0;
class Pipe
{
private:
    int x=0, y=0;
    static int gapSize;
public:

    bool passedPlayer = false;

    static void SetGapSize(int newGapSize)
    {
        gapSize = newGapSize;
    }

    Pipe()
    {
        y = rand() % (23 - gapSize + 2);
        x = 79;
        int i;
        for(i = 0; i < y; i++)
        {
            gotoxy(x, i);
            printf("%c",219);
        }
        i += gapSize;
        for(; i < 24; i++)
        {
            gotoxy(x, i);
            printf("%c",219);
        }
    }

    ~Pipe()
    {

    }

    bool Move(int birdX)
    {
        x--;

        if(x == -1)
        {
            x+=1;
            int i;
            for(i = 0; i < y; i++)
            {
                gotoxy(x, i);
                printf(" ");
            }
            i += gapSize;
            for(; i < 24; i++)
            {
                gotoxy(x, i);
                printf(" ");
            }
            return true;
        }

        if(birdX - 1 == x)
        {
            gotoxy(0,24);
            setcolor(14);
            printf("%d",++score);
            printf("\a");
            passedPlayer = true;
        }

        setcolor(10);
        int i;
        for(i = 0; i < y; i++)
        {
            gotoxy(x, i);
            printf("%c ",219);
        }
        i += gapSize;
        for(; i < 24; i++)
        {
            gotoxy(x, i);
            printf("%c ",219);
        }

        return false;
    }

    bool CheckCollisionWith(int x, int y)
    {
        if(this->x != x)
        {
            return false;
        }
        if(y < this->y || y > this->y + gapSize - 1)
        {
            return true;
        }
        return false;
    }

    friend class Bird;
};
int Pipe::gapSize;

class Pipes
{
private:

    int numberOfPipes = 0;
    double timeOfLastMovement = 0, timeOfLastSpawn = 0;
    double timeBetweenMovement=0, timeBetweenSpawn=0;
    Pipe** pipes = NULL;

    void AddPipe()
    {
        Pipe* pipe = new Pipe();
        pipes =(Pipe**)realloc(pipes, sizeof(Pipe*) * (numberOfPipes + 1));
        pipes[numberOfPipes] = pipe;
        numberOfPipes++;
    }

    void RemoveFirstPipe()
    {
        Pipe** temp = (Pipe**)malloc(sizeof(Pipes*) * (numberOfPipes - 1));
        for(int i = 1; i < numberOfPipes; i++)
        {
            temp[i - 1] = pipes[i];
        }
        (*(pipes[0])).~Pipe();
        free(pipes);
        pipes = temp;
        numberOfPipes--;
    }

    void MoveAll(int birdX)
    {
        if(numberOfPipes == 0)
        {
            return;
        }

        int i = 1;

        if((*(pipes[0])).Move(birdX))
        {
            RemoveFirstPipe();
            i--;
        }

        for(; i < numberOfPipes; i++)
        {
            (*(pipes[i])).Move(birdX);
        }
    }

public:

    Pipes(double moveSpeed, double spawnSpeed)
    {
        timeBetweenMovement = moveSpeed;
        timeBetweenSpawn = spawnSpeed * moveSpeed;
        AddPipe();
    }

    void Update(int birdX)
    {
        long double currentTime = Time::GetTime();
        if(currentTime - timeOfLastMovement >= timeBetweenMovement)
        {
            timeOfLastMovement = currentTime;
            MoveAll(birdX);
        }
        if(currentTime - timeOfLastSpawn >= timeBetweenSpawn)
        {
            timeOfLastSpawn = currentTime;
            AddPipe();
        }

    }

    ~Pipes()
    {
        while(numberOfPipes > 0)
        {
            RemoveFirstPipe();
        }
        free(pipes);
    }
    friend class Bird;
};

class Bird
{
private:
    int x=0, intY=0;
    double y=0;

    bool gravityDecreased = false;
    double gravityAmount = 15;
    long double timeOfLastJump = 0;
    double gravityReset = 0.4;
    double decreasedGravityDivider = 5;
    double jumpHeight = 1.6;
public:
    Bird(int x = 5, int y = 12, double gravityAmount = 15, double jumpHeight = 1.6, double gravityReset = 0.4, double decreasedGravityDivider = 5)
    {
        this->x = x;
        this->y = y;
        this->gravityAmount = gravityAmount;
        this->jumpHeight = jumpHeight;
        this->gravityReset = gravityReset;
        this->decreasedGravityDivider = decreasedGravityDivider;
        gotoxy(x,y);
        printf("%c",219);
    }

    void AffectGravity()
    {
        double yChange = 0;
        if(gravityDecreased == true)
        {
            //yChange = Time::DeltaTime() * gravityAmount / decreasedGravityDivider;
        }
        else
        {
            yChange = Time::DeltaTime() * gravityAmount;
        }
        y += yChange;
    }

    void CheckForGravityReset()
    {
        if(gravityDecreased == true)
        {
            long double currentTime = Time::GetTime();
            if(currentTime >= timeOfLastJump + gravityReset)
            {
                gravityDecreased = false;
            }
        }
    }

    void CheckPositionChange()
    {
        int nowY = (int)y;
        if(nowY != intY)
        {
            Move(nowY);
        }
    }

    void Move(int newY)
    {
        gotoxy(x, intY);
        printf(" ");
        gotoxy(x, newY);
        setcolor(4);
        printf("%c",219);
        intY = newY;
    }

    bool CheckCollision(Pipes* pipes)
    {
        if(intY < 0 || intY > 23)
        {
            return true;
        }
        for(int i = 0; i < pipes->numberOfPipes; i++)
        {
            if(!(*(pipes->pipes[i])).passedPlayer)
            {
                return (*(pipes->pipes[i])).CheckCollisionWith(x, intY);
            }
        }
        return false;
    }

    bool Jump(Pipes* pipes)
    {
        y -= jumpHeight;
        CheckPositionChange();
        timeOfLastJump = Time::GetTime();
        gravityDecreased = true;
        return CheckCollision(pipes);
    }

    bool Update(Pipes* pipes)
    {
        CheckForGravityReset();
        AffectGravity();
        CheckPositionChange();
        if(CheckCollision(pipes))
        {
            return true;
        }
        return false;
    }

    int GetX()
    {
        return x;
    }
};

int main()
{
    srand(time(NULL));
    hidecursor();
    bool gameEnded;
    do
    {
        system("cls");
        int gapSize;
        double moveSpeed;
        int gapsBetweenPipes;
        double gravityScale;
        double jumpHeight;
        double gravityReset;
        double decreasedGravityDivider;

        printf("Welcome to flappy bird... Recommended settings: 5, 0.05, 30, 15, 1.6, 0.4, 5\n");
        printf("Enter gap size of pipe: (enter 0 to load recommended settings) ");
        scanf("%d",&gapSize);
        if(gapSize != 0)
        {
            printf("Enter time between moving pipes: ");
            scanf("%lf",&moveSpeed);
            printf("Enter gap size between two pipes: ");
            scanf("%d",&gapsBetweenPipes);
            printf("Enter gravity scale: ");
            scanf("%lf",&gravityScale);
            printf("Enter jump height: ");
            scanf("%lf",&jumpHeight);
            printf("Enter gravity reset timer: ");
            scanf("%lf",&gravityReset);
            printf("Enter decreased gravity divider: ");
            scanf("%lf",&decreasedGravityDivider);

            if(gapSize <= 0 || moveSpeed <= 0 || gapsBetweenPipes <= 0 || gapSize >= 25 || gapsBetweenPipes < 1 || gravityScale < 0 || jumpHeight < 0 || gravityReset < 0 || decreasedGravityDivider <= 0)
            {
                printf("Error! Some values aren't correct, no value can be < 0 and some can't be 0");
                return -1;
            }
        }

        system("cls");

        Pipe::SetGapSize((gapSize == 0) ? 5 : gapSize);
        Time::Start();
        Bird bird = (gapSize == 0) ? Bird() : Bird(5, 12, gravityScale,jumpHeight,gravityReset,decreasedGravityDivider);
        Pipes pipes = (gapSize == 0) ?  Pipes(.05, 30) : Pipes(moveSpeed, gapsBetweenPipes);
        gameEnded = false;
        long double lastFrameTime = 0;
        do
        {
            do
            {
                long double thisFrameTime = Time::GetTime();
                Time::SetDeltaTime(thisFrameTime - lastFrameTime);
                lastFrameTime = thisFrameTime;
                pipes.Update(bird.GetX());
                if(bird.Update(&pipes))
                {
                    gameEnded = true;
                    break;
                }
            }while(!kbhit());
            if(gameEnded)
            {
                break;
            }
            char input = getch();
            if(input == 27)
            {
                break;
            }
            if(input == 32)
            {
                if(bird.Jump(&pipes))
                {
                    gameEnded = true;
                    break;
                }
            }
        }while(1);
        setcolor(7);
        pipes.~Pipes();
        system("cls");
        gotoxy(31,12);
        printf("YOU DIED!");
        gotoxy(31,13);
        printf("Score: %d",score);
        score = 0;
        if(gameEnded)
        {
            gotoxy(31,14);
            printf("Do you want to play again?");
            gotoxy(31,15);
            printf("Enter - YES");
            gotoxy(31,16);
            printf("ESC - NO");
            int input;
            do{
                input = getch();
            }while(input != 27 && input != 13);
            if(input == 27)
            {
                gameEnded = false;
            }
        }
    }
    while(gameEnded);
    return 0;
}
