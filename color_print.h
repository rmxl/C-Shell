#ifndef COLOR_PRINT_H
#define COLOR_PRINT_H

#include <stdio.h>
#include "./commonheader.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define HBLU  "\e[1;94m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define HCYN  "\e[1;36m"

void printColor(char* color, char* string);

#endif
