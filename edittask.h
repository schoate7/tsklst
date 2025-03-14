#ifndef edittask_h
#define edittask_h

#include "Task.h"

void changeCompletionStatus(Task* firstTask, int listLength);

EditResult* deleteTask(Task* firstTask, Task* lastTask, int listLength);

void updateTask(Task* firstTask, Task* lastTask, int listLength);

#endif