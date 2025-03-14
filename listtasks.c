#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "Task.h"

#define NUM_BASE 10
#define LIST_MENU "[L]ist all tasks | By [P]riority | By [C]ompletion | By [D]ate | [M]ain Menu: "
#define TABLE_HEADER "\n|-\033[1m#\033[0m-|-------------------------\033[1mTask Name\033[0m------------------------|-\033[1mPriority\033[0m-|--\033[1mDue Date\033[0m--|-\033[1mCompletion\033[0m-|\n"
#define ROW_SEPARATOR "|---|----------------------------------------------------------|----------|------------|------------|\n"

//TODO: List by date

bool checkIfDateHasPassed(char* date){
    if(date[0] == 'N'){
        return false;
    }

    time_t currentTime;
    struct tm* timeInfo;
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    int day = timeInfo -> tm_mday;
    int month = timeInfo -> tm_mon + 1;
    int year = timeInfo ->tm_year + 1900;

    char* dueYear = malloc(4*sizeof(char));
    char* dueYearEndPtr;
    strncpy(dueYear, date + 6, 4);
    int dueYearInt = strtol(dueYear, &dueYearEndPtr, NUM_BASE);

    char* dueMonth = malloc(2*sizeof(char));
    char* dueMonthEndPtr;
    strncpy(dueMonth, date, 2);
    int dueMonthInt = strtol(dueMonth, &dueMonthEndPtr, NUM_BASE);

    char* dueDay = malloc(2*sizeof(char));
    char* dueDayEndPtr;
    strncpy(dueDay, date + 3, 2);
    int dueDayInt = strtol(dueDay, &dueDayEndPtr, NUM_BASE);
    free(dueYear); free(dueMonth); free(dueDay);

    if (dueYearInt > year){
        return false;
    }else if (dueYearInt == year){
        if(dueMonthInt > month){
            return false;
        }else if (dueMonthInt < month){
            return true;
        }else if (dueMonthInt == month){
            if(dueDayInt > day){
                return false;
            }else{
                return true;
            }
        }
    }
}

char* centerStringToColumnLength(char* str, int len){
    if(str == NULL || len <= 0){
        return NULL;
    }

    int slen = strlen(str);
    char* returnStr = malloc((len + 1) * sizeof(char));    

    if (slen < len){
        int paddingLeft = (len - slen) / 2;
        int paddingRight = len - slen - paddingLeft;

        memset(returnStr, ' ', len);
        returnStr[len] = '\0'; 
        memcpy(returnStr + paddingLeft, str, slen);
        return returnStr;

    }
    strncpy(returnStr, str, len);
    returnStr[len] = '\0';
    return returnStr;
}

char* padStringToColumnLength(char* str, int len){
    if(str == NULL || len <= 0){
        return NULL;
    }

    int slen = strlen(str);
    char* returnStr = malloc((len + 1) * sizeof(char));
    
    if(slen < len){
        strcpy(returnStr, str);
        for(int i = slen; i<len; i++){
            returnStr[i] = ' ';
        }
        returnStr[len] = '\0';
        return returnStr;
    }
    strncpy(returnStr, str, len);
    returnStr[len] = '\0';
    return returnStr;
}

void printTaskLine(Task* task){
    bool dueDatePassed = checkIfDateHasPassed(task->dueDate);
    Description* description = task -> description;
    int taskPriority = task->priority;
    char* i = malloc(6 * sizeof(char));
    snprintf(i, 6, "%d", task->index);
    char* index = centerStringToColumnLength(i, 3);
    char* taskName = padStringToColumnLength(description -> data, 56);
    char* priorityLabel = centerStringToColumnLength(taskPriority == 0 ? "Low\0" : taskPriority == 2 ? "High\0" : "Medium\0", 8);
    char* dueDate = centerStringToColumnLength(task -> dueDate, 10);
    char* completionStatus = centerStringToColumnLength(task -> completed == true ? "Complete\0" : "Incomplete\0", 10);

    printf("|\033[1m%s\033[0m", index);
    printf("| %s ", taskName);
    taskPriority == 2 ? printf("|\033[31m %s \033[0m", priorityLabel) : printf("| %s ", priorityLabel);
    dueDatePassed ? printf("|\033[31m %s \033[0m", dueDate) : printf("| %s ", dueDate);
    task-> completed == true ? printf("|\033[32m %s \033[0m|\n", completionStatus): printf("| %s |\n", completionStatus);
    printf(ROW_SEPARATOR);
    free(i); free(index); free(taskName); free(priorityLabel); free(dueDate); free(completionStatus);
}

