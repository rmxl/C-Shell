#include "./customCommands.h"

int customCommands(char* inputCommand, char* args[], int argsNum, char* rootDir, char* prevDir, int* timeTakenByLastCommand, BgProcess** BgProcesses, int* bgProcNum){
    char configFilePath[PATH_LENGTH_MAX] = {0};
    sprintf(configFilePath, "%s/.myshrc", rootDir);
    FILE* fp = fopen(configFilePath, "r");
    if(fp == NULL){
        return -1;
    }
    int aliasCommandLen = 5;
    int match = 0;
    while(!feof(fp)){
            char lineFromConfigFile[INPUT_LENGTH_MAX] = {0};
            fgets(lineFromConfigFile, INPUT_LENGTH_MAX, fp);
            char* saveptr;
            char tempLine[INPUT_LENGTH_MAX];
            strcpy(tempLine, lineFromConfigFile);
            char* firstWordOfLine = strtok_r(tempLine, " ", &saveptr);
            if(firstWordOfLine == NULL){
                fclose(fp);
                if(match == 1) return 0;
                return -1;
            }
            if(firstWordOfLine[0] == '#'){
                // comment
                continue;
            }
            
            else if(strcmp(firstWordOfLine, "alias") == 0){
                //check for the aliased command
                char* aliasName = strtok_r(NULL, " ", &saveptr);
                if(strcmp(aliasName, inputCommand) == 0){
                    match = 1;
                    char* equalSign = strtok_r(NULL, " ", &saveptr);
                    if(equalSign != NULL && strcmp(equalSign, "=") == 0){
                        // equal sign is present
                        char* newCommand = strtok_r(NULL, " ", &saveptr);
                        if(newCommand == NULL){
                            fclose(fp);
                            return -3;
                        }
                        int commandStartPos = aliasCommandLen + 1 + strlen(inputCommand) + 1 + 1 + 1; // alias + space + aliasName + space + = + space

                        int ec = executeCommand(lineFromConfigFile + commandStartPos, rootDir, prevDir, inputCommand, timeTakenByLastCommand, BgProcesses, bgProcNum); 
                        if(ec == 0) {
                            fclose(fp);
                            return 0;
                        }
                        else {
                            fclose(fp);
                            return -2;
                        }
                }
                else {
                    // no equal sign
                    fclose(fp);
                    return -3;
                }
            }
        }
        else {
            if(firstWordOfLine[strlen(firstWordOfLine) - 1] != '\n'){
                // add a new line character
                int x = strlen(firstWordOfLine);    
                firstWordOfLine[x] = '\n';
                firstWordOfLine[x+1] = '\0';                
            }
            if(firstWordOfLine[strlen(firstWordOfLine) - 2] == ')' && firstWordOfLine[strlen(firstWordOfLine) - 3] == '('){
                // this is a custom function
                char temp1[INPUT_LENGTH_MAX+2] = {0};
                strcpy(temp1, inputCommand);
                strcat(temp1, "()\n");
                if(strcmp(temp1, firstWordOfLine) != 0){
                    // no match
                    continue;   
                }
                match = 1;
                if(fgets(lineFromConfigFile, INPUT_LENGTH_MAX, fp) == NULL){
                    fclose(fp);
                    return -3;
                }
                char funcCommands[INPUT_LENGTH_MAX] = {0};
                fgets(funcCommands, INPUT_LENGTH_MAX, fp);
                while(strcmp(funcCommands, "}\n") != 0){
                    //execute each command inside the function
                    char funcCommandActual[INPUT_LENGTH_MAX] = {0};
                    for(int i = 0, k = 0; i < strlen(funcCommands); i++){
                        if(funcCommands[i] == '\"'){
                            // command line argument
                            i++;
                            if(funcCommands[i] == '$'){
                                i++;
                                int j = 0;
                                char number[INPUT_LENGTH_MAX] = {0};
                                while(funcCommands[i] != '\"' && funcCommands[i] != '\n'){
                                    number[j] = funcCommands[i];
                                    i++;
                                    j++;
                                }
                                long num = strtol(number, NULL, 10);
                                if(num == 0){
                                    printColor("ERROR : Argument number is invalid.", RED);
                                    fclose(fp);
                                    return -4;
                                }
                                if(num > argsNum){
                                    printColor("ERROR: Argument number is out of bounds.", RED);
                                    fclose(fp);
                                    return -4;
                                }
                                strcat(funcCommandActual, args[num - 1]); 
                                k += strlen(args[num-1]);
                            } 
                        }
                        else{
                            //normal
                            funcCommandActual[k] = funcCommands[i];
                            k++;
                        }
                    }
                    int ec = executeCommand(funcCommandActual, rootDir, prevDir, inputCommand, timeTakenByLastCommand, BgProcesses, bgProcNum);
                    if(ec == 0) {
                        if(fgets(funcCommands, INPUT_LENGTH_MAX, fp) == NULL){
                            if(match == 1) return 0;
                            else return -1;
                        }
                        continue;
                    }
                    else{
                        fclose(fp);
                        return -2;
                    }
                }
            }
            else continue;
        }

    }
    fclose(fp);
    if(match == 0) return -1;
    else return 0;
}
