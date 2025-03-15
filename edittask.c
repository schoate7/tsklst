#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "Task.h"

static void completeTask(Task* firstTask, int listLength){
    Task* currentTask = firstTask;
    int inputIndex = -1;
    int i = 1;
    
    if(listLength<1){
        printf("List is empty, returning to main menu...\n");
    }

    inputIndex = getTaskPrompt("\nEnter a task number to update (0 to quit): ", listLength);

    if(inputIndex == 0){
        printf("Returning to main menu...\n\n");
        return;
    }

    while(currentTask != NULL && currentTask->index != inputIndex){
        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
            i++;
        }else{
            currentTask = NULL;
        }
    }

    char changeResponse;

    if(currentTask == NULL){
        printf("Error finding task, returning to main menu...\n\n");
        return;
    }

    printf("Task name: \"%s\"\n", currentTask->description->data);

    while(changeResponse != 'Y' && changeResponse != 'N'){
        changeResponse = currentTask->completed ? getChar("Task is marked complete, set to incomplete? (Y/N): ") : getChar("Task is marked incomplete, set to complete? (Y/N): ");
        if(changeResponse != 'Y' && changeResponse != 'N'){
            printf("Invalid response, enter Y or N.\n");
        }
    }

    if (changeResponse == 'Y' && currentTask->completed){
        currentTask->completed = false;
        printf("Task %i changed to incomplete.\n\n", inputIndex);
        return;
    }else if (changeResponse == 'Y' && !currentTask->completed){
        currentTask->completed = true;
        printf("Task %i changed to complete.\n\n", inputIndex);
        return;
    }else{
        printf("No change made, returning to main menu...\n\n");
        return;
    }
}

void completedTaskHandler(Task* firstTask, int listLength){
    int opType = 0; 
    
    do{
        opType = getInt("Do you wish to complete a task (1), or clear completed tasks (2)?: ");
        if(opType < 1 || opType >= 2){
            printf("Invalid entry, valid options: 1, 2.\n");
        }
    }while(opType < 0 || opType >= 2);

    opType == 1 ? completeTask(firstTask, listLength) : printf("Clear completed tasks not yet developed...\n\n");
}

void updateTask(Task* firstTask, Task* lastTask, int listLength){
    Task* currentTask = firstTask;
    int inputIndex = -1;
    int i = 1;

    if(listLength<1){
        printf("List is empty, returning to main menu...\n");
        return;
    }

    while(inputIndex == -1 || inputIndex > listLength){
        inputIndex = getInt("\nEnter a task number to update (0 to quit): ");
        if(inputIndex > listLength){
            printf("Task out of range, please input a number between 1 and %i, 0 to quit.\n", listLength);
        }else if (inputIndex == 0){
            printf("Returning to main menu...\n\n");
            return;
        }
    }

    while(currentTask != NULL && currentTask->index != inputIndex){
        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
            i++;
        }else{
            currentTask = NULL;
        }
    }

    if(currentTask == NULL){
        printf("Task not found, returning to main menu...\n\n");
        return;
    }

    char* descIn;
    char changeDueDateResponse;
    char* newDueDate;
    enum Priority newPriority;
    char* currentTaskPriorityLabel = currentTask->priority == 0 ? "Low\0" : currentTask->priority == 2 ? "High\0" : "Medium\0";
    char changePriorityResponse;
    char newPrioritySelection;

    printf("Current title: %s\n", currentTask->description->data);
    descIn = getString("Enter new title (0 to leave unchanged): ");
    if(descIn[0] != '0'){
        free(currentTask->description);
        currentTask->description = createDescription(descIn);
    }else{
        printf("Leaving title unchanged...\n");
    }

    printf("Current due date: %s\n", currentTask->dueDate);
    changeDueDateResponse = getChar("Change due date? ");
    if(changeDueDateResponse == 'Y'){
        Date* dateStruct = malloc(sizeof(Date));
        newDueDate = getDueDate("Do you want to set a new date (Y), or remove the existing date (N)? ", dateStruct);
        free(currentTask->dueDate);
        free(currentTask->dueDateStruct);
        currentTask -> dueDate = newDueDate;
        currentTask -> dueDateStruct = dateStruct;
    }else{
        printf("Leaving due date unchanged...\n");
    }

    printf("Current priority: %s\n", currentTaskPriorityLabel);
    changePriorityResponse = getChar("Change priority? ");
    if(changePriorityResponse == 'Y'){
        newPrioritySelection = getChar("Enter priority [L]ow | [M]edium | [H]igh: ");
        switch(newPrioritySelection){
            case 'L':
                newPriority = LOW;
                break;
            case 'M':
                newPriority = MEDIUM;
                break;
            case 'H':
                newPriority = HIGH;
                break;
            default:
                newPriority = MEDIUM;
                break;
        }
        currentTask->priority = newPriority;
    }else{
        printf("Leaving priority unchanged...\n");
    }
    printf("\n");
}

EditResult* deleteTask(Task* firstTask, Task* lastTask, int listLength){
    EditResult* result = malloc(sizeof(EditResult));
    Task* currentTask = firstTask;
    Task* first = firstTask;
    int deleteIndex = -1;

    while(deleteIndex == -1 || deleteIndex > listLength){
        deleteIndex = getInt("\nEnter task number to delete (0 to quit): ");
        if(deleteIndex > listLength){
            printf("Task out of range, please input a number between 1 and %i, 0 to quit.\n", listLength);
        }else if (deleteIndex == 0){
            result -> firstTask = firstTask;
            result -> lastTask = lastTask;
            result -> listLength = listLength;
            printf("Returning to main menu...\n\n");
            return result;
        }
    }    
    
    int i = 1;

    while(currentTask != NULL && currentTask->index != deleteIndex){
        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
            i++;
        }else{
            currentTask = NULL;
        }
    }

    if (currentTask == NULL){
        printf("Task not found, returning to main menu...\n\n");
        result -> firstTask = NULL;
        result -> lastTask = NULL;
        result -> listLength = 0;
        return result;
    }

    if(currentTask != NULL && deleteIndex == i){
        Task* previous = currentTask-> previousTask;
        Task* next = currentTask-> nextTask;

        if(previous == NULL && next == NULL){
            first = NULL;
        }else if (previous != NULL && next == NULL){
            previous -> nextTask = NULL;

        }else if (previous == NULL && next != NULL){
            next->previousTask = NULL;
            first = next;
        }else{
            previous->nextTask = next;
            next->previousTask = previous;
        }
        free(currentTask->description);
        free(currentTask);        
    }

    if(first != NULL){
        int y = 1;
        currentTask = first;

        while(currentTask->nextTask != NULL){
            currentTask -> index = y;
            currentTask = currentTask->nextTask;
            y++;
        }
        currentTask -> index = y;

        result -> firstTask = first;
        result -> lastTask = currentTask;
        result -> listLength = y;
    }else{
        result -> firstTask = NULL;
        result -> lastTask = NULL;
        result -> listLength = 0;
    }
    printf("Delete processed successfully.\n\n");
    return result;
}