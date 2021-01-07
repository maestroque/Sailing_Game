#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ENTER 13
#define WIDTH 100
#define HEIGHT 33
#define MAX_PLAYERS 5
#define MAX_YSTEPS 3
#define MAX_XSTEPS 6

HANDLE hConsole;

struct player
{
    char name[20];
    char symbol;
    int X;
    int Y;
    char input[100];

    bool skipRound;
    bool checkpoint1;
    bool checkpoint2;
    bool finish;
    bool hasCrushed;
};

void move(struct player * pplayer);
void gotoXY(short column, short line);
void getBackgroundColor(int x, int y, bool red);
void drawMap();
void deleteMessage(int line);
int windGeneration(int *slope, int *scale);
int windEffect(int slope, int scale, char input[100]);
void windPush(int slope, int scale, int effectFactor, struct player * pplayer);
void getOutOfLand(struct player * pplayer, int slope);
void HideConsoleCursor();
void moveConstraint(char key, bool *limitReached, int *countX, int *countY, bool* XlimitReached, bool* YlimitReached);
void windPoint(int slope);

char map[33][101] = {
    "::::::::::::oooooooooooooooooXXXXXXXXXXXXXXXXXXXXooooXXXXooooooooooooooooooo::::::::::::::::::::::::",
    ":::::::::::ooooooooooooooXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXooooooooooooooooooo::::::::::::::::",
    ":::::::::ooooooooooooooXXXXXX;;;;;;;;;;;;;;;XXXXXXXXXXXXXXXXXXXXXXXXXXXXXoooooooooooooooooooo:::::::",
    ":::::::ooooooooooooooXXXXXX;;               ;;;;XXXXXXXXXXXXXXXXXXXXXXXXXXXoooooooooooooooooooooo:::",
    ":::::ooooooooXXXXXXXXXXXXX;                     ;;;;;;;;;;;;;;;;;;;;;;;XXXXXXXXoooooooooooooooooooo:",
    "::oooooooXXXXXXXXXXXXXXXX;;                                            ;;;XXXXXXXXXooooooooooooooooo",
    "::ooooooXXXXXXXXXXX;;;;;;                                                 ;;;XXXXXXXoooooooooooooooo",
    ":oooooXXXXXXXXX;;;;    1                                                     ;;;;;XXXXXXXXXooooooooo",
    "oooooXXXXXXX;;;        1          ;;;;;;;;;;;;                                    ;;;;;XXXXXoooooooo",
    "oooooXXXXXX;           1   ;;;;;;;XXXXXXXXXXXX;;;                                      ;XXXXXXoooooo",
    "ooooXXXXXXX;           ;;;;XXXXXXXXXXXXXXXXXXXXXX;;                                     ;XXXXXXooooo",
    "ooooXXXXXX;            ;XXXXXXXXXoooooXXXXXXXXXXXXX;                                     ;XXXXXooooo",
    "ooooXXXXXX;           ;XXXXXXXXXoooooooooooXXXXXXXXX;;                                   ;XXXXXooooo",
    "oooXXXXXX;             ;XXXXooooooooooooooooooXXXXXXXX;                                  ;XXXXXooooo",
    "ooXXXXXX;              ;;XXXXXooooooooo:::::ooooXXXXXXX;;;;                              ;XXXXXXoooo",
    "oXXXXXXX;                ;XXXXXXXXooooo::::::oooooXXXXXXXXX;;;                           ;XXXXXXoooo",
    "XXXXXXXX;                 ;XXXXXXXXooo:::::::::oooooXXXXXXXXXX;;;                       ;XXXXXXooooo",
    "oXXXXXX;                  ;XXXXXXXXooo:::::::oooooooXXXXXXXXXXXX;fffffffffffffffffffffff;XXXXXXooooo",
    "XXXXXX;                    ;XXXXXXXooo::::::ooooooXXXXXXXXXXXXX;                       ;XXXXXXoooooo",
    "XXXX;;                      ;XXXXXXooo::::ooooXXXXXXXXXXXXXXX;;                        ;XXXXXooooooo",
    "XXX;                         ;XXXXXooo::ooooXXXXXXXXXXX;;;;;;                          ;XXXXXooooooo",
    "oXXX;                       ;XXXXXXoooooooXXXXXXXXXX;;;                                ;XXXXXooooooo",
    "oXXX;                        ;XXXXXXoooooXXXXXXXXXX;                                    ;XXXXXXooooo",
    "ooXXX;;;                    ;XXXXXXXXXXXXXXXXXXXXX;                                     ;XXXXXXooooo",
    "oooXXXXX;;;;                ;XXXXXXXXXXXXXXXXXXX;;                                      ;XXXXXXooooo",
    "ooooXXXXXXXX;;             ;;;XXXXXXXXXXXXX;;;;;                                        ;XXXXXXooooo",
    "::oooooXXXXXXX;;              ;;;;;;;;;;;;;                                            ;XXXXXXXoooo:",
    "::::ooooooXXXXXX;                    2                                               ;;XXXXXXooooo::",
    ":::::oooooooXXXX;                    2                                      ;;;;;;;;;XXXXXXXooooo:::",
    ":::::oooooooXXXXX;;;;                2                                  ;;;;XXXXXXXXXXXXXXXoooo:::::",
    "::::::oooooooXXXXXXXX;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;                ;;;XXXXXXXXXXXXXXooooooo:::::::",
    "::::::::::oooooooXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX;;;;;;;;;;;;;;;;XXXXXXXXXXXXXXooooooo::::::::::",
    ":::::::::::::ooooXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXooooooooooo::::::::::::"
};

