#include <iostream>
#include <csignal>

#include <unistd.h>
#include <fcntl.h>

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
            // Update 9/23: We actually have to account for args that aren't spotted (such as <, >, |)
            int argListSize = com.numArgs() + 1;
            int argsFilled = 0;
            if (com.redirIn()) { argListSize++; }
            char * argList[argListSize];
            // First arg is command name, next are the flags, etc

            for (int i = 0; i < com.numArgs(); i++) {
                // Should iterate through the command's args and add them to our char argList
                // Just because that is what needed to pass through execvp() 
                argList[i] = (char*) com.args()[i].c_str();
                argsFilled++;
            }
            

            if (com.redirIn()) {
                argList[argsFilled] = (char*)com.inputRedirectFile().c_str();
                argsFilled++;
            }

            argList[argsFilled] = NULL;
            
            
            if (fork() == 0) {
                // 9/24: figured out how to redirect to another file
                // Use open to open another file stream
                if (com.redirOut()) {
                    int fd = open(com.outputRedirectFile().c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                    dup2(fd, 1);
                    close(fd);
                }

                // 9/26: figured out how to redirect with input, fucking feeling good.
                if (com.redirIn()) {
                    char *c = (char *) calloc(100, sizeof(char));
                    int fd = open(com.inputRedirectFile().c_str(), O_RDONLY);
                    if (fd < 0) { perror("r1"); exit(1); }
                    int sz = read(fd, c, 10);
                }

                int status_code = execvp(argList[0], argList);

                if (status_code == -1) {
                    cout << "Terminated Incorrectly" << endl;
                    return 1;
                }
            }
        }

        // prompt for and read next command
	    cout << ">>>> ";
        com.read();
    }
    
    cout << "Thank you for using mini-shell. We now return you to your regularly scheduled shell!" << endl;
}
