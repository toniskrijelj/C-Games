#include "battleship.h"

void setcolor(int ForgC)  /// funkcija za menjanje boja
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
void draw(tabla** tabla)   /// ispis mapa igraca
{
    gotoxy(0,0);
    printf(" ");
    for(int i = 0; i < 10 + 1; i += 1)
    {
        for(int j = 0; j < 10 + 2; j += 1)
        {
            if(i == 0 && j < 10)
            {
                setcolor(LIGHT_GRAY);
                printf(" %d",j+1);
            }
            else
            {
                if(j == 0)
                {
                    setcolor(LIGHT_GRAY);
                    printf("%c",(64+i));
                }
                else if(i > 0 && j > 1)
                {
                    if(tabla[i-1][j-2].object == 0 && tabla[i-1][j-2].pogodjeno == NE)
                    {
                        setcolor(LIGHT_GRAY);
                        printf(". ");
                    }
                    else if(tabla[i-1][j-2].object == 0 && tabla[i-1][j-2].pogodjeno == DA)
                    {
                        setcolor(LIGHT_GRAY);
                        printf("x ");
                    }
                    else if(tabla[i-1][j-2].pogodjeno == DA)
                    {
                        setcolor(CYAN);
                        printf("x ");
                    }
                    else
                    {
                        setcolor(RED);
                        printf("x ");
                    }
                }
                else if(j==1)
                    printf(" ");
            }
        }
        printf("\n");
    }
}
void inicijalizacija(tabla** tabla1, tabla** tabla2,tabla** pogodjena_tabla_1,tabla** pogodjena_tabla_2) /// inicijalizacija mape
{
    for(int i = 0; i < 10; i += 1)
    {
        for(int j = 0; j < 10; j += 1)
        {
            tabla1[i][j].object = 0;
            tabla1[i][j].pogodjeno = NE;
            tabla1[i][j].brod_je_tu = NE;
            tabla2[i][j].object = 0;         /// brod_je_tu koristimo kao pomoc
            tabla2[i][j].brod_je_tu = NE;   /// kod pomeranja broda prilikom postavljanja brodova
            tabla2[i][j].pogodjeno = NE;
            pogodjena_tabla_1[i][j].pogodjeno = NE;
            pogodjena_tabla_1[i][j].object = 0;
            pogodjena_tabla_2[i][j].pogodjeno = NE;
            pogodjena_tabla_2[i][j].object = 0;
        }
    }
}
void getInput(int* variable) /// funkcija za unos
{
    gotoxy(79,24);
    *variable = getch();
    if(*variable == 224)
        *variable = getch();
}
void colprintxy(int color,const char* text,int x,int y)
{
    setcolor(color);
    gotoxy(x*2+2,y+1)     /// funkcija za spojeno postavljanje boje, i ispisa na ekran
    printf(text);
}
void update_collision(tabla** tabla,int x, int y,int type) /// funkcija koja proverava promenu dodira brodova na mapi
{
    /// polja oko prosledjenog polja i updejtuje ih
    if(x >= 0 && x <= 9 && y >= 0 && y <= 9)
    {
        if(x > 0)
        {
            if(tabla[y][x-1].object==1)
            {
                colprintxy(((type == SUMMON) ? RED : LIGHT_GRAY),"x ",x-1,y);
                tabla[y][x].brod_je_tu = (type == DESTROY) ? DA : NE;         /// imamo kada postavljamo brod (SUMMON)
            }                                                                /// i kada ga uklanjamo (DESTROY)
            else if(tabla[y][x-1].brod_je_tu == DA)
            {
                colprintxy(CYAN,"x ",x-1,y);
            }
        }
        if(y > 0)
        {
            if(tabla[y-1][x].object==1)
            {
                colprintxy(((type == SUMMON) ? RED : LIGHT_GRAY),"x ",x,y-1);
            }
            else if(tabla[y-1][x].brod_je_tu == DA)
            {
                colprintxy(CYAN,"x ",x,y-1);
            }
        }
        if(x < 9)
        {
            if(tabla[y][x+1].object==1)                                        /// puno if-ova radi sigurnosti da necemo otici van
            {
                colprintxy(((type == SUMMON) ? RED : LIGHT_GRAY),"x ",x+1,y); /// dometa matrice 10x10
            }
            else if(tabla[y][x+1].brod_je_tu == DA)
            {
                colprintxy(CYAN,"x ",x+1,y);
            }
        }
        if(y < 9)
        {
            if(tabla[y+1][x].object==1)
            {
                colprintxy(((type == SUMMON) ? RED : LIGHT_GRAY),"x ",x,y+1);
            }                                                            ///     ako SUMMON-ujemo (postavljamo brodic)
            else if(tabla[y+1][x].brod_je_tu == DA)                      /// svi vec postavljeni prodovi oko njega
            {
                colprintxy(CYAN,"x ",x,y+1);                           ///      bice crveni da bi prikazalo da se tu
            }                                                         ///   ne moze postaviti brod
        }
        if(tabla[y][x].object==1)
        {
            colprintxy(((type == SUMMON) ? RED : LIGHT_GRAY),"x ",x,y);        /// kada uklanjamo brod (type != SUMMON)
        }                                                                     /// brodice koje smo dodirivali, vise ne dodirujemo
        else                                                                 /// i vracamo ih na sivu boju kao znak da je sve ok
        {
            colprintxy((type == SUMMON) ? CYAN : LIGHT_GRAY,((type == SUMMON) ? "x " : ". "),x,y);
            tabla[y][x].brod_je_tu = (type == SUMMON) ? DA : NE;
        }                                                              /// tamo gde pomerimo brod (gde je trenutno brod)
    }                                                                 /// ispisujemo plavom bojom radi lakseg pregleda
}
int check_error(tabla** tabla, int x, int y,Direction direction)   /// funkcija koja proverava da li se brod sudara
{                                                                 ///   sa vec postojecim brodom (da li se brod moze postaviti ili ne)
    if(x > 0)
    {
        if(tabla[y][x-1].object==1)
            return 1;
    }
    if(x < 9)
    {
        if(tabla[y][x+1].object==1)
            return 1;
    }
    if(y > 0)
    {
        if(tabla[y-1][x].object==1)
            return 1;
    }
    if(y < 9)
    {
        if(tabla[y+1][x].object==1)
            return 1;
    }
    if((y-direction.Vertical >= 0) && (x+direction.Horizontal <= 9))
        if(tabla[y-direction.Vertical][x+direction.Horizontal].brod_je_tu == DA)
            return check_error(tabla,x+direction.Horizontal,y-direction.Vertical,direction);
    return 0;
}
void postavi_brod(tabla** tabla, int naziv_broda,int IGRAC) /// funkcija koja postavlja brod
{
    Brod brod = {naziv_broda,0,0,{1,0}};              /// pocetna inicijalizacija broda
    if(IGRAC==KOMP)
    {
        if(rand()%2 == 1)
        {
            brod.direction.Horizontal = 1;
            brod.direction.Vertical = 0;
        }
        else
        {
            brod.direction.Horizontal = 0;
            brod.direction.Vertical = 1;
        }
        brod.x = rand()%(10-(naziv_broda-1)*brod.direction.Horizontal);
        brod.y = rand()%(10-(naziv_broda-1)*brod.direction.Vertical)+((naziv_broda-1)*brod.direction.Vertical);
    }
    for(int i = 0; i < brod.len; i += 1)
    {
        update_collision(tabla,brod.x+i*brod.direction.Horizontal,brod.y-i*brod.direction.Vertical,SUMMON);
    } /// postavljanje celog pocetnog broda
    int q;
    int input;

    while(kbhit())
        getch();

    do
    {

        do
        {
            if(IGRAC==COVEK)
                getInput(&input);  /// provera unosa
            else
            {
                int broj = rand()%6;  /// randomizacija unosa za kompjuter
                switch(broj)
                {
                case 0:
                    input = RIGHT;
                    break;
                case 1:
                    input = LEFT;
                    break;
                case 2:
                    input = UP;
                    break;
                case 3:
                    input = DOWN;
                    break;
                case 4:
                    input = ENTER;
                    break;
                case 5:
                    input = SPACE;
                    break;
                }
            }
            gotoxy(26,14);
            printf("                             ");
            gotoxy(79,24);
            q = 0;            ///  promenljiva q se koristi kada se iz broda unistava samo jedano polje
            switch(input)
            {
            case UP:
            {
                if(brod.y > (brod.direction.Vertical*(brod.len-1)))
                {
                    for(int i = 0,j = 0; i < brod.len || j < 1; i += 1)
                    {
                        if(i == brod.len)
                        {
                            i = 0;
                            j = 1;
                        }
                        if(j==0)
                        {
                            if(brod.direction.Vertical==horizontal || q == 0)
                            {
                                update_collision(tabla,brod.x+brod.direction.Horizontal*i,brod.y,DESTROY);
                                q = 1;
                            }
                        }
                        else
                            update_collision(tabla,brod.x+brod.direction.Horizontal*i,brod.y-brod.direction.Vertical*i-1,SUMMON);

                    }
                    brod.y -= 1;
                }
            }
            break;
            case DOWN:
            {
                if(brod.y < 9)
                {
                    for(int i = 0,j = 0; i < brod.len || j < 1; i += 1)
                    {
                        if(i == brod.len)
                        {
                            i = 0;
                            j = 1;
                        }
                        if(j == 0)
                        {
                            if(brod.direction.Vertical==horizontal || q == 0)
                            {
                                update_collision(tabla,brod.x+brod.direction.Horizontal*i,brod.y-((brod.len-1)*brod.direction.Vertical),DESTROY);
                                q = 1;
                            }
                        }
                        else
                            update_collision(tabla,brod.x+brod.direction.Horizontal*i,brod.y-brod.direction.Vertical*i+1,SUMMON);
                    }
                    brod.y += 1;
                }
            }
            break;
            case LEFT:
            {
                if(brod.x > 0)
                {
                    for(int i = 0,j = 0; i < brod.len || j < 1; i += 1)
                    {
                        if(i == brod.len)
                        {
                            i = 0;
                            j = 1;
                        }
                        if(j == 0)
                        {
                            if(brod.direction.Vertical==vertical || q == 0)
                            {
                                update_collision(tabla,brod.x+((brod.len-1)*brod.direction.Horizontal),brod.y-i*brod.direction.Vertical,DESTROY);
                                q = 1;
                            }
                        }
                        else
                            update_collision(tabla,brod.x+((brod.len-1-i)*brod.direction.Horizontal)-1,brod.y-brod.direction.Vertical*i,SUMMON);
                    }
                    brod.x -= 1;
                }
            }
            break;
            case RIGHT:
            {
                if(brod.x < ( 9 - brod.direction.Horizontal*(brod.len-1)))
                {
                    for(int i = 0, j = 0; i < brod.len || j < 1; i += 1)
                    {
                        if(i == brod.len)
                        {
                            i = 0;
                            j = 1;
                        }
                        if(j == 0)
                        {
                            if(brod.direction.Vertical==vertical || q == 0)
                            {
                                update_collision(tabla,brod.x,brod.y-(brod.direction.Vertical*i),DESTROY);
                                q = 1;
                            }
                        }
                        else
                            update_collision(tabla,brod.x+i*brod.direction.Horizontal+1,brod.y-brod.direction.Vertical*i,SUMMON);
                    }
                    brod.x += 1;
                }
            }
            break;
            case SPACE:
            {
                if(brod.y > (brod.direction.Horizontal*(brod.len-2)) && brod.x < (9-(brod.direction.Vertical*(brod.len-2))))
                {
                    for(int i = 0,j = 0; i < brod.len || j < 1; i += 1)
                    {
                        if(i == brod.len)
                        {
                            i = 0;
                            j = 1;
                        }
                        if(j == 0)
                        {
                            update_collision(tabla,brod.x+brod.direction.Horizontal*i,brod.y-brod.direction.Vertical*i,DESTROY);
                        }
                        else
                            update_collision(tabla,brod.x+i*brod.direction.Vertical,brod.y-i*brod.direction.Horizontal,SUMMON);

                    }
                    brod.direction.Horizontal = (brod.direction.Vertical == DA) ? DA : NE;
                    brod.direction.Vertical = (brod.direction.Vertical == DA) ? NE : DA;
                }
            }
            break;
            }
        }
        while(input != ENTER);
        if(check_error(tabla,brod.x,brod.y,brod.direction)==0)
            break;
        else
        {
            setcolor(YELLOW);
            gotoxy(26,14);
            printf("NIJE MOGUCE POSTAVITI BROD!");
        }
    }
    while(1);
    for(int i = 0; i < brod.len; i += 1)
    {
        colprintxy(LIGHT_GRAY,"x ",brod.x+brod.direction.Horizontal*i,brod.y-brod.direction.Vertical*i);
        tabla[brod.y-brod.direction.Vertical*i][brod.x+brod.direction.Horizontal*i].object = 1;
        tabla[brod.y-brod.direction.Vertical*i][brod.x+brod.direction.Horizontal*i].direction.Horizontal = brod.direction.Horizontal;
        tabla[brod.y-brod.direction.Vertical*i][brod.x+brod.direction.Horizontal*i].direction.Vertical = brod.direction.Vertical;
        tabla[brod.y-brod.direction.Vertical*i][brod.x+brod.direction.Horizontal*i].brod_len = brod.len;
    }
}
void popuni_tabelu(tabla** tabla,int IGRAC)
{
    postavi_brod(tabla,NOSAC_AVIONA,IGRAC); /// 1x4 brod

    postavi_brod(tabla,KRSTARICA,IGRAC);
    postavi_brod(tabla,KRSTARICA,IGRAC); /// 2 * 1x3 brod

    postavi_brod(tabla,RAZARAC,IGRAC);
    postavi_brod(tabla,RAZARAC,IGRAC); /// 3 * 1x2 brod
    postavi_brod(tabla,RAZARAC,IGRAC);

    postavi_brod(tabla,PODMORNICA,IGRAC);
    postavi_brod(tabla,PODMORNICA,IGRAC);
    postavi_brod(tabla,PODMORNICA,IGRAC); /// 4 * 1x1 brod
    postavi_brod(tabla,PODMORNICA,IGRAC);
}
void ucitaj_sliku(const char* name) /// ucitava sliku welcome screen-a
{
    system("cls");
    FILE* open = fopen(name,"r");
    if(open!=NULL)
    {
        for(int j=0; j<25; j+=1)
            for(int i=0; i<80; i+=1)
            {
                fscanf(open,"%*[\n]");
                fscanf(open,"%c",&at[j][i].symbol);
            }
        for(int j=0; j<25; j+=1)
            for(int i=0; i<80; i+=1)
            {
                if(j > 0 && i == 0)
                    printf("\n");
                fscanf(open,"%d",&at[j][i].boja);
                setcolor(at[j][i].boja);
                if(j!=24 || i != 79)
                    printf("%c",at[j][i].symbol);
            }
    }
}
int welcome_screen() /// funkcija za dobrodoslicu
{
    ucitaj_sliku("welcome_screen.txt");
    int izbor = QUIT,input = RIGHT;
    do
    {
        if(input == LEFT || input == RIGHT)
        {
            izbor = (izbor == PLAY) ? QUIT : PLAY;
            setcolor((izbor == PLAY ? GREEN : WHITE));
            gotoxy(18,15)
            printf("----------");
            gotoxy(18,16)
            printf("|  PLAY  |");
            gotoxy(18,17)
            printf("----------");
            setcolor((izbor == QUIT ? GREEN : WHITE));
            gotoxy(50,15)
            printf("----------");
            gotoxy(50,16)
            printf("|  QUIT  |");
            gotoxy(50,17)
            printf("----------");
        }
        gotoxy(79,24);
        input = getch();
        if(input == 224)
            input = getch();
    }
    while(input!=ENTER);
    return izbor;
}
int update_bomba(tabla** tabla,int x_pomeraj,int y_pomeraj,int x_bomba, int y_bomba) /// update promene polozaja bombe
{
    if(x_bomba+x_pomeraj > 9 || x_bomba+x_pomeraj < 0 || y_bomba + y_pomeraj > 9 || y_bomba + y_pomeraj < 0)
        return 0;
    if(tabla[y_bomba][x_bomba].pogodjeno == SKROZ)
        colprintxy(RED,"x",x_bomba,y_bomba);
    else if(tabla[y_bomba][x_bomba].pogodjeno == DA && tabla[y_bomba][x_bomba].brod_je_tu == DA)
        colprintxy(CYAN,"x",x_bomba,y_bomba);
    else if(tabla[y_bomba][x_bomba].pogodjeno == DA)
        colprintxy(LIGHT_GRAY,"x",x_bomba,y_bomba);
    else
        colprintxy(LIGHT_GRAY,".",x_bomba,y_bomba);
    colprintxy(YELLOW,"x",x_bomba+x_pomeraj,y_bomba+y_pomeraj);
    return 1;
}

