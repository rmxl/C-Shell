#include "./reveal.h"

int compareSize(const void *const A, const void *const B)
{
    return strcmp((*(struct dirent **) A)->d_name, (*(struct dirent **) B)->d_name);
}

int printInColor(char* path, struct dirent* dirent, int allInfoShow){
    struct stat fileStat;
    char fullPath[PATH_LENGTH_MAX] = {0};
    sprintf(fullPath, "%s/%s", path, dirent->d_name);
    if (lstat(fullPath, &fileStat) == -1) {
        printColor("ERROR : lstat failed.", RED);
        return -1;
    }

    if(allInfoShow == 0){
        if(S_ISDIR(fileStat.st_mode)){
            printColor(dirent->d_name, BLU);
        }
        else if(S_ISREG(fileStat.st_mode)){
            if(S_IXUSR & fileStat.st_mode) {
                printColor(dirent->d_name, GRN);
            } 
            else {
                printColor(dirent->d_name, WHT);
            }
        }
        else{
            printColor(dirent->d_name, WHT);
        }
    }

    else{
        if(S_ISLNK(fileStat.st_mode)) printf(";");
        else if(S_ISDIR(fileStat.st_mode)) printf("d");
        else printf("-");

        printf("%c", (fileStat.st_mode & S_IRUSR) ? 'r' : '-');
        printf("%c", (fileStat.st_mode & S_IWUSR) ? 'w' : '-');
        printf("%c", (fileStat.st_mode & S_IXUSR) ? 'x' : '-');

        printf("%c", (fileStat.st_mode & S_IRGRP) ? 'r' : '-');
        printf("%c", (fileStat.st_mode & S_IWGRP) ? 'w' : '-');
        printf("%c", (fileStat.st_mode & S_IXGRP) ? 'x' : '-');

        printf("%c", (fileStat.st_mode & S_IROTH) ? 'r' : '-');
        printf("%c", (fileStat.st_mode & S_IWOTH) ? 'w' : '-');
        printf("%c", (fileStat.st_mode & S_IXOTH) ? 'x' : '-');

        printf(". ");
        printf("%ld ", fileStat.st_nlink);

        struct passwd* passwd = getpwuid(fileStat.st_uid);
        printf("%s ", passwd->pw_name);

        struct group* group = getgrgid(fileStat.st_gid);
        printf("%s ", group->gr_name);

        printf("%ld ", fileStat.st_size);

        struct tm* tm = localtime(&fileStat.st_mtime);
        if(tm == NULL){
            printColor("ERROR : Time conversion failed.", RED);
            return -1;
        }
        char timestr[128];
        int ch = strftime(timestr, sizeof(timestr), "%c", tm);
        if(ch == 0){
            printColor("ERROR : Time string formatting failed.", RED);
            return -1;
        }
        printf("%s ", timestr);

        if(S_ISDIR(fileStat.st_mode)){
            printColor(dirent->d_name, BLU);
        }
        else if(S_ISREG(fileStat.st_mode)){
            if(S_IXUSR & fileStat.st_mode) {
                printColor(dirent->d_name, GRN);
            } 
            else {
                printColor(dirent->d_name, WHT);
            }
        }
        else{
            printColor(dirent->d_name, WHT);
        }
    }
    return 0;
}

int reveal(char** args, int argsNum, char* rootDir, char* prevDir){
    int allFilesShow = 0;
    int allInfoShow = 0;
    int i = 0;
    for(; i < argsNum; i++){
        char c;
        c = args[i][0];
        if(c == '-' && strlen(args[i]) > 1){   
            for(int j = 1; j < strlen(args[i]); j++){
                c = args[i][j];
                if(c == 'a'){
                    allFilesShow = 1;
                }
                else if(c == 'l'){
                    allInfoShow = 1;
                }
                else{
                    printColor("ERROR : Invalid argument.", RED);
                    return -1;
                }
            }
        }
        else break;
    }
    char path[PATH_LENGTH_MAX] = {0};
    if(i == argsNum){
        getcwd(path, PATH_LENGTH_MAX);
    }
    else{
        strcpy(path, args[argsNum - 1]);
    }

    if(path[0] == '~'){
        char temp[PATH_LENGTH_MAX];
        strcpy(temp, rootDir);
        strcat(temp, path + 1);
        strcpy(path, temp);
    }
    else if(strcmp(path, "-") == 0){
        if(strlen(prevDir) == 0){
            printColor("ERROR : No previous directory established yet.", RED);
            return -1;
        }
        strcpy(path, prevDir);
    }

    DIR* d = opendir(path);
    if(d == NULL){
        switch(errno){
            case ENAMETOOLONG:
                printColor("ERROR : Path name is too long.", RED);
                break;
            case EACCES:
                printColor("ERROR : Permission denied.", RED);
                break;
            case ENOENT:
                printColor("ERROR : No such file or directory.", RED);
                break;
            case ENOTDIR:
                printColor("ERROR : Not a directory.", RED);
                break;
            default:
                printColor("ERROR : Unknown error.", RED);
                break;
        }
        return -1;
    }

    int count = 0;
    struct dirent** list;
    struct dirent* dirent;
    while((dirent = readdir(d)) != NULL){
        count++;
    }

    list = (struct dirent**) malloc(count * sizeof(struct dirent*));
    if(list == NULL){
        printColor("ERROR : Memory allocation failed.", RED);
        return -1;
    }

    rewinddir(d);

    count = 0;
    while((dirent = readdir(d)) != NULL){
        list[count] = dirent;
        count++;
    }

    qsort(list, count, sizeof(*list), compareSize);

    for(int i = 0; i < count; i++){
        if(allFilesShow == 1){
            int ec = printInColor(path, list[i], allInfoShow);
            if(ec == -1) return -1;
        }
        else{
            if(list[i]->d_name[0] != '.'){
                int ec = printInColor(path, list[i], allInfoShow);
                if(ec == -1) return -1;
            }
        }
    }

    closedir(d);
    free(list);

    return 0;
}
