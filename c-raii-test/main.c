# include <stdio.h>
#include "log.h"
#include "auto_cleanup_macro.h"
void clean_up(int *final_value)
{
    printf("Cleaning up\n");
    printf("Final value: %d\n",*final_value);
}

// command line to run code clear && clang main.c -o main && ./main

/* // Macro to use __cleanup__ attribute
#define AUTO_CLEANUP(var, cleanup_func) \
    var __attribute__((__cleanup__(cleanup_func))) */

 int main(int argc, char **argv)
 {
     /* declare cleanup attribute along with initiliazation
     Without the cleanup attribute, this is equivalent
     to:int avar = 1;
     int avar __attribute__ ((__cleanup__(clean_up))) = 1;
     avar = 5;
     */

    AUTO_CLEANUP(int avar2, clean_up) = 1;
    avar2 = 5;

    AUTO_CLEANUP(myLog log, log_clean_up);

    log_write();

     return 0;
 }
