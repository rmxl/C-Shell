#include "./linkedlist.h"

BgProcess* addBgProcess(BgProcess* head, int pid, char* command, int isRunning, int bgInvocationNum){
    if(head == NULL){
        BgProcess* newBgProcess = (BgProcess*) malloc(sizeof(BgProcess));
        newBgProcess->pid = pid;
        newBgProcess->command = strdup(command);
        newBgProcess->next = NULL;
        newBgProcess->isRunning = isRunning;
        newBgProcess->bgInvocationNum = bgInvocationNum;
        return newBgProcess;
    }

    BgProcess* current = head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = (BgProcess*) malloc(sizeof(BgProcess));
    current->next->pid = pid;
    current->next->command = strdup(command);
    current->next->isRunning = isRunning;
    current->next->bgInvocationNum = bgInvocationNum;
    current->next->next = NULL;

    return head;
}

BgProcess* removeBgProcess(BgProcess* head, int pid){
    BgProcess* current = head;
    BgProcess* prev = NULL;
    while(current != NULL){
        if(current->pid == pid){

            if(prev == NULL){
                if(current->next == NULL){
                    free(current->command);
                    free(current);
                    return NULL;
                }
                head = current->next;
            }
            else{
                prev->next = current->next;
            }
            free(current->command);
            free(current);
            return head;
        }
        prev = current;
        current = current->next;
    } 
    return head;
}

BgProcess* getProcess(BgProcess* head, int pid){
    BgProcess* current = head;
    while(current != NULL){
        if(current->pid == pid){
            return current;
        }
        current = current->next;
    }
    return NULL;
}
