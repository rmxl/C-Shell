#include "./parser.h"

void skipSpaces(char* s, int* ptr) {
    while (s[*ptr] == ' ' || s[*ptr] == '\t') {
        (*ptr)++;
    }
}

int parseCommand(char* command, char* commandName, char* args[], int* backgroundProcess, int* argsNum, int* ptr) {
    int i = 0;
    
    skipSpaces(command, ptr);

    if (command[*ptr] == '\n' || command[*ptr] == '\0') {
        return -1;
    }

    while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0') {
        commandName[i] = command[*ptr];
        i++;
        (*ptr)++;
    }
    commandName[i] = '\0';

    *argsNum = 0;
    int j;

    if (command[*ptr] == '&') {
        *backgroundProcess = 1;
        (*ptr)++;
        skipSpaces(command, ptr);
        return 0;
    }

    
    while (command[*ptr] != '\n' && command[*ptr] != '\0') {
        skipSpaces(command, ptr);

        if (command[*ptr] == '\n' || command[*ptr] == '\0') {
            break;
        }

        if (command[*ptr] == '&') {
            *backgroundProcess = 1;
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

        while (command[*ptr] != ' ' && command[*ptr] != '\t' && command[*ptr] != '\n' && command[*ptr] != '\0' && command[*ptr] != '&') {
            args[*argsNum][j] = command[*ptr];
            j++;
            (*ptr)++;
        }
        args[*argsNum][j] = '\0';
        (*argsNum)++;
        if (command[*ptr] == '&') {
            *backgroundProcess = 1;
            (*ptr)++;
            skipSpaces(command, ptr);
            return 0;
        }
    }
    return 0;
}
