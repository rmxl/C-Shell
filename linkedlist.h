#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define INPUT_LENGTH_MAX 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BgProcess{
    int pid;
    char* command;
    int isRunning;
    int bgInvocationNum;
    struct BgProcess *next;
} BgProcess;

BgProcess* addBgProcess(BgProcess* head, int pid, char* command, int isRunning, int bgInvocationNum);
BgProcess* removeBgProcess(BgProcess* head, int pid);
BgProcess* getProcess(BgProcess* head, int pid);

#endif
