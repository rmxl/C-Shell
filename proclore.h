#ifndef PROCLORE_H
#define PROCLORE_H

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

void proclore(int id);    

#endif
