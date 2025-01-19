#ifndef HOP_H
#define HOP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "./color_print.h"
#include "./commonheader.h"

int hop(char* str, char* rootDir, char* prevDir);

#endif