int main()
{
    system("mode 650");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    struct player players[MAX_PLAYERS];
    struct player winners[MAX_PLAYERS];
    int player_number;
    int slope, scale, effectFactor;

    printf("How many players are there? ");
    scanf("%d", &player_number);

    for (int i = 0; i < player_number; i++)
    {
        system("cls");
        printf("Enter the name of player %d: ", i + 1);
        scanf("%s", players[i].name);
        fflush(stdin);
        printf("Enter the symbol for player %d: ", i + 1);
        players[i].symbol = getchar();
        players[i].X = 70 + 2*i;
        players[i].Y = 18;

        players[i].skipRound = false;
        players[i].checkpoint1 = false;
        players[i].checkpoint2 = false;
        players[i].finish = false;
        players[i].hasCrushed = false;
    }

    system("cls");
    drawMap();

    SetConsoleTextAttribute(hConsole, 16);

    for (int i = 0; i < player_number; i++)
    {
        gotoXY(players[i].X, players[i].Y);
        putch(players[i].symbol);
    }

    while (true)
    {
        HideConsoleCursor();
        SetConsoleTextAttribute(hConsole, 07);
        for (int i = 0; i < player_number; i++)
        {
            gotoXY(0, 34);

            if (players[i].skipRound)
            {
                printf("%s lost their turn because of shallow waters!\n", players[i].name);
                Sleep(2000);
            }

            else
            {
                printf("Its %s's move\n", players[i].name);

                int counter = 0;
                char key = ' ';
                int countX = 0;
                int countY = 0;
                bool limitReached = false;
                bool XlimitReached = false;
                bool YlimitReached = false;

                while (key != ENTER)
                {
                    moveConstraint(key, &limitReached, &countX, &countY, &XlimitReached, &YlimitReached);
                    key = getch();
                    players[i].input[counter] = key;
                    if(limitReached) 
                    {
                        key = ENTER;
                        players[i].input[counter] = key;
                    }
                    else if(YlimitReached && ((key == 'w') || (key == 's'))) counter--;
                    else if(XlimitReached && ((key == 'a') || (key == 'd'))) counter--;  
                    counter++;
                }
            }

            deleteMessage(34);
            deleteMessage(35);
            deleteMessage(36);
        }

        for (int i = 0; i < player_number; i++)
        {
            if (!players[i].skipRound)
            {
                move(&(players[i]));
            }
        }

        SetConsoleTextAttribute(hConsole, 07);
        gotoXY(0, 34);
        printf("The players wanted to go there. But Wild is the Wind...");
        Sleep(2000);
        deleteMessage(34);
        windGeneration(&slope, &scale);
        if(scale != 0)
        {   gotoXY(0, 34);
            printf("This round the wind had: Slope of %d and Scale of %d", slope, scale);
            windPoint(slope);
        }
        else 
        {
            gotoXY(0, 34);
            puts("The wind was calm this round....");
        }
        Sleep(3000);
        for(int i = 0; i <= 4; i++)
        {
            deleteMessage(35+i);
        }
        
        for(int j = 0; j < player_number; j++)
        {
            if (players[j].skipRound)
            {
                players[j].skipRound = false;
            }
            else
            {
                effectFactor = windEffect(slope, scale, players[j].input);
                windPush(slope, scale, effectFactor, &(players[j]));
                if (map[players[j].Y][players[j].X] == ';') players[j].skipRound = true;
            } 
        }

        for(int i = 0; i < player_number; i++)
        {
            for(int j = 0; j < player_number; j++)
            {
                if((i == j) || ((players[i].hasCrushed == true) && (players[j].hasCrushed == true))) continue;
                else
                {
                    if((players[i].Y == players[j].Y) && (players[i].X == players[j].X))
                    {
                        players[i].hasCrushed = true;
                        players[j].hasCrushed = true;
                        deleteMessage(34);
                        gotoXY(0, 34);
                        printf("Oh no! %s and %s have crashed into each other!\n", players[i].name, players[j].name);
                        getBackgroundColor(players[i].X, players[i].Y, false);
                        for(int k = 0; k < 2; k++)
                        {
                            gotoXY(players[i].X, players[i].Y);
                            putch('X');
                            Sleep(1000);
                            gotoXY(players[i].X, players[i].Y);
                            putch(' ');
                            Sleep(1000);
                        }
                    } 
                }
            }
        }

        deleteMessage(34);
        for(int i = 0; i < player_number; i++)
        {
            if(players[i].hasCrushed)
            {
                players[i].X = 70 + 2*i;
                players[i].Y = 18;
                getBackgroundColor(players[i].X, players[i].Y, false);
                gotoXY(players[i].X, players[i].Y);
                putch(players[i].symbol);
                players[i].hasCrushed = false;
                players[i].checkpoint1 = false;
                players[i].checkpoint2 = false;
                players[i].finish = false;
                players[i].skipRound = false;
            }
        }

        int number_of_winners = 0;
        for (int i = 0; i < player_number; i++)
        {
            if ((players[i].finish) && (players[i].Y < 18))
            {
                winners[number_of_winners] = players[i];
                number_of_winners++;
            }
        }

        if (number_of_winners == 1)
        {
            gotoXY(0, 34);
            SetConsoleTextAttribute(hConsole, 07);
            printf("%s has won! Congratulations!", winners[0].name);
            Sleep(5000);
            exit(0);
        }

        if (number_of_winners > 1)
        {
            gotoXY(0, 34);
            SetConsoleTextAttribute(hConsole, 07);
            printf("There has been a tie between: ", winners[0].name);
            for (int i = 0; i < number_of_winners - 2; i++)
            {
                printf("%s, ", winners[i].name);
            }
            printf("%s ", winners[number_of_winners - 2].name);
            printf("and %s", winners[number_of_winners - 1].name);
            Sleep(5000);
            exit(0);
        }
    }

    return 0;
}

void HideConsoleCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void getBackgroundColor(int x, int y, bool red)
{
    int extra;
    if (red) extra = 4;
    else extra = 0;
    switch (map[y][x])
    {
    case ' ':
        SetConsoleTextAttribute(hConsole, 16 + extra);
        break;
    case '1':
        SetConsoleTextAttribute(hConsole, 16 + extra);
        break;
    case '2':
        SetConsoleTextAttribute(hConsole, 16 + extra);
        break;
    case 'f':
        SetConsoleTextAttribute(hConsole, 16 + extra);
        break;
    case 'o':
        SetConsoleTextAttribute(hConsole, 160 + extra);
        break;
    case 'X':
        SetConsoleTextAttribute(hConsole, 96 + extra);
        break;
    case ':':
        SetConsoleTextAttribute(hConsole, 32 + extra);
        break;
    case ';':
        SetConsoleTextAttribute(hConsole, 48 + extra);
        break;
    
    default:
        SetConsoleTextAttribute(hConsole, 07);
        break;
    }
}

void drawMap()
{
    for(int i = 0; i < 33; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            getBackgroundColor(j, i, false);
            putch(' ');
        }
        SetConsoleTextAttribute(hConsole, 07);
        putch('\n');
    }
}

void deleteMessage(int line)
{
    SetConsoleTextAttribute(hConsole, 07);
    for (int i = 0; i < 150; i++)
    {
        gotoXY(i, line);
        putch(' ');
    }
}

