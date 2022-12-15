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

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/*-------------------------------------------------------------------*
 * Global Constants
 *-------------------------------------------------------------------*/

#define TAB_SIZE 4

/* Characters */
#define NUL_CHAR '\0'

#define QUOTE_CHAR  '\''
#define STRING_CHAR '"'
#define ESCAPE_CHAR '\\'

#define EXT_CHAR '.'

#define LF_CHAR    '\n'
#define CR_CHAR    '\r'
#define TAB_CHAR   '\t'
#define SPACE_CHAR ' '
#define BS_CHAR    '\b'
#define BLANK_CHAR '\xFF'
#define DEL_CHAR   '\x7F'

#define EOF_CHAR '\x1A'

#define START_COMMENT_CHAR '/'
#define LONG_COMMENT_CHAR  '*'
#define LINE_COMMENT_CHAR  '/'

#define PREPROC_CHAR '#'

/* Strings */
#define DQUOTE_STRING "\""

#define BEG_COMM_STRING "/*"
#define END_COMM_STRING "*/"

#define EOL_CHARS      "\r\n"
#define RESERVED_CHARS "\xFF\x7F"
#define SPACE_CHARS    "\t " RESERVED_CHARS ""
#define BLANK_CHARS    SPACE_CHARS EOL_CHARS

#define INT_STRING    "int " \
                      "int8_t "
#define TYPES1_STRING "void "     \
                      "bool "     \
                      "char "     \
                      "short "    \
                      "int "      \
                      "long "     \
                      "float "    \
                      "float **"  \
                      "double "   
#define TYPES2_STRING "Short "    \
                      "Float "    \
                      "Word16 "   \
                      "Word16 **" \
                      "UWord16 "  \
                      "Word32 "   \
                      "Word32 **" \
                      "UWord32 "  
#define TYPES3_STRING "int8_t "     \
                      "int16_t "    \
                      "int16_t **"  \
                      "int32_t "    \
                      "int32_t ** " \
                      "int64_t "    \
                      "uint16_t "   \
                      "uint32_t "   \
                      "uint64_t "
#define TYPES_STRING    TYPES1_STRING TYPES2_STRING TYPES3_STRING
#define TYPE_MOD_STRING "signed "   \
                        "unsigned " \
                        "auto "     \
                        "register "
#define TYPEDEF_STRING "typedef "       \
                       "enum "          \
                       "struct "        \
                       "union "         \
                       "Decoder_State " \
                       "Encoder_State "
#define CONST_STRING   "const"
#define STORAGE_STRING "extern " \
                       "static " CONST_STRING " "

#define DATA_DEF_STRING TYPES_STRING TYPE_MOD_STRING TYPEDEF_STRING STORAGE_STRING

typedef enum
{
    FORWARD = 1,
    BACKWARDS = -1
} MOVE_DIRECTION;

/*-------------------------------------------------------------------*
 * Global Macros
 *-------------------------------------------------------------------*/

#define STRING_LENGTH( str ) ( sizeof( str ) - 1 )

#define DQUOTE( str ) DQUOTE_STRING str DQUOTE_STRING

#define nitems( st ) ( sizeof( st ) / sizeof( ( st )[0] ) )
#define unless( cf ) if ( !( cf ) )
#define until( cf )  while ( !( cf ) )

#ifndef __FUNCTION__
#define __FUNCTION__ "undefined"
#endif

#endif /* #ifndef __CONSTANTS_H__ */
