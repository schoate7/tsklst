#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "addtask.h"
#include "common.h"
#include "edittask.h"
#include "fileops.h"
#include "listtasks.h"
#include "movetask.h"
#include "Task.h"

#define TITLE_BAR "------------------------------------\033[1mtsklst home\033[0m------------------------------------\n"
#define SAVE_WARNING "\033[33mChanges must be saved before quitting.\033[0m\n"
#define COMMAND_LIST "[L]ist | [A]dd | [D]elete | [E]dit | [M]ove | [C]omplete | [O]pen | [S]ave | [H]elp | [Q]uit:  "

void home(char *fname[]){
    bool runMode = true;
    char selection = ' ';
    int currentIndex = 0;

    Task *firstTask = NULL;
    Task *returnTask = NULL;
    Task *lastTask = NULL;
    EditResult *editReturn = NULL;

    printf(TITLE_BAR);
    printf(SAVE_WARNING);
    
    while(runMode){
        selection = getChar(COMMAND_LIST);
        switch(selection){
            case 'L':
                printTaskList(firstTask, currentIndex);
                break;
            case 'A':
                printf("\n");
                returnTask = addTask(firstTask, lastTask);
                if (firstTask == NULL){
                    currentIndex++;
                    firstTask = returnTask;
                    firstTask -> nextTask == NULL;
                    lastTask = returnTask;
                    firstTask -> index = currentIndex;
                }else{
                    currentIndex++;
                    lastTask -> nextTask = returnTask;
                    returnTask -> previousTask = lastTask;
                    lastTask = returnTask;
                    lastTask -> index = currentIndex;
                    lastTask -> nextTask = NULL;
                }
                break;
            case 'D':
                editReturn = deleteTask(firstTask, lastTask, currentIndex);
                firstTask = editReturn -> firstTask;
                lastTask = editReturn -> lastTask;
                currentIndex = editReturn -> listLength;
                free(editReturn);
                break;
            case 'E':
                updateTask(firstTask, lastTask, currentIndex);
                break;
            case 'M':
                editReturn = moveTask(firstTask, lastTask, currentIndex);
                if(editReturn != NULL && editReturn->firstTask != NULL){
                    firstTask = editReturn->firstTask;
                    lastTask = editReturn->lastTask;
                    free(editReturn);
                }else{
                    free(editReturn);
                }
                break;
            case 'C':
                changeCompletionStatus(firstTask, currentIndex);
                break;
            case 'S':
                saveList(firstTask, currentIndex);
                break;
            case 'O':
                editReturn = openFile();
                if(editReturn != NULL){
                    firstTask = editReturn->firstTask;
                    lastTask = editReturn->lastTask;
                    currentIndex = editReturn->listLength;
                }
                break;
            case 'Q':
                while(selection != 'Y' && selection != 'N'){
                    selection = getChar("Do you want to save changes (Y/N)? ");
                    if(selection != 'Y' && selection != 'N'){
                        printf("Invalid input, please select Y or N.\n");
                    }else if(selection == 'Y'){
                        saveList(firstTask, currentIndex);
                    }
                }
                runMode = false;
                break;
        }
    }
}

int main(int argc, char *argv[]){
    char *fname;
    home(&fname);
    return 0;
}