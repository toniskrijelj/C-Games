#ifndef POMERISCREEN_H_INCLUDED
#define POMERISCREEN_H_INCLUDED

HANDLE hStdout;
void ScrollConsoleUp(int iRows)
{
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    SMALL_RECT srctWindow;
    if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo));
    srctWindow = csbiInfo.srWindow;
    if ( srctWindow.Top >= iRows )
    {
        srctWindow.Top -= (SHORT)iRows;
        srctWindow.Bottom -= (SHORT)iRows;
        if (! SetConsoleWindowInfo(
                   hStdout,
                   TRUE,
                   &srctWindow));
    }
}



#endif // POMERISCREEN_H_INCLUDED
