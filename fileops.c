#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edittask.h"
#include "Task.h"

#define BASE_NUM 10

int parseCSVRow(char* line, Task* taskPtr, Task* prvTaskPtr){
    char* column;
    int count = 0;

    int index;
    char* descStr;
    Description* desc;
    int priorityNum;
    enum Priority priority;
    char* dueDate;
    int completionNum;
    bool completion;
    Date* dateStruct = calloc(1, sizeof(Date));

    column = strtok(line, ",");
    char* columnEndPtr;
    while(column != NULL){
        switch(count){
            case 0:
                index = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            case 1:
                descStr = column;
                desc = createDescription(descStr);
                break;
            case 2:
                priorityNum = strtol(column, &columnEndPtr, BASE_NUM);
                priority = priorityNum == 0 ? LOW : priorityNum == 2 ? HIGH : MEDIUM; 
                break;
            case 3:
                dueDate = column;
                break;
            case 4:
                completionNum = strtol(column, &columnEndPtr, BASE_NUM);
                completion = completionNum;
                break;
            case 5:
                dateStruct->year = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            case 6:
                dateStruct->month = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            case 7:
                dateStruct->day = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            default:
                return 0;
        }
        count++;
        column = strtok(NULL, ",");
    }

    taskPtr->index = index;
    taskPtr->description = desc;
    taskPtr->priority = priority;
    taskPtr->dueDate = calloc(12, sizeof(char));
    strcpy(taskPtr->dueDate, dueDate);
    taskPtr->completed = completion;
    taskPtr->previousTask = prvTaskPtr;
    taskPtr->dueDateStruct = dateStruct;

    return 1;
}

EditResult* openFile(){
    EditResult* openedList = malloc(sizeof(EditResult));
    FILE *f;
    char* filename = malloc(100*sizeof(char));

    printf("\nEnter file name to open: ");
    scanf("%s", filename);

    if(strlen(filename)<1){
        printf("No file name provided, returning...\n\n");
        return NULL;
    }

    f = fopen(filename, "r");
    if (f == NULL){
        perror("Error opening file\n\n");
        openedList = NULL;
        return openedList;
    }

    char line[300];
    int lineIndex = 0;
    Task* prvTask = NULL;

    while(fgets(line, 300, f)){
        line[strcspn(line, "\r\n")] = '\0';

        if(lineIndex == 0){
            lineIndex++;
        }else {
            Task* task = malloc(sizeof(Task));
            if(parseCSVRow(line, task, prvTask)){
                if(lineIndex == 1){
                    openedList->firstTask = task;
                }
                if(prvTask != NULL){
                    prvTask->nextTask = task;
                }
                prvTask = task;
                lineIndex++;
            }else{
                printf("A row skipped when opening file due to data integrity issue.\n");
            }
        }
    }
    lineIndex--;

    openedList->lastTask = prvTask;
    openedList->listLength = lineIndex;

    printf("File \"%s\" opened successfully...\n\n", filename);

    fclose(f);
    free(filename);

    return openedList;
}

int saveList(Task* firstTask, int listLength){
    FILE *f;
    char* filename;

    Task* currentTask = firstTask;

    if(listLength == 0 || firstTask == NULL){
        printf("Nothing to save, returning to main menu...\n\n");
    }
    
    printf("\nEnter the filename to save (default is list): ");
    scanf("%s", filename);

    if(strlen(filename)<1){
        strcpy(filename, "list");
    }
    strcat(filename, ".csv");

    f = fopen(filename, "w");
    if (f == NULL) {
        perror("Error opening file\n\n");
        return 1;
    }

    fprintf(f, "%s,%s,%s,%s,%s", "index", "name", "priority", "duedate", "completionstatus\n");

    int taskPriority;
    int taskCompletion;

    while(currentTask->nextTask != NULL){
        taskPriority = currentTask -> priority;
        taskCompletion = currentTask -> completed;
        fprintf(f, "%i,%s,%i,%s,%i,%i,%i,%i\n", currentTask->index, currentTask->description->data, taskPriority, 
            currentTask->dueDate, taskCompletion, currentTask->dueDateStruct->year, currentTask->dueDateStruct->month, currentTask->dueDateStruct->day);
        currentTask = currentTask->nextTask;
    }
    
    taskPriority = currentTask -> priority;
    taskCompletion = currentTask -> completed;
    fprintf(f, "%i,%s,%i,%s,%i,%i,%i,%i", currentTask->index, currentTask->description->data, taskPriority, 
        currentTask->dueDate, taskCompletion, currentTask->dueDateStruct->year, currentTask->dueDateStruct->month, currentTask->dueDateStruct->day);

    fclose(f);
    printf("List saved to %s...\n\n", filename);
    return 0;
}