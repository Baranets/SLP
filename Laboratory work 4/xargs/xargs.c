#include "xargs.h"


// Выводит справку о команде
void printHelp() {
    printf("Usage: xargs [cmd [args ...]]\n");
}

// Читаем STDIN  и возвращаем кол-во строк
int readSTDIN(char* stdin_lines[]) {
    char buffer[ARG_MAX];
    read(XARGS_STDIN, buffer, ARG_MAX);
    int count = 1;
    char* tmp = strtok(buffer, "\n");
    stdin_lines[0] = tmp;
    while (tmp != NULL) {
        tmp = strtok(0, "\n");
        stdin_lines[count] = tmp;
        count++;
    }
    return count;
}

void prepareARGS(char* UNIX_ARG, char* IN_arr[], int IN_count) {
    int count = 0;

    for (int i = 0; i < IN_count - 1; i++) {
        for (int j = 0; j < (int) strlen(IN_arr[i]); j++) {
            UNIX_ARG[count] = '\\';
            count++;
            UNIX_ARG[count] = IN_arr[i][j];
            count++;
        }    
    }
}

int getSize(char* IN_arr[], int IN_count) {
    int size = 0;
    for (int i = 0; i < IN_count - 1; i++) {
        size += (int) strlen(IN_arr[i]);
    }
    return size;
}
