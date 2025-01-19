#include "./activities.h"

int compareProcesses(const void *a, const void *b) {
    BgProcess *procA = (BgProcess *)a;
    BgProcess *procB = (BgProcess *)b;
    return strcmp(procA->command, procB->command);
}

void splitList(BgProcess* head, BgProcess** a, BgProcess** b){
    BgProcess* slow = head;
    BgProcess* fast = head->next;

    while(fast != NULL){
        fast = fast->next;
        if(fast != NULL){
            slow = slow->next;
            fast = fast->next;
        }
    }

    *a = head;
    *b = slow->next;
    slow->next = NULL;
}

BgProcess* merge(BgProcess* a, BgProcess* b){
    BgProcess* result = NULL;

    if(a == NULL) return b;
    if(b == NULL) return a;

    if(compareProcesses(a, b) < 0){
        result = a;
        result->next = merge(a->next, b);
    }
    else{
        result = b;
        result->next = merge(a, b->next);
    }

    return result;
}

void mergeSort(BgProcess **head){
    if(head == NULL) return;
    BgProcess* start = *head;
    BgProcess *a, *b;

    if(start == NULL || start->next == NULL) return;

    splitList(start, &a, &b);
    mergeSort(&a);
    mergeSort(&b);

    *head = merge(a, b);
}

void activities(BgProcess** head, int* bgProcNum){
    if(*head == NULL){
        printColor("No background processes to show.", RED);
        return;
    }

    mergeSort(head); // to print process lexicographically

    BgProcess* current = *head;
    while(current!=NULL){
        printf("%d : %s - %s\n", current->pid, current->command, current->isRunning ? "Running" : "Stopped");
        current = current->next;
    }
    return;
} 
