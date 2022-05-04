#include <string.h>
#include <ctype.h>
#include "string_ext.h"

#ifndef HAVE_STRUPR
char *strupr(char *s)
{
    char *t = s;
    while ((*t = toupper(*t)) != '\0') ++t;
    return s;
}
#endif /* !HAVE_STRUPR */

#ifndef HAVE_STRCMPI
int strcmpi(const char *a, const char *b)
{
    char c1, c2;
    do {
        c1 = toupper(*a++);
        c2 = toupper(*b++);
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}
#endif /* !HAVE_STRCMPI */
