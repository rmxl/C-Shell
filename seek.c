#include "./seek.h"

void outputPath(char* path, int colorMode){
    char currentDir[PATH_LENGTH_MAX];

    if(getcwd(currentDir, sizeof(currentDir)) != NULL) {
        int currentDirLen = (int) strlen(currentDir);

        if (strncmp(path, currentDir, currentDirLen) == 0 && path[currentDirLen] == '/') {
            if(colorMode == 1){
                printf(GRN);
                printf("./%s\n", path + currentDirLen + 1);
                printf(RESET);
            }
            else if(colorMode == 2){
                printf(BLU);
                printf("./%s\n", path + currentDirLen + 1);
                printf(RESET);
            }
        } else {
            if(colorMode == 1){
                printf(GRN);
                printf("%s\n", path);
                printf(RESET);
            }
            else if(colorMode == 2){
                printf(BLU);
                printf("%s\n", path);
                printf(RESET);
            }
        }
    }else {
        perror("getcwd");
        printf("%s\n", path);
    }
}

int seek(char** args, int argsNum, char* rootDir, char* prevDir){
    int directoriesOnly = 0;
    int filesOnly = 0;
    int specialHandle = 0;
    int i = 0;
    for(; i < argsNum; i++){
        char c;
        c = args[i][0];
        if(c == '-' && strlen(args[i]) > 1){   
            for(int j = 1; j < strlen(args[i]); j++){
                c = args[i][j];
                if(c == 'd'){
                    directoriesOnly = 1;
                }
                else if(c == 'f'){
                    filesOnly = 1;
                }
                else if(c == 'e'){
                    specialHandle = 1;
                }
                else{
                    printColor("ERROR : Invalid argument.", RED);
                    return -1;
                }
            }
        }
        else break;
    }
    if(argsNum - i > 2){
        printColor("ERROR : Too many arguments.", RED);
        return -1;
    }

    if(directoriesOnly == 1 && filesOnly == 1){
        printColor("ERROR : Invalid flags.", RED);
        return -1;
    }

    
    if(i == argsNum){
        printColor("ERROR : No target name provided.", RED);
        return -1   ;
    }
    
    char targetName[INPUT_LENGTH_MAX] = {0};
    strcpy(targetName, args[i]);

    char targetDir[PATH_LENGTH_MAX] = {0};
    if(i == argsNum - 1){
        strcpy(targetDir, ".");
    }
    else{
        strcpy(targetDir, args[argsNum - 1]);
    }

    if(targetDir[0] == '~'){
        char temp[PATH_LENGTH_MAX];
        strcpy(temp, rootDir);
        strcat(temp, targetDir + 1);
        strcpy(targetDir, temp);
    }
    else if(strcmp(targetDir, "-") == 0){
        if(strlen(prevDir) == 0){
            printColor("ERROR : No previous directory established yet.", RED);
            return -1;
        }
        strcpy(targetDir, prevDir);
    }
    else if(targetDir[0] != '/' && targetDir[0] != '.'){
        char temp[PATH_LENGTH_MAX] = "./";   
        strcat(temp, targetDir);
        strcpy(targetDir, temp);
    }

    char* paths[] = {targetDir, NULL};
    FTS* fts = fts_open(paths, FTS_PHYSICAL | FTS_NOCHDIR, NULL);

    if(fts == NULL){
        printColor("ERROR : Failed to open file stream.", RED);
        return -1;
    }

    FTSENT* node;
    int count = 0;
    char specialCase[PATH_LENGTH_MAX] = {0};
    char specialCaseType;
    while((node = fts_read(fts)) != NULL){
        if(node->fts_info & FTS_D){
            if(filesOnly == 0){
                if(strcmp(node->fts_name, targetName) == 0){
                    outputPath(node->fts_path, 2);
                    count++;
                    if(specialHandle == 1 && count == 1){
                        strcpy(specialCase, node->fts_path);
                        specialCaseType = 'd';
                    }
                }
            }
        }
        else if(node->fts_info & FTS_F){
            if(directoriesOnly == 0){
                char* ptr;
                char* name = strtok_r(node->fts_name, ".", &ptr);
                if(strcmp(name, targetName) == 0){
                    outputPath(node->fts_path, 1);
                    count++;
                    if(specialHandle == 1 && count == 1){
                        strcpy(specialCase, node->fts_path);
                        specialCaseType = 'f';
                    }
                }
            }
        }
    }

    if(count == 1 && specialHandle == 1){
        if(specialCaseType == 'd'){
            int rc = chdir(specialCase);

            if(rc == -1){
                switch(errno){
                    case ENAMETOOLONG:
                        printColor("ERROR : Path name is too long.", RED);
                        break;
                    case EACCES:
                        printColor("ERROR : Permission denied.", RED);
                        break;
                    case ENOENT:
                        printColor("ERROR : No such file or directory.", RED);
                        break;
                    case ENOTDIR:
                        printColor("ERROR : Not a directory.", RED);
                        break;
                    default:
                        printColor("ERROR : Unknown error.", RED);
                        break;
                }
                return -1;
             }
        }
        else if(specialCaseType == 'f'){
            FILE* fp = fopen(specialCase, "r");
            if(fp == NULL){
                switch(errno){
                    case ENAMETOOLONG:
                        printColor("ERROR : Path name is too long.", RED);
                        break;
                    case EACCES:
                        printColor("ERROR : Permission denied.", RED);
                        break;
                    default:
                        printColor("ERROR : Unknown error.", RED);
                        break;
                }
                return -1;
            }
            char c;
            printf("\n");
            while((c = fgetc(fp)) != EOF){
                printf("%c", c);
            }
            fclose(fp);
        }
    }

    if(count == 0){
        printf("No match found!\n");
    }

    fts_close(fts);
    return 0;
}
