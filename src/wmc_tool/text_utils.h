/*
 * (C) 2022 copyright VoiceAge Corporation. All Rights Reserved.
 *
 * This software is protected by copyright law and by international treaties. The source code, and all of its derivations,
 * is provided by VoiceAge Corporation under the "ITU-T Software Tools' General Public License". Please, read the license file
 * or refer to ITU-T Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".
 *
 * Any use of this software is permitted provided that this notice is not removed and that neither the authors nor
 * VoiceAge Corporation are deemed to have made any representations as to the suitability of this software
 * for any purpose nor are held responsible for any defects of this software. THERE IS NO WARRANTY FOR THIS SOFTWARE.
 *
 * Authors: Guy Richard, Vladimir Malenovsky (Vladimir.Malenovsky@USherbrooke.ca)
 */

#ifndef __TEXT_UTILS_H__
#define __TEXT_UTILS_H__

#include <string.h> 
#include <ctype.h>  
#include "constants.h"

/*-------------------------------------------------------------------*
 * Global Typedefs
 *-------------------------------------------------------------------*/

class mem2str
{
  private:
    char chr, *ps, *pe;
    void init( char *ps, char *pe )
    {
        /* Save Character */
        chr = *pe;
        /* Replace by NUL Char */
        *pe = '\0';
        /* Save Pointers */
        mem2str::pe = pe;
        mem2str::ps = ps;
    }

  public:
    mem2str( char *ps, char *pe ) { init( ps, pe ); }
    mem2str( char *ps, size_t s ) { init( ps, ps + s ); }

    operator char *() { return ps; };
    ~mem2str() { *pe = chr; /* Restore Character */ }
};


/*-------------------------------------------------------------------*
 * Global Macros
 *-------------------------------------------------------------------*/

#define PREV_CHAR( str ) ( ( str )[-1] )
#define NEXT_CHAR( str ) ( ( str )[1] )

#define IS_EOL_CHAR( chr )      ( ( chr ) == CR_CHAR || ( chr ) == LF_CHAR )
#define IS_EOL_SEQ( str )       ( *( str ) == CR_CHAR && NEXT_CHAR( str ) == LF_CHAR )
#define IS_RESERVED_CHAR( chr ) ( ( chr ) == BLANK_CHAR || ( chr ) == DEL_CHAR )
#define IS_SPACE_CHAR( chr )    ( ( chr ) == SPACE_CHAR || ( chr ) == TAB_CHAR || \
                               IS_RESERVED_CHAR( chr ) )
#define IS_BLANK_CHAR( chr ) ( IS_SPACE_CHAR( chr ) || IS_EOL_CHAR( chr ) )

#define IS_DIGIT_CHAR( chr ) ( ( chr ) >= '0' && ( chr ) <= '9' )
#define IS_LOWER_CHAR( chr ) ( ( chr ) >= 'a' && ( chr ) <= 'z' )
#define IS_UPPER_CHAR( chr ) ( ( chr ) >= 'A' && ( chr ) <= 'Z' )
#define IS_ALPHA_CHAR( chr ) ( IS_LOWER_CHAR( chr ) || IS_UPPER_CHAR( chr ) )

/*
 * Characters Accepted are:
 *   Alphanumerics
 *   Digits
 *   Underscore
 *   Dollar Sign
 *
 * see: http://msdn.microsoft.com/en-us/library/565w213d.aspx
 */

#define IS_IDENTIFIER_START_CHAR( chr ) ( IS_ALPHA_CHAR( chr ) || ( chr ) == '_' || ( chr ) == '$' )
#define IS_IDENTIFIER_CHAR( chr )       ( IS_IDENTIFIER_START_CHAR( chr ) || IS_DIGIT_CHAR( chr ) )
#define IS_CHAR_IN_STRING( chr, str ) ( strchr( str, ( chr ) ) != NULL && ( chr ) != NUL_CHAR )

#define memstr( ps, end ) ( (char *) mem2str( ps, end ) )

inline char *Skip_Identifier( char *ptr, MOVE_DIRECTION dir = FORWARD )
{
    while ( ptr += dir, IS_IDENTIFIER_CHAR( *ptr ) )
        ;
    return ptr;
}

/*-------------------------------------------------------------------*
 * Prototypes
 *-------------------------------------------------------------------*/

char *strcsub( char *str, const char chr1, const char chr2 );
char *strrpbrk( const char *str1, const char *str2 );
char *r_strpbrk( const char *str1, const char *str2 );
char *strpbrkn( const char *str1, const char *str2 );
char *r_strpbrkn( const char *str1, const char *str2 );
char *strnistr( const char *str1, const char *str2, size_t len );
int strlno( const char *str );
int strcno( const char *str );
char *strlend( const char *str );
char *strnechr( char *str, char chr );

char *memwordcmp( const char *mem, const char *words );
char *memwordicmp( const char *mem, const char *words );
char *strwordistr( const char *mem, const char *words );
char *mempwordicmp( const char *mem, const char *words );

char *itos( char *str, int value );

char *Fix_FileName( char *ptr );

char* strip(char* str, char c);
int Extract_Value_From_File(
    const char* filename,               /* pathname to an existing text file */
    const char* line_keyword            /* unique string to search for within the file (the value will be extracted on the same line, after this keyword) */
);
char* Extract_String_From_File(
    const char* filename,               /* pathname to an existing text file */
    const char* line_keyword,           /* unique string to search for within the file (the string following this keyword will be extracted) */
    char* return_str                    /* returned string */
);

inline char *stristr( const char *str1, const char *str2 )
{
    return strnistr( str1, str2, strlen( str2 ) );
}

inline char *strupr( const char *str1, const char *str2 )
{
    char* ptr_str1, *ptr_str2;
    ptr_str1 = (char*)str1;
    ptr_str2 = (char*)str2;

    while ( ( *ptr_str1++ = toupper( *ptr_str2++ ) ) != NUL_CHAR )
        ;
    return ptr_str1;
}

inline char *strstr2( const char *str1, const char *str2 )
{
    char *ptr = (char *) strstr( str1, str2 );
    return ptr;
}

inline char *strpbrk2( const char *str, const char *chars )
{
    char *ptr = (char *) strpbrk( str, chars );
    return ptr;
}

inline char *r_strpbrk2( const char *str, const char *chars )
{
    char *ptr = r_strpbrk( str, chars );
    return ptr;
}

#endif /* #ifndef __TEXT_UTILS_H__ */
