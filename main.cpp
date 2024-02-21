#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
//#include <termios.h>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
//#include <termios.h>
#include <time.h>
#include <conio.h>
string gray = "\033[30m";
string red = "\033[1;31m";
string green = "\033[1;32m";
string yellow = "\033[1;33m";
string blue = "\033[1;34m";
string purple = "\033[1;35m";
string cyan = "\033[1;36m";
string white = "\033[1l;37m";
string reset = "\033[0m";

struct{
    int score;
    string username;
    int time;
} player[100];
int width, length;
int temp_shape[4][4];
int rotate_count = 0;
void how_to_play();
void menu();
void left(int** arr, int x, int y);
void right(int** arr, int x, int y);
void down(int** arr,int x,int y);
void print(int** arr);
char getch();
void shape(int** arr, int x, int y, int num);
int difficutly();
void rotate(int** arr, int x, int y);
void get_username();
void size_of_game(int& length, int& width);
void save_score(int diffucult, int player_count);
void score_read();
bool check_down(int** arr, int x, int y);
bool check_rotation(int** arr, int x, int y);
void rotate_counterclockwise(int num);
bool check_left(int** arr, int x, int y);
bool check_right(int** arr, int x, int y);
bool check_row(int** arr, int score);
void resume();
int random_shape(int difficulty_stage);
bool gameover(int** arr);
int gameover_menu(int score);
void show_info(string str, int points, int time, int difficulty_stage, int next_shape);
bool kbhit();
void sort_score(int difficult);
int leaderboard_difficulty();
void print1(int** arr)
{
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}
int main()
{
    int player_count = 0;
    menu();
    char inp = getch();
    int desiredFPS = 20;
    std::chrono::milliseconds frameTime(10000 / desiredFPS);
    while(inp!='4')
    {
        if(inp == '1')
        {
            player_count ++;
            system("clear");
            get_username();
            getline (cin, player[player_count].username);
            system("clear");
            int dificulty_stage = difficutly();
            system("clear");
            size_of_game(length, width);
            system("clear");
            length += 6;
            width += 2;

            int** arr = new int*[length];
            for(int i = 0; i < length; i++){
                arr[i] = new int[width];
            }
            for(int i = 0; i < length; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    arr[i][j] = 0 ;
                }
            }
            for(int j = 0; j < width; j++)
            {
                arr[0][j]= -1;
                arr[length-1][j] = -1;
            }
            for(int i = 0; i < length; i++)
            {
                arr[i][0]= -1;
                arr[i][width-1] = -1;
            }
            player[player_count].score = 0;
            int x = 1;
            int y = width/2-2;
            int next_shape = random_shape(dificulty_stage);
            int current_shape = random_shape(dificulty_stage); 
            shape(arr, x, y, current_shape);

            std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::steady_clock::duration>  begining_time = std::chrono::steady_clock::now();
            while(gameover(arr))
            {
                
                std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::steady_clock::duration>  frameStart = std::chrono::steady_clock::now();
                std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::steady_clock::duration>  continue_time = std::chrono::steady_clock::now();
                auto thread_time = std::chrono::duration_cast<std::chrono::seconds>(continue_time - begining_time);
                
                while(!kbhit())
                {
                    if(check_down(arr, x, y))
                    {
                        player[player_count].score += 5;
                        down(arr, x, y);
                        x++;
                    }
                    system("clear");
                    show_info(player[player_count].username, player[player_count].score, player[player_count].time, dificulty_stage, next_shape);
                    print(arr);
                    std::this_thread::sleep_for(frameTime);
                    continue_time = std::chrono::steady_clock::now();
                    thread_time = std::chrono::duration_cast<std::chrono::seconds>(continue_time - begining_time);
                    player[player_count].time = thread_time.count();
                    if(!check_down(arr, x, y))
                    {
                        for(int i = x; i < x+4; i++)
                        {
                            for(int j = y; j < y+4; j++)
                            {
                                if(arr[i][j] > 0)
                                arr[i][j] = current_shape*(-1);
                            }
                        }
                        player[player_count].score += 10;
                        check_row(arr, player[player_count].score);
                        x = 1;
                        y = width/2-2;
                        current_shape = next_shape;
                        next_shape = random_shape(dificulty_stage);
                        shape(arr, x, y, current_shape);
                        rotate_count = 0;

                    }
                    if(!gameover(arr))
                    {
                        save_score(dificulty_stage, player_count);
                        system("clear");
                        int inp3 = gameover_menu(player[player_count].score);
                        if(inp3 == 1)
                        {
                            inp = 1;
                            break;
                        }
                        if(inp3 == 2)
                        {
                            return 0;
                        }
                    }
                }
                    
                
                system("clear");
                player[player_count].time = thread_time.count();
                show_info(player[player_count].username, player[player_count].score, player[player_count].time, dificulty_stage, next_shape);
                print(arr);
                char inp1 = getch();
                if(inp1 == 'r')
                {
                    rotate_count ++;
                    rotate_counterclockwise(current_shape);
                    if(check_rotation(arr, x, y))
                    {
                        for(int i = 0; i < 4; i ++)
                        {
                            for(int j = 0; j < 4; j++)
                            {
                                if(arr[i+x][j+y] > 0)
                                {
                                    arr[i+x][j+y] = 0;
                                }
                                if(temp_shape[i][j]!=0)
                                {
                                    arr[i+x][j+y] = temp_shape[i][j];
                                }
                            }
                        }
                    }
                }
                if(inp1 == 27)
                {
                    system("clear");
                    char inp2;
                    resume();
                    inp2 = getch();
                    if(inp2=='1')
                    {
                        continue;
                        
                    }
                    if(inp2=='2')
                    {
                        system("clear");
                        menu();
                        inp = getch();
                        break;

                    }
                }
                if(inp1 == ' ')
                {
                    while(check_down(arr, x, y))
                    {
                        player[player_count].score += 5;
                        down(arr, x, y);
                        x++;
                        
                    }
                }
                if(inp1 == 'a' || inp1 == 'A')
                {
                    if(check_left(arr, x, y))
                    {
                        left(arr, x, y);
                        y--;
                    }
                }
                if(inp1 == 'd' || inp1 == 'D')
                {
                    if(check_right(arr, x, y))
                    {
                        right(arr, x, y);
                        y++;
                    }
                }
                if(!check_down(arr, x, y))
                    {
                        for(int i = x; i < x+4; i++)
                        {
                            for(int j = y; j < y+4; j++)
                            {
                                if(arr[i][j] > 0)
                                arr[i][j] = current_shape*(-1);
                            }
                        }
                        player[player_count].score += 10;
                        check_row(arr, player[player_count].score);
                        x = 1;
                        y = width/2-2;
                        current_shape = next_shape;
                        next_shape = random_shape(dificulty_stage);
                        shape(arr, x, y, current_shape);
                        rotate_count = 0;
                    }
                std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::steady_clock::duration>  frameEnd = std::chrono::steady_clock::now();
                auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
                
                // If the frame was rendered faster than the desired frame time, sleep to limit the FPS
                if (frameDuration < frameTime) {
                    std::this_thread::sleep_for(frameTime - frameDuration);
                    
            }
        }
        if(!gameover(arr))
            {
                system("clear");
                save_score(dificulty_stage, player_count);
                int inp3 = gameover_menu(player[player_count].score);
                if(inp3 == 1)
                {
                    system("clear");
                    menu();
                    inp = getch();
                }
                if(inp3 == 2)
                {
                    return 0;
                }
            }
        }
        if(inp == '2')
        {
            how_to_play();
            getch();
            system("clear");
            menu();
            inp = getch();
        }
        if(inp == '3')
        {

            int leaderboard_stage = leaderboard_difficulty();
            system("clear");
            sort_score(leaderboard_stage);
            break;
        }
    }
}

