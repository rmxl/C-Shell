#include "./get_input.h"

BgProcess* bgProcesses = NULL;
int bgProcNum = 0;

void handle_sigchld(int sig) {
    while (1) {
        int status;
        int pid = (int) waitpid(-1, &status, WNOHANG);
        if (pid <= 0) {
            if(errno == EINTR){
                continue;
            }
            else break;
        }
        
        printf("[%d] ", bgProcNum);

        bgProcesses = removeBgProcess(bgProcesses, pid);
        bgProcNum--;
        
        // if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
        //     printf(" exited normally (%d)\n", pid);
        // }
        // else if(WIFSIGNALED(status)){
        //     printf(" exited abnormally (%d) with signal %d\n", pid, WTERMSIG(status));
        // }
        if(WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf(" exited abnormally (%d) with signal %d\n", pid, WEXITSTATUS(status));
        }         
        else if(WIFSIGNALED(status)){
            printf(" exited abnormally (%d) with signal %d\n", pid, WTERMSIG(status));
        }
        else if(WIFEXITED(status)) {
            printf(" exited normally (%d)\n", pid);
        }
        
    }
    return;
}

void freeBgProcesses(BgProcess* bgProcesses){
    BgProcess* temp = bgProcesses;
    while(temp != NULL){
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
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    for(;;){
        prompt(rootDir, timeTakenByLastCommand, commandName);
        timeTakenByLastCommand = 0;
        char inputString[INPUT_LENGTH_MAX] = {0};
        fgets(inputString, INPUT_LENGTH_MAX, stdin);
        if(inputString[INPUT_LENGTH_MAX - 1] != '\0' && inputString[INPUT_LENGTH_MAX - 1] != '\n'){
            printColor("ERROR : Commands entered are too large.", RED);
            continue;
        }

        int e = executeCommand(inputString, rootDir, prevDir, commandName, &timeTakenByLastCommand, &bgProcesses, &bgProcNum);

        if(e == -1){
            freeBgProcesses(bgProcesses);
            printf("Exiting shell.\n");
            break;
        }
    }
}
 