#ifndef NEONATE_H
#define NEONATE_H

#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <dirent.h>
#include "./color_print.h"
#include "./commonheader.h"

int neonate(int time);

#endif