int vec_bacena_bomba(tabla** tabla,int x,int y)
{
    if(tabla[y][x].pogodjeno == DA || tabla[y][x].pogodjeno == SKROZ)
        return 1;
    else
        return 0;
}

void unisti_ceo_brod(tabla** _tabla,tabla** pogadjanja,int x,int y)
{
    for(int i = 0; i < _tabla[y][x].brod_len; i+=1)
    {
        pogadjanja[y-(_tabla[y][x].direction.Vertical*i)][x+(_tabla[y][x].direction.Horizontal*i)].pogodjeno = SKROZ;
        colprintxy(RED,"x",x+(_tabla[y][x].direction.Horizontal*i),y-(_tabla[y][x].direction.Vertical*i));
    }
}

int proveri_ceo_brod(tabla** _tabla,tabla** pogadjanja,int x, int y)
{
    for(int i = 0; i < _tabla[y][x].brod_len-1; i+=1)
    {
        if((y+_tabla[y][x].direction.Vertical) <= 9 && (x-_tabla[y][x].direction.Horizontal) >= 0)
        {
            if(_tabla[y+_tabla[y][x].direction.Vertical][x-_tabla[y][x].direction.Horizontal].object == 1)
            {
                x -= _tabla[y][x].direction.Horizontal;
                y += _tabla[y][x].direction.Vertical;
            }
            else
                break;
        }
        else
            break;
    }
    for(int i = 0; i < _tabla[y][x].brod_len; i+=1)
    {
        if(pogadjanja[y-(_tabla[y][x].direction.Vertical*i)][x+(_tabla[y][x].direction.Horizontal*i)].pogodjeno == DA)
        {
        }
        else
            return 0;
    }
    unisti_ceo_brod(_tabla,pogadjanja,x,y);
    return 1;
}


