#include "battleship.h"

int main()
{
    bot_bomb.x = -1;
    srand(time(NULL));
    tabla** tabla_prvog = malloc(10*(sizeof(tabla*)));
    tabla** tabla_drugog = malloc(10*(sizeof(tabla*)));
    tabla** gadjanja_prvog = malloc(10*(sizeof(tabla*)));
    tabla** gadjanja_drugog = malloc(10*(sizeof(tabla*)));
    for(int i = 0; i < 10; i += 1)
    {
        tabla_prvog[i] = malloc(sizeof(tabla)*10);  /// dodeljivanje memorije
        tabla_drugog[i] = malloc(sizeof(tabla)*10); ///     svakoj tabli
        gadjanja_prvog[i] = malloc(sizeof(tabla)*10);
        gadjanja_drugog[i] = malloc(sizeof(tabla)*10);
    }

    inicijalizacija(tabla_prvog,tabla_drugog,gadjanja_prvog,gadjanja_drugog); /// postavljanje u inicijalno stanje

    if(welcome_screen()==1)  /// provera da li se uopste igra
    {
        system("cls");
        printf("(1) Igrac protiv igraca\n(2) Igrac protiv kompa");
        int izbor;
        do
        {
            fflush(stdin);
            printf("\n\nIzaberi:\n\n>> ");
            scanf("%d",&izbor);
            if(izbor != 1 && izbor != 2)
                printf("\nNeispravan unos!");  ///  zastita od ne ispravnog unosa
        }
        while(izbor != 1 && izbor != 2);
        system("cls");
        draw(tabla_prvog);
        if(izbor == COVEK)
            colprintxy(LIGHT_GRAY,"IGRAC 1 POSTAVLJA BRODOVE",14,5);
        else
            colprintxy(LIGHT_GRAY,"IGRAC POSTAVLJA BRODOVE",14,5);

        popuni_tabelu(tabla_prvog,COVEK);

        system("cls");
        gotoxy(34,12);
        printf("CEKA SE ZAMENA...");
        Sleep(2000);
        system("cls");
        draw(tabla_drugog);
        if(izbor == COVEK)
            colprintxy(LIGHT_GRAY,"IGRAC 2 POSTAVLJA BRODOVE",14,5);
        else
            colprintxy(LIGHT_GRAY,"KOMP POSTAVLJA BRODOVE",14,5);
        popuni_tabelu(tabla_drugog,izbor); /// popunjavanje tabla
        int ko_igra_prvi = rand()%2;
        if(izbor == COVEK || ko_igra_prvi == 0)
            while(1)
            {
                system("cls");
                setcolor(LIGHT_GRAY);
                gotoxy(7,15);
                if(izbor == COVEK)
                    printf("IGRA 1 IGRAC");
                else
                    printf("IGRA IGRAC");
                if(pogadjaj(tabla_drugog,gadjanja_prvog,COVEK)==1)
                {
                    pobedio(1,izbor);
                    break;
                }
                Sleep(250);
                system("cls");
                gotoxy(34,12);
                printf("ZAMENA IGRACA...");
                Sleep(250);
                system("cls");
                setcolor(LIGHT_GRAY);
                gotoxy(7,15);
                if(izbor == COVEK)
                    printf("IGRA 2 IGRAC");
                else
                    printf("IGRA KOMP");
                if(pogadjaj(tabla_prvog,gadjanja_drugog,izbor)==1)
                {
                    pobedio(2,izbor);
                    break;
                }
                Sleep(250);
                system("cls");
                gotoxy(34,12);
                printf("ZAMENA IGRACA...");
                Sleep(250);
            }
        else
            while(1)
            {
                system("cls");
                setcolor(LIGHT_GRAY);
                gotoxy(7,15);
                printf("IGRA KOMP");
                if(pogadjaj(tabla_prvog,gadjanja_drugog,KOMP)==1)
                {
                    pobedio(2,izbor);
                    break;
                }
                Sleep(250);
                system("cls");
                gotoxy(34,12);
                printf("ZAMENA IGRACA...");
                Sleep(250);
                system("cls");
                setcolor(LIGHT_GRAY);
                gotoxy(7,15);
                printf("IGRA IGRAC");
                if(pogadjaj(tabla_drugog,gadjanja_prvog,COVEK)==1)
                {
                    pobedio(1,izbor);
                    break;
                }
                Sleep(250);
                system("cls");
                gotoxy(34,12);
                printf("ZAMENA IGRACA...");
                Sleep(250);
            }
    }
    for(int i = 0; i < 10; i += 1)
    {
        free(tabla_prvog[i]);
        free(tabla_drugog[i]);
    }
    free(tabla_prvog);
    free(tabla_drugog); /// oslobadjanje memorije
}
