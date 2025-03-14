#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "Task.h"


EditResult* moveTask(Task* firstTask, Task* lastTask, int listLength){
    EditResult* newEndPoints = calloc(1, sizeof(EditResult));
    newEndPoints->firstTask = NULL;

    if(listLength == 0 || firstTask == NULL){
        printf("List is empty, nothing to move...\n\n");
        return newEndPoints;
    }

    int taskToMoveIndex = -1;
    int taskToFollowIndex = -1;

    Task* taskToMove;
    Task* taskToMovePrev;
    Task* taskToMoveNext;
    Task* taskToFollow;


    // Get user input, index of the task to be moved
    while(taskToMoveIndex < 0 || taskToMoveIndex > listLength){
        taskToMoveIndex = getInt("Enter task number to move (0 to quit): ");

        if(taskToMoveIndex>listLength){
            printf("Entry exceeds list length, enter number between 1 and %i\n", listLength);
        }else if(taskToMoveIndex == 0){
            printf("Aborting task move, returning to main menu...\n\n");
            return newEndPoints;
        }
    }
    
    Task* currentTask = firstTask;

    // Get pointer to task to be moved
    for (int i = 0; i<=listLength; i++){
        if(currentTask->index == taskToMoveIndex){
            taskToMove = currentTask;
            currentTask = NULL;
            break;
        }else if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
        }else if(currentTask->nextTask == NULL){
            printf("Error moving task, returning to main menu...\n\n");
            return newEndPoints;
        }
    }

    while(taskToFollowIndex < 0 || taskToFollowIndex > listLength){
        taskToFollowIndex = getInt("Enter task number to move behind (0 to front of list): ");

        if(taskToFollowIndex == taskToMoveIndex){
            printf("No change required, returning...\n\n");
            return newEndPoints;
        }else if (taskToFollowIndex > listLength){
            printf("Entry exceeds list length, enter number between 0 and %i\n", listLength);
        }

    }

    taskToMoveNext = taskToMove->nextTask;
    taskToMovePrev = taskToMove->previousTask;

    if(taskToMove->previousTask != NULL){
        taskToMove->previousTask->nextTask = taskToMove->nextTask;
    }

    if(taskToMove->nextTask != NULL){
        taskToMove->nextTask->previousTask = taskToMove->previousTask;
    }

    currentTask = lastTask;

    if(taskToFollowIndex == 0){
        taskToMove->nextTask = firstTask;
        firstTask->previousTask = taskToMove;
        taskToMove->previousTask = NULL;
    }else if(taskToFollowIndex == listLength){
        taskToMove->previousTask = lastTask;
        lastTask->nextTask = taskToMove;
        taskToMove->nextTask = NULL;
    }else{
        for(int i = listLength; i>=taskToFollowIndex; i--){
            if(currentTask->index == taskToFollowIndex){
                break;
            }else if(currentTask->previousTask != NULL){
                currentTask = currentTask-> previousTask;
            }else{
                printf("Error moving task, returning to main menu...\n\n");
                return newEndPoints;
            }
        }

        taskToFollow = currentTask;

        taskToFollow->nextTask->previousTask = taskToMove;
        taskToMove->nextTask = taskToFollow->nextTask;
        taskToFollow->nextTask = taskToMove;
        taskToMove->previousTask = taskToFollow;

    }

    if(taskToFollowIndex == 0){
        newEndPoints->firstTask = taskToMove;
    }else if(taskToMoveIndex == 1){
        newEndPoints->firstTask = taskToMoveNext;
    }else{
        newEndPoints->firstTask = firstTask;
    }

    if(taskToFollowIndex == listLength){
        newEndPoints->lastTask = taskToMove;
    }else if(taskToMoveIndex == listLength){
        newEndPoints->lastTask = taskToMovePrev;
    }else{
        newEndPoints->lastTask = lastTask;
    }

    newEndPoints->listLength = listLength;

    Task* reIndexPtr = newEndPoints->firstTask;

    for(int i = 1; i<=listLength; i++){
        reIndexPtr->index = i;

        if(reIndexPtr->nextTask != NULL){
            reIndexPtr = reIndexPtr->nextTask;
        }
    }

    printf("Task moved successfully...\n\n");

    return newEndPoints;
}