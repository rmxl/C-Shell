#ifndef CUSTOM_H
#define CUSTOM_H

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
#include "./commonheader.h" 
#include "./execute.h"

#define MAX_ALIAS_LENGTH 100

int customCommands(char* inputCommand, char* args[], int argsNum, char* rootDir, char* prevDir, int* timeTakenByLastCommand, BgProcess** BgProcesses, int* bgProcNum);

#endif
