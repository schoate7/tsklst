#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "Task.h"

#define DESCRIPTION_PROMPT "Enter description: "
#define DUE_DATE_PROMPT "Add a due date (Y/N)?: "
#define PRIORITY_PROMPT "Enter priority [L]ow | [M]edium | [H]igh: "

int addTask(Task** firstTask, Task** lastTask, int listLength){
    Task* newTask = malloc(sizeof(Task));
    Date* dateStruct = malloc(sizeof(Date));

    char* nameInput = NULL;
    int nameInputLength = 0;
    char* name;
    char priorityResponse;
    char dueDateResponse;    

    while(nameInput == NULL){
        nameInput = getString(DESCRIPTION_PROMPT);
        nameInputLength = strlen(nameInput);
        name = calloc(nameInputLength, sizeof(char));
        strncpy(name, nameInput, nameInputLength);
    }
    newTask->name = name;
    
    getDueDate(DUE_DATE_PROMPT, dateStruct);
    newTask->dueDate = dateStruct;

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

    listLength++;
    newTask->index = listLength;

    if(*firstTask == NULL && *lastTask == NULL){
        *firstTask = newTask;
        *lastTask = newTask;
        return 1;
        
    }else if(*firstTask != NULL && *lastTask != NULL){
        (*lastTask) -> nextTask = newTask;
        newTask -> previousTask = *lastTask;
        *lastTask = newTask;

        return 1;
    }else{
        printf("Error adding task, returning to main menu...\n\n");
        return 0;
    }
}