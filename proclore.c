#include "./proclore.h"

void proclore(int id){
    printf("PID : %d\n", id);

    // /proc/pid/status contains the info of any process

    char path[PATH_LENGTH_MAX] = {0};
    sprintf(path, "/proc/%d/status", id);
    FILE* fp = fopen(path, "r");
    if(fp == NULL){
        printColor("ERROR : Process does not exist", RED);
        return;
    }
    char tempStr[INPUT_LENGTH_MAX] = {0};
    while(fgets(tempStr, INPUT_LENGTH_MAX, fp)){
        if(strstr(tempStr, "State:\t") != NULL){  
            printf("Process Status : %c", tempStr[7]);
            break;
        }
    }
    int processGroupID = getpgid(id);
    int fgID = tcgetpgrp(STDIN_FILENO);
    if(processGroupID == id && fgID == processGroupID){
        //foreground process
        printf("+\n");
    }
    else{
        //background process
        printf("\n");
    }

    printf("Process Group : %d\n", processGroupID);

    while(fgets(tempStr, INPUT_LENGTH_MAX, fp)){
        if(strstr(tempStr, "VmSize:\t") != NULL){ 
            int ptr = 8;
            while(tempStr[ptr] == ' ' || tempStr[ptr] == '\t'){
                ptr++;
            }
            printf("Virtual Memory : %s", tempStr + ptr);
            break;
        }
    }

    fclose(fp);

    sprintf(path, "/proc/%d/exe", id); // to read the exe path name
    fp = fopen(path, "r");
    if(fp == NULL){
        printColor("ERROR : Error while reading exe path.", RED);
        return;
    }
    char exePath[PATH_LENGTH_MAX] = {0};
    int er = readlink(path, exePath, PATH_LENGTH_MAX);
    if(er == -1){
        printColor("ERROR : Error while reading exe path", RED);
        return;
    }
    char currentDir[PATH_LENGTH_MAX];
    if(getcwd(currentDir, sizeof(currentDir)) == NULL){
        perror("getcwd");
        printf("Executable Path : %s\n", exePath);
        return;
    }
    int currentDirLen = (int) strlen(currentDir);
    if(strncmp(exePath, currentDir, currentDirLen) == 0){
        printf("Executable Path : ./%s\n", exePath + currentDirLen + 1);
    }
    else{
        printf("Executable Path : %s\n", exePath);
    }
    fclose(fp);
    return;
}
