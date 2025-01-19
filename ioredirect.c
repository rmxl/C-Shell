#include "./ioredirect.h"

int IORedirect(int inputRedirect, int outputRedirect, char *inputFilePATH, char *outputFilePATH, char *rootDir){

        if(inputRedirect !=0 && inputFilePATH[0] == '~'){
            char temp[PATH_LENGTH_MAX] = {0};
            strcpy(temp, rootDir);
            strcat(temp, inputFilePATH + 1);
            strcpy(inputFilePATH, temp);
        }
        if(outputRedirect !=0 && outputFilePATH[0] == '~'){
            char temp[PATH_LENGTH_MAX] = {0};
            strcpy(temp, rootDir);
            strcat(temp, outputFilePATH + 1);
            strcpy(outputFilePATH, temp);
        }

        if(inputRedirect != 0 || outputRedirect != 0){
            int fd1 = -1, fd2 = -1;
            if(inputRedirect == 1 && outputRedirect != 0){
                fd1 = open(inputFilePATH, O_RDONLY);
                if(fd1 == -1){
                    printColor("ERROR : Input file does not exist.", RED);
                    return IOREDIRECT_NORMAL_FAILURE;
                }
                if(outputRedirect == 1){
                    fd2 = open(outputFilePATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fd2 == -1){
                        printColor("ERROR : Output file could not be created.", RED);
                        return IOREDIRECT_NORMAL_FAILURE;
                    }
                }
                else if(outputRedirect == 2){
                    fd2 = open(outputFilePATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if(fd2 == -1){
                        printColor("ERROR : Output file could not be created.", RED);
                        return IOREDIRECT_NORMAL_FAILURE;
                    }
                }
                int rc = dup2(fd1, STDIN_FILENO);
                if(rc == -1){
                    printColor("ERROR : Duplication failed.", RED);
                    return IOREDIRECT_SYSCALL_FAILURE;
                }
                rc = dup2(fd2, STDOUT_FILENO);
                if(rc == -1){
                    printColor("ERROR : Duplication failed.", RED);
                    dup2(stdin_copy, STDIN_FILENO);
                    return IOREDIRECT_SYSCALL_FAILURE;
                }            
            }   
            else if(inputRedirect != 0 && outputRedirect == 0){
                int fd1 = open(inputFilePATH, O_RDONLY);
                if(fd1 == -1){
                    printColor("ERROR : Input file does not exist.", RED);
                    dup2(stdout_copy, STDOUT_FILENO);
                    dup2(stdin_copy, STDIN_FILENO);
                    return IOREDIRECT_NORMAL_FAILURE;
                }
                int rc = dup2(fd1, STDIN_FILENO);
                if(rc == -1){
                    printColor("ERROR : Duplication failed.", RED);
                    dup2(stdout_copy, STDOUT_FILENO);
                    dup2(stdin_copy, STDIN_FILENO);
                    return IOREDIRECT_SYSCALL_FAILURE;
                }
            }
            else if(inputRedirect == 0 && outputRedirect != 0){
                int fd2;
                if(outputRedirect == 1){
                    fd2 = open(outputFilePATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fd2 == -1){
                        printColor("ERROR : Output file could not be created.", RED);
                        dup2(stdout_copy, STDOUT_FILENO);
                        dup2(stdin_copy, STDIN_FILENO);
                        return IOREDIRECT_NORMAL_FAILURE;
                    }
                }
                else{
                    fd2 = open(outputFilePATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if(fd2 == -1){
                        printColor("ERROR : Output file could not be created.", RED);
                        dup2(stdout_copy, STDOUT_FILENO);
                        dup2(stdin_copy, STDIN_FILENO);
                        return IOREDIRECT_NORMAL_FAILURE;
                    }
                }
                int rc = dup2(fd2, STDOUT_FILENO);
                if(rc == -1){
                    printColor("ERROR : Duplication failed.", RED);
                    dup2(stdin_copy, STDIN_FILENO);
                    dup2(stdout_copy, STDOUT_FILENO);
                    return IOREDIRECT_SYSCALL_FAILURE;
                }
            }
        }
    return IOREDIRECT_SUCCESS;
}
