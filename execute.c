#include "./execute.h"

void freeArgs(char *args[], int argsNum)
{
    for (int i = 0; i < argsNum; i++)
    {
        free(args[i]);
    }
}

int executeSubCommand(char* command, char *rootDir, char *prevDir, char *commandName, int *timeTakenByLastCommand, BgProcess **BgProcesses, int *bgProcNum, char *args[], int argsNum, int* checkIfCommandHasLog, int* backgroundProcess){

    if (strcmp(commandName, "hop") == 0)
    {
        hop(command, rootDir, prevDir);
        return EXECUTESUBCOMMAND_SUCCESS;
    }

    else if (strcmp(commandName, "reveal") == 0)
    {
        time_t start, end;
        time(&start);
        reveal(args, argsNum, rootDir, prevDir);
        time(&end);
        double time_taken = difftime(end, start);
        *timeTakenByLastCommand = (int)time_taken;
        return EXECUTESUBCOMMAND_SUCCESS;
    }

    else if (strcmp(commandName, "seek") == 0)
    {
        time_t start, end;
        time(&start);
        seek(args, argsNum, rootDir, prevDir);
        time(&end);
        double time_taken = difftime(end, start);
        *timeTakenByLastCommand = (int)time_taken;
        return EXECUTESUBCOMMAND_SUCCESS;
    }

    else if (strcmp(commandName, "proclore") == 0)
    {
        if (argsNum == 0)
        {
            int shellID = getpid();
            proclore(shellID);
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else if (argsNum == 1)
        {
            long val;
            char *endpointer;
            val = strtol(args[0], &endpointer, 10);
            if (*endpointer != '\0')
            {
                printColor("ERROR : Invalid argument.", RED);
                return EXECUTESUBCOMMAND_NORMAL_FAILURE;
            }
            else proclore((int)val);
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else
        {
            printColor("ERROR : Invalid number of arguments.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
    }

    else if (strcmp(commandName, "log") == 0)
    {

        *checkIfCommandHasLog = 1;

        if (argsNum == 0)
        {
            logCommand(command, 1, -1, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else if (strcmp(args[0], "purge") == 0)
        {
            logCommand(command, 2, -1, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else if (strcmp(args[0], "execute") == 0)
        {
            long val;
            char *endpointer;
            val = strtol(args[1], &endpointer, 10);
            if (*endpointer != '\0')
            {
                printColor("ERROR : Invalid argument.", RED);
                return EXECUTESUBCOMMAND_NORMAL_FAILURE;
            }
            else
            {
                logCommand(command, 3, (int)val, rootDir, prevDir, commandName, timeTakenByLastCommand, BgProcesses, bgProcNum);
            }
        }
        else
        {
            printColor("ERROR : Invalid argument.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        return EXECUTESUBCOMMAND_SUCCESS;
    }

    else if(strcmp(commandName, "activities") == 0){
        activities(BgProcesses, bgProcNum);
        return EXECUTESUBCOMMAND_SUCCESS;
    }

    else if(strcmp(commandName, "ping") == 0){
        if(argsNum != 2){
            printColor("ERROR : Invalid number of arguments.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        long val1, val2;
        char *endpointer;
        val1 = strtol(args[0], &endpointer, 10);
        if (*endpointer != '\0')
        {
            printColor("ERROR : Invalid argument.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        val2 = strtol(args[1], &endpointer, 10);
        if (*endpointer != '\0')
        {
            printColor("ERROR : Invalid argument.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        int rc = ping((int)val1, (int)val2);
        if(rc == 0){
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else{
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
    }

    else if(strcmp("bg", commandName) == 0){
        if(argsNum != 1){
            printColor("ERROR : Invalid number of arguments.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        long val;
        char *endpointer;
        val = strtol(args[0], &endpointer, 10);
        if (*endpointer != '\0')
        {
            printColor("ERROR : Invalid argument.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        int rc = fgbg(BgProcesses, bgProcNum, 0, (int)val, timeTakenByLastCommand);
        if(rc == 0){
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else{
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
    }

    else if(strcmp("fg", commandName) == 0){
        if(argsNum != 1){
            printColor("ERROR : Invalid number of arguments.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        long val;
        char *endpointer;
        val = strtol(args[0], &endpointer, 10);
        if (*endpointer != '\0')
        {
            printColor("ERROR : Invalid argument.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        int rc = fgbg(BgProcesses, bgProcNum, 1, (int)val, timeTakenByLastCommand);
        if(rc == 0){
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else{
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
    }
    
    else if(strcmp(commandName, "iMan") == 0){
        if(argsNum == 0){
            printColor("ERROR : Invalid number of arguments.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        
        int ec = iman(args[0]);
        if(ec == 0){
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else{
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }           
    }

    else if(strcmp("neonate", commandName) == 0){
        int ec = -1;
        if(argsNum == 0){
            ec = neonate(-1);
        }   
        else if(argsNum != 2) {
            printColor("ERROR : Invalid number of arguments.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        else if(strcmp(args[0], "-n") != 0){
            printColor("ERROR : Invalid option.", RED);
        } 
        else {
            long val;
            char *endpointer;
            val = strtol(args[1], &endpointer, 10);
            if (*endpointer != '\0')
            {
                printColor("ERROR : Invalid argument.", RED);
                return EXECUTESUBCOMMAND_NORMAL_FAILURE;
            }
            if(val <= 0){
                printColor("ERROR : Invalid argument.", RED);
                return EXECUTESUBCOMMAND_NORMAL_FAILURE;
            }
            ec = neonate((int)val);
        }

        if(ec == 0){
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else{
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
    }

    else if (strcmp(commandName, "exit") == 0)
    {
        return EXECUTESUBCOMMAND_EXIT;
    }
    else
    {
        int ec = customCommands(commandName, args, argsNum, rootDir, prevDir, timeTakenByLastCommand, BgProcesses, bgProcNum);

        if (ec == 0)
        {
            return EXECUTESUBCOMMAND_SUCCESS;
        }
        else if (ec == -1)
        {
        }
        else if (ec == -2)
        {
            return EXECUTESUBCOMMAND_EXIT;
        }
        else if (ec == -3)
        {
            printColor("ERROR : Invalid syntax.", RED);
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
        else if (ec == -4)
        {
            return EXECUTESUBCOMMAND_NORMAL_FAILURE;
        }
    }

    int ec = execCommand(commandName, timeTakenByLastCommand, BgProcesses, bgProcNum, args, argsNum, *backgroundProcess);

    if(ec == 0){
        return EXECUTESUBCOMMAND_SUCCESS;
    }
    else if(ec == -2){
        return EXECUTESUBCOMMAND_CRITICAL_FAILURE;
    }
    return EXECUTESUBCOMMAND_SUCCESS;
}

int executeCommand(char *inputString, char *rootDir, char *prevDir, char *commandName, int *timeTakenByLastCommand, BgProcess **BgProcesses, int *bgProcNum)
{

    char buffer[INPUT_LENGTH_MAX] = {0};
    strcpy(buffer, inputString);
    char *saveptr;
    char *command = strtok_r(inputString, ";", &saveptr);
    int ptr = 0;

    int checkIfCommandHasLog = 0;

    while (command != NULL)
    {

        int backgroundProcess = 0;
        char *args[INPUT_LENGTH_MAX];
        int argsNum = 0;
        int inputRedirect = 0;
        int outputRedirect = 0;
        char inputFilePATH[PATH_LENGTH_MAX] = {0};
        char outputFilePATH[PATH_LENGTH_MAX] = {0};
        int pipePresent = 0;

        int pc = parseCommand(command, commandName, args, &backgroundProcess, &argsNum, &ptr, &inputRedirect, &outputRedirect, inputFilePATH, outputFilePATH, &pipePresent);

        if (pc == -1)
        {
            command = strtok_r(NULL, ";", &saveptr);
            ptr = 0;
            continue;
        }

        else if (pc == -5){
            printColor("ERROR : Invalid use of |.", RED);
            command = strtok_r(NULL, ";", &saveptr);
            ptr = 0;
            continue;
        }
        
        else if(pc == -6){
            printColor("ERROR : Invalid syntax.", RED);
            command = strtok_r(NULL, ";", &saveptr);
            ptr = 0;
            continue;
        }

        if(pipePresent == 1){
            int ec = executePipe(command, rootDir, prevDir, commandName, timeTakenByLastCommand, BgProcesses, bgProcNum, args, argsNum, &ptr, inputRedirect, outputRedirect, inputFilePATH, outputFilePATH, &backgroundProcess);

            if(ec == EXECUTEPIPE_CRITICAL_FAILURE){
                return 0;
            }
            else if(ec == EXECUTEPIPE_NORMAL_FAILURE){
                break;
            }
            else if(ec == EXECUTEPIPE_SUCCESS){ 
                freeArgs(args, argsNum);
                continue;
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
            continue;   
        }
        else if(ioec == IOREDIRECT_SUCCESS){
        }
        
        int ec = executeSubCommand(command, rootDir, prevDir, commandName, timeTakenByLastCommand, BgProcesses, bgProcNum, args, argsNum, &checkIfCommandHasLog, &backgroundProcess);

            if(inputRedirect != 0){
                int rc = dup2(stdin_copy, STDIN_FILENO);
                if(rc == -1){
                    printColor("ERROR : Duplication failed.", RED);
                    return -1;
                }
            }

            if(outputRedirect != 0){
                int rc = dup2(stdout_copy, STDOUT_FILENO);
                if(rc == -1){
                    printColor("ERROR : Duplication failed.", RED);
                    return -1;
                }
            }

        if(ec == EXECUTESUBCOMMAND_SUCCESS){
            if(backgroundProcess == 0 && pipePresent == 0){
                command = strtok_r(NULL, ";", &saveptr);
                ptr = 0;
            }
            freeArgs(args, argsNum);
            continue;
        }
        else if(ec == EXECUTESUBCOMMAND_NORMAL_FAILURE){
            freeArgs(args, argsNum);
            continue;
        }
        else if(ec == EXECUTESUBCOMMAND_EXIT){
            return -1;
        }
        else if(ec == EXECUTESUBCOMMAND_CRITICAL_FAILURE){
            break;
        }

    }

    if (checkIfCommandHasLog == 0)
    {
        logCommand(buffer, 0, -1, NULL, NULL, NULL, NULL, NULL, NULL);
    }

    return 0;
}

