#include "./pipe.h"

int countPipes(char *command, int* ptr)
{
    int count = 0;
    for (int i = *ptr; i < strlen(command); i++)
    {
        if(command[i] == '&'){
            return count;
        }
        if (command[i] == '|')
        {
            count++;
        }
    }
    return count;
}

int executePipe(char *command, char *rootDir, char *prevDir, char *commandName, int *timeTakenByLastCommand, BgProcess **BgProcesses, int *bgProcNum, char *args[], int argsNum, int* ptr, int inputRedirect, int outputRedirect, char inputFilePATH[], char outputFilePATH[], int* backgroundProcess)
{
    int pd_in = STDIN_FILENO;
    int stdin_copy2 = dup(STDIN_FILENO);
    int stdout_copy2 = dup(STDOUT_FILENO);

    int numPipes = countPipes(command, ptr) + 2;

    for(int i = 0 ; i < numPipes; i++){
        int pipefd[2];

        if(pd_in != STDIN_FILENO){
            if(dup2(pd_in, STDIN_FILENO) == -1) {
                printColor("ERROR : Duplication failed.", RED);
                close(pd_in);
                return EXECUTEPIPE_CRITICAL_FAILURE;
            }   
            close(pd_in);
        }

        if(pipe(pipefd) == -1){
            printColor("ERROR : Pipe failed.", RED);
            close(pipefd[0]);
            return EXECUTEPIPE_CRITICAL_FAILURE;
        }

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if(i == numPipes - 1){
            if(dup2(stdout_copy2, STDOUT_FILENO) == -1){
                printColor("ERROR : Duplication failed.", RED);
                close(pipefd[0]);
                return EXECUTEPIPE_CRITICAL_FAILURE;
            }
        }

        int ioec;
        if(inputRedirect != 0 || outputRedirect != 0){
            ioec = IORedirect(inputRedirect, outputRedirect, inputFilePATH, outputFilePATH, rootDir);
        }
        else{
            ioec = -1;
        }

        if(ioec == IOREDIRECT_SYSCALL_FAILURE || ioec == IOREDIRECT_NORMAL_FAILURE){
            close(pipefd[0]);
            close(pipefd[1]);
            dup2(stdout_copy2, STDOUT_FILENO);
            close(stdout_copy2);
            dup2(stdin_copy2, STDIN_FILENO);
            close(stdin_copy2);          
            return EXECUTEPIPE_NORMAL_FAILURE;
        }
        else if(ioec == IOREDIRECT_SUCCESS){
        }

        int checkIfCommandHasLog = 0;

        int ec = executeSubCommand(command, rootDir, prevDir, commandName, timeTakenByLastCommand, BgProcesses, bgProcNum, args, argsNum, &checkIfCommandHasLog, backgroundProcess);

        if(ec == EXECUTESUBCOMMAND_EXIT){
            return EXECUTEPIPE_CRITICAL_FAILURE; 
        }
        else if(ec == EXECUTESUBCOMMAND_CRITICAL_FAILURE){
            dup2(stdout_copy2, STDOUT_FILENO);
            close(stdout_copy2);
            dup2(stdin_copy2, STDIN_FILENO);
            close(stdin_copy2);
            return EXECUTEPIPE_CRITICAL_FAILURE;
        }
        else if(ec == EXECUTESUBCOMMAND_NORMAL_FAILURE){
            dup2(stdout_copy2, STDOUT_FILENO);
            close(stdout_copy2);
            dup2(stdin_copy2, STDIN_FILENO);
            close(stdin_copy2);
            return EXECUTEPIPE_NORMAL_FAILURE;
        }
        else if(ec == 0){
            
        }

        if(inputRedirect != 0){
            dup2(stdin_copy, STDIN_FILENO);
        }

        if(outputRedirect != 0){
            dup2(stdout_copy, STDOUT_FILENO);
        }

        int pipePresent = 0;
        int pc = 0;
        inputRedirect = outputRedirect = pipePresent = *backgroundProcess = 0;
        
        if(i < numPipes - 1) pc = parseCommand(command, commandName, args, backgroundProcess, &argsNum, ptr, &inputRedirect, &outputRedirect, inputFilePATH, outputFilePATH, &pipePresent);

        if(pc == -1){
            dup2(stdout_copy2, STDOUT_FILENO);
            close(stdout_copy2);
            dup2(stdin_copy2, STDIN_FILENO);
            close(stdin_copy2);

            if(i < numPipes - 1){
                close(pipefd[0]);
                close(pipefd[1]);
            }

            printColor("ERROR : Invalid use of |.", RED);

            return EXECUTEPIPE_CRITICAL_FAILURE;
        }

        else if(pc < 0){
            dup2(stdout_copy2, STDOUT_FILENO);
            close(stdout_copy2);
            dup2(stdin_copy2, STDIN_FILENO);
            close(stdin_copy2);

            if(i < numPipes - 1){
                close(pipefd[0]);
                close(pipefd[1]);
            }

            printColor("ERROR : Invalid syntax.", RED);

            return EXECUTEPIPE_CRITICAL_FAILURE;
        }

        pd_in = pipefd[0];
    }
    dup2(stdout_copy2, STDOUT_FILENO);
    close(stdout_copy2);
    dup2(stdin_copy2, STDIN_FILENO);
    close(stdin_copy2);
    return EXECUTEPIPE_SUCCESS;
}
