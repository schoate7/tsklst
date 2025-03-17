#ifndef fileops_h
#define fileops_h

#include "Task.h"

int saveList(Task* firstTask, int listLength);

int openFile(char* fname, Task** firstTask, Task** lastTask);

#endif