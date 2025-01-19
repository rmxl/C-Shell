#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define INPUT_LENGTH_MAX 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BgProcess{
    int pid;
    char* command;
    struct BgProcess *next;
} BgProcess;

BgProcess* addBgProcess(BgProcess* head, int pid, char* command);
BgProcess* removeBgProcess(BgProcess* head, int pid);

#endif
