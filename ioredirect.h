#ifndef IO_H
#define IO_H

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

typedef enum IOREDIRECT_ERROR_CODES{
    IOREDIRECT_SUCCESS,
    IOREDIRECT_SYSCALL_FAILURE,
    IOREDIRECT_NORMAL_FAILURE,
} IOREDIRECT_ERROR_CODES;

int IORedirect(int inputRedirect, int outputRedirect, char *inputFilePATH, char *outputFilePATH, char *rootDir);

#endif
 