#ifndef PING_H
#define PING_H

#include "./exec.h"
#include <signal.h>
#include <stdio.h>
#include <signal.h>
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
#include "./customCommands.h"
#include "./activities.h"

int ping(int pid, int signal);

#endif
