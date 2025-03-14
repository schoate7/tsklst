#ifndef common_h
#define common_h
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

char* getString(char prompt[]);

char getChar(char prompt[]);

int getInt(char prompt[]);

float getFloat(char prompt[]);

void printCommandList();

char* getDueDate(char* prompt);

#endif