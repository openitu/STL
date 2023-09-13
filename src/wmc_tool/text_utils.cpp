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

#include <string.h>     
#include <stdlib.h>    
#include <stdio.h>     
#include "text_utils.h" 
#include "wmc_tool.h"  
#include "constants.h"  


#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef _MSC_VER 
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#endif

/*-------------------------------------------------------------------*
 * strcsub (string character substitute)
 *
 * Replaces all Occurrences of 'chr1' with 'chr2'
 *
 * NOTES: This Function returns a 'char *'
 *-------------------------------------------------------------------*/
char *strcsub( char *str, const char chr1, const char chr2 )
{
    char *ptr = str;

    while ( ( ptr = strchr( ptr, chr1 ) ) != NULL )
    {
        *ptr++ = chr2;
    }

    /* Return Pointer to String */
    return str;
}

/*-------------------------------------------------------------------*
 * strrpbrk (reverse strpbrk)
 *
 * Searches (backwards) in 'str1' for the 1st Occurrence
 *     of any Character in 'str2'.
 *
 * NOTES: This Function returns a 'char *' like 'strpbrk'
 *-------------------------------------------------------------------*/
char *strrpbrk( const char *str1, const char *str2 )
{
    /* Find End of String */
    char *ptr = (char *) strchr( str1, NUL_CHAR );

    /*
    * Search 'str1' backwards for 1st Occurence of Chars in 'str2'
    * When 'str1' is Reached, the loop will end.
    */
    while ( --ptr >= str1 && strchr( str2, *ptr ) == NULL )
        ;

    /* Start of String? */
    if ( ptr < str1 )
    { /* Yes */
        return NULL;
    }

    /* Return Pointer to Character Found */
    return ptr;
}

/*-------------------------------------------------------------------*
 * r_strpbrk (reverse strpbrk)
 *
 * Searches (backwards) in 'str1' for the 1st Occurrence
 *     of any Character in 'str2'.
 *
 * NOTES: 1) String must be Prefixed by the NUL Char
 *        2) This Function returns a 'char *' like 'strpbrk'
 *-------------------------------------------------------------------*/
char *r_strpbrk( const char *str1, const char *str2 )
{
    /*
    * Search 'str1' backwards for 1st Occurence of Chars in 'str2'
    * When '*str1' is NUL, the loop will end.
    */
    while ( strchr( str2, *str1 ) == NULL )
    {
        str1--;
    }

    /* End of String? */
    if ( *str1 == NUL_CHAR )
    { /* Yes */
        return NULL;
    }

    /* Return Pointer to Character Found */
    return (char *) str1;
}

/*-------------------------------------------------------------------*
 * strpbrkn
 *
 * Searches in 'str1' for the 1st Occurrence
 *   of any Character NOT in 'str2'.
 *
 * NOTES: 1) This Function returns a 'char *' like 'strpbrk'
 *-------------------------------------------------------------------*/
char *strpbrkn( const char *str1, const char *str2 )
{
    /*
    * Search 'str1' for 1st Occurence of Chars NOT in 'str2'
    */
    str1 += strspn( str1, str2 );

    /* End of String? */
    if ( *str1 == NUL_CHAR )
    { /* Yes */
        return NULL;
    }

    /* Return Pointer to Character Found */
    return (char *) str1;
}

/*-------------------------------------------------------------------*
 * r_strpbrkn (reverse strpbrkn)
 *
 * Searches (backwards) in 'str1' for the 1st Occurrence
 *     of any Character NOT in 'str2'.
 *
 * NOTES: 1) String must be Prefixed by the NUL Char
 *        2) This Function returns a 'char *' like 'strpbrk'
 *-------------------------------------------------------------------*/
char *r_strpbrkn( const char *str1, const char *str2 )
{
    /*
    * Search 'str1' backwards for 1st Occurence of Chars NOT in 'str2'
    * When '*str1' is NUL, the loop will end.
    */
    while ( *str1 != NUL_CHAR && strchr( str2, *str1 ) != NULL )
    {
        str1--;
    }

    /* End of String? */
    if ( *str1 == NUL_CHAR )
    { /* Yes */
        return NULL;
    }

    /* Return Pointer to Character Found */
    return (char *) str1;
}

