#ifndef STRING_EXT_H
#define STRING_EXT_H

#ifdef _MSC_VER
#   define HAVE_STRUPR
#   define HAVE_STRCMPI
#endif

#ifdef __CYGWIN__
#   define HAVE_STRUPR
#endif

#ifndef HAVE_STRUPR
extern char *strupr(char *s);
#endif /* !HAVE_STRUPR */

#ifndef HAVE_STRCMPI
extern int strcmpi(const char *a, const char *b);
#endif /* !HAVE_STRCMPI */

#endif /* !STRING_EXT_H */
