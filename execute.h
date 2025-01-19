#ifndef EXECUTE_H
#define EXECUTE_H

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

int executeCommand(char* inputString, char* rootDir, char* prevDir, char* commandName, int* timeTakenByLastCommand,BgProcess** BgProcesses, int* bgProcNum);

#endif
