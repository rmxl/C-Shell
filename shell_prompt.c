#include "./shell_prompt.h"

void prompt(char* originalDir, int timeTakenByLastCommand, char* lastCommand){
    char currentDir[PATH_LENGTH_MAX];
    memset(currentDir, 0, PATH_LENGTH_MAX);

    if(getcwd(currentDir, PATH_LENGTH_MAX) == NULL){
        switch(errno){
            case ERANGE:
            case ENAMETOOLONG:
                printColor("ERROR : The path name is too large.", RED);
                break;
            default:
                printColor("ERROR : Unknown error.", RED);
        }
        return;
    }

    char* homeDirCheck = strstr(currentDir, originalDir);

    char username[HOST_LENGTH_MAX] = {0};
    if(getlogin_r(username, HOST_LENGTH_MAX) != 0){
        printColor("ERROR : Failed to get username.", RED);
    }

    char hostname[HOST_LENGTH_MAX] = {0};

    if(gethostname(hostname, HOST_LENGTH_MAX) != 0){
        printColor("ERROR : Failed to get hostname.", RED);
    }

    // print prompt
    printf(HCYN);
    printf("<%s@%s:", username, hostname);
    if(homeDirCheck){
        if(strcmp(currentDir, originalDir) == 0){
            printf("~");
        }
        else{
            int x = (int) strlen(originalDir);
            printf("~%s", currentDir + x);
        }
    }
    
    else{
        printf("%s", currentDir);
    }

    if(timeTakenByLastCommand > 2){
        // print last command and time taken
        printf(" %s : %ds>", lastCommand, timeTakenByLastCommand);
    }
    else printf(">");
    printf(RESET);

    return;
}