int pogodjeno(tabla** sa_brodovima,tabla** pogadjanja,int x,int y,int IGRAC)
{
    if(sa_brodovima[y][x].object == 1)
    {
        pogadjanja[y][x].object = 1;
        pogadjanja[y][x].brod_je_tu = DA;
        pogadjanja[y][x].pogodjeno = DA;

        if(proveri_ceo_brod(sa_brodovima,pogadjanja,x,y))
        {
            colprintxy(RED,"x",x,y);
            if(IGRAC == KOMP)
                bot_bomb.x = -1;
        }
        else
            colprintxy(CYAN,"x",x,y);
    }
    else
    {
        pogadjanja[y][x].pogodjeno = DA;
        pogadjanja[y][x].brod_je_tu = NE;
        if(IGRAC == KOMP)
            bot_bomb.x = -1;
        return 0;
    }
    return 1;
}

int check_win(tabla** sa_brodovima,tabla** pogadjanja)
{
    for(int i = 0; i < 10; i+=1)
    {
        for(int j = 0; j < 10; j += 1)
        {
            if(sa_brodovima[i][j].object == 1)
                if(pogadjanja[i][j].pogodjeno!=SKROZ)
                    return 0;
        }
    }
    return 1;
}

int proveri_pored(tabla** _tabla,int x,int y)
{
    if(x > 0)
        if(_tabla[y][x-1].pogodjeno == SKROZ)
            return 0;
    if(x < 9)
        if(_tabla[y][x+1].pogodjeno == SKROZ)
            return 0;
    if(y > 0)
        if(_tabla[y-1][x].pogodjeno == SKROZ)
            return 0;
    if(y < 9)
        if(_tabla[y+1][x].pogodjeno == SKROZ)
            return 0;
    return 1;
}

