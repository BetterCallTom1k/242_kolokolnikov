/* Link-time interpositioning for fred and john.
 * Compiled with -DLINKTIME; linked with -Wl,--wrap,fred -Wl,--wrap,john.
 * The linker rewrites: calls to fred -> __wrap_fred,
 *                       calls to __real_fred -> original fred.
 */
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
