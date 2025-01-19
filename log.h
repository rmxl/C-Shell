#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "./color_print.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "./execute.h"
#include "./linkedlist.h"   
#include "./commonheader.h"

#define MAX_HISTORY 15

void logCommand(char* command, int mode, int idx, char* rootDir, char* prevDir, char* commandName, int* timeTakenByLastCommand, BgProcess** BgProcesses, int* bgProcNum);

#endif

