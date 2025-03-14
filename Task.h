#ifndef Task_h
#define Task_h
#include <stdbool.h>
#include <stdlib.h>

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

Description* createDescription(char* input);

void destructDescription(Description* desc);

#endif
