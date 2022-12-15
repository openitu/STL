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

#ifndef __WMC_TOOL_H__
#define __WMC_TOOL_H__

/*-------------------------------------------------------------------*
 * Switches
 *-------------------------------------------------------------------*/

#define WMC_TOOL_VERSION_NO  "1.4"          /* Current version */
/*#define DEBUG_PRINT*/                     /* For debugging purposes */

/*-------------------------------------------------------------------*
 * Typedefs
 *-------------------------------------------------------------------*/

typedef enum
{
    NO_ERR,

    ERR_GENERAL = 0x000,
    ERR_CMD_LINE = 0x010 + ERR_GENERAL,
    ERR_HELP,
    ERR_MISSING_ARGS,
    ERR_INVALID_OPTION,
    ERR_NO_FILE_SPEC,
    ERR_ABORT,
    ERR_FIXED_MODE_OPTIONS,
    ERR_FILE = 0x020 + ERR_GENERAL,
    ERR_FILE_FIND,
    ERR_FILE_FIND_FAIL,
    ERR_FILE_OPEN,
    ERR_FILE_OPEN_WRITING,
    ERR_FILE_WRITE,
    ERR_FILE_SEEK,
    ERR_FILE_EMPTY,
    ERR_FILE_READ,
    ERR_FILE_INVALID_CHAR,
    ERR_MEM = 0x040 + ERR_GENERAL,
    ERR_MEM_FILE,
    ERR_MEM_PARSING,

    ERR_PARSING = 0x100,
    ERR_BAD_NUMBER,
    ERR_MORE_THAN_1_DEFAULT,
    ERR_INVALID_EXPRESSION,
    ERR_UNBALANCED = 0x010 + ERR_PARSING,
    ERR_UNBALANCED_QUOTES,
    ERR_UNENDING_COMMENT,
    ERR_UNBALANCED_BRACES,
    ERR_UNBALANCED_PARENTHESES,
    ERR_UNBALANCED_BRACKETS,
    ERR_MISSING = 0x020 + ERR_PARSING,
    ERR_MISSING_SYMBOL,
    ERR_EXPECTED_EOS, /* End of Statement*/
    ERR_EXPECTED_VALUE,
    ERR_MISSING_WHILE,
    ERR_CMD = 0x040 + ERR_PARSING,
    ERR_CMD_NOT_FIRST,
    ERR_CMD_INVALID,
    ERR_CMD_BAD_SYNTAX,

    ERR_UNSUPPORTED = 0x200,
    ERR_MULTI_LINE_CSTE,
    ERR_LINE_CONTINUATION,
    ERR_TOKEN_PASTING,
    ERR_POUND,
    ERR_FUNCTION_NAME,
    ERR_TOO_MANY_VAR_ARGS_FUNC,
    ERR_OPERATOR_NOT_ALLOWED,
    ERR_DO_REQUIRES_BLOCK,

    ERR_FORMAT = 0x400,
    ERR_COUNTING_MACRO_INVALID_NUMBER,

    ERR_OTHER = 0x800,
    ERR_INTERNAL,
    ERR_INCOMPATIBLE_VERSION,
} TOOL_ERROR;

typedef enum
{
    NO_WARNING,

    WARN_LINE_COMMENT,
    WARN_EMPTY_DEF,
    WARN_MACRO_SKIPPED,
    WARN_INITIALIZATION_SKIPPED,
    WARN_SUPERFLUOUS_EOS_REMOVED,
    WARN_CODE_BLOCK_DATA_DECL,
    WARN_FUNC_SKIPPED,
    WARN_AVOID_KEYWORD,
    WARN_PARENTHESES_ADDED,
    WARN_LOCAL_MACRO_SKIPPED,
    WARN_AMBIGUOUS_CONTEXT_4_OP,
    WARN_DIVIDE_AS_MULTIPLY,
    WARN_MODULO_AS_BITAND,
    WARN_FOR_COMPLEX_CONDITION,
    WARN_SUPERFLUOUS_PARENTHESES_REMOVED,
    WARN_MANUAL_MACRO_IN_NONSKIPPED_REGION,

    WARN_ALL = 99,
    WARN_UNSPECIFIED = 999,
} TOOL_WARNING;

/*-------------------------------------------------------------------*
 * Error Messages
 *-------------------------------------------------------------------*/

#define ERROR_MSG_MEMORY      "Cannot Allocate %s bytes"
#define ERROR_MSG_MEM_FILE    ERROR_MSG_MEMORY " for " DQUOTE( "%s" )
#define ERROR_MSG_MEM_PARSING ERROR_MSG_MEMORY " for Parsing"
#define ERROR_MSG_SEEK        "Cannot Seek " DQUOTE( "%s" )
#define ERROR_MSG_FILE_EMPTY  "Empty " DQUOTE( "%s" )

/*-------------------------------------------------------------------*
 * Global File I/O Utilities
 *-------------------------------------------------------------------*/

char** add_file_to_list(const char* filename, char** file_list, int* nfiles);
char** free_list_of_strings(char** file_list, int nfiles);
char** add_files_in_dir_to_list(const char* dir, const char* pattern, char** file_list, int* nfiles);

#endif /* #ifndef __WMC_TOOL_H__ */
