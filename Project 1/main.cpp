#include <iostream>
#include <csignal>

#include <unistd.h>

#include "Command.hpp"

/*****************************************************
 *  Example usage of the Command class for project 1 *
 *****************************************************/

int main(void)
{
    Command com;
    int num=1; // keep track of number fo commands. 
    
    // prompt for and read in first command. 
    cout << ">>>> ";
    com.read();
    
    while(com.name() != "exit")
    {
        // print out current command
        cout << num++ << ")" << com << endl;

        const string * args = com.args();

        if (com.name() == "cd") {
            if (com.numArgs() > 1) {
                chdir(args[1].c_str());
            }
            else {
                chdir("..");
            }
        }
        else {
            // This works in a Linux (MacOS) setting, just have to adapt to what we need.
            char * arg[2];

            string ls = "ls";

            arg[0] = (char*)ls.c_str();
            arg[1] = NULL;

            if (execvp(arg[0], arg) == -1) {
                cout << "Cringe" << endl;
            }
            else {
                cout << "Not Cringe" << endl;
            }
        }

        cout << getcwd(0,0) << endl;
        
        // prompt for and read next command
	    cout << ">>>> ";
        com.read();
    }
    
    cout << "Thank you for using mini-shell. We now return you to your regularly scheduled shell!" << endl;
}
