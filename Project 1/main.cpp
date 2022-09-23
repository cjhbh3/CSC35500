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
            // Need 1 additional spot as the argList has to be Null-Terminated
            int argListSize = com.numArgs() + 1;
            char * argList[argListSize];
            // First arg is command name, next are the flags, etc

            for (int i = 0; i < com.numArgs(); i++) {
                // Should iterate through the command's args and add them to our char argList
                // Just because that is what needed to pass through execvp()
                argList[i] = (char*)com.args()[i].c_str();
            }

            argList[argListSize-1] = NULL;

            if (execvp(argList[0], argList) == -1) {
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
