#include "./color_print.h"

void printColor(char* string, char* color){
    dprintf(stdout_copy, "%s%s%s\n", color, string, RESET);
}
