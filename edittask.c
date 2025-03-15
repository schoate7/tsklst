#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "Task.h"

//Move delete to it's own function, which loops and includes clearing completed tasks (batch)

#define EDIT_MENU "\nChange: [N]ame | [D]ue Date | [P]riority | [C]ompletion ||| [S]elect Another Task"

/*
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
    */

/*
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
    */

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

static int changeCompletion (Task* taskPtr){
    Task* currentTask = taskPtr;
    char changeResponse;

    while(changeResponse != 'Y' && changeResponse != 'N'){
        changeResponse = currentTask->completed ? getChar("Task is marked complete, set to incomplete? (Y/N): ") : getChar("Task is marked incomplete, set to complete? (Y/N): ");
        if(changeResponse != 'Y' && changeResponse != 'N'){
            printf("Invalid response, enter Y or N.\n");
        }
    }

    if (changeResponse == 'Y' && currentTask->completed){
        currentTask->completed = false;
        printf("Status changed to incomplete.\n");
        return 1;
    }else if (changeResponse == 'Y' && !currentTask->completed){
        currentTask->completed = true;
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

    //printf("Current priority: %s\n", currentTaskPriorityLabel);
    while(newPrioritySelection != 'L' && newPrioritySelection != 'M' && newPrioritySelection != 'H'){
        newPrioritySelection = getChar("Enter priority [L]ow | [M]edium | [H]igh: ");
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
    
    if(taskPtr->dueDateStruct != NULL && taskPtr->dueDateStruct->year != 0){
        //printf("Current due date: %s\n", taskPtr->dueDate);
        while(dueDateResponse != 'N' && dueDateResponse != 'R'){
            dueDateResponse = getChar("Set [n]ew due date, or [r]emove existing due date?: ");
        }
        if(dueDateResponse == 'N'){
            newDueDate = getDueDate(NULL, dateStruct);
            free(taskPtr->dueDate);
            free(taskPtr->dueDateStruct);
            taskPtr->dueDate = newDueDate; //Can remove with deprecation
            taskPtr->dueDateStruct = dateStruct;
            return 1;
        }else{
            strncpy(taskPtr->dueDate, "None", 4); //Can remove with deprecation
            taskPtr->dueDateStruct->day = 0;
            taskPtr->dueDateStruct->month = 0;
            taskPtr->dueDateStruct->year = 0;
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
            newDueDate = getDueDate(NULL, dateStruct);
            free(taskPtr->dueDate);
            free(taskPtr->dueDateStruct);
            taskPtr->dueDate = newDueDate; //Can remove with deprecation
            taskPtr->dueDateStruct = dateStruct;
            return 1;
        }
    }
    return 0;
}

static int changeName(Task* taskPtr){
    char* nameInput;
    char* namePtr;
    int inputLength;

    //printf("Current task name %s", taskPtr);
    nameInput = getString("Enter new title (x to leave unchanged): ");
    inputLength = strlen(nameInput);

    if(inputLength == 1 && (nameInput[0] == 'X' || nameInput[0] == 'x')){
        printf("Leaving title unchaged...\n");
        return 0;
    }else{
        //To be deprecated with Description struct
        free(taskPtr->description);
        taskPtr->description = createDescription(nameInput);
        
        //free(taskPtr->name); //(activate once converted)
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
    }
    return;
}