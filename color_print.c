#include "./color_print.h"

void printColor(char* string, char* color){
    printf("%s%s%s\n", color, string, RESET);
}
