#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_BASE 10
#define DAY_MON_LEN 2
#define YEAR_LEN 4
#define DATE_LEN 10

char* getString(char prompt[]){
    char* input = (char*)malloc(128);
    printf("%s", prompt);
    scanf(" ");
    if(fgets(input, 128, stdin) == NULL){
        free(input);
        return NULL;
    }else{
        input[strcspn(input, "\n")] = '\0';    

        return input;
    }
}

char getChar(char prompt[]){
    char c = ' ';
    char *input = (char*)malloc(64);
    printf("%s", prompt);
    scanf(" ");
    fgets(input, 64, stdin);
    if(input != NULL){
        c = toupper(input[0]);
        free(input);
    }
    return c;
}

int getInt(char prompt[]){
    int i = 0;
    char *input = (char*)malloc(64);
    char* endPtr;
    printf("%s", prompt);
    scanf(" ");
    fgets(input, 64, stdin);
    if(input != NULL){
        i = strtol(input, &endPtr, NUM_BASE);
        free(input);
    }
    return i;
}

int getFloat(char prompt[]){
    float f = 0.0;
    char *input = (char*)malloc(64);
    char* endPtr;
    printf("%s", prompt);
    scanf(" ");
    fgets(input, 64, stdin);
    if(input != NULL){
        f = strtod(input, &endPtr);
        free(input);
    }
    return f;
}

char* getDueDate(char* prompt){
    char* dueDate = malloc(DATE_LEN * sizeof(char));
    char* dueDateEndPtr;
    char* dueMonth = malloc(DAY_MON_LEN * sizeof(char));
    char* dueMonthEndPtr;
    char* dueDay = malloc(DAY_MON_LEN * sizeof(char));
    char* dueDayEndPtr;
    char* dueYearIn;
    char* dueYear = malloc(YEAR_LEN * sizeof(char));
    char* dueYearEndPtr;

    if(dueDate == NULL || dueMonth == NULL || dueDay == NULL || dueYear == NULL){
        printf("Error allocating memory, returning.\n\n");
        return "None";
    }

    char dueDateResponse;

    dueDateResponse = getChar(prompt);
    
    if(dueDateResponse == 'Y'){    
        bool futureDate = false;
        int inputLength;
        int dueDayInt, dueMonthInt, dueYearInt;

        time_t currentTime;
        struct tm* timeInfo;
        time(&currentTime);
        timeInfo = localtime(&currentTime);
        int day = timeInfo -> tm_mday;
        int month = timeInfo -> tm_mon + 1;
        int year = timeInfo ->tm_year + 1900;
        while(!futureDate){
            inputLength = 0;
            while(inputLength<1 || inputLength > 2){
                dueDay = getString("Enter day: ");
                inputLength = strlen(dueDay);
                if(inputLength < 1 || inputLength > 2){
                    printf("Day must be one or two digits.\n");
                }else{
                    dueDayInt = strtol(dueDay, &dueDayEndPtr, NUM_BASE);
                    if(dueDayInt<1 || dueDayInt>31){
                        inputLength = 0;
                        printf("Day must be between 1 and 31\n");
                    }
                    if(inputLength == 1){
                        dueDay[1] = dueDay[0];
                        dueDay[0] = '0';
                    }
                }
            }
            inputLength = 0;
            while(inputLength < 1 || inputLength > 2){
                dueMonth = getString("Enter month: ");
                inputLength = strlen(dueMonth);
                if(inputLength < 1 || inputLength > 2){
                    printf("Month must be one or two digits.\n");
                }else{
                    dueMonthInt = strtol(dueMonth, &dueMonthEndPtr, NUM_BASE);
                    if(dueMonthInt<1 || dueMonthInt>12){
                        inputLength = 0;
                        printf("Month must be between 1 and 12");
                    }
                    if (inputLength == 1){
                        dueMonth[1] = dueMonth[0];
                        dueMonth[0] = '0';
                    }
                }
            }
            inputLength = 0;
            while(inputLength != 2 && inputLength != 4){
                dueYearIn = getString("Enter year: ");
                inputLength = strlen(dueYearIn);
                if(inputLength != 2 && inputLength !=4){
                    printf("Year must be two or four digits");
                }else{
                    if(inputLength == 2){
                        strcpy(dueYear, "20");
                        strcat(dueYear, dueYearIn);
                    }else{
                        strcpy(dueYear, dueYearIn);
                    }
                    dueYearInt = strtol(dueYear, &dueYearEndPtr, NUM_BASE);
                    if(dueYearInt < year){
                        inputLength = 0;
                        printf("Year in the past, must be current or future.\n");
                    }
                }
            }
            if (dueYearInt > year){
                futureDate = true;
            }else if (dueYearInt == year){
                if(dueMonthInt > month){
                    futureDate = true;
                }else if (dueMonthInt < month){
                    printf("Date in the past, due date must be in the future.\n");
                }else if (dueMonthInt == month){
                    if(dueDayInt > day){
                        futureDate = true;
                    }else{
                        printf("Date in the past, due date must be in the future.\n");
                    }
                }
            }
        }
        strcpy(dueDate, dueMonth);
        strcat(dueDate, "/");
        strcat(dueDate, dueDay);
        strcat(dueDate, "/");
        strcat(dueDate, dueYear);
    }else{
        strcpy(dueDate, "None");
    }
    free(dueDay); free(dueMonth); free(dueYear); free(dueYearIn);
    return dueDate;
}