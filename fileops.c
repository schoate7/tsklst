#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Task.h"

#define BASE_NUM 10

static int parseCSVRow(char* line, Task* taskPtr, Task* prvTaskPtr){
    char* column;
    int count = 0;

    int index;
    char* name;
    int nameLength;
    int priorityNum;
    enum Priority priority;
    char* dueDate;
    int completion;
    Date* dateStruct = calloc(1, sizeof(Date));

    column = strtok(line, ",");
    char* columnEndPtr;
    while(column != NULL){
        switch(count){
            case 0:
                index = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            case 1:
                nameLength = strlen(column);
                name = calloc(nameLength, sizeof(char));
                strncpy(name, column, nameLength);
                break;
            case 2:
                priorityNum = strtol(column, &columnEndPtr, BASE_NUM);
                priority = priorityNum == 0 ? LOW : priorityNum == 2 ? HIGH : MEDIUM; 
                break;
            case 3:
                completion = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            case 4:
                dateStruct->year = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            case 5:
                dateStruct->month = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            case 6:
                dateStruct->day = strtol(column, &columnEndPtr, BASE_NUM);
                break;
            default:
                return 0;
        }
        count++;
        column = strtok(NULL, ",");
    }

    taskPtr->index = index;
    taskPtr->name = name;
    taskPtr->priority = priority;
    taskPtr->completed = completion;
    taskPtr->previousTask = prvTaskPtr;
    taskPtr->dueDate = dateStruct;

    return 1;
}

int openFile(char* fname, Task** firstTask, Task** lastTask){
    FILE *f;
    char* filename = malloc(100*sizeof(char));

    if(fname == NULL){
        printf("\nEnter file name to open: ");
        scanf("%s", filename);

        if(strlen(filename)<1){
            printf("No file name provided, returning...\n\n");
            return 0;
        }
    }else{
        strncpy(filename, fname, strlen(fname));
    }

    f = fopen(filename, "r");
    if (f == NULL){
        perror("Error opening file\n\n");
        return 0;
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
                    *firstTask = task;
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

    *lastTask = prvTask;

    printf("File \"%s\" opened successfully...\n\n", filename);

    fclose(f);
    free(filename);

    return lineIndex;
}

int saveList(Task* firstTask, int listLength){
    FILE *f;
    char* filename = calloc(100, sizeof(char));

    Task* currentTask = firstTask;

    if(listLength == 0 || firstTask == NULL){
        printf("Nothing to save, returning to main menu...\n\n");
    }
    
    printf("\nEnter the filename to save (default is list): ");
    scanf("%95s", filename);

    if(strlen(filename)<1){
        strcpy(filename, "list");
    }
    strcat(filename, ".csv");

    f = fopen(filename, "w");
    if (f == NULL) {
        perror("Error saving file\n\n");
        return 1;
    }

    fprintf(f, "%s,%s,%s,%s,%s,%s,%s\n", "index", "name", "priority", "completionstatus", "dueYear", "dueMonth", "dueDay");

    int taskPriority;
    int taskCompletion;

    while(currentTask->nextTask != NULL){
        taskPriority = currentTask -> priority;
        taskCompletion = currentTask -> completed;
        fprintf(f, "%i,%s,%i,%i,%i,%i,%i\n", currentTask->index, currentTask->name, taskPriority, 
            taskCompletion, currentTask->dueDate->year, currentTask->dueDate->month, currentTask->dueDate->day);
        currentTask = currentTask->nextTask;
    }
    
    taskPriority = currentTask -> priority;
    taskCompletion = currentTask -> completed;
    fprintf(f, "%i,%s,%i,%i,%i,%i,%i", currentTask->index, currentTask->name, taskPriority, 
        taskCompletion, currentTask->dueDate->year, currentTask->dueDate->month, currentTask->dueDate->day);

    fclose(f);
    printf("List saved to %s...\n\n", filename);
    free(filename);
    return 0;
}