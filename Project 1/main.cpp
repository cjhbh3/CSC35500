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


            // 9/24: figured out how to redirect to another file
            // Use open to open another file stream
            if (com.redirOut()) {
                int fd = open(com.outputRedirectFile().c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                dup2(fd, 1);
                close(fd);
            }

            // 9/26: figured out how to redirect with input
            if (com.redirIn()) {
                char *c = (char *) calloc(100, sizeof(char));
                int fd = open(com.inputRedirectFile().c_str(), O_RDONLY);
                if (fd < 0) { perror("r1"); exit(1); }
                int sz = read(fd, c, 10);
            }


            if (!com.pipeOut()) {
                if (com.backgrounded()) {
                    int pid = fork();
                    if (pid != 0) {
                        
                    } 
                    else {
                        int pid = fork();
                        if (pid != 0) {
                            waitpid(pid, NULL, 0);
                            cout<< "Completed: PID = " << pid << " :: Command = "<< com << endl;
                            exit(1);
                        }
                        else {
                            // Normal execute
                            int status_code = execvp(argList[0], argList);

                            if (status_code == -1) {
                                cout << "Terminated Incorrectly" << endl;
                                return 1;
                            }
                        }
                    }
                }
                else {
                    if (fork() == 0) {
                        // Normal execute
                        int status_code = execvp(argList[0], argList);

                        if (status_code == -1) {
                            cout << "Terminated Incorrectly" << endl;
                            return 1;
                        }
                    }
                    waitpid(0, NULL, 0);
                }
            }
            else {
                Command com2;  
                int com2ArgListSize = com.numArgs() + 1;
                int args2Filled = 0;
                if (com2.redirIn()) { com2ArgListSize++; }
                char * com2ArgList[com2ArgListSize];

                com2.read();
                const string * args2 = com2.args();

                cout << com2 << endl;

                /* Redo literally evertying we already did for com1*/
                // Add the pipes

               
                
                // First arg is command name, next are the flags, etc

                for (int i = 0; i < com2.numArgs(); i++) {
                    // Should iterate through the command's args and add them to our char argList
                    // Just because that is what needed to pass through execvp() 
                    com2ArgList[i] = (char*) com2.args()[i].c_str();
                    args2Filled++;
                }
                

                if (com2.redirIn()) {
                    com2ArgList[argsFilled] = (char*)com2.inputRedirectFile().c_str();
                    args2Filled++;
                }

                com2ArgList[argsFilled] = NULL;
                

                // 9/24: figured out how to redirect to another file
                // Use open to open another file stream
                if (com2.redirOut()) {
                    int fd = open(com2.outputRedirectFile().c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                    dup2(fd, 1);
                    close(fd);
                }

                // 9/26: figured out how to redirect with input
                if (com2.redirIn()) {
                    char *c = (char *) calloc(100, sizeof(char));
                    int fd = open(com2.inputRedirectFile().c_str(), O_RDONLY);
                    if (fd < 0) { perror("r1"); exit(1); }
                    int sz = read(fd, c, 10);
                }


                int p[2];
                if (pipe(p) == -1) {
                    perror("Pipe failed");
                    exit(1);
                }

                if (fork() == 0) {
                    close(STDOUT_FILENO);
                    dup(p[1]);
                    close(p[0]);
                    close(p[1]);

                    execvp(argList[0], argList);
                    perror("execvp of 1st child");
                    exit(1);
                }

                if (fork() == 0) {
                    close(STDIN_FILENO);
                    dup(p[0]);
                    close(p[1]);
                    close(p[0]);

                    execvp(com2ArgList[0], com2ArgList);
                    perror("execvp of 2nd child");
                    exit(1);
                }

                close(p[0]);
                close(p[1]);
                wait(0);
                wait(0);
            }
                
        }

        // prompt for and read next command
	    cout << ">>>> ";
        com.read();
    }
    
    cout << "Thank you for using mini-shell. We now return you to your regularly scheduled shell!" << endl;
}