void gotoXY(short column, short line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void move(struct player * pplayer)
{
    char * pinput = (*pplayer).input;

    gotoXY((*pplayer).X, (*pplayer).Y);
    getBackgroundColor((*pplayer).X, (*pplayer).Y, false);
    putch(' ');

    int counter = 0;

    while ((*pplayer).input[counter] != ENTER)
    {
        
        switch ((*pplayer).input[counter]) 
        {
        case 'w':
            ((*pplayer).Y)--;
            if ((map[(*pplayer).Y][(*pplayer).X] == ';') || (map[(*pplayer).Y][(*pplayer).X] == 'X'))
            {
                ((*pplayer).Y)++;
            }
            break;
        case 's':
            ((*pplayer).Y)++;
            if ((map[(*pplayer).Y][(*pplayer).X] == ';') || (map[(*pplayer).Y][(*pplayer).X] == 'X'))
            {
                ((*pplayer).Y)--;
            }
            break;
        case 'a':
            ((*pplayer).X)--;
            if ((map[(*pplayer).Y][(*pplayer).X] == ';') || (map[(*pplayer).Y][(*pplayer).X] == 'X'))
            {
                ((*pplayer).X)++;
            }
            break;
        case 'd':
            ((*pplayer).X)++;
            if ((map[(*pplayer).Y][(*pplayer).X] == ';') || (map[(*pplayer).Y][(*pplayer).X] == 'X'))
            {
                ((*pplayer).X)--;
            }
            break;
        default:
            break;
        }

        if ((map[(*pplayer).Y][(*pplayer).X] == '1'))
        {
            (*pplayer).checkpoint1 = true;
        }
        if ((map[(*pplayer).Y][(*pplayer).X] == '2') && ((*pplayer).checkpoint1))
        {
            (*pplayer).checkpoint2 = true;
        }
        if ((map[(*pplayer).Y][(*pplayer).X] == 'f') && ((*pplayer).checkpoint1) && ((*pplayer).checkpoint2))
        {
            (*pplayer).finish = true;
        }

        counter++;
    }
    (*pplayer).input[counter] = '\0';

    gotoXY((*pplayer).X, (*pplayer).Y);
    getBackgroundColor((*pplayer).X, (*pplayer).Y, true);
    putch((*pplayer).symbol);
}

int windGeneration(int *slope, int *scale)
{
    int A[3] = {-2, -1, 0};
    static int prevSlope = 1;
    static int prevScale = 0;
    srand(time(NULL));

    int variability1 = rand()%3;
    *slope = 1 + (abs(prevSlope + A[variability1]))%8;
    prevSlope = *slope;

    int variability2 = rand()%3;
    *scale = (abs(prevScale + A[variability2]))%3;
    prevScale = *scale;
}

int windEffect(int slope, int scale, char input[100])
{
    int steps = strlen(input);
    int effectFactor = 1 + steps/3;

    return effectFactor;
}

void windPush(int slope, int scale, int effectFactor, struct player * pplayer)
{
    
    gotoXY((*pplayer).X, (*pplayer).Y);
    getBackgroundColor((*pplayer).X, (*pplayer).Y, false);
    putch(' ');

    switch(slope)
    {
        case 1:
            ((*pplayer).Y) = ((*pplayer).Y) - effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 1);
            }
            break;
        case 2:
            ((*pplayer).Y) = ((*pplayer).Y) - effectFactor*scale; 
            ((*pplayer).X) = ((*pplayer).X) + effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 2);
            }
            break;
        case 3: 
            ((*pplayer).X) = ((*pplayer).X) + effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 3);
            }
            break;    
        case 4:
            ((*pplayer).Y) = ((*pplayer).Y) + effectFactor*scale; 
            ((*pplayer).X) = ((*pplayer).X) + effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 4);
            }
            break;
        case 5:
            ((*pplayer).Y) = ((*pplayer).Y) + effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 5);
            }
            break;
        case 6:
            ((*pplayer).Y) = ((*pplayer).Y) + effectFactor*scale; 
            ((*pplayer).X) = ((*pplayer).X) - effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 6);
            }
            break;
        case 7: 
            ((*pplayer).X) = ((*pplayer).X) - effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 7);
            }
            break;
        case 8:
            ((*pplayer).Y) = ((*pplayer).Y) - effectFactor*scale; 
            ((*pplayer).X) = ((*pplayer).X) - effectFactor*scale;
            if ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
            {
                getOutOfLand(pplayer, 8);
            }
            break;
    }
    gotoXY((*pplayer).X, (*pplayer).Y);
    getBackgroundColor((*pplayer).X, (*pplayer).Y, false);
    putch((*pplayer).symbol);
    SetConsoleTextAttribute(hConsole, 07);
}

