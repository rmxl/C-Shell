#include "./ping.h"

int ping(int pid, int signal){
    signal = signal % 32;
    int rc = kill(pid, signal);
    if(rc == 0){
        printf("Sent signal %d to process with pid %d\n", signal, pid);
        return 0;
    }
    else{
        switch(errno){
            case EPERM:
                printColor("ERROR : Permission denied", RED);
                break;
            case ESRCH:
                printColor("ERROR : No such process exists.", RED);
                break;
            case EINVAL:
                printColor("ERROR : Invalid signal", RED);
                break;
            default:
                printColor("ERROR : Unknown error", RED);
                break;
        }
        return -1;
    }
}
