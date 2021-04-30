#include <sys/auxv.h>

unsigned long __rap_cookie;

__attribute__((used)) __attribute__((constructor)) void __rap_init()
{
    unsigned long addr = getauxval(AT_RANDOM);

    // the 1st sizeof(void*) is used for the canary, we can use the 2nd
    addr+= sizeof(void*);
    __rap_cookie = *((unsigned long*)addr);
}