void getOutOfLand(struct player * pplayer, int slope)
{
    while ((map[(*pplayer).Y][(*pplayer).X] == 'X') || (map[(*pplayer).Y][(*pplayer).X] == 'o'))
    {
        switch (slope)
        {
        case 1:
            (*pplayer).Y += 1;
            break;
        case 2:
            (*pplayer).Y += 1;
            (*pplayer).X -= 1;
            break;
        case 3:
            (*pplayer).X -= 1;
            break;
        case 4:
            (*pplayer).Y -= 1;
            (*pplayer).X -= 1;
            break;
        case 5:
            (*pplayer).Y -= 1;
            break;
        case 6:
            (*pplayer).Y -= 1;
            (*pplayer).X += 1;
            break;
        case 7:
            (*pplayer).X += 1;
            break;
        case 8:
            (*pplayer).Y += 1;
            (*pplayer).X += 1;
            break;
        default:
            break;
        }
    }
}

void moveConstraint(char key, bool* limitReached, int* countX, int* countY, bool* XlimitReached, bool* YlimitReached)
{
    switch(key)
    {
        case 'w':
            (*countY)++;
            if(*YlimitReached == true) (*countY)--;
            break;
        case 's':
            (*countY)++;
            if(*YlimitReached == true) (*countY)--;
            break;
        case 'a':
            (*countX)++;
            if(*XlimitReached == true) (*countX)--;
            break;
        case 'd':
            (*countX)++;
            if(*XlimitReached == true) (*countX)--;
            break;
        default:
            break;
    } 
    deleteMessage(35);
    SetConsoleTextAttribute(hConsole, 07);
    gotoXY(0, 35);
    printf("Horizontal Steps: %d | Vertical Steps: %d", MAX_XSTEPS-*countX, MAX_YSTEPS-*countY);  

    if((*countX == MAX_XSTEPS) && (*countY == MAX_YSTEPS))
    {
        deleteMessage(36);
        gotoXY(0, 36);
        SetConsoleTextAttribute(hConsole, 07);
        puts("You can't make more steps!\n");
        *limitReached = true;
        Sleep(2000);
        deleteMessage(36);
        
    }

    else if(*countX == MAX_XSTEPS)
    {
        deleteMessage(36);
        gotoXY(0, 36);
        SetConsoleTextAttribute(hConsole, 07);
        puts("You can't make more steps in the horizontal direction!\n");
        *XlimitReached = true;
    }
    else if(*countY == MAX_YSTEPS)
    {
        deleteMessage(36);
        gotoXY(0, 36);
        SetConsoleTextAttribute(hConsole, 07);
        puts("You can't make more steps in the vertical direction!\n");
        *YlimitReached = true;
    }
}

void windPoint(int slope)
{
    switch(slope)
    {
        case 1:
            gotoXY(0, 35);
            puts("    N\n    |\nW   |   E\n\n    S");
            break;
        case 2:
            gotoXY(0, 35);
            puts("    N\n     /\nW   /   E\n\n    S");
            break;
        case 3:
            gotoXY(0, 35);
            puts("    N\n\nW   --- E\n\n    S");
            break;
        case 4:
            gotoXY(0, 35);
            puts("    N\n\nW   \\   E\n     \\\n    S");
            break;
        case 5:
            gotoXY(0, 35);
            puts("    N\n\nW   |   E\n    |\n    S");
            break;
        case 6:
            gotoXY(0, 35);
            puts("    N\n\nW   /   E\n  /\n    S");
            break;
        case 7:
            gotoXY(0, 35);
            puts("    N\n\nW ---   E\n\n    S");
            break;
    
        case 8:
            gotoXY(0, 35);
            puts("    N\n   \\\nW   \\   E\n\n    S");
            break;
        
    }
}