#include "./exec.h"

extern int fgPID;

int execCommand(char *commandName, int *timeTakenByLastCommand, BgProcess **BgProcesses, int *bgProcNum, char **args, int argsNum, int backgroundProcess){
    time_t start, end;
    if (!backgroundProcess)
    {
        time(&start);
    }

    sigset_t sigchld_mask;

    if (backgroundProcess)
    {
        // we want to wait for process to be added to list before it exits
        sigemptyset(&sigchld_mask);
        sigaddset(&sigchld_mask, SIGCHLD);
        sigprocmask(SIG_BLOCK, &sigchld_mask, NULL);
    }

    int pid = fork();

    if (pid == -1)
    {
        printColor("ERROR : Fork failed.", RED);
        return -2;
    }

    else if (pid == 0)
    {        
        if(backgroundProcess) {
            if(setpgrp() == -1) {
                printColor("ERROR : setpgrp failed.", RED);
                exit(EXIT_FAILURE);
            }
        }
        else {
            if(setsid() == -1) {
                printColor("ERROR : setsid failed.", RED);
                exit(EXIT_FAILURE);
            }
        }
        char *execArgs[argsNum + 2];
        execArgs[0] = strdup(commandName);
        for (int i = 0; i < argsNum; i++)
        {
            execArgs[i + 1] = args[i];
        }
        execArgs[argsNum + 1] = NULL;

        execvp(execArgs[0], execArgs);

        dprintf(stdout_copy, "%sERROR : \'%s\' is not a valid command.\n%s", RED, commandName, RESET);
        exit(EXIT_FAILURE);
    }

    else
    {
        if (!backgroundProcess)
        {
            int status;
            fgPID = pid;
            strcpy(fgProcName, commandName);
            int rc_wait = waitpid(pid, &status, WUNTRACED);
            fgPID = -1;
            if (rc_wait == -1 && errno != EINTR)
            {
                printColor("ERROR : Wait failed.", RED);
                return -2;
            }
            time(&end);
            double time_taken = difftime(end, start);
            *timeTakenByLastCommand = (int)time_taken;
            return 0;
        }
        else
        {
            (*bgProcNum)++;
            dup2(stdin_copy, STDIN_FILENO);
            dup2(stdout_copy, STDOUT_FILENO);
            printf("[%d] %d\n", *bgProcNum, pid);
            *BgProcesses = addBgProcess(*BgProcesses, pid, commandName, 1, *bgProcNum);
            sigprocmask(SIG_UNBLOCK, &sigchld_mask, NULL);
            return 0;
        }
    }
}
