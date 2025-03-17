#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "Task.h"

#define DEL_MENU_PROMPT "Delete [T]ask | Delete All [C]ompleted || [R]eturn to Menu: "

static void deleteTask(Task** firstTask, Task** lastTask, Task* currentTask){
    Task* previous = currentTask-> previousTask;
    Task* next = currentTask-> nextTask;

    if(previous == NULL && next == NULL){
        *firstTask = NULL;
        *lastTask = NULL;
    }else if (previous != NULL && next == NULL){
        previous -> nextTask = NULL;
        *lastTask = previous;
    }else if (previous == NULL && next != NULL){
        next->previousTask = NULL;
        *firstTask = next;
    }else{
        previous->nextTask = next;
        next->previousTask = previous;
    }
    free(currentTask->name);
    free(currentTask->dueDate);
    free(currentTask);
}

static int deleteCompletedTasks(Task** firstTask, Task** lastTask, int listLength){
    Task* completedTasks[listLength];
    Task* currentTask = *firstTask;
    int completedIndex = 0;

    while(currentTask != NULL){
        if(currentTask->completed){
            completedTasks[completedIndex] = currentTask;
            completedIndex++;
        }

        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
        }else{
            currentTask = NULL;
        }
    }

    for(int i = 0; i<completedIndex; i++){
        Task* currentTask = completedTasks[i];
        deleteTask(firstTask, lastTask, currentTask);
    }
    if(completedIndex>0){
        int i = reIndexTasks(firstTask, lastTask);
        printf("%i completed tasks deleted.\n\n", completedIndex);
        return i;
    }else{
        printf("No completed tasks found, no changes made.\n\n");
        return 0;
    }
}

static int deleteSingleTask(Task** firstTask, Task** lastTask, int listLength){
    Task* currentTask = *firstTask;
    Task* first;
    int deleteIndex;

    printf("\n");
    deleteIndex = getTaskPrompt("Enter task number to delete (0 to return): ", listLength);
    if(deleteIndex == 0){
        return 0;
    }

    currentTask = getSelectedTask(*firstTask, deleteIndex);

    if (currentTask == NULL){
        printf("Task not found, returning to main menu...\n\n");
        return 0;
    }

    if(currentTask != NULL){
        deleteTask(firstTask, lastTask, currentTask);     
    }

    if(*firstTask != NULL){
        int i = reIndexTasks(firstTask, lastTask);
        printf("Delete processed successfully.\n\n");
        return i;
    }else{
        *lastTask = NULL;
        return 0;
    }
}

int deleteMenu(Task** firstTask, Task** lastTask, int listLength){
    char userSelection;
    int returnLength;

    printf("\n");
    while(userSelection != 'T' && userSelection != 'C' && userSelection != 'R'){
        userSelection = getChar(DEL_MENU_PROMPT);
        switch(userSelection){
            case 'T':
                returnLength = deleteSingleTask(firstTask, lastTask, listLength);
                break;
            case 'C':
                returnLength = deleteCompletedTasks(firstTask, lastTask, listLength);
                break;
            case 'R':
                printf("Returning to main menu...\n\n");
                break;
            default:
                printf("Invalid selection, please enter one of [TCR].\n");
                break;
        }
    }
    return returnLength;
}