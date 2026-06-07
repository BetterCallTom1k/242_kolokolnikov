#ifdef LINKTIME
#include <stdio.h>
#include "lib.h"

void __real_fred(int arg);
void __real_john(int arg);

void __wrap_fred(int arg)
{
    printf("fred is called\n");
    __real_fred(arg);
}

void __wrap_john(int arg)
{
    printf("john is called\n");
    __real_john(arg);
}
#endif
