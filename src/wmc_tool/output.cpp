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

#include <cstring>
#include <stdarg.h>   
#include <stdio.h>    
#include <string>     
#include <sys/stat.h> 
#include "output.h"   
#include "constants.h" 


/*-------------------------------------------------------------------*
 * Local Constants
 *-------------------------------------------------------------------*/

#define ELIPSIS_MSG    "..."

/*-------------------------------------------------------------------*
 * LOCAL DATA
 *-------------------------------------------------------------------*/

static char error_msg[256] = "";

/*-------------------------------------------------------------------*
 * GLOBAL FUNCTIONS
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 * Print
 *
 * NOTE: Prints to the Error Console
 *-------------------------------------------------------------------*/
void Print( const char *format, ... )
{
    va_list args;

    va_start( args, format );

    /*  Added to Prevent an Ordering Problem on Cygwin*/
    /* Flush 'stdout' Console */
    fflush( stdout );

    /* Print to 'stderr' */
    vfprintf( stderr, format, args );

    /*  Added to Correct Ordering Problem on Cygwin*/
    /* Flush 'stderr' Console */
    fflush( stderr );

    va_end( args );
}

/*-------------------------------------------------------------------*
 * Process_Message
 *
 * NOTE: Adjust Message if Too Long for Line
 *-------------------------------------------------------------------*/
void Process_Message( char *buffer,
                      size_t count,
                      const char *format,
                      va_list argptr )
{
    int n, nChars;
    char *ptr, *ptr2;

    /* Leave Room for for LF */
    count -= 1;
    /* Format to Buffer */
    nChars = vsnprintf( buffer, count, format, argptr );
    /* Set Length (if Undefined) */
    if ( nChars < 0 )
        nChars = (int) count;
    /* Add LF (also Terminates the String) */
    strcpy( buffer + nChars, "\n" );
    /* Calculate Overflow */
    nChars -= MAX_CHARS_PER_LINE - 1;
    /* All can be Written on Same Line? */
    if ( nChars > 0 )
    { /* No */
        /* Search for 1st Double Quote */
        for ( ptr = buffer; *ptr != '"' && *ptr != NUL_CHAR; ptr++ )
            ;
        /* Found it? */
        if ( *ptr != NUL_CHAR )
        { /* Yes */
            /* Search for Following Double Quote */
            ptr2 = ptr;
            do
            {
                ptr2++;
            } while ( *ptr2 != '"' && *ptr2 != NUL_CHAR );
            /* Found it? */
            if ( *ptr2 != NUL_CHAR )
            { /* Yes */
                /* Part that is not Truncated */
                ptr += 12;
                /* Leave Room for Elipsis Message */
                ptr += STRING_LENGTH( ELIPSIS_MSG );
                /* Able to Truncate? */
                if ( ptr < ptr2 )
                { /* Yes */
                    /* Calculate Room */
                    n = (int) ( ptr2 - ptr );
                    /* Don't Truncate Name more Than Necessary */
                    if ( n > nChars )
                        n = nChars;
                    /* Add Elipsis */
                    strcpy( ptr2 - n - STRING_LENGTH( ELIPSIS_MSG ), ELIPSIS_MSG );
                    /* Truncate Name */
                    strcpy( ptr2 - n, ptr2 );
                    /* Adjust Overflow */
                    nChars -= n;
                }
            }
        }
        /* All can be Written on Same Line? */
        if ( nChars > 0 )
        { /* No */
            /* Truncate Message */
            strcpy( buffer + MAX_CHARS_PER_LINE - STRING_LENGTH( ELIPSIS_MSG ),
                    ELIPSIS_MSG "Message Truncated!" "\n" );
        }
    }
}

/*-------------------------------------------------------------------*
 * Error
 *
 * NOTE: Set Error Message
 *-------------------------------------------------------------------*/
void Error( const char *format, ... )
{
    va_list args;
    char full_format[256] = "E%03x: ";

    /* Concatenate Error Code & Format String (also add '!') */
    strcat(full_format, format);
    strcat(full_format, "!");

    /* Process Message (LF added by Process_Message) */
    va_start(args, format);
    Process_Message( error_msg, STRING_LENGTH( error_msg ), full_format, args );
    va_end( args );

    return;
}

/*-------------------------------------------------------------------*
 * Print_Error
 *
 * NOTE: Prints Error Message to the Console
 *-------------------------------------------------------------------*/
void Print_Error( const char *msg )
{
    /*  Added to Prevent an Ordering Problem on Cygwin  */
    /* Flush 'stdout' Console */
    fflush( stdout );

    /* Print to 'stderr' */
    if (msg == NULL)
    {
        fputs(error_msg, stderr);   /* Print Last Error if 'msg' is NULL */
    }
    else
    {
        fputs(msg, stderr);
    }

    /*  Added to Correct Ordering Problem on Cygwin  */
    /* Flush 'stderr' Console */
    fflush( stderr );

    return;
}

/*-------------------------------------------------------------------*
 * Print_Warning
 * 
 * NOTE: Prints Warning Message to the Console
 *-------------------------------------------------------------------*/
void Print_Warning( const char *format, ... )
{
    char warning_msg[256]; /* Includes all (NUL, LF, ...)*/

    va_list args;
    char full_format[256] = "W%03i: ";

    /* Concatenate Warning Code & Format Strings */
    strcat(full_format, format);

    /* Process Message (LF added by Process_Message) */
    va_start( args, format );
    Process_Message( warning_msg, STRING_LENGTH( warning_msg ), full_format, args );
    va_end( args );

    /* Print */
    fputs( warning_msg, stdout );
}

