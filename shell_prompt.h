#ifndef SHELL_PROMPT_H
#define SHELL_PROMPT_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "./color_print.h"
#include "./commonheader.h"

#define HOST_LENGTH_MAX 256

void prompt(char* rootDir, int timeTakenByLastCommand,char* lastCommand);

#endif