/*-------------------------------------------------------------------*
 * strnistr (Case Insensitive Substring Locate)
 *-------------------------------------------------------------------*/
char *strnistr( const char *str1, const char *str2, size_t len2 )
{
    char chr = toupper( *str2 );

    /* Adjust */
    str2++;
    len2--;
    do
    {
        /* Find 1st Char of 'str2' in 'str1' */
        while ( toupper( *str1 ) != chr && *str1 != NUL_CHAR )
            str1++;
        /* End of String? */
        if ( *str1 == NUL_CHAR )
        { /* Yes */
            return NULL;
        }
        /* Advance (to next Char of 'str1') */
        str1++;
    } while ( _strnicmp( str1, str2, len2 ) != 0 );

    return (char *) str1 - 1;
}

/*-------------------------------------------------------------------*
 * strcno (String Column Number)
 *
 * Count the Number of Columns from the Beginning of the Line String.
 *
 * NOTES: 1) String must be Prefixed by CR or LF & the NUL Char
 *        2) This Function Never Fails (returns 0 if empty string)
 *-------------------------------------------------------------------*/
int strcno( const char *str )
{
    char* p_str = (char*)str;

    /* Columns are Numbered from 1 to n */
    int n = 0;

    while ( *p_str != NUL_CHAR )
    {
        p_str--;
        if ( *p_str != DEL_CHAR ) /* Do no Count Deleted Characters*/
        {
            n++;
        }
        /* Is Character CR or LF? */
        if ( IS_EOL_CHAR( *p_str) )
        { /* Yes */
            break;
        }
    }

    return n;
}

/*-------------------------------------------------------------------*
 * strlno (String Line Number)
 *
 * Count the Number of Lines from the Beginning of the File String.
 *
 * NOTES: 1) String must be Prefixed by CR or LF & the NUL Char
 *        2) This Function Never Fails (returns 0 if empty string)
 *-------------------------------------------------------------------*/
int strlno( const char *str )
{
    char* p_str = (char *)str;

    /* Lines are Numbered from 1 to n */
    int line_no;

    p_str--;

    for ( line_no = 0; *p_str != NUL_CHAR; line_no++ )
    {
        /* Find CR or LF? */
        while ( !IS_EOL_CHAR( *p_str ) )
        {
            p_str--;
        }

        /* One Skip for the EOL Character itself */
        p_str--;
        /* is it is a CR+LF? */
        if ( IS_EOL_SEQ( p_str ) )
        { /* Yes */
            /* One more Skip for the CR */
            p_str--;
        }
    }

    return line_no;
}

/*-------------------------------------------------------------------*
 * strlend (String Line End)
 *
 * Find String Line End.
 *
 * NOTE: Return Pointer to Last CR or LF of Line or
 *       NULL if End of String is Reached before Line End.
 *-------------------------------------------------------------------*/
char *strlend( const char *str )
{
    char* ptr_str = (char*)str;

    /* Search for CR or LF */
    if ( (ptr_str = strpbrk(ptr_str, EOL_CHARS ) ) != NULL )
    {
        /* Skip the LF if it is a CR+LF */
        if ( IS_EOL_SEQ(ptr_str) )
        {
            /* One Extra Skip for 2nd Char */
            ptr_str++;
        }
        /* Return Pointer to Last CR or LF of Line */
    }

    return ptr_str;
}

/*-------------------------------------------------------------------*
 * strnechr (String Not Escaped Char)
 *
 * Searches String for the 1st Occurrence of Character 'chr'
 * that is Not Prefixed by the ESCAPE Char.
 *-------------------------------------------------------------------*/
char *strnechr( char *str, char chr )
{
    char temp[] = { ESCAPE_CHAR, chr, NUL_CHAR };

    str--;
    do
    {
        str = strpbrk( str + 2, temp );
    } while ( str != NULL && *str == ESCAPE_CHAR );

    return str;
}

/*-------------------------------------------------------------------*
 * memwordcmp (Memory Word Compare)
 *
 * Compare Memory to see if it Matches any Words in
 *   "Space Separated String".
 *-------------------------------------------------------------------*/
