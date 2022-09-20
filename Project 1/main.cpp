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
            const char * arg_list[com.numArgs()];

            for (int i = 0; i < com.numArgs(); i++) {
                arg_list[i] = args[i].c_str();
            }

            for (int i = 0; i < com.numArgs(); i++) {
                cout << arg_list[i] << endl;
            }

            int status = execvp(com.name().c_str(), arg_list);

            if (status == -1) {
                cout << "Did not work" << endl;
            }

        }

        cout << getcwd(0,0) << endl;
        
        // prompt for and read next command
	    cout << ">>>> ";
        com.read();
    }
    
    cout << "Thank you for using mini-shell. We now return you to your regularly scheduled shell!" << endl;
}
