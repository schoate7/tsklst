#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "Task.h"

#define DESCRIPTION_PROMPT "Enter description: "
#define DUE_DATE_PROMPT "Add a due date (Y/N)? "
#define PRIORITY_PROMPT "Enter priority [L]ow | [M]edium | [H]igh: "

Task* addTask(Task* firstTask, Task* lastTask){
    Task* newTask = malloc(sizeof(Task));
    Description* desc;
    char* dueDate = malloc(10 * sizeof(char));

    char* descIn = NULL;
    char priorityResponse;
    char dueDateResponse;    

    while(descIn == NULL){
        descIn = getString(DESCRIPTION_PROMPT);
        desc = createDescription(descIn);
    }
    
    dueDate = getDueDate(DUE_DATE_PROMPT);

    priorityResponse = getChar(PRIORITY_PROMPT);
    printf("\n");

    switch(priorityResponse){
        case 'L':
            newTask->priority = LOW;
            break;
        case 'M':
            newTask->priority = MEDIUM;
            break;
        case 'H':
            newTask->priority = HIGH;
            break;
        default:
            newTask->priority = MEDIUM;
            break;
    }
    newTask -> description = createDescription(desc -> data);
    newTask -> dueDate = dueDate;

    if(firstTask == NULL && lastTask == NULL){
        return newTask;
        
    }else if(firstTask != NULL && lastTask == NULL){
        firstTask -> nextTask = newTask;
        newTask -> previousTask = lastTask;
        return newTask;

    }else{
        lastTask -> nextTask = newTask;
        newTask -> previousTask = lastTask;
        return newTask;
    }
}