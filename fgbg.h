#ifndef FGBG_H
#define FGBG_H

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
#include "./get_input.h"

int fgbg(BgProcess** BgProcesses,  int* bgProcNum, int mode, int pid, int* timeTakenByLastCommand);

#endif
 