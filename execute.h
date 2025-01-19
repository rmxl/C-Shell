#ifndef EXECUTE_H
#define EXECUTE_H

#include "./exec.h"
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "./color_print.h"
#include "./shell_prompt.h"
#include "./parser.h"
#include "./hop.h"
#include "./reveal.h"
#include "./seek.h"
#include "./log.h"
#include "./proclore.h"
#include "./linkedlist.h"
#include "./commonheader.h"
#include "./customCommands.h"
#include "./activities.h"
#include "./ping.h"
#include "./fgbg.h"
#include "./iman.h"
#include "./neonate.h"
#include "./get_input.h"
#include "./ioredirect.h"
#include "./pipe.h"

typedef enum EXECUTESUBCOMMAND_ERROR_CODES{
    EXECUTESUBCOMMAND_SUCCESS,
    EXECUTESUBCOMMAND_CRITICAL_FAILURE,
    EXECUTESUBCOMMAND_NORMAL_FAILURE,
    EXECUTESUBCOMMAND_EXIT
} EXECUTESUBCOMMAND_ERROR_CODES;

int executeCommand(char* inputString, char* rootDir, char* prevDir, char* commandName, int* timeTakenByLastCommand,BgProcess** BgProcesses, int* bgProcNum);

int executeSubCommand(char* command, char *rootDir, char *prevDir, char *commandName, int *timeTakenByLastCommand, BgProcess **BgProcesses, int *bgProcNum, char *args[], int argsNum, int* checkIfCommandHasLog, int* backgroundProcess);

#endif
 