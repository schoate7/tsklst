#ifndef Task_h
#define Task_h

enum Priority{
    LOW,
    MEDIUM,
    HIGH
};

typedef struct Date{
    int year;
    int month;
    int day;
}Date;

typedef struct Task{
    int index;
    char* name;
    enum Priority priority;
    Date* dueDate;
    int completed;

    struct Task *previousTask;
    struct Task *nextTask;
}Task;

#endif
