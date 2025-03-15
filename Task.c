#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "Task.h"

Description* createDescription(char* input){
    Description* desc = (Description*)calloc(1, sizeof(Description));

    if (desc == NULL){
        return NULL;
    }

    int len = strlen(input);
    desc -> data = (char*)calloc((len + 1 ), sizeof(char));

    if (desc -> data == NULL){
        free(desc);
        return NULL;
    }

    if(strcspn(input, "\n") < strlen(input)){
        input[strcspn(input, "\n")] = '\0';    
    }    
    
    strcpy(desc->data, input);
    desc -> length = len;

    return desc;
}

void destructDescription(Description* desc){
    free(desc -> data);
    free(desc);
}