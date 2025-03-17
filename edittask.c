#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "listtasks.h"
#include "Task.h"

#define EDIT_MENU "\nChange: [N]ame | [D]ue Date | [P]riority | [C]ompletion || [S]elect Another Task: "

static int changeCompletion (Task* taskPtr){
    Task* currentTask = taskPtr;
    char changeResponse;

    while(changeResponse != 'Y' && changeResponse != 'N'){
        changeResponse = currentTask->completed ? getChar("\nTask is marked complete, set to incomplete? (Y/N): ") : getChar("\nTask is marked incomplete, set to complete? (Y/N): ");
        if(changeResponse != 'Y' && changeResponse != 'N'){
            printf("Invalid response, enter Y or N.\n");
        }
    }

    if (changeResponse == 'Y' && currentTask->completed){
        currentTask->completed = 0;
        printf("Status changed to incomplete.\n");
        return 1;
    }else if (changeResponse == 'Y' && !currentTask->completed){
        currentTask->completed = 1;
        printf("Task changed to complete.\n");
        return 1;
    }else{
        printf("No change made.\n");
        return 0;
    }
    return 0;
}

static int changePriority(Task* taskPtr){
    enum Priority originalPriority = taskPtr->priority;
    char newPrioritySelection;
    char* currentTaskPriorityLabel = taskPtr->priority == 0 ? "Low\0" : taskPtr->priority == 2 ? "High\0" : "Medium\0";

    while(newPrioritySelection != 'L' && newPrioritySelection != 'M' && newPrioritySelection != 'H'){
        newPrioritySelection = getChar("\nEnter priority [L]ow | [M]edium | [H]igh: ");
        switch(newPrioritySelection){
            case 'L':
                taskPtr->priority = LOW;
                break;
            case 'M':
                taskPtr->priority = MEDIUM;
                break;
            case 'H':
                taskPtr->priority = HIGH;
                break;
            default:
                printf("Must select a priority [L, M, or H].\n");
                break;
        }
    }
    int changed = originalPriority != taskPtr->priority ? 1 : 0;

    return changed;
}

static int changeDueDate(Task* taskPtr){
    char dueDateResponse;
    char* newDueDate;
    Date* dateStruct = calloc(1, sizeof(Date));
    
    if(taskPtr->dueDate != NULL && taskPtr->dueDate->year != 0){
        while(dueDateResponse != 'N' && dueDateResponse != 'R'){
            dueDateResponse = getChar("Set [n]ew due date, or [r]emove existing due date?: ");
        }
        if(dueDateResponse == 'N'){
            getDueDate(NULL, dateStruct);
            free(taskPtr->dueDate);
            taskPtr->dueDate = dateStruct;
            return 1;
        }else{
            taskPtr->dueDate->day = 0;
            taskPtr->dueDate->month = 0;
            taskPtr->dueDate->year = 0;
            return 1;
        }
    }else{
        printf("Task does not have a due date.\n");
        while(dueDateResponse != 'N' && dueDateResponse != 'Y'){
            dueDateResponse = getChar("Add a due date (Y/N)?: ");
        }
        if(dueDateResponse == 'N'){
            printf("Making no changes...\n");
            return 0;
        }else{
            getDueDate(NULL, dateStruct);
            free(taskPtr->dueDate);
            taskPtr->dueDate = dateStruct;
            return 1;
        }
    }
    return 0;
}

static int changeName(Task* taskPtr){
    char* nameInput;
    char* namePtr;
    int inputLength;

    nameInput = getString("\nEnter new title (x to leave unchanged): ");
    inputLength = strlen(nameInput);

    if(inputLength == 1 && (nameInput[0] == 'X' || nameInput[0] == 'x')){
        printf("Leaving title unchaged...\n");
        return 0;
    }else{        
        free(taskPtr->name);
        namePtr = calloc(inputLength, sizeof(char));
        strncpy(namePtr, nameInput, strlen(nameInput));
        taskPtr->name = namePtr;
        return 1;
    }
    return 0;
}

void editTaskMenu(Task* firstTask, int listLength){
    int changeCount = 0;
    char userResponse;
    int taskIndex = 1;
    Task* selectedTask;

    if (listLength < 1){
        printf("\nList is empty, returning to main menu...\n\n");
        return;
    }

    printf("\n");
    while(taskIndex != 0){
        taskIndex = getTaskPrompt("Select a task to edit (0 to return): ", listLength);
        if(taskIndex == 0){
            printf("Returning to main menu...\n\n");
            return;
        }

        selectedTask = getSelectedTask(firstTask, taskIndex);

        if(selectedTask == NULL){
            printf("Error opening task, returning to main menu...\n\n");
            return;
        }

        printf(TABLE_HEADER);
        printTaskLine(selectedTask);
        printf("\n");

        while (userResponse != 'S'){
            userResponse = getChar(EDIT_MENU);
            switch(userResponse){
                case 'N':
                    changeCount += changeName(selectedTask);
                    break;
                case 'D':
                    changeCount += changeDueDate(selectedTask);
                    break;
                case 'P':
                    changeCount += changePriority(selectedTask);
                    break;
                case 'C':
                    changeCount += changeCompletion(selectedTask);
                    break;
                case 'S':
                    printf("\n");
                    break;
                default:
                    printf("Invalid entry, please enter one of [NDPCS].\n");
                    break;
            }
        }
        userResponse =' ';
    }
    return;
}