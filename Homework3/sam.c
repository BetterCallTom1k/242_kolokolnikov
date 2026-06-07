#include <stdio.h>
#include <math.h>
#include <string.h>

void sam(char *arg)
{
    size_t len = strlen(arg);
    printf("sam: you passed \"%s\" (len=%zu, sqrt=%.4f)\n",
           arg, len, sqrt((double)len));
}
