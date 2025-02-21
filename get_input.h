#ifndef GET_INPUT_H
#define GET_INPUT_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "./color_print.h"
#include "./shell_prompt.h"
#include "./parser.h"
#include "./hop.h"
#include "./reveal.h"
#include "./seek.h"
#include "./log.h"
#include "./execute.h"
#include "./linkedlist.h"
#include "./commonheader.h"

extern int fgPID;
extern int stdin_copy, stdout_copy;
extern char fgProcName[INPUT_LENGTH_MAX];

#endif
