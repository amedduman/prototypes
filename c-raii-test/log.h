#pragma once
#include <stdio.h>

typedef struct 
{
    int my_value;
}myLog;

void log_clean_up(myLog* log) 
{
    printf("hello from log clean up\n");
}

void log_write()
{
    printf("hello from log\n");
}