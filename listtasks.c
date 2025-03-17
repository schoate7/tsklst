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
    return false;
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

void printTaskLine(Task* task){
    bool dueDatePassed = checkIfDateHasPassed(task->dueDate);
    int taskPriority = task->priority;
    char* i = malloc(6 * sizeof(char));
    snprintf(i, 6, "%d", task->index);
    char* index = centerStringToColumnLength(i, INDEX_PADDING);
    char* taskName = padStringToColumnLength(task->name, TASK_NAME_PADDING);
    char* priorityLabel = centerStringToColumnLength(taskPriority == 0 ? "Low\0" : taskPriority == 2 ? "High\0" : "Medium\0", PRIORITY_PADDING);
    if(task->completed == 1){
        free(priorityLabel);
        priorityLabel = centerStringToColumnLength(" -- ", PRIORITY_PADDING);
    }
    char* dueDate = calloc(10, sizeof(char));
    if(task->dueDate->year == 0){
        dueDate = centerStringToColumnLength("None", DUE_DATE_PADDING);
    }else{
        sprintf(dueDate, "%02i/%02i/%i", task->dueDate->month, task->dueDate->day, task->dueDate->year);
        dueDate = centerStringToColumnLength(dueDate, DUE_DATE_PADDING);
    }
    char* completionStatus = centerStringToColumnLength(task -> completed == true ? "Complete\0" : "Incomplete\0", COMPLETION_PADDING);

    printf("|\033[1m%s\033[0m", index);
    printf("| %s ", taskName);

    if (taskPriority == 2 && task->completed == 0){
        printf("|\033[33m %s \033[0m", priorityLabel);
    }else if (taskPriority == 0 && task->completed == 0){
        printf("|\033[34m %s \033[0m", priorityLabel);
    }else if(taskPriority == 1 && task->completed == 0){
        printf("|\033[35m %s \033[0m", priorityLabel);
    }else{
        printf("| %s ", priorityLabel);
    }
    if(dueDatePassed && task->completed == 0){
        printf("|\033[31m %s \033[0m", dueDate);
        printf("|\033[31m %s \033[0m|\n", completionStatus);
    }else if (task->completed == 1){
        dueDate = centerStringToColumnLength(" -- ", DUE_DATE_PADDING);
        printf("| %s ", dueDate);
        printf("| \033[32m%s \033[0m|\n", completionStatus);
    }else{
        printf("| %s ", dueDate);
        printf("| %s |\n", completionStatus);
    }

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

// If originalDate > newDate, returns 1; 0 if not
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
    Task* dueDatePassed[listLength];
    Task* futureDueDate[listLength];

    int noDateIndex = 0;
    int dueDatePassedIndex = 0;
    int futureDateIndex = 0;

    Task* currentTask = firstTask;

    while(currentTask != NULL){
        if(currentTask-> dueDate == NULL || currentTask->dueDate->year == 0 && currentTask->completed == 0){
            noDate[noDateIndex] = currentTask;
            noDateIndex++;
        }else if (currentTask->dueDate != NULL && currentTask->dueDate->year > 0 
                    && checkIfDateHasPassed(currentTask->dueDate) == true && currentTask->completed == 0){
            int i = 0;
            while(i < dueDatePassedIndex || dueDatePassedIndex == 0){
                if(dueDatePassedIndex == 0){
                    dueDatePassed[i] = currentTask;
                    dueDatePassedIndex++;
                    break;
                }else{
                    int d = compareDates(dueDatePassed[i]->dueDate, currentTask->dueDate);
                    if(d == 1){
                        for(int y = dueDatePassedIndex; y>i; y--){
                            dueDatePassed[y] = dueDatePassed[y-1];
                        }
                        dueDatePassed[i] = currentTask;
                        dueDatePassedIndex++;
                        break;
                    }else{
                        i++;
                        if (i == dueDatePassedIndex){
                            dueDatePassed[i] = currentTask;
                            dueDatePassedIndex++;
                            break;
                        }
                    }
                }                
            }
        }else if (currentTask->dueDate != NULL && currentTask->dueDate->year > 0 
                    && checkIfDateHasPassed(currentTask->dueDate) == false && currentTask->completed == 0){
            int i = 0;
            while(i < futureDateIndex || futureDateIndex == 0){
                if(futureDateIndex == 0){
                    futureDueDate[i] = currentTask;
                    futureDateIndex++;
                    break;
                }else{
                    int d = compareDates(futureDueDate[i]->dueDate, currentTask->dueDate);
                    if(d == 1){
                        for(int y = futureDateIndex; y>i; y--){
                            futureDueDate[y] = futureDueDate[y-1];
                        }
                        futureDueDate[i] = currentTask;
                        futureDateIndex++;
                        break;
                    }else{
                        i++;
                        if (i == futureDateIndex){
                            futureDueDate[i] = currentTask;
                            futureDateIndex++;
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

    if(dueDatePassedIndex>0){
        printf("\n\033[1mTasks with \033[31mpast due\033[0m\033[1m dates:\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<dueDatePassedIndex; i++){
            currentTask = dueDatePassed[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo tasks with past due dates in list.\n");
    }

    if(futureDateIndex>0){
        printf("\n\033[1mTasks with future due dates:\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<futureDateIndex; i++){
            currentTask = futureDueDate[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo tasks with future due dates in list.\n");
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
        printf("\n\033[1m\033[31mIncomplete\033[0m\033[1m Tasks:\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<incompleteIndex; i++){
            currentTask = incomplete[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo incomplete tasks in list.\n");
    }

    if(completeIndex>0){
        printf("\n\033[1m\033[32mComplete\033[0m Tasks:");
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
        if(currentTask->completed == 0){
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
        }

        if(currentTask->nextTask != NULL){
            currentTask = currentTask->nextTask;
        }else{
            currentTask = NULL;
        }
    }

    if(highIndex>0){
        printf("\n\033[1mPriority: \033[33mHigh\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<highIndex; i++){
            currentTask = highPriority[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo high priority tasks in list.\n");
    }

    if(medIndex>0){
        printf("\n\033[1mPriority: \033[35mMedium\033[0m");
        printf(TABLE_HEADER);
        for(int i=0; i<medIndex; i++){
            currentTask = medPriority[i];
            printTaskLine(currentTask);
        }
    }else{
        printf("\nNo medium priority tasks in list.\n");
    }

    if(lowIndex>0){
        printf("\n\033[1mPriority: \033[34mLow\033[0m");
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