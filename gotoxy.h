#ifndef GOTOXY_H_INCLUDED
#define GOTOXY_H_INCLUDED
#include <windows.h>
#define gotoxy(x,y) { SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { (x),(y)});}

#endif // GOTOXY_H_INCLUDED
