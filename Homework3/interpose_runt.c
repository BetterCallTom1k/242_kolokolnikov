#ifdef RUNTIME
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include "lib.h"

void bill(char *arg)
{
    void (*real_bill)(char *) = dlsym(RTLD_NEXT, "bill");
    printf("bill is called\n");
    real_bill(arg);
}

void sam(char *arg)
{
    void (*real_sam)(char *) = dlsym(RTLD_NEXT, "sam");
    printf("sam is called\n");
    real_sam(arg);
}
#endif
