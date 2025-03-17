#ifndef common_h
#define common_h

#define TABLE_HEADER "\n|-\033[1m#\033[0m-|-------------------------\033[1mTask Name\033[0m------------------------|-\033[1mPriority\033[0m-|--\033[1mDue Date\033[0m--|-\033[1mCompletion\033[0m-|\n"
#define ROW_SEPARATOR "|---|----------------------------------------------------------|----------|------------|------------|\n"

#include "Task.h"

/* Input handlers */
char* getString(char prompt[]);
char getChar(char prompt[]);
int getInt(char prompt[]);
float getFloat(char prompt[]);

/* Get functions */
/*
    Gets a due date in the future (day + 1 and later).
    @param prompt - A string to use to prompt the user.
    @param date - A date struct, referenced pointer is modified with new due date data.
*/
void getDueDate(char* prompt, Date* date);
int getTaskPrompt(char* prompt, int listLength);
Task* getSelectedTask(Task* firstTask, int taskIndex);

/* Utility functions */
int reIndexTasks(Task** firstTask, Task** lastTask);
int checkUnsavedChanges(int changeCount);

#endif