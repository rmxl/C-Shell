#include "./fgbg.h"

int fgbg(BgProcess** BgProcesses, int* bgProcNum, int mode, int pid, int* timeTakenByLastCommand){
    if(mode == 0){
        // bg
        if(kill(pid, SIGCONT) == -1){
            switch(errno){
                case EPERM:
                    printColor("ERROR : Permission denied.", RED);
                    break;
                case ESRCH:
                    printColor("ERROR : No such process.", RED);
                    break;
                default:
                    printColor("ERROR : Unknown error.", RED);
                    break;
            }
            return -1;
        }
        return 0;
    }
    else{
        //fg

        int pgid = getpgid(pid);
        int shellpgid = getpgid(0);

        if(tcsetpgrp(0, pgid) == -1){
            switch(errno){
                case EPERM:
                    printColor("ERROR : Permission denied.", RED);
                    break;
                case EINVAL:
                    printColor("ERROR : No such process exists.", RED);
                    break;
                case ENOTTY:
                    printColor("ERROR : Not a terminal.", RED);
                    break;
                default:
                    printColor("ERROR : Unknown error.", RED);
                    break;
            }
            return -1;
        }

        if(kill(pid, SIGCONT) == -1){
            switch(errno){
                case EPERM:
                    printColor("ERROR : Permission denied.", RED);
                    break;
                case ESRCH:
                    printColor("ERROR : No such process.", RED);
                    break;
                default:
                    printColor("ERROR : Unknown error.", RED);
                    break;
            }
            return -1;
        }
        time_t start, end;
        time(&start);
        fgPID = pid;
        (*bgProcNum)--;
        int status;
        int spawnedByThisShell = 1;
        BgProcess* bgProcess = getProcess(*BgProcesses, pid);
        if(bgProcess == NULL){
            spawnedByThisShell = 0;
        }

        strcpy(fgProcName, bgProcess->command);
        
        *BgProcesses = removeBgProcess(*BgProcesses, pid);

        int wc = waitpid(pid, &status, WUNTRACED);
        time(&end);
        double time_taken = difftime(end, start);
        *timeTakenByLastCommand = (int)time_taken;
        fgPID = -1;
        
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);

        if(tcsetpgrp(0, shellpgid) == -1){
            printColor("ERROR : tcsetpgrp failed.", RED);
            return -1;
        }

        if(wc == -1 && errno != EINTR) {
            if(spawnedByThisShell){
                printColor("ERROR : waitpid", RED);
            }
            else{
                // spawned by someone else so we don't wait for them
            }
        }

        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
                
        return 0;            
    }
}