int pronadji_polje_za_bota(tabla** _tabla)
{
    int plus_il_minus = rand()%2;
    if(plus_il_minus==0)
        plus_il_minus = -1;
    for(int i = rand()%10;i >= 0 && i <= 9;i+=plus_il_minus)
    {
        for(int j = rand()%10; j >= 0 && j <= 9; j+=plus_il_minus)
        {
            if(_tabla[i][j].pogodjeno==NE && (proveri_pored(_tabla,j,i)))
            {
                bot_bomb.x = j;
                bot_bomb.y = i;
                return 0;
            }
        }
    }
    return 1;
}
int pogadjaj(tabla** sa_brodovima,tabla** pogadjanja,int IGRAC)
{
    static int komp_x_original,komp_y_original,next; ///
    int q;
    draw(pogadjanja);
    struct
    {
        int x, y;
    } bomba;

    if(IGRAC==KOMP)
    {
        bomba.x = rand()%10;
        bomba.y = rand()%10;
    }
    else
    {
        bomba.x = 0;
        bomba.y = 0;
    }
    int input;
    colprintxy(YELLOW,"x",bomba.x,bomba.y);
    while(kbhit())
        getch();
    do
    {
        if(check_win(sa_brodovima,pogadjanja)==1)
            return 1;
        do
        {
            if(IGRAC==KOMP)
            {
                if(bot_bomb.x == -1)
                {
                    while(pronadji_polje_za_bota(pogadjanja));
                    komp_x_original = bot_bomb.x;
                    komp_y_original = bot_bomb.y;
                    next = 1;
                    q = 0;
                }
                else
                {
                    q = 1;
                    goto skip;
                    reset:
                        bot_bomb.x = komp_x_original;
                        bot_bomb.y = komp_y_original;
                    skip:
                    switch(next)
                    {
                    case 1:
                        if(bot_bomb.x < 9 && pogadjanja[bot_bomb.y][bot_bomb.x+1].pogodjeno == NE && proveri_pored(pogadjanja,bot_bomb.x+1,bot_bomb.y))
                            bot_bomb.x+=1;
                        else{
                            next+=1;
                            goto reset;
                          }
                        break;
                    case 2:
                        if(bot_bomb.x > 0 && pogadjanja[bot_bomb.y][bot_bomb.x-1].pogodjeno == NE && proveri_pored(pogadjanja,bot_bomb.x-1,bot_bomb.y))
                            bot_bomb.x-=1;
                        else{
                            next+=1;
                            goto reset;
                        }
                        break;
                    case 3:
                        if(bot_bomb.y > 0 && pogadjanja[bot_bomb.y-1][bot_bomb.x].pogodjeno == NE && proveri_pored(pogadjanja,bot_bomb.x,bot_bomb.y-1))
                            bot_bomb.y-=1;
                        else{
                            next+=1;
                            goto reset;
                        }
                        break;
                    case 4:
                        if(bot_bomb.y < 9 && pogadjanja[bot_bomb.y+1][bot_bomb.x].pogodjeno == NE && proveri_pored(pogadjanja,bot_bomb.x,bot_bomb.y+1))
                            bot_bomb.y+=1;
                        else{
                            next+=1;
                            goto reset;
                        }
                    }
                }
            }
            do
            {
                if(IGRAC == COVEK)
                {
                    input = getch();
                    if(input == 224)
                        input = getch();
                }
                else
                {
                    Sleep(100);
                    if(bomba.x < bot_bomb.x)
                        input = RIGHT;
                    else if(bomba.x > bot_bomb.x)
                        input = LEFT;
                    else if(bomba.y < bot_bomb.y)
                        input = DOWN;
                    else if(bomba.y > bot_bomb.y)
                        input = UP;
                    else
                        input = ENTER;
                }
                gotoxy(26,14);
                printf("                              ");
                gotoxy(79,24);
                switch(input)
                {
                case RIGHT:
                    if(update_bomba(pogadjanja,1,0,bomba.x,bomba.y))
                        bomba.x+=1;
                    break;
                case LEFT:
                    if(update_bomba(pogadjanja,-1,0,bomba.x,bomba.y))
                        bomba.x-=1;
                    break;
                case UP:
                    if(update_bomba(pogadjanja,0,-1,bomba.x,bomba.y))
                        bomba.y-=1;
                    break;
                case DOWN:
                    if(update_bomba(pogadjanja,0,1,bomba.x,bomba.y))
                        bomba.y+=1;
                    break;
                }
            }
            while(input != ENTER);

            if(vec_bacena_bomba(pogadjanja,bomba.x,bomba.y)==0)
                break;
            else
            {
                setcolor(YELLOW);
                gotoxy(26,14);
                printf("TU JE VEC BACENA BOMBA!");
            }
        }
        while(1);
        if(IGRAC==KOMP)
            Sleep(150);

    }while(pogodjeno(sa_brodovima,pogadjanja,bomba.x,bomba.y,IGRAC));

    if(IGRAC==KOMP && q == 1)
    {
        next += 1;
        bot_bomb.x = komp_x_original;
        bot_bomb.y = komp_y_original;
    }
    colprintxy(LIGHT_GRAY,"x",bomba.x,bomba.y);
    return 0;
}

void pobedio(int igrac,int jel_komp)
{
    system("cls");
    setcolor(LIGHT_GRAY);
    gotoxy(34,12)
    if(jel_komp == KOMP && igrac == KOMP)
    {
        printf("POBEDIO JE KOMP!\n\n\n");
    }
    else if(jel_komp == KOMP)
    {
        printf("POBEDIO SI!\n\n\n");
    }
    else
    {
        printf("POBEDIO JE %d IGRAC\n\n\n",igrac);
    }

    Sleep(2000);
}