void print(int** arr)
{
    string gray = "\033[30m";
    string red = "\033[1;31m";
    string green = "\033[1;32m";
    string yellow = "\033[1;33m";
    string blue = "\033[1;34m";
    string purple = "\033[1;35m";
    string cyan = "\033[1;36m";
    string white = "\033[1;37m";
    string reset = "\033[0m";
    for(int i = 5; i < length-1; i++)
    {
        cout << "                           ";
        for(int j = 1; j < width-1; j++)
        {
            if(arr[i][j]==0)
            {
                cout << white;
            }
            if(arr[i][j]==1 || arr[i][j]==-1)
            {
                cout << green;
            }
            if(arr[i][j]==2 || arr[i][j]==-2)
            {
                cout << blue;
            }
            if(arr[i][j]==3 || arr[i][j]==-3)
            {
                cout << yellow;
            }
            if(arr[i][j]==4 || arr[i][j]==-4)
            {
                cout << purple;
            }
            if(arr[i][j]==5 || arr[i][j]==-5)
            {
                cout << red;
            }
            if(arr[i][j]==6 || arr[i][j]==-6)
            {
                cout << cyan;
            }
            if(arr[i][j]==7 || arr[i][j]==-7)
            {
                cout << gray;
            }

            cout << "\u250f\u2501\u2513";
            cout << reset;
        }
        cout << "\n";
        cout << "                           ";
        for(int j = 1; j < width-1; j++)
        {
            if(arr[i][j]==0)
            {
                cout << white;
            }
            if(arr[i][j]==1  || arr[i][j]==-1)
            {
                cout << green;
            }
            if(arr[i][j]==2 || arr[i][j]==-2)
            {
                cout << blue;
            }
            if(arr[i][j]==3 || arr[i][j]==-3)
            {
                cout << yellow;
            }
            if(arr[i][j]==4 || arr[i][j]==-4)
            {
                cout << purple;
            }
            if(arr[i][j]==5 || arr[i][j]==-5)
            {
                cout << red;
            }
            if(arr[i][j]==6 || arr[i][j]==-6)
            {
                cout << cyan;
            }
            if(arr[i][j]==7 || arr[i][j]==-7)
            {
                cout << gray;
            }
            cout << "\u2517\u2501\u251b";
            cout << reset;
            
        }
        cout << "\n";
    }
}
void left(int** arr, int x, int y){
    for (int i = x; i < x+4; i++){
        for (int j = y; j < y + 4; j++){
            if (arr[i][j] > 0)
                swap(arr[i][j], arr[i][j-1]);
        }
    }
}
void right(int** arr, int x, int y){
    for (int i = x; i < x+4; i++){
        for (int j = y+3; j >= y; j--){
            if (arr[i][j] > 0)
                swap(arr[i][j], arr[i][j+1]);
        }
    }
}
void down(int** a ,int x,int y){
    for (int i = x + 3; i >= x; i--){
            for (int j = y; j < y + 4; j++){
                if (i < length - 1 && a[i][j] > 0)
                    swap(a[i][j], a[i + 1][j]);
            }
        }
}

