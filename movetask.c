#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "Task.h"

int moveTask(Task** firstTask, Task** lastTask, int listLength){

    if(listLength == 0 || firstTask == NULL){
        printf("\nList is empty, nothing to move...\n\n");
        return 0;
    }

    int taskToMoveIndex = -1;
    int taskToFollowIndex = -1;

    Task* taskToMove;
    Task* taskToMovePrev;
    Task* taskToMoveNext;
    Task* taskToFollow;

    taskToMoveIndex = getTaskPrompt("\nEnter task number to move (0 to quit): ", listLength);

    if(taskToMoveIndex == 0){
        printf("Aborting task move, returning to main menu...\n\n");
        return 0;
    }

    taskToFollowIndex = getTaskPrompt("Enter task number to move behind (0 to front of list): ", listLength);

    // Get pointer to task to be moved
    Task* currentTask = *firstTask;
    for (int i = 0; i<=listLength; i++){
        if(currentTask->index == taskToMoveIndex){
            taskToMove = currentTask;
            currentTask = NULL;
            break;
        }else if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
        }else if(currentTask->nextTask == NULL){
            printf("Error moving task, returning to main menu...\n\n");
            return 0;
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

    currentTask = *lastTask;

    if(taskToFollowIndex == 0){
        taskToMove->nextTask = *firstTask;
        (*firstTask)->previousTask = taskToMove;
        taskToMove->previousTask = NULL;
    }else if(taskToFollowIndex == listLength){
        taskToMove->previousTask = *lastTask;
        (*lastTask)->nextTask = taskToMove;
        taskToMove->nextTask = NULL;
    }else{
        for(int i = listLength; i>=taskToFollowIndex; i--){
            if(currentTask->index == taskToFollowIndex){
                break;
            }else if(currentTask->previousTask != NULL){
                currentTask = currentTask-> previousTask;
            }else{
                printf("Error moving task, returning to main menu...\n\n");
                return 0;
            }
        }

        taskToFollow = currentTask;

        taskToFollow->nextTask->previousTask = taskToMove;
        taskToMove->nextTask = taskToFollow->nextTask;
        taskToFollow->nextTask = taskToMove;
        taskToMove->previousTask = taskToFollow;
    }

    if(taskToFollowIndex == 0){
        *firstTask = taskToMove;
    }else if(taskToMoveIndex == 1){
        *firstTask = taskToMoveNext;
    }

    if(taskToFollowIndex == listLength){
        *lastTask = taskToMove;
    }else if(taskToMoveIndex == listLength){
        *lastTask = taskToMovePrev;
    }

    reIndexTasks(firstTask, lastTask);

    printf("Task moved successfully...\n\n");
    return 1;
}