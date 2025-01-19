#ifndef PIPE_H
#define PIPE_H

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
#include "./linkedlist.h"
#include "./get_input.h"
#include "./commonheader.h"
#include "./execute.h"

typedef enum EXECUTEPIPE_ERROR_CODES{
    EXECUTEPIPE_SUCCESS,
    EXECUTEPIPE_CRITICAL_FAILURE,
    EXECUTEPIPE_NORMAL_FAILURE,
} EXECUTEPIPE_ERROR_CODES;

int executePipe(char *command, char *rootDir, char *prevDir, char *commandName, int *timeTakenByLastCommand, BgProcess **BgProcesses, int *bgProcNum, char *args[], int argsNum, int* ptr, int inputRedirect, int outputRedirect, char inputFilePATH[], char outputFilePATH[], int* backgroundProcess);

#endif
 