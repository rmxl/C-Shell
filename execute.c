#include "./execute.h"

void freeArgs(char* args[], int argsNum){
    for(int i = 0; i < argsNum; i++){
        free(args[i]);
    }
}

int executeCommand(char* inputString, char* rootDir, char* prevDir, char* commandName, int* timeTakenByLastCommand, BgProcess** BgProcesses, int* bgProcNum){
    
    char buffer[INPUT_LENGTH_MAX] = {0};
    strcpy(buffer, inputString);
    char* command = strtok(inputString, ";");
    int ptr = 0;

    int checkIfCommandHasLog = 0;

    while(command != NULL){
        int backgroundProcess = 0;
        char* args[INPUT_LENGTH_MAX];
        int argsNum = 0;

        int pc = parseCommand(command, commandName, args, &backgroundProcess, &argsNum, &ptr);

        if(pc == -1){
            command = strtok(NULL, ";");
            ptr = 0;
            continue;
        }

        if(strcmp(commandName, "hop") == 0){
            hop(command, rootDir, prevDir);
            command = strtok(NULL, ";");
            ptr = 0;

            freeArgs(args, argsNum);            
            continue;
        }

        else if(strcmp(commandName, "reveal") == 0){
            time_t start, end;
            time(&start);
            reveal(args, argsNum, rootDir, prevDir);
            time(&end);
            double time_taken = difftime(end, start);
            *timeTakenByLastCommand = (int) time_taken; 
            command = strtok(NULL, ";");
            ptr = 0;

            freeArgs(args, argsNum); 
            continue;                
        }

        else if(strcmp(commandName, "seek") == 0){
            time_t start, end;
            time(&start);
            seek(args, argsNum, rootDir, prevDir); 
            time(&end);
            double time_taken = difftime(end, start);
            *timeTakenByLastCommand = (int) time_taken;
            command = strtok(NULL, ";");
            ptr = 0;

            freeArgs(args, argsNum); 
            continue;                
        }

        else if(strcmp(commandName, "proclore") == 0){
            if(argsNum == 0){
                int shellID = getpid();
                proclore(shellID);

                command = strtok(NULL, ";");
                ptr = 0;
                 
                freeArgs(args, argsNum); 
                continue;
            }
            else if(argsNum == 1){
                long val;
                char* endpointer;
                val = strtol(args[0], &endpointer, 10);
                if(*endpointer != '\0'){
                    printColor("ERROR : Invalid argument.", RED);
                }
                else proclore((int) val);

                command = strtok(NULL, ";");
                ptr = 0;
                
                freeArgs(args, argsNum); 
                continue;
            }
            else{
                printColor("ERROR : Invalid number of arguments.", RED);
                command = strtok(NULL, ";");
                ptr = 0;
                freeArgs(args, argsNum);
                continue;
            }
        }

        else if(strcmp(commandName, "log") == 0){

            checkIfCommandHasLog = 1;

            if(argsNum == 0){
                logCommand(command, 1, -1, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if(strcmp(args[0], "purge") == 0){
                logCommand(command, 2, -1, NULL, NULL, NULL, NULL, NULL, NULL);
            }
            else if(strcmp(args[0], "execute") == 0){
                long val;
                char* endpointer;
                val = strtol(args[1], &endpointer, 10);
                if(*endpointer != '\0'){
                    printColor("ERROR : Invalid argument.", RED);
                }
                else {
                    logCommand(command, 3, (int) val, rootDir, prevDir, commandName, timeTakenByLastCommand, BgProcesses, bgProcNum);
                }
            }
            else{
                printColor("ERROR : Invalid argument.", RED);
            }
            command = strtok(NULL, ";");
            ptr = 0;
            
            freeArgs(args, argsNum); 
            continue;
        }

        else if(strcmp(commandName, "exit") == 0){
            return -1;
        }

        time_t start, end;
        if(!backgroundProcess){
            time(&start);
        }

        sigset_t sigchld_mask;

        if(backgroundProcess){
            sigemptyset(&sigchld_mask);
            sigaddset(&sigchld_mask, SIGCHLD);
            sigprocmask(SIG_BLOCK, &sigchld_mask, NULL);    
        }

        int pid = fork();

        if(pid == -1){
            printColor("ERROR : Fork failed.", RED);
            break;
        }
        
        else if(pid == 0){
            char* execArgs[argsNum + 2];
            execArgs[0] = strdup(commandName);
            for(int i = 0; i < argsNum; i++){
                execArgs[i+1] = args[i];
            }                
            execArgs[argsNum + 1] = NULL;
            execvp(execArgs[0], execArgs);
            
            printf(RED);
            printf("ERROR : \'%s\' is not a valid command.\n", commandName);
            printf(RESET);
            exit(EXIT_FAILURE);
        }

        else{
            if(!backgroundProcess){
                int status;
                int rc_wait = waitpid(pid, &status, 0);
                if(rc_wait == -1){
                    printColor("ERROR : Wait failed.", RED);
                    break;
                }

                time(&end);
                double time_taken = difftime(end, start);
                *timeTakenByLastCommand = (int) time_taken;
                command = strtok(NULL, ";");
                ptr = 0;
            }
            else {
                (*bgProcNum)++;
                printf("[%d] %d\n", *bgProcNum, pid);
                *BgProcesses = addBgProcess(*BgProcesses, pid, commandName);
                sigprocmask(SIG_UNBLOCK, &sigchld_mask, NULL);
            }                
            freeArgs(args, argsNum); 
        }
    }

    if(checkIfCommandHasLog == 0){
        logCommand(buffer, 0, -1, NULL, NULL, NULL, NULL, NULL, NULL);
    }

    return 0;
}

