#ifndef EXEC_H
#define EXEC_H

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

int execCommand(char* commandName, int* timeTakenByLastCommand, BgProcess** BgProcesses, int* bgProcNum, char** args, int argsNum, int backgroundProcess);

#endif
 