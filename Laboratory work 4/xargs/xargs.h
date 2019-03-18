#ifndef LAB4_XARGS_H
#define LAB4_XARGS_H

#define ARG_MAX 2048
#define XARGS_STDIN 0
#define XARGS_STDOUT 1
#define XARGS_STDERR 2

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

void printHelp();
int readSTDIN(char* stdin_lines[]);
void prepareARGS(char* UNIX_ARG, char* IN_arr[], int IN_count);
int getSize(char* IN_arr[], int IN_count);

#endif //LAB4_XARGS_H