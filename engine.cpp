#include "engine.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

//for user input without enter
#ifdef _WIN32
#include <conio.h> // for Windows
#else
#include <termios.h> // for macOS and Linux
#include <unistd.h>
#include <fcntl.h>
#endif



using std::cout, std::endl;

Engine::Engine() : is_running(true), x_grid(40), y_grid(20), x_player(10), y_player(10), score(0), x_food(0), y_food(0) {
    std::cout << "\033[2J\033[1;1H";
    spawnFood(); // random spawn for first food position
};


void Engine::run () {
    while (is_running){ // game-loop
        input();
        update();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // 33 millisecond buffer
    }
    cout << "Game Over!" << endl;
    return;
}


void Engine::render () {
    std::string output = "\033[H";

    for (unsigned x = 0; x <= x_grid+1; x++){ // print upper border
        output += '-';
    }
    output += '\n';


    for (unsigned y = 0; y < y_grid; y++){
        output += '|'; //print left border
        for (unsigned x = 0; x < x_grid; x++){ 
            if (x == x_player and y == y_player){
                output += '@'; // print player
            } else if (x==x_food and y==y_food){
                output += 'o'; // print food
            } else {
                output += ' '; // print empty board
            }
        }
        output += "|\n"; // print right border
    }

    
    for (unsigned x = 0; x <= x_grid+1; x++){ // print lower border
        output += '-';
    }
    output += '\n';
    output += "Score: " + std::to_string(score) + "\n";
    cout << output << std::flush;
}


bool isKBhit(){
    #ifdef _WIN32
    return _kbhit() != 0;

    #else
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt); // old terminal settings
    newt=oldt;
    
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // non-blocking mode for input

    ch=getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore old terminal settings
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF){
        ungetc(ch, stdin);
        return true;
    }
    return false;
    #endif
}


void Engine::input (){
    if(isKBhit()){
        #ifdef _WIN32
        char key = _getch(); // get character on Windows
        #else
        char key = getchar(); // get character on macOS/Linux
        #endif
        switch(key){
            case 'w': y_player--; break;
            case 'a': x_player--; break;
            case 's': y_player++; break;
            case 'd': x_player++; break;
            case 'q': is_running=false; break;
        }
    }
}


void Engine::update (){ 
    // don't let player go out of map
    if (x_player<0){x_player=0;}
    if (y_player<0){y_player=0;}
    if (x_player>=x_grid){x_player=x_grid-1;}
    if (y_player>=y_grid){y_player=y_grid-1;}
    
    // check if player reached food
    if (x_player == x_food and y_player == y_food){
        score+=100;
        spawnFood();
    }
}


void Engine::spawnFood (){ // update food coordinates
    srand(time(0));
    do{
    x_food = rand()%(x_grid-1) + 1;
    y_food = rand()%(y_grid-1) + 1;
    } while (x_food==x_player and y_food == y_player);
}