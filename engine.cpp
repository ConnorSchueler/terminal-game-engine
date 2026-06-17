#include "engine.hpp"
#include "iostream"

//for user input without enter
#include "termios.h"
#include "unistd.h"
#include "fcntl.h"


using std::cout, std::endl;

Engine::Engine() : is_running(true), x_grid(40), y_grid(20), x_player(10), y_player(10) {};


void Engine::run () {
    while (is_running){ //game-loop
        input();
        update();
        render();
        usleep(33000); // 33 millisecond buffer
    }
    cout << "Game Over!" << endl;
    return;
}


void Engine::render () {
    std::cout << "\033[2J\033[1;1H";

    for (unsigned x = 0; x <= x_grid+1; x++){ //print upper border
        cout << '-';
    }
    cout << endl;


    for (unsigned y = 0; y < y_grid; y++){
        cout << '|'; //print left border
        for (unsigned x = 0; x < x_grid; x++){ // print empty board
            if (x == x_player and y == y_player){
                cout << '@';
            } else {
                cout << ' ';
            }
        }
        cout << '|' << endl; // print right border
    }

    
    for (unsigned x = 0; x <= x_grid+1; x++){ //print lower border
        cout << '-';
    }
    cout << endl;
}

bool kbhit(){
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt); //old terminal settings
    newt=oldt;
    
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // non-blocking mode for input

    ch=getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //restore old terminal settings
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF){
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

void Engine::input (){
    if(kbhit()){
        char key = getchar();
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
    if (x_player<0){x_player=0;}
    if (y_player<0){y_player=0;}
    if (x_player>=x_grid){x_player=x_grid-1;}
    if (y_player>=y_grid){y_player=y_grid-1;}
}