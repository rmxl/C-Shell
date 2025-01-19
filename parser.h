#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./color_print.h"
#include "./commonheader.h"

void skipSpaces(char* s, int* ptr);
int parseCommand(char* command, char* commandName, char* args[], int* backgroundProcess, int* argsNum, int* ptr, int* inputRedirect, int* outputRedirect, char* inputFilePATH, char* outputFilePATH, int* pipePresent);

#endif
