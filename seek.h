#ifndef SEEK_H
#define SEEK_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "./color_print.h"
#include <dirent.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "./commonheader.h"

int seek(char** args, int argsNum, char* rootDir, char* prevDir);

#endif