char *memwordcmp( const char *mem, const char *words )
{
    char *ptr, *ptr_words;

    ptr_words = (char *)words;

    while ( *ptr_words != NUL_CHAR )
    {
        ptr = (char *)mem;
        /* Compare */
        while ( *ptr == *ptr_words && *ptr_words != SPACE_CHAR )
        {
            ptr++;
            ptr_words++;
        }
        /* Compare Succeeded? */
        if ( *ptr_words == SPACE_CHAR || *ptr_words == NUL_CHAR )
        { /* Yes */
            /* Memory Word End? */
            if ( !IS_IDENTIFIER_CHAR( *ptr ) )
            { /* Yes */
                /* Done */
                return ptr;
            }
        }
        /* Advance */
        ptr_words += strcspn(ptr_words, SPACE_CHARS );
        ptr_words += strspn(ptr_words, SPACE_CHARS );
    }

    return NULL;
}

/*-------------------------------------------------------------------*
 * memwordicmp (Memory Word Case Insensitive Compare)
 *
 * Compare Memory to see if it Matches any Words in
 *   "Space Separated String".
 *-------------------------------------------------------------------*/
char *memwordicmp( const char *mem, const char *words )
{
    char *ptr, *ptr_words;

    ptr_words = (char *)words;

    while ( *ptr_words != NUL_CHAR )
    {
        ptr = (char *)mem;

        /* Compare */
        while ( toupper( *ptr ) == toupper( *ptr_words) && *ptr_words != SPACE_CHAR )
        {
            ptr++;
            ptr_words++;
        }
        /* Compare Succeeded? */
        if ( *ptr_words == SPACE_CHAR || *ptr_words == NUL_CHAR )
        { /* Yes */
            /* Memory Word End? */
            if ( !IS_IDENTIFIER_CHAR( *ptr ) )
            { /* Yes */
                /* Done */
                return ptr;
            }
        }
        /* Advance */
        ptr_words += strcspn(ptr_words, SPACE_CHARS );
        ptr_words += strspn(ptr_words, SPACE_CHARS );
    }

    return NULL;
}

/*-------------------------------------------------------------------*
 * strwordistr (String Word Case Insensitive Substring Locate)
 *
 * Compare Memory to see if it Matches any Substring Words in
 *   "Space Separated String".
 *-------------------------------------------------------------------*/
char *strwordistr( const char *mem, const char *words )
{
    size_t len;
    char* ptr_words;

    ptr_words = (char*)words;

    while ( *ptr_words != NUL_CHAR )
    {
        /* Get Length of Word */
        len = strcspn(ptr_words, SPACE_CHARS );

        /* Compare */
        if ( strnistr( mem, ptr_words, len ) != NULL )
            return ptr_words;

        /* Advance */
        ptr_words += len;
        ptr_words += strspn(ptr_words, SPACE_CHARS );
    }

    return NULL;
}

/*-------------------------------------------------------------------*
 * mempwordicmp (Memory Prefix Word Case Insensitive Compare)
 *
 * Compare Memory to see if it Matches any Prefix Words in
 *   "Space Separated String".
 *-------------------------------------------------------------------*/
char *mempwordicmp( const char *mem, const char *words )
{
    char *ptr, *ptr_words;

    ptr_words = (char*)words;
    while ( *ptr_words != NUL_CHAR )
    {
        ptr = (char *)mem;

        /* Compare */
        while ( toupper( *ptr ) == toupper( *ptr_words) && *ptr_words != SPACE_CHAR )
        {
            ptr++;
            ptr_words++;
        }
        /* Compare Succeeded? */
        if ( *ptr_words == SPACE_CHAR || *ptr_words == NUL_CHAR )
        { /* Yes */
            /* Done */
            return ptr;
        }
        /* Advance */
        ptr_words += strcspn(ptr_words, SPACE_CHARS );
        ptr_words += strspn(ptr_words, SPACE_CHARS );
    }

    return NULL;
}

/*-------------------------------------------------------------------*
 * itos (Integer to String)
 *-------------------------------------------------------------------*/
