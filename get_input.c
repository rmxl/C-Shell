#include "./get_input.h"

BgProcess* bgProcesses = NULL;
int bgProcNum = 0;
int fgPID = -1;
char fgProcName[INPUT_LENGTH_MAX] = {0};
int stdin_copy, stdout_copy;

void hangle_sigstop(int sig){
    //handle ctrl z
    if(fgPID != -1){
        if(kill(fgPID, SIGSTOP) == -1){
            printColor("ERROR : Could not stop process.", RED);
            return;
        }
        if(getProcess(bgProcesses, fgPID) == NULL){
            bgProcNum++;
            bgProcesses = addBgProcess(bgProcesses, fgPID, fgProcName, 0, bgProcNum);
        }
        fgPID = -1;
    }
    printf("\n");
    return;
}

void handle_sigchld(int sig) {
    // handle the status change of child
    while (1) {
        int status;
        int pid = (int) waitpid(-1, &status, WUNTRACED | WNOHANG | WCONTINUED);
        if (pid <= 0) {
            if(errno == EINTR){
                continue;
            }
            else break;
        }

        BgProcess* bgProcess = getProcess(bgProcesses, pid);

        if(bgProcess == NULL){
            continue;
        }

        if(WIFSTOPPED(status)){
            bgProcess->isRunning = 0;
            break;
        }
        else if(WIFCONTINUED(status)){
            bgProcess->isRunning = 1;
            break;
        }
        else{
            dprintf(stdout_copy, "[%d] %s", bgProcess->bgInvocationNum, bgProcess->command);
            

            bgProcesses = removeBgProcess(bgProcesses, pid);
            bgProcNum--;
            
            if(WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                dprintf(stdout_copy, " exited abnormally (%d) with signal %d\n", pid, WTERMSIG(status));

            }         
            else if(WIFSIGNALED(status)){
                dprintf(stdout_copy, " exited abnormally (%d) with signal %d\n", pid, WTERMSIG(status));
            }
            else if(WIFEXITED(status)) {
                dprintf(stdout_copy, " exited normally (%d)\n", pid);
            }
            break;
        }
    }
    return;
}

void handle_sigint(int sig){
    // handle ctrl c
    if(fgPID != -1){
        kill(fgPID, SIGINT);
    }
    fgPID = -1;
    printf("\n");
    return;
}

void freeBgProcesses(BgProcess* bgProcesses){
    BgProcess* temp = bgProcesses;
    while(temp != NULL){
        kill(temp->pid, SIGKILL);
        BgProcess* next = temp->next;
        free(temp->command);
        free(temp);
        temp = next;
    }
    return;
}

int main(){
    printf("Welcome to my custom shell.\n\n");
    char rootDir[PATH_LENGTH_MAX] = {0};
    getcwd(rootDir, PATH_LENGTH_MAX);

    char prevDir[PATH_LENGTH_MAX] = {0};
    
    int timeTakenByLastCommand = 0;
    char commandName[INPUT_LENGTH_MAX] = {0};

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa2;
    memset(&sa2, 0, sizeof(sa2));
    sa2.sa_handler = &handle_sigint;
    sigemptyset(&sa2.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGINT, &sa2, NULL) == -1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa3;
    memset(&sa3, 0, sizeof(sa3));
    sa3.sa_handler = &hangle_sigstop;
    sigemptyset(&sa3.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGTSTP, &sa3, NULL) == -1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    fgPID = -1;

    stdin_copy = dup(STDIN_FILENO);
    stdout_copy = dup(STDOUT_FILENO);

    for(;;){
        prompt(rootDir, timeTakenByLastCommand, commandName);
        timeTakenByLastCommand = 0;
        char inputString[INPUT_LENGTH_MAX] = {0};
        if(fgets(inputString, INPUT_LENGTH_MAX, stdin) == NULL){
            if(feof(stdin)){    
                //ctrl d
                printf("\n");
                if(fgPID != -1){
                    kill(fgPID, SIGKILL);
                }
                BgProcess* temp = bgProcesses;
                while(temp != NULL){
                    kill(temp->pid, SIGKILL);
                    temp = temp->next;
                }
                freeBgProcesses(bgProcesses);
                printf("Exiting shell.\n");
                close(stdin_copy);
                close(stdout_copy);
                exit(EXIT_SUCCESS);
            }
            else{
                //ctrl c
                clearerr(stdin);
                continue;
            }
        }
        if(inputString[INPUT_LENGTH_MAX - 1] != '\0' && inputString[INPUT_LENGTH_MAX - 1] != '\n'){
            printColor("ERROR : Commands entered are too large.", RED);
            continue;
        }

        int e = executeCommand(inputString, rootDir, prevDir, commandName, &timeTakenByLastCommand, &bgProcesses, &bgProcNum);

        if(e == -1){
            freeBgProcesses(bgProcesses);
            printf("Exiting shell.\n");
            close(stdin_copy);
            close(stdout_copy);
            break;
        }
    }
    return 0;
}
 