void menu()
{
    cout << "                  \u250f";
    for(int i = 0; i < 44; i++)
    {
        cout << "\u2501";
    }
    cout << "\u2513\n";
    cout << "                  \u2503   " << red << "\u2501\u2501\u2533\u2501\u2501" << green <<"  \u250f\u2501\u2501\u2501\u2501" << yellow << "  \u2501\u2501\u2533\u2501\u2501" << blue << "  \u250f\u2501\u2501\u2501\u2513" << purple <<"  \u2501\u2533\u2501 " << cyan << " \u250f\u2501\u2501\u2501\u2501  "<< reset << " \u2503\n";
    cout << "                  \u2503  " << red << "   \u2503 " << green << "   \u2523\u2501\u2501\u2501\u2501   " << yellow << " \u2503  " << blue << "  \u2523\u2501\u2501\u2501\u251b  " << purple << " \u2503  " << cyan <<" \u2517\u2501\u2501\u2501\u2513   " << reset << "\u2503\n";
    cout << "                  \u2503    " << red << " \u2503" << green << "    \u2517\u2501\u2501\u2501\u2501  " << yellow << "  \u2503  " << blue << "  \u2503 \u2572" << purple << "    \u2501\u253b\u2501" << cyan << "  \u2501\u2501\u2501\u2501\u251b  " << reset << " \u2503\n";
    cout << "                  \u2517";
    for(int i = 0; i < 44; i++)
    {
        cout << "\u2501";
    }
    cout << "\u251b\n\n";
    // for(int i = 0; i < 45; i++)
    // {
    //     cout << "  ";
    // }
    gray = "\033[30m";
    red = "\033[7;31m";
    green = "\033[7;32m";
    yellow = "\033[1;33m";
    blue = "\033[1;34m";
    purple = "\033[1;35m";
    cyan = "\033[7;36m";
    reset = "\033[0m";

    cout << "                          " << green << "\u250c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
    cout << reset << "                          " << green << "\u2502         PLAY         \u2502\n";
    cout << reset << "                          " << green << "\u2502       (press 1)      \u2502\n";
    cout << reset << "                          " << green << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
    cout << reset << "                          " << cyan << "\u250c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510 "<< yellow << " \u250c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
    cout << reset << "                          " << cyan << "\u2502  How to \u2502 " << yellow << " \u2502  Leader \u2502\n" ;
    cout << reset << "                          " << cyan << "\u2502 PLAY(2) \u2502 " << yellow << " \u2502 Board(3)\u2502\n" ;
    cout << reset << "                          " << cyan << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518 " << yellow << " \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n" ;
    cout << reset << "                               " << red << "\u250c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n";
    cout << reset << "                               " << red << "\u2502  EXIT(4)  \u2502\n";
    cout << reset << "                               " << red << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n";
    gray = "\033[30m";
    red = "\033[0;31m";
    green = "\033[0;32m";
    yellow = "\033[0;33m";
    blue = "\033[0;34m";
    purple = "\033[0;35m";
    cyan = "\033[0;36m";
    reset = "\033[0m";
}
void shape(int** arr, int x, int y, int num)
{
    switch (num)
    {
    case 1:
        arr[x+3][y] = 1;
        arr[x+3][y+1] = 1;
        arr[x+3][y+2] = 1;
        arr[x+3][y+3] = 1;
        break;

    case 2:
        arr[x+2][y] = 2, arr[x+3][y] = 2, arr[x+3][y+1] = 2, arr[x+3][y+2] = 2;
        break;

    case 3:
        arr[x+2][y+3] = 3, arr[x+3][y+3] = 3, arr[x+3][y+2] = 3, arr[x+3][y+1] = 3;
        break;

    case 4:
        arr[x+2][y+1] = 4, arr[x+3][y+2] = 4, arr[x+2][y+2] = 4, arr[x+3][y+1] = 4;
        break;

    case 5:
        arr[x+2][y+1] = 5, arr[x+2][y+2] = 5, arr[x+3][y] = 5, arr[x+3][y+1] = 5;
        break;

    case 6:
        arr[x+2][y+1] = 6, arr[x+3][y] = 6, arr[x+3][y+1] = 6, arr[x+3][y+2] = 6;
        break;

    case 7:
        arr[x+2][y] = 7, arr[x+2][y+1] = 7, arr[x+3][y+1] = 7, arr[x+3][y+2] = 7;
        break;
    }
}
void rotate(int** arr, int x, int y) {
    int temp = 0;
    int n = 4;
    for (int i = 0; i < n / 2; i++) {
        for (int j = i; j < n - i - 1; j++) {
            temp = arr[i + x][j + y];
            arr[i + x][j + y] = arr[n - 1 - j + x][i + y];
            arr[n - 1 - j + x][i + y] = arr[n - 1 - i + x][n - 1 - j + y];
            arr[n - 1 - i + x][n - 1 - j + y] = arr[j + x][n - 1 - i + y];
            arr[j + x][n - 1 - i + y] = temp;
        }
    }
}