char *itos( char *str, int value )
{
    size_t i, n;
    char *str2;
    int digits = 3;

    /* Convert Value to String */
    sprintf(str, "%d", value);

    /* Get String Length */
    i = strlen( str );
    /* Go To End of String */
    str += i;

    /* String has a Minus Sign? */
    if ( value < 0 )
    { /* Yes */
        /* Do not Process Sign */
        i--;
    }

    /* Calculate # of Separations */
    n = ( i - 1 ) / digits;
    /* Setup Pointer for # of Separations */
    str2 = str + n;

    /* Copy End of String */
    *str2-- = *str--;

    /* Initialize Digit Counter */
    n = digits + 1;
    /* Process whole String (except sign) */
    do
    {
        /* Decrement Digit Counter */
        n--;
        /* Time to Insert Separator? */
        if ( n == 0 )
        { /* Yes */
            /* Insert Separator */
            *str2-- = ',';
            /* Reset Digit Counter */
            n = digits;
        }
        /* Copy Digit */
        *str2-- = *str--;
    } while ( --i );

    /* String has a Minus Sign? */
    if ( value >= 0 )
    { /* No */
        str++;
    }

    return str;
}

/*-------------------------------------------------------------------*
 * Fix_FileName (Remove Invalid Identifiers from FileName (Stop at '.')
 *-------------------------------------------------------------------*/
char *Fix_FileName( char *ptr )
{
    do
    {
        /* Identifier Char? */
        if ( !IS_IDENTIFIER_CHAR( *ptr ) )
        { /* No */
            /* Replace by '_' */
            *ptr = '_';
        }
        /* Advance */
        ptr++;
    }
    /* Stop at End or Filename Extension */
    while ( *ptr != NUL_CHAR && *ptr != EXT_CHAR );

    return ptr;
}


/*-------------------------------------------------------------------*
 * Extract Int Value from File
 *-------------------------------------------------------------------*/

int Extract_Value_From_File(
    const char* filename,               /* pathname to an existing text file */
    const char* line_keyword            /* unique string to search for within the file (the value will be extracted on the same line, after this keyword) */
)
{
    FILE* fp;
    char line[512], * ptr, * ptr2;
    int result = -1;

    if ((fp = fopen(filename, "rt")) == NULL)
    {
        return -1;
    }

    while (fgets(line, 512, fp) != NULL)
    {
        if ((ptr = strstr(line, line_keyword)) != NULL)
        {
            while (!isdigit(*ptr++));
            fclose(fp);
            return strtol(--ptr, &ptr2, 10);
        }
    }

    if (fp)
    {
        fclose(fp);
    }

    return result;
}

/*-------------------------------------------------------------------*
 * Extract String from File
 *-------------------------------------------------------------------*/

char * Extract_String_From_File(
    const char* filename,               /* pathname to an existing text file */
    const char* line_keyword,           /* unique string to search for within the file (the string following this keyword will be extracted) */
    char *return_str                    /* returned string */
)
{
    FILE* fp;
    int len;
    char line[512], *ptr, *ptr_end;

    /* initialize with empty string */
    strcpy(return_str, "");

    if ((fp = fopen(filename, "rt")) == NULL)
    {
        return return_str;
    }

    while (fgets(line, 512, fp) != NULL)
    {
        if ((ptr = strstr(line, line_keyword)) != NULL)
        {
            /* extract the name */
            ptr += strlen(line_keyword);

            /* skip leading spaces */
            while (!(IS_ALPHA_CHAR(*ptr)) && !(IS_DIGIT_CHAR(*ptr)) && *ptr != '_')
            {
                ptr++;
            }

            /* proceed to the end of the name */
            ptr_end = ptr + 1;
            while (*ptr_end != SPACE_CHAR && *ptr_end != ')')
            {
                ptr_end++;
            }

            fclose(fp);
            len = (int)(ptr_end - ptr);
            strncpy(return_str, ptr, len);
            return_str[len] = '\0';

            return return_str;
        }
    }

    if (fp)
    {
        fclose(fp);
    }

    return return_str;
}

