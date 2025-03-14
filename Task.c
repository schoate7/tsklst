#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum Priority{
    LOW,
    MEDIUM,
    HIGH
};

typedef struct Description{
    char* data;
    int length;

}Description;

typedef struct Task{
    Description* description;
    char* dueDate;
    int index;
    bool completed;
    enum Priority priority;

    struct Task *previousTask;
    struct Task *nextTask;

}Task;

typedef struct EditResult{
    Task* firstTask;
    Task* lastTask;
    int listLength;
}EditResult;

typedef struct TaskGroup{
    Task* taskPtr;
    Task* nextTask;
    
}TaskGroup;

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