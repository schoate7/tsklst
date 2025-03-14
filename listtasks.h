#ifndef listtasks_h
#define listtasks_h

#include "Task.h"

char* centerStringToColumnLength(char* str, int len);

char* padStringToColumnLength(char* str, int len);

void printTaskLine(Task* task);

void printTaskList(Task* firstTask, int listLength);

#endif