#include "./neonate.h"

struct termios original_mode;

int printMostRecentProc(){
    // /proc/pid/stat has a entry which tells the time when the process was started in unix timestamp
    char path[] = "/proc";
    DIR* d = opendir(path);
    if(d == NULL){
        printColor("ERROR : opendir failed.", RED);
        return -1;
    }
    struct dirent* dirent;
    unsigned long long maxTime = 1;
    int pid = -1;
    while((dirent = readdir(d)) != NULL){
        if(strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0){
            continue;
        }
        char fileToRead[PATH_LENGTH_MAX] = {0};
        strcpy(fileToRead, "/proc/");
        strcat(fileToRead, dirent->d_name);
        strcat(fileToRead, "/stat");
        FILE* file = fopen(fileToRead, "r");
        if(file == NULL){
            continue;
        }
        char buffer[1024] = {0};
        if(fgets(buffer, 1024, file) == NULL){
            fclose(file);
            continue;
        }
        char* token = strtok(buffer, " ");
        for(int i = 0; i < 21; i++){//22 is the time entry
            token = strtok(NULL, " ");
        }
        unsigned long long val = 0;
        if(sscanf(token, "%llu", &val) != 1){
            fclose(file);
            continue;
        }
        if(val > maxTime){
            pid = (int) strtol(dirent->d_name, NULL, 10);
            if(pid == 0){
                fclose(file);
                continue;
            }
            maxTime = val;
        }
        
        fclose(file);
    }
    closedir(d);
    return pid;
}

int enableRawMode(){
    // we want to quit as soon as user types x so we need raw mode
    struct termios new;
    tcgetattr(STDIN_FILENO, &original_mode);
    new = original_mode;
    new.c_lflag &= ~(ECHO | ICANON);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);   
    return 0; 
}

int disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_mode);
    fcntl(STDIN_FILENO, F_SETFL, 0);
    return 0;
}

int neonate(int arg){
    if(arg == -1) arg = 5;
    enableRawMode();
    char c;
    while(1){
        time_t start = time(NULL);
        time_t end = start + arg;
        int pid = printMostRecentProc();
        if(pid == -1){
            disableRawMode();
            return 0;
        }
        printf("%d\n", pid);
        while(time(NULL) < end){
            if(read(STDIN_FILENO, &c, 1) == 1){
                if(c == 'x'){
                    disableRawMode();
                    return 0;
                }
            }
        }
    }
}
