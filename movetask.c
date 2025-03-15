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

    taskToMoveIndex = getTaskPrompt("Enter task number to move (0 to quit): ", listLength);

    if(taskToMoveIndex == 0){
        printf("Aborting task move, returning to main menu...\n\n");
        return newEndPoints;
    }

    taskToFollowIndex = getTaskPrompt("Enter task number to move behind (0 to front of list): ", listLength);

    // Get pointer to task to be moved
    Task* currentTask = firstTask;
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
    reIndexList(reIndexPtr, listLength);

    printf("Task moved successfully...\n\n");

    return newEndPoints;
}