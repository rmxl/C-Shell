#include "./hop.h"

int hopCommandLen = 4;

int changeDir(char* inputPath){
    int rc = chdir(inputPath);
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
    return 0;
}

int hop(char* inputPathN, char* rootDir, char* prevDir){
    char inputPath[PATH_LENGTH_MAX] = {0};
    char* s = strstr(inputPathN, "hop");
    strcpy(inputPath, s + hopCommandLen);
    inputPath[strlen(inputPathN) - 1 - hopCommandLen] = '\0';

    char* saveptr;
    char* token = strtok_r(inputPath, " ", &saveptr);

    char currDir[PATH_LENGTH_MAX] = {0};
    getcwd(currDir, PATH_LENGTH_MAX);

    if(token == NULL || strcmp(token, "") == 0 || strcmp(token, " ") == 0 || strcmp(token, "\t") == 0 || strcmp(token, "\n") == 0){
        strcpy(prevDir, currDir);
        int rc = changeDir(rootDir);
        if(rc == -1){
            return -1;
        }
        printf("%s\n", rootDir);
        return 0;        
    }

    int firstTime = 0;
    
    while(token != NULL){
        if(strcmp(token, "~") == 0){
            int rc = changeDir(rootDir);
            if(rc == -1){
                if(firstTime != 0){
                    strcpy(prevDir, currDir);   
                }
                return -1;
            }
            printf("%s\n", rootDir);
        }
        else if(strcmp(token, "-") == 0){
            if(strlen(prevDir) == 0){
                printColor("ERROR : No previous directory established yet.", RED);
                if(firstTime != 0){
                    strcpy(prevDir, currDir);   
                }
                return -1;
            }
            int rc = changeDir(prevDir);
            if(rc == -1){
                if(firstTime != 0){
                    strcpy(prevDir, currDir);   
                }
                return -1;
            }
            printf("%s\n", prevDir);
        }
        else{
            if(token[0] == '~'){
                char temp[PATH_LENGTH_MAX] = {0};
                strcpy(temp, rootDir);
                strcat(temp, token + 1);
                int rc = changeDir(temp);
                if(rc == -1){
                    if(firstTime != 0){
                        strcpy(prevDir, currDir);   
                    }
                    return -1;
                }
                getcwd(temp, PATH_LENGTH_MAX);
                printf("%s\n", temp);
                token = strtok_r(NULL, " ", &saveptr);
                continue;
            }
            int rc = changeDir(token);
            if(rc == -1){
                if(firstTime != 0){
                    strcpy(prevDir, currDir);   
                }
                return -1;
            }
            char temp[PATH_LENGTH_MAX] = {0};
            getcwd(temp, PATH_LENGTH_MAX);
            firstTime = 1;
            printf("%s\n", temp);
        }

        token = strtok_r(NULL, " ", &saveptr);
    }

    strcpy(prevDir, currDir);

    return 0;
}
