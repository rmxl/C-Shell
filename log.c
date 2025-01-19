#include "./log.h"
#include <string.h>

typedef enum logCommands{
    logStore,
    logPrint,
    logPurge,
    logExecute
} logCommands;


int checkLength(FILE* file) {
    int count = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    return count;
}

int isLastCommandSame(FILE* file, const char* command) {
    char theLastCommand[INPUT_LENGTH_MAX] = {0};
    char c;
    int pos = 0;
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long i;
    for (i = fileSize - 2; i >= 0; i--) {
        fseek(file, i, SEEK_SET);
        c = fgetc(file);
        if (c == '\n') {
            break;
        }
    }

    while ((i++ == -1)||((c = fgetc(file)) != EOF && c != '\n')) {
        theLastCommand[pos++] = c;
    }
    theLastCommand[pos] = '\n';
    theLastCommand[pos + 1] = '\0';

    return strcmp(theLastCommand, command) == 0;
}

void logCommand(char* command, int mode, int idx, char* rootDir, char* prevDir, char* commandName, int* timeTakenByLastCommand, BgProcess** BgProcesses, int* bgProcNum) {
    char user[INPUT_LENGTH_MAX];
    getlogin_r(user, INPUT_LENGTH_MAX);
    char historyFile[PATH_LENGTH_MAX];
    char tempFile[PATH_LENGTH_MAX];
    sprintf(historyFile, "/home/%s/.myShellHistory", user);
    sprintf(tempFile, "/home/%s/.myShellTemp", user);

    logCommands cm = mode;

    if (cm == logStore) {  
        // a command was executed other than log
        FILE* file = fopen(historyFile, "r");
        if(file == NULL){
            file = fopen(historyFile, "w");
            fclose(file);
            file = fopen(historyFile, "r");
        }
        if (checkLength(file) == MAX_HISTORY) {
            //if the history file has reached its maximum limit
            if(isLastCommandSame(file, command)){
                fclose(file);
                return;
            }
            fclose(file);
            file = fopen(historyFile, "r");
            FILE* temp = fopen(tempFile, "w");
            if(temp == NULL){
                printColor("ERROR : Error in creating temp file.", RED);
                return;
            }
            char c;

            while ((c = fgetc(file)) != '\n');

            while ((c = fgetc(file)) != EOF) {
                fputc(c, temp);
            }

            fclose(file);
            remove(historyFile);
            rename(tempFile, historyFile);


            if(command[strlen(command) - 1] != '\n'){
                fprintf(temp, "%s\n", command);
            }
            else fprintf(temp, "%s", command);
            fclose(temp);
            
        } else {
            // space is there
            rewind(file);
            if (!isLastCommandSame(file, command)) {
                FILE* appendFile = fopen(historyFile, "a");
                if(appendFile == NULL){
                    printColor("ERROR : Error in appending to history file.", RED);
                    return;
                } 

                if(command[strlen(command) - 1] != '\n'){
                    fprintf(appendFile, "%s\n", command);
                }
                else fprintf(appendFile, "%s", command);
                fclose(appendFile);
            }
            fclose(file);
        }
    }
    else if(cm == logPrint){
        /// log print
        FILE* file = fopen(historyFile, "r");
        if(file == NULL){
            printColor("ERROR : No history exists.", RED);
            return;
        }
        char c = fgetc(file);
        while(!feof(file)){
            printf("%c", c);
            c = fgetc(file);
        }
        fclose(file);
    }
    else if(cm == logPurge){
        // empty log file
        FILE* file = fopen(historyFile, "w");
        fclose(file);
    }
    else if (cm == logExecute){
        // execute a command from history
        char command[PATH_LENGTH_MAX];
        FILE* file = fopen(historyFile, "r");
        if(file == NULL){
            printColor("ERROR : No history exists.", RED);
            return;
        }
        char c;
        int len = checkLength(file);
        if(idx <= 0 || idx > len){
            printColor("ERROR : No such command exists.", RED);
            fclose(file);
            return;
        }
        rewind(file);
        for(int i = len - idx; i > 0; i--){
            while((c = fgetc(file)) != EOF && c != '\n');
        }
        if(feof(file)){
            printColor("ERROR : No such command exists.", RED);
            fclose(file);
            return;
        }
        char cmd[PATH_LENGTH_MAX];
        int pos = 0;
        while((c = fgetc(file)) != '\n'){
            cmd[pos++] = c;
        }
        cmd[pos] = '\n';
        cmd[pos + 1] = '\0';  
        fclose(file);
        strcpy(command, cmd);
        executeCommand(command, rootDir, prevDir, commandName, timeTakenByLastCommand, BgProcesses, bgProcNum);;
        return;
    }
}
