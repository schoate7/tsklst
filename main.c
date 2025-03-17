#include <stdio.h>
#include <stdlib.h>

#include "addtask.h"
#include "common.h"
#include "deletetask.h"
#include "edittask.h"
#include "fileops.h"
#include "listtasks.h"
#include "movetask.h"
#include "Task.h"

#define TITLE_BAR "------------------------------------\033[1mtsklst home\033[0m------------------------------------\n"
#define SAVE_WARNING "\033[33mChanges must be saved before quitting.\033[0m\n"
#define COMMAND_LIST "[L]ist | [A]dd | [E]dit | [M]ove | [D]elete | [O]pen | [S]ave | [H]elp | [Q]uit:  "

void home(char *fname){
    int runMode = 1;
    int changeCount = 0;

    char selection = ' ';
    int returnValue;

    Task *firstTask = NULL;
    Task *lastTask = NULL;
    int listLength = 0;

    printf(TITLE_BAR);
    printf(SAVE_WARNING);

    if(fname != NULL){
        listLength = openFile(fname, &firstTask, &lastTask);
    }
    
    while(runMode == 1){
        selection = getChar(COMMAND_LIST);
        switch(selection){
            case 'L':
                listTasksHandler(firstTask, listLength);
                break;
            case 'A':
                printf("\n");
                returnValue = addTask(&firstTask, &lastTask, listLength);
                if(returnValue != 0){
                    changeCount++;
                    listLength++;
                }
                break;
            case 'D':
                returnValue = deleteMenu(&firstTask, &lastTask, listLength);
                if (returnValue != listLength){
                    changeCount++;
                    listLength = returnValue;
                }
                break;
            case 'E':
                editTaskMenu(firstTask, listLength);
                break;
            case 'M':
                changeCount += moveTask(&firstTask, &lastTask, listLength);
                break;
            case 'S':
                saveList(firstTask, listLength);
                changeCount = 0;
                break;
            case 'O':
                returnValue = checkUnsavedChanges(changeCount);
                if(returnValue == 1){
                    saveList(firstTask, listLength);
                }
                listLength = openFile(NULL, &firstTask, &lastTask);
                changeCount = 0;
                break;
            case 'Q':
                returnValue = checkUnsavedChanges(changeCount);
                if(returnValue == 1){
                    saveList(firstTask, listLength);
                }
                runMode = 0;
                break;
        }
    }
}

int main(int argc, char *argv[]){
    char *fname;
    
    if (argc >= 2){
        home(argv[1]);
    }else{
        fname = NULL;
        home(NULL);
    }
    return 0;
}