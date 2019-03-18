#include "stdlib.h"
#include "xargs.h"

int main(int argc, char* argv[]) {
    char* CMD = malloc(255 * sizeof(char));
    char** ARGV = NULL;
    int ARGC = NULL;

    CMD = "echo";
    if (argc > 1) CMD = argv[1];
    if (argc > 2) {
        ARGV = malloc(ARG_MAX * sizeof(char));
        ARGC = argc - 2;
        for (int i = 0; i < ARGC; i++) {
            ARGV[i] = argv[i+2];
        }
    }

    char** IN_arr = malloc(ARG_MAX * sizeof(char));
    int IN_count = readSTDIN(IN_arr);

    char* UNIX_ARG = (char*) malloc(sizeof(char) * (getSize(IN_arr, IN_count) * 2));
    prepareARGS(UNIX_ARG, IN_arr, IN_count);

    char* UNIX_CMD = (char*) malloc(sizeof(char) * ARG_MAX * 2);

    strcpy(UNIX_CMD, CMD);

    unsigned int CMD_len = strlen(CMD);
    UNIX_CMD[CMD_len] = ' ';
    CMD_len++;
    for (int i = 0; i < ARGC; i++) {
        for(unsigned int j = 0; j < strlen(ARGV[i]); j++) {
            UNIX_CMD[CMD_len] = '\\';
            CMD_len++;
            UNIX_CMD[CMD_len] = ARGV[i][j];
            CMD_len++;
        }
        UNIX_CMD[CMD_len] = ' ';
        CMD_len++;
    }
    strcat(UNIX_CMD, " ");
    strcat(UNIX_CMD, UNIX_ARG);

    system(UNIX_CMD);

    free(UNIX_ARG);
    free(IN_arr);
    free(ARGV);
    free(CMD);
    return 0;
}
