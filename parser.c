#include "./parser.h"

void skipSpaces(char* s, int* ptr) {
    while (s[*ptr] == ' ' || s[*ptr] == '\t') {
        (*ptr)++;
    }
}

int parseCommand(char* command, char* commandName, char* args[], int* backgroundProcess, int* argsNum, int* ptr, int* inputRedirect, int* outputRedirect, char* inputFilePATH, char* outputFilePATH, int* pipePresent) {
    int i = 0;
    
    skipSpaces(command, ptr);

    if (command[*ptr] == '\n' || command[*ptr] == '\0' || command[*ptr] == ';') {
        return -1;
    }

    while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0' ) {
        if(command[*ptr] == '|'){
            // pipe is first
            return -5;
        }

        else if(command[*ptr] == '>' || command[*ptr] == '<'){
            // redirection is first
            return -6;
        }

        commandName[i] = command[*ptr];
        i++;
        (*ptr)++;
    }
    commandName[i] = '\0';

    // if(strcmp("|", commandName) == 0) {
    //     // pipe is first
    //     return -5;
    // }

    // else if(strcmp(">", commandName) == 0 || strcmp(">>", commandName) == 0 || strcmp("<", commandName) == 0) {
    //     // redirection is first
    //     return -6;
    // }

    *argsNum = 0;
    int j;

    skipSpaces(command, ptr);


    if (command[*ptr] == '&') {
        *backgroundProcess = 1;
        (*ptr)++;
        skipSpaces(command, ptr);
        if(command[*ptr] == '|'){
            // *pipePresent = 1;
            // (*ptr)++;
            // skipSpaces(command, ptr);
            return -5;
        }
        return 0;
    }
    
    if(command[*ptr] == '<'){
        // input redirection
        (*ptr)++;
        skipSpaces(command, ptr);
        i = 0;
        while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
            
            if(command[*ptr] == '|'){
                // pipe is first
                return -5;
            }
            else if(command[*ptr] == '>' || command[*ptr] == '<'){
                // redirection is first
                return -6;
            }

            inputFilePATH[i] = command[*ptr];
            i++;
            (*ptr)++;
        }
        inputFilePATH[i] = '\0';
        *inputRedirect = 1;
        skipSpaces(command, ptr);
    }
    if(command[*ptr] == '>'){
        // output redirection
        if(command[*ptr + 1] == '>'){
            (*ptr)++;
            (*ptr)++;
            skipSpaces(command, ptr);
            i = 0;
            while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {

                if(command[*ptr] == '|'){
                    // pipe is first
                    return -5;
                }
                else if(command[*ptr] == '>' || command[*ptr] == '<'){
                    // redirection is first
                    return -6;
                }

                outputFilePATH[i] = command[*ptr];
                i++;
                (*ptr)++;
            }
            outputFilePATH[i] = '\0';
            *outputRedirect = 2;
            skipSpaces(command, ptr);
        }
        else{
            (*ptr)++;
            skipSpaces(command, ptr);
            i = 0;
            while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
                
                if(command[*ptr] == '|'){
                    // pipe is first
                    return -5;
                }
                else if(command[*ptr] == '>' || command[*ptr] == '<'){
                    // redirection is first
                    return -6;
                }

                outputFilePATH[i] = command[*ptr];
                i++;
                (*ptr)++;
            }
            outputFilePATH[i] = '\0';
            *outputRedirect = 1;
            skipSpaces(command, ptr);
        }
    }

    if(command[*ptr] == '|'){
        // pipe
        *pipePresent = 1;
        (*ptr)++;
        skipSpaces(command, ptr);
        return 0;
    }

    
    while (command[*ptr] != '\n' && command[*ptr] != '\0') {
        skipSpaces(command, ptr);

        if (command[*ptr] == '\n' || command[*ptr] == '\0') {
            break;
        }

        if(command[*ptr] == '<'){
            (*ptr)++;
            skipSpaces(command, ptr);
            i = 0;
            while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
                
                if(command[*ptr] == '|'){
                    // pipe is first
                    return -5;
                }
                else if(command[*ptr] == '>' || command[*ptr] == '<'){
                    // redirection is first
                    return -6;
                }

                inputFilePATH[i] = command[*ptr];
                i++;
                (*ptr)++;
            }
            inputFilePATH[i] = '\0';
            *inputRedirect = 1;
            skipSpaces(command, ptr);
        }
        if(command[*ptr] == '>'){
            if(command[*ptr + 1] == '>'){
                (*ptr)++;
                (*ptr)++;
                skipSpaces(command, ptr);
                i = 0;
                while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
                    
                    if(command[*ptr] == '|'){
                        // pipe is first
                        return -5;
                    }
                    else if(command[*ptr] == '>' || command[*ptr] == '<'){
                        // redirection is first
                        return -6;
                    }

                    outputFilePATH[i] = command[*ptr];
                    i++;
                    (*ptr)++;
                }
                outputFilePATH[i] = '\0';
                *outputRedirect = 2;
                skipSpaces(command, ptr);
            }
            else{
                (*ptr)++;
                skipSpaces(command, ptr);
                i = 0;
                while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
                    
                    if(command[*ptr] == '|'){
                        // pipe is first
                        return -5;
                    }
                    else if(command[*ptr] == '>' || command[*ptr] == '<'){
                        // redirection is first
                        return -6;
                    }

                    outputFilePATH[i] = command[*ptr];
                    i++;
                    (*ptr)++;
                }
                outputFilePATH[i] = '\0';
                *outputRedirect = 1;
                skipSpaces(command, ptr);
            }
        }



        if(command[*ptr] == '\n' || command[*ptr] == '\0'){
            break;
        }

        if (command[*ptr] == '&') {
            *backgroundProcess = 1;
            (*ptr)++;
            skipSpaces(command, ptr);
            if(command[*ptr] == '|'){
                return -5;
            }
            return 0;
        }

        if(command[*ptr] == '|'){
            *pipePresent = 1;
            (*ptr)++;
            skipSpaces(command, ptr);
            return 0;
        }

        j = 0;
        args[*argsNum] = (char*)malloc(INPUT_LENGTH_MAX);
        if (args[*argsNum] == NULL) {
            printColor("ERROR : Memory allocation failed.", RED);
            return -1;
        }

        while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0' && command[*ptr] != '&' && command[*ptr] != '<' && command[*ptr] != '>' && command[*ptr] != '|') {
            if(command[*ptr] == '\''){
                (*ptr)++;
                while(command[*ptr] != '\''){
                    args[*argsNum][j] = command[*ptr];
                    j++; (*ptr)++;
                }
                (*ptr)++;
                break;
            }
            if(command[*ptr] == '\"'){
                (*ptr)++;
                while(command[*ptr] != '\"'){
                    args[*argsNum][j] = command[*ptr];
                    j++; (*ptr)++;
                }
                (*ptr)++;
                break;
            }
            args[*argsNum][j] = command[*ptr];
            j++;
            (*ptr)++;
        }
        args[*argsNum][j] = '\0';
        (*argsNum)++;
        skipSpaces(command, ptr);
        if (command[*ptr] == '&') {
            *backgroundProcess = 1;
            (*ptr)++;
            skipSpaces(command, ptr);
            if(command[*ptr] == '|'){
                return -5;
            }
            return 0;
        }
        if(command[*ptr] == '<'){
            (*ptr)++;
            skipSpaces(command, ptr);
            i = 0;
            while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
                
                if(command[*ptr] == '|'){
                    // pipe is first
                    return -5;
                }
                else if(command[*ptr] == '>' || command[*ptr] == '<'){
                    // redirection is first
                    return -6;
                }

                inputFilePATH[i] = command[*ptr];
                i++;
                (*ptr)++;
            }
            inputFilePATH[i] = '\0';
            *inputRedirect = 1;
            skipSpaces(command, ptr);
        }
        
        if(command[*ptr] == '>'){
            if(command[*ptr + 1] == '>'){
                (*ptr)++;
                (*ptr)++;
                skipSpaces(command, ptr);
                i = 0;
                while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
                    
                    if(command[*ptr] == '|'){
                        // pipe is first
                        return -5;
                    }
                    else if(command[*ptr] == '>' || command[*ptr] == '<'){
                        // redirection is first
                        return -6;
                    }

                    outputFilePATH[i] = command[*ptr];
                    i++;
                    (*ptr)++;
                }
                outputFilePATH[i] = '\0';
                *outputRedirect = 2;
                skipSpaces(command, ptr);
            }
            else{
                (*ptr)++;
                skipSpaces(command, ptr);
                i = 0;
                while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {

                    if(command[*ptr] == '|'){
                        // pipe is first
                        return -5;
                    }
                    else if(command[*ptr] == '>' || command[*ptr] == '<'){
                        // redirection is first
                        return -6;
                    }

                    outputFilePATH[i] = command[*ptr];
                    i++;
                    (*ptr)++;
                }
                outputFilePATH[i] = '\0';
                *outputRedirect = 1;
                skipSpaces(command, ptr);
            }
        }
        if(command[*ptr] == '|'){
            *pipePresent = 1;
            (*ptr)++;
            skipSpaces(command, ptr);
            return 0;
        }
    }
    return 0;
}
