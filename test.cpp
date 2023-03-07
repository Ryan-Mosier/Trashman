//
// Created by ryanm on 3/7/2023.
//

#include <string>
#include <chrono>
#include <iostream>
#include <thread>
#include <termios.h>
#include <unistd.h>

using namespace std;

int main(){
    while(true){
        string input;

        /*#region Input*/
        struct termios orig_termios;  // store original terminal settings

        tcgetattr(STDIN_FILENO, &orig_termios);  // get current terminal settings
        struct termios new_termios = orig_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);  // set non-canonical mode and turn off echoing
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);  // apply new terminal settings

        auto start = std::chrono::steady_clock::now();  // get the start time

        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count() <
               1000)  // loop for 1 second
        {
            char c;
            if (read(STDIN_FILENO, &c, 1) == 1)  // read one character of input
            {
                input += input;
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);  // restore original terminal settings

        /*#endregion*/

        cout << "Your input was: " << input;
        if(input == "e"){
            return 1;
        }
    }

}