#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "Task.h"

#define NUM_BASE 10
#define LIST_MENU "[L]ist all tasks | By [P]riority | By [C]ompletion | By [D]ate | [R]eturn: "
#define TABLE_HEADER "\n|-\033[1m#\033[0m-|-------------------------\033[1mTask Name\033[0m------------------------|-\033[1mPriority\033[0m-|--\033[1mDue Date\033[0m--|-\033[1mCompletion\033[0m-|\n"
#define ROW_SEPARATOR "|---|----------------------------------------------------------|----------|------------|------------|\n"
#define INDEX_PADDING 3
#define TASK_NAME_PADDING 56
#define PRIORITY_PADDING 8
#define DUE_DATE_PADDING 10
#define COMPLETION_PADDING 10


static bool checkIfDateHasPassed(Date* dueDate){
    if(dueDate->year == 0){
        return false;
    }

    time_t currentTime;
    struct tm* timeInfo;
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    int day = timeInfo -> tm_mday;
    int month = timeInfo -> tm_mon + 1;
    int year = timeInfo ->tm_year + 1900;

    if (dueDate->year > year){
        return false;
    }else if (dueDate->year == year){
        if(dueDate->month > month){
            return false;
        }else if (dueDate->month < month){
            return true;
        }else if (dueDate->month == month){
            if(dueDate->day > day){
                return false;
            }else{
                return true;
            }
        }
    }
}

static char* centerStringToColumnLength(char* str, int len){
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

static char* padStringToColumnLength(char* str, int len){
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

static void printTaskLine(Task* task){
    bool dueDatePassed = checkIfDateHasPassed(task->dueDateStruct);
    Description* description = task -> description;
    int taskPriority = task->priority;
    char* i = malloc(6 * sizeof(char));
    snprintf(i, 6, "%d", task->index);
    char* index = centerStringToColumnLength(i, INDEX_PADDING);
    char* taskName = padStringToColumnLength(description -> data, TASK_NAME_PADDING);
    char* priorityLabel = centerStringToColumnLength(taskPriority == 0 ? "Low\0" : taskPriority == 2 ? "High\0" : "Medium\0", PRIORITY_PADDING);
    char* dueDate = calloc(10, sizeof(char));
    if(task->dueDateStruct->year == 0){
        dueDate = centerStringToColumnLength("None", DUE_DATE_PADDING);
    }else{
        sprintf(dueDate, "%02i/%02i/%i", task->dueDateStruct->month, task->dueDateStruct->day, task->dueDateStruct->year);
        char* dueDateOld = dueDate;
        dueDate = centerStringToColumnLength(dueDate, DUE_DATE_PADDING);
        free(dueDateOld);
    }
    char* completionStatus = centerStringToColumnLength(task -> completed == true ? "Complete\0" : "Incomplete\0", COMPLETION_PADDING);

    printf("|\033[1m%s\033[0m", index);
    printf("| %s ", taskName);
    taskPriority == 2 ? printf("|\033[31m %s \033[0m", priorityLabel) : printf("| %s ", priorityLabel);
    dueDatePassed ? printf("|\033[31m %s \033[0m", dueDate) : printf("| %s ", dueDate);
    task-> completed == true ? printf("|\033[32m %s \033[0m|\n", completionStatus): printf("| %s |\n", completionStatus);
    printf(ROW_SEPARATOR);
    free(i); free(index); free(taskName); free(priorityLabel); free(dueDate); free(completionStatus);
}

static void listAllTasks(Task* firstTask){
    Task* currentTask = firstTask;

    printf(TABLE_HEADER);

    while(currentTask != NULL){
        printTaskLine(currentTask);
        currentTask = currentTask -> nextTask;
        fflush(stdout);
    }  
    printf("\n");
}

//Returns 1 if newDate is earlier than originalDate, 0 if not, tie returns 0
static int compareDates(Date* originalDate, Date* newDate){
    if(newDate->year<originalDate->year){
        return 1;
    }else if(newDate->year>originalDate->year){
        return 0;
    }else{
        if(newDate->month<originalDate->month){
            return 1;
        }else if (newDate->month>originalDate->month){
            return 0;
        }else{
            if(newDate->day<originalDate->day){
                return 1;
            }else if (newDate->day>originalDate->month){
                return 0;
            }
        }
    }
    return 0;
}

static void listTasksbyDate(Task* firstTask, int listLength){
    Task* noDate[listLength];
    Task* withDate[listLength];

    int noDateIndex = 0;
    int withDateIndex = 0;

    Task* currentTask = firstTask;

    while(currentTask != NULL){
        if(currentTask-> dueDateStruct == NULL || currentTask->dueDateStruct->year == 0){
            noDate[noDateIndex] = currentTask;
            noDateIndex++;
        }else if (currentTask->dueDateStruct != NULL && currentTask->dueDateStruct->year > 0){
            int i = 0;
            while(i < withDateIndex || withDateIndex == 0){
                if(withDateIndex == 0){
                    withDate[i] = currentTask;
                    withDateIndex++;
                    break;
                }else{
                    int d = compareDates(withDate[i]->dueDateStruct, currentTask->dueDateStruct);
                    if(d == 1){
                        for(int y = withDateIndex; y>i; y--){
                            withDate[y] = withDate[y-1];
                        }
                        withDate[i] = currentTask;
                        withDateIndex++;
                        break;
                    }else{
                        i++;
                        if (i == withDateIndex){
                            withDate[i] = currentTask;
                            withDateIndex++;
                            break;
                        }
                    }
                }                
            }
        }

        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
        }else{
            currentTask = NULL;
        }
    }

    if(withDateIndex>0){
        printf("\n\033[1mTasks with due dates:\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<withDateIndex; i++){
            currentTask = withDate[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo tasks with due dates in list.\n");
    }

    if(noDateIndex>0){
        printf("\n\033[1mTasks without due dates:\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<noDateIndex; i++){
            currentTask = noDate[i];
            printTaskLine(currentTask);
        }
        printf("\n");
    }else{
        printf("\nNo tasks without due dates in list.\n\n");
    }
}

static void listTasksByCompletion(Task* firstTask, int listLength){
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

static void listTasksByPriority(Task* firstTask, int listLength){  
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

void listTasksHandler(Task* firstTask, int listLength){
    if(firstTask == NULL){
        printf("\nTask list is empty, nothing to display.\n\n");
        return;
    }

    char userSelection;

    printf("\nSelect list display option\n");
    while(userSelection != 'R'){
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
                listTasksbyDate(firstTask, listLength);
                break;
            case 'R':
                printf("Returning to main menu...\n\n");
                break;
            default:
                printf("Invalid input, please enter L, P, C, D or R.\n");
                break;
        }
    }
    return;
}