void how_to_play()
{
    
    cout <<green << "\u256d";
    for (int i = 0; i < 65; i++){
        cout << "\u2500";
    }
    cout << "\u256e\n";
    cout << "\u2502" << green << "                           " << blue << "HOW TO PLAY                           " << green << "\u2502\n" ;
    cout << "\u2502";
    for (int i = 0; i < 65; i++){
        cout << "\u2500";
    }
    cout << "\u2502\n";
    cout << "\u2502" << cyan << "1. Objective: " << reset << "The goal of Tetris is to complete lines by fitting " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "different shaped blocks (tetrominoes) together, without leaving  " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "any gaps.                                                        " << green <<  "\u2502\n" ;
    cout << "\u2502";
    for (int i = 0; i < 65; i++){
        cout << "\u2504";
    }
    cout << "\u2502\n";
    cout << "\u2502" << cyan << "2. Controls:" << reset << "                                                     " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- Use the A / D keys to move the falling tetromino left or right." << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- Press esc to puase the game anywhere in your gaming loop.      " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- Press the Space bar to make the tetromino fall faster.         " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- Press the 'R' key to rotate the tetromino.                " << green <<  "\u2502\n" ;
    cout << "\u2502";
    for (int i = 0; i < 65; i++){
        cout << "\u2504";
    }
    cout << "\u2502\n";
    cout << "\u2502" << cyan << "3. Gameplay:" << reset << "                                                     " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- Tetrominoes will fall from the top of the screen.Your job is to" << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "position and rotate them to create complete lines at the bottom. " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- When a line is full,it will disappear,and you will earn points." << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- As the game progresses, the tetrominoes will fall faster,      " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << " making it more challenging to position them.                    " << green <<  "\u2502\n" ;
    cout << "\u2502";
    for (int i = 0; i < 65; i++){
        cout << "\u2504";
    }
    cout << "\u2502\n";
    cout << "\u2502" << cyan << "4. Game over:" << reset << "                                                    " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- If the stack of tetrominoes reaches the top of the screen,     " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "the game ends.                                                   " << green <<  "\u2502\n" ;
    cout << "\u2502";
    for (int i = 0; i < 65; i++){
        cout << "\u2504";
    }
    cout << "\u2502\n";
    cout << "\u2502" << cyan << "5. Have Fun!:" << reset << "                                                    " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- Enjoy the puzzle action of Tetris and aim for high scores!     " << green <<  "\u2502\n" ;
    cout << "\u2502" << reset << "- Click any keys to back to menu ;)                              " << green <<  "\u2502\n" ;
    cout << "\u2570";
    for (int i = 0; i < 65; i++){
        cout << "\u2500";
    }
    cout << "\u256f\n" << reset;
}
int difficutly()
{
    cout << purple <<"\u256d";
    for(int i = 0; i < 30; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256e\n";
    cout << "\u2502";
    cout << cyan << "Select game difficulty:       " << purple;
    cout << "\u2502\n";
    cout << "\u2502";
    for(int i = 0; i < 30; i++)
    {
        cout << "\u2504";
    }
    cout << "\u2502\n";
    cout << "\u2502";
    cout << green << "1. Easy                       " << purple;
    cout << "\u2502\n";
    cout << "\u2502";
    cout << yellow << "2. Medium                     " << purple;
    cout << "\u2502\n";
    cout << "\u2502";
    cout << red << "3. Hard                       " << purple;
    cout << "\u2502\n";
    cout << "\u2570";
    for(int i = 0; i < 30; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256f\n";
    char inp;
    inp = getch();
    if(inp=='1')
    {
        return 1;
    }
    if(inp == '2')
    {
        return 2;
    }
    if(inp == '3')
    {
        return 3;
    }
}
int leaderboard_difficulty()
{
    cout << purple <<"\u256d";
    for(int i = 0; i < 40; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256e\n";
    cout << "\u2502";
    cout << cyan << "Select Leaderboard difficulty players   " << purple;
    cout << "\u2502\n";
    cout << "\u2502";
    for(int i = 0; i < 40; i++)
    {
        cout << "\u2504";
    }
    cout << "\u2502\n";
    cout << "\u2502";
    cout << green << "1. Easy                                 " << purple;
    cout << "\u2502\n";
    cout << "\u2502";
    cout << yellow << "2. Medium                               " << purple;
    cout << "\u2502\n";
    cout << "\u2502";
    cout << red << "3. Hard                                 " << purple;
    cout << "\u2502\n";
    cout << "\u2570";
    for(int i = 0; i < 40; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256f\n";
    char inp;
    inp = getch();
    if(inp=='1')
    {
        return 1;
    }
    if(inp == '2')
    {
        return 2;
    }
    if(inp == '3')
    {
        return 3;
    }
}
void get_username()
{
    cout << blue << "\u256d";
    for(int i = 0; i < 20; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256e\n";
    cout << "\u2502" << green << "Enter your username:" << blue << "\u2502\n";
    cout << "\u2570";
    for(int i = 0; i < 20; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256f\n" << reset;
}
void size_of_game(int& length, int& width)
{
    cout << blue << "\u256d";
    for(int i = 0; i < 50; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256e\n";
    cout << "\u2502" << green << "Enter your desired length and width of gameboard: " << blue << "\u2502\n";
    cout << "\u2502";
    for(int i = 0; i < 50; i++)
    {
        cout << "\u2504";
    }
    cout << "\u2502\n";
    cout << "\u2502";
    cout << cyan << "Width: " << reset;
    cin >> width;
    cout  << cyan << "\u2502";
    cout << "length: " << reset;
    cin >> length;
    cout << blue << "";
    cout << "\u2570";
    for(int i = 0; i < 50; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256f\n";
}
void save_score(int diffucult, int player_count)
{
    string filename;
    if(diffucult == 1)
    {
        filename = "scores_Easy.txt";
    }
    if(diffucult == 2)
    {
        filename = "scores_Med.txt";
    }
    if(diffucult == 3)
    {
        filename = "scores_Hard.txt";
    }
    ofstream file(filename, ios_base::app);
    if (file.is_open()) {//check if the file is correctly oppened or not
        file << player[player_count].username << " " << player[player_count].score << " " << player[player_count].time << " " << "\n"; //save datas in file
        file.close();//close the file
    } 
    else {
        cerr << "Error opening the file" << endl;
    }
}
// void score_read(int diffucult)
// {
//     string filename;
//     if(diffucult == 1)
//     {
//         filename = "scores_Easy.txt";
//     }
//     if(diffucult == 2)
//     {
//         filename = "scores_Med.txt";
//     }
//     if(diffucult == 3)
//     {
//         filename = "scores_Hard.txt";
//     }
//     ifstream file(filename);

//     if (file.is_open()) {//check if the file is correctly oppened or not
//         string line;
//         cout << "The top players are: \n";
//         while (getline(file, line)) {  // Read the file line by line
//                 cout << line << endl;  // Output each line to the console
//         }
//         file.close();  // Close the file when done
//     } 
//     else {
//             cerr << "Failed to open the file: " << filename <<  endl;
//     }
// }

void sort_score(int diffucult)
{   int cnt;
    struct{
        int score;
        string username;
        int time;
    }temp[100];
    string filename;
    if(diffucult == 1)
    {
        filename = "scores_Easy.txt";
    }
    if(diffucult == 2)
    {
        filename = "scores_Med.txt";
    }
    if(diffucult == 3)
    {
        filename = "scores_Hard.txt";
    }
    ifstream file(filename);

    if (file.is_open()) {//check if the file is correctly oppened or not
        string str;
        cnt = 0;
        while (getline(file, str)) {  // Read the file line by line
            int i = 0; 
            istringstream ss(str);
            string word;
            while (ss >> word) 
            {
                if(i==0)
                {
                    temp[cnt].username = word;
                }
                if(i == 1)
                {
                    temp[cnt].score = stoi(word);
                }
                if(i==2)
                {
                    temp[cnt].time = stoi(word);
                }
                i++;
            }
            cnt++;
        }

        for (int i = 0; i < cnt; i++) {
            for (int j = 0; j < cnt - i; j++) {
                if (temp[j+1].score > temp[j].score) {
                    swap(temp[j].score, temp[j + 1].score);
                    swap(temp[j].time, temp[j + 1].time);
                    swap(temp[j].username, temp[j + 1].username);
                }
            }
        }
        file.close();  // Close the file when done
        // for(int i = 0; i < cnt; i++)
        // {
        //     cout << i+1 << ". " << temp[i].username << " scored " << temp[i].score << "at" << temp[i].time << "seconds\n";
        // }
    } 
    cout << cyan << "               \u2554";
    for(int i = 0; i < 35; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557\n";
    cout << "               \u2551            " << green << "LEADERBOARD            " << cyan << "\u2551\n";
    cout << "               \u2560";
    for(int i = 0; i < 14; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2566"; 
    for(int i = 0; i < 10; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2566"; 
    for(int i = 0; i < 9; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2563\n";
    cout << "               " << "\u2551" << blue << "     Name     " << cyan << "\u2551" << blue << "   Score  "  << cyan << "\u2551" << blue <<"  Time   "  << cyan << "\u2551\n";
    int num = 1;
    for(int k = 0; k < cnt; k++)
    {
        cout << "               " << "\u255f";
        for(int i = 0; i < 14; i++)
        {
            cout << "\u2500";
        }
        cout << "\u256b"; 
        for(int i = 0; i < 10; i++)
        {
            cout << "\u2500";
        }
        cout << "\u256b"; 
        for(int i = 0; i < 9; i++)
        {
            cout << "\u2500";
        }
        cout << "\u2562\n";
        
        cout << "               \u2551";
        for(int i = 0; i < (14-temp[num].username.length()) / 2; i++)
        {
            cout << " ";
        }
        cout << temp[num].username;
        for(int i = 0; i < (14-temp[num].username.length()) / 2; i++)
        {
            cout << " ";
        }
        if(temp[num].username.length()%2==1)
        {
            cout << " ";
        }
        cout << "\u2551";
        for(int i = 0; i < (10-log(temp[num].score)) / 2+1; i++)
        {
            cout << " ";
        }
        cout << temp[num].score;
        for(int i = 0; i < (10-log(temp[num].score)) / 2+1; i++)
        {
            cout << " ";
        }
        
        if(temp[num].score > 99 && temp[num].score < 1000)
        {
            cout << " ";
        }
        cout << "\u2551";
        for(int i = 0; i < (10-log(temp[num].time)) / 2-1; i++)
        {
            cout << " ";
        }
        cout << temp[num].time;
        for(int i = 0; i < (10-log(temp[num].time)) / 2-1; i++)
        {
            cout << " ";
        }
        
        if(temp[num].score > 9 && temp[num].time < 100)
        {
            cout << " ";
        }
        cout << "\u2551\n";
        num ++;
    }
}
bool check_down(int** arr, int x, int y)
{
    for (int i = x; i < x + 4; i++){
        for (int j = y; j < y + 4; j++){
            if (arr[i][j] > 0 && arr[i + 1][j] < 0)
                return false;
        }
    }
    return true;
}
bool check_rotation(int** arr, int x, int y){
    for (int i = x; i < x + 4; i++){
        for (int j = y; j < y + 4; j++){
            if (temp_shape[i - x][j - y] > 0 && arr[i][j] < 0)
                return false;
        }
    }
    return true;
}
void rotate_counterclockwise(int num){
    
    int **shape_check = new int*[4];
    for(int i = 0; i < 4; i++)
    {
        shape_check[i] = new int[4];
    }
    shape(shape_check, 0, 0, num);
    int cnt = 4;
    for(int k = 0; k < rotate_count; k++){
        for (int i = 0; i < cnt; i++){
            for (int j = 0; j < cnt; j++){
                temp_shape[cnt -1 - j][i] = shape_check[i][j];
            }
        }
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                shape_check[i][j] = temp_shape[i][j];
            }
        }
    }
    delete[] shape_check;
}
bool check_left(int** arr, int x, int y){
    for (int j = y + 3; j >= y; j--){
        for (int i = x; i < x + 4; i++){
            if (arr[i][j] > 0 && arr[i][j - 1] < 0)
                return false;
        }
    }
    return true;
}
bool check_right(int** arr, int x, int y){
    for (int j = y; j < y + 4; j++){
        for (int i = x; i < x + 4; i++){
            if (arr[i][j] > 0 && arr[i][j + 1] < 0)
                return false;
        }
    }
    return true;
}
bool check_row(int** arr, int score)
{ 
    for(int i = length - 2; i > 4; i--)
    {
        bool flag = true;
        for(int j = 1; j < width - 1; j++)
        {
            if(arr[i][j]==0)
            {
                flag = false;
                break;
            }
        }

        if(flag)
        {
            score += 25;
            for(int j = 1; j < width-1; j++)
            {
                arr[i][j]=0;
            }
            for (int k = i; k > 0; k--){
                for (int j = 1; j < width-1; j++){
                        swap(arr[k][j], arr[k + 1][j]);
                }
            }
        }
        for(int j = 0; j < width; j++)
        {
            arr[length-1][j]=-1;
        }
    }
}
void resume()
{
    cout << cyan << "\u256d";
    for(int i = 0; i < 35; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256e\n";
    cout << "\u2502" << blue << "     The game is paused.           " << cyan << "\u2502\n"; 
    cout << "\u2502" << blue << " Would you like to countinue?      " << cyan << "\u2502\n"; 
    cout << "\u2502";
     for(int i = 0; i < 35; i++)
    {
        cout << "\u2504";
    }
    cout << "\u2502\n";

    cout << "\u2502" << green << " 1.Yes (countinue to previous game)" << cyan << "\u2502\n";
    cout << "\u2502" << red << " 2.No (Close the game)             " << cyan << "\u2502\n";
    cout << "\u2570";
    for(int i = 0; i < 35; i++)
    {
        cout << "\u2500";
    }
    cout << "\u256f\n" << reset;

}

int random_shape(int difficulty_stage)
{
    int ls[15];
    if(difficulty_stage==1)
    {
        int temp[] = {1, 1, 1, 1, 4, 4, 4, 6, 6, 7, 2, 3, 5, 2, 3};
        for(int i = 0; i < 15; i++)
        {
            ls[i] = temp[i];
        }
    }
    if(difficulty_stage==2)
    {
        int temp[] = {1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
        for(int i = 0; i < 15; i++)
        {
            ls[i] = temp[i];
        }
    }
    if(difficulty_stage==3)
    {
        int temp[] = {1, 2, 2, 3, 3, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7};
        for(int i = 0; i < 15; i++)
        {
            ls[i] = temp[i];
        }
    }
    int temp = rand()%15;
    return ls[temp];
}

bool gameover(int** arr)
{
    for(int j = 1; j < width-1; j++)
    {
        if(arr[5][j]<0)
        {
            return false;
        }
    }
    return true;
}
int gameover_menu(int score)
{
    cout << cyan << "                   \u2554";
    for(int i = 0; i < 35; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557\n";
    cout << "                   \u2551" << purple << "   OOPS! SEEMS LIKE YOU LOST:(     " << cyan << "\u2551\n";
    cout << "                   \u2551" << purple << "      But you scored " << score  << "      ";
    for(int i = 0; i < 10 - log(score)+1; i++)
    {
        cout << " ";
    }
    cout << cyan << "\u2551\n";
    cout << "                   \u255f";
    for(int i = 0; i < 35; i++)
    {
        cout << "\u2500";
    }
    cout << "\u2562\n";
    cout << "                   \u2551" << blue  << " What would you like to do now     "  << cyan << "\u2551\n";
    cout << "                   \u2551" << green << "  1. Back to menu                  "  << cyan << "\u2551\n";
    cout << "                   \u2551" << red <<  "  2. Exit                          " << cyan << "\u2551\n";
    cout << "                   \u255a";
    for(int i = 0; i < 35; i++)
    {
        cout << "\u2550";
    }
    cout << "\u255d\n" << reset;
    char inp;
    inp = getch();
    if(inp == '1')
    {
        return 1;
    }
    if(inp == '2')
    {
        return 2;
    }
}
void show_info(string str, int points, int time, int difficulty_stage, int next_shape)
{
    int** arr = new int*[4];
    for(int i = 0 ;i < 4; i++)
    {
        arr[i] = new int[4];
    }
    cout << cyan << "\u2554";
    for(int i = 0; i < 25; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557\n";
    cout << "\u2551" << green << " Player name: " << str;
    for(int i = 0; i < 11 - str.length(); i++)
    {
        cout << " ";
    }
    cout << cyan << "\u2551\n";
    cout << "\u2551" << green << " Points: " << points;
    for(int i = 0; i < 12; i++)
    {
        cout << " ";
    }
    if(points<10)
    {
        cout << "   ";
    }
    else if ((points)<100)
    {
        cout << "  ";
    }
    else if(points<1000)
    {
        cout << " ";
    }
    cout << cyan << "\u2551\n";
    cout << "\u2551" << green << " Time: " << time;
    if(time<10)
    {
        cout << "   ";
    }
    else if ((time)<100)
    {
        cout << "  ";
    }
    else if(time<1000)
    {
        cout << " ";
    }
    for(int i = 0; i < 14; i++)
    {
        cout << " ";
    }
    cout << cyan << "\u2551\n";
    cout << "\u2551" << green << " Game level: ";
    if(difficulty_stage==1)
    {
        cout << "Easy";
    }
    if(difficulty_stage==2)
    {
        cout << "Medi";
    }
    if(difficulty_stage==3)
    {
        cout << "Hard";
    }
    for(int i = 0; i < 8; i++)
    {
        cout << " ";
    }
    cout << cyan << "\u2551\n";
    cout << "\u255a";
    for(int i = 0; i < 25; i++)
    {
        cout << "\u2550";
    }
    cout << "\u255d\n";
    cout << cyan << "\u2554";
    for(int i = 0; i < 25; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557\n";
    cout << "\u2551" << green << " Next Shape:             " << cyan << "\u2551\n";
    shape(arr, 0, 0, next_shape);
    for(int i = 1; i < 4; i++)
    {
        cout << cyan << "\u2551   ";
        for(int j = 0; j < 4; j++)
        {
            if(arr[i][j]==0)
            {
                cout << "   ";
            }
            if(arr[i][j]==1)
            {
                cout << green << "\u250f\u2501\u2513";
            }
            if(arr[i][j]==2)
            {
                cout << blue << "\u250f\u2501\u2513";
            }
            if(arr[i][j]==3)
            {
                cout << yellow << "\u250f\u2501\u2513";
            }
            if(arr[i][j]==4 )
            {
                cout << purple<< "\u250f\u2501\u2513";
            }
            if(arr[i][j]==5)
            {
                cout << red << "\u250f\u2501\u2513";
            }
            if(arr[i][j]==6)
            {
                cout << cyan << "\u250f\u2501\u2513";
            }
            if(arr[i][j]==7)
            {
                cout << gray << "\u250f\u2501\u2513";
            }
            cout << reset;
        }
        for (int i = 0; i < 10; i++)
        {
            cout << " ";
         }
        cout <<cyan<< "\u2551";
        
        
        cout << "\n";
        cout <<cyan << "\u2551   ";
        for(int j = 0; j < 4; j++)
        {
            if(arr[i][j]==0)
            {
                cout << "   ";
            }
            if(arr[i][j]==1)
            {
                cout << green << "\u2517\u2501\u251b";
            }
            if(arr[i][j]==2)
            {
                cout << blue << "\u2517\u2501\u251b";
            }
            if(arr[i][j]==3)
            {
                cout << yellow << "\u2517\u2501\u251b";
            }
            if(arr[i][j]==4 )
            {
                cout << purple<< "\u2517\u2501\u251b";
            }
            if(arr[i][j]==5)
            {
                cout << red << "\u2517\u2501\u251b";
            }
            if(arr[i][j]==6)
            {
                cout << cyan << "\u2517\u2501\u251b";
            }
            if(arr[i][j]==7)
            {
                cout << gray << "\u2517\u2501\u251b";
            }
            cout << reset;
        }
        for (int i = 0; i < 10; i++)
        {
            cout << " ";
         }
        cout <<cyan<< "\u2551";
        cout << "\n";

    }
         cout << "\u255a";
         for(int i = 0; i < 25; i++)
         {
            cout << "\u2550";
         }
         cout << "\u255d\n";
    }
bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}
char getch() {
        char buf = 0; //dude that is too hard to explain, think like i just simply use conio.h:)))))
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}