void listAllTasks(Task* firstTask){
    Task* currentTask = firstTask;

    printf(TABLE_HEADER);

    while(currentTask != NULL){
        printTaskLine(currentTask);
        currentTask = currentTask -> nextTask;
        fflush(stdout);
    }  
    printf("\n");
}

void listTasksByCompletion(Task* firstTask, int listLength){
    Task* complete[listLength];
    Task* incomplete[listLength];

    int completeIndex = 0;
    int incompleteIndex = 0;

    Task* currentTask = firstTask;

    while(currentTask != NULL){
        if(currentTask->completed){
            complete[completeIndex] = currentTask;
            completeIndex++;
        }else if(!currentTask->completed){
            incomplete[incompleteIndex] = currentTask;
            incompleteIndex++;
        }

        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
        }else{
            currentTask = NULL;
        }
    }

    if(incompleteIndex>0){
        printf("\n\033[1mIncomplete Tasks:\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<incompleteIndex; i++){
            currentTask = incomplete[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo incomplete tasks in list.\n");
    }

    if(completeIndex>0){
        printf("\n\033[1m\033[32mComplete Tasks:\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<completeIndex; i++){
            currentTask = complete[i];
            printTaskLine(currentTask);
        }
        printf("\n");
    }else{
        printf("\nNo complete tasks in list.\n\n");
    }
}

void listTasksByPriority(Task* firstTask, int listLength){  
    Task* highPriority[listLength];
    Task* medPriority[listLength];
    Task* lowPriority[listLength];

    int highIndex = 0;
    int medIndex = 0;
    int lowIndex = 0;

    Task* currentTask = firstTask;

    while(currentTask != NULL){
        if(currentTask->priority == HIGH){
            highPriority[highIndex] = currentTask;
            highIndex++;
        }else if(currentTask->priority == MEDIUM){
            medPriority[medIndex] = currentTask;
            medIndex++;
        }else if(currentTask->priority == LOW){
            lowPriority[lowIndex] = currentTask;
            lowIndex++;
        }

        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
        }else{
            currentTask = NULL;
        }
    }

    if(highIndex>0){
        printf("\n\033[1mPriority: \033[31mHigh\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<highIndex; i++){
            currentTask = highPriority[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo high priority tasks in list.\n");
    }

    if(medIndex>0){
        printf("\n\033[1mPriority: Medium\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<medIndex; i++){
            currentTask = medPriority[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo medium priority tasks in list.\n");
    }

    if(lowIndex>0){
        printf("\n\033[1mPriority: Low\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<lowIndex; i++){
            currentTask = lowPriority[i];
            printTaskLine(currentTask);
        }
        printf("\n");
    }else{
        printf("\nNo low priority tasks in list.\n\n");
    }
}

void printTaskList(Task* firstTask, int listLength){
    if(firstTask == NULL){
        printf("\nTask list is empty, nothing to display.\n\n");
        return;
    }

    char userSelection;

    printf("\nSelect list display option\n");
    while(userSelection != 'M'){
        userSelection = getChar(LIST_MENU);
        switch(userSelection){
            case 'L':
                listAllTasks(firstTask);
                break;
            case 'P':
                listTasksByPriority(firstTask, listLength);
                break;
            case 'C':
                listTasksByCompletion(firstTask, listLength);
                break;
            case 'D':
                printf("Date TBD\n");
                break;
            case 'M':
                printf("Returning to main menu...\n\n");
                break;
            default:
                printf("Invalid input, please enter L, P, C, D or M.\n");
                break;
        }
    }
    return;
}