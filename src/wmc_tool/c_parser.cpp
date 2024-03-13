/*
 * (C) 2023 copyright VoiceAge Corporation. All Rights Reserved.
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


#include <stdio.h>      
#include <string.h>     
#include <stdlib.h>     
#include <ctype.h>      
#include <limits.h>     
#include <sys/stat.h>   
#include "c_parser.h"   
#include "wmc_tool.h"   
#include "constants.h"  
#include "parsing_defs.h" 
#include "text_utils.h"
#include "output.h"    

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef _MSC_VER 
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#endif

/*-------------------------------------------------------------------*
 * Local Constants
 *-------------------------------------------------------------------*/

#define RULES_NONE 0x00000000
#define RULES_SAME 0x00000001
#define RULE_NI    0x00000002

/*****************************************
 * Allocation Sizes
 *****************************************/

#define PARSE_TBL_MEM_INCREMENT   512  /* in # of entries (yields 4kB) */
#define INSERT_TBL_MEM_INCREMENT  256  /* in # of entries (yields 4kB) */
#define FCTCALL_TBL_MEM_INCREMENT 512  /* in # of entries (yields 4kB) */
#define POINTER_TBL_MEM_INCREMENT 2048 /* in # of entries (yields 4kB) */

/*****************************************
 * Instrumentation Strings
 *****************************************/

#define INSTRUMENTATION_DEF1 _
#define INSTRUMENTATION_DEF2 $

#define INSTRUMENTATION_STRING1 "_"
#define INSTRUMENTATION_STRING2 "$"

#define INSTRUMENTATION_CHAR1 '_'
#define INSTRUMENTATION_CHAR2 '$'

#define WORD_INSTRUMENT_STRING    INSTRUMENTATION_STRING1
#define LOGICAL_INSTRUMENT_STRING INSTRUMENTATION_STRING1 INSTRUMENTATION_STRING1
#define TERNARY_INSTRUMENT_STRING INSTRUMENTATION_STRING1

/* all except those that are used using a call*/
#define INSTRUMENTATION_STRINGS   LOGICAL_INSTRUMENT_STRING " " TERNARY_INSTRUMENT_STRING " "
#define IS_INSTRUMENTATION_CHAR( chr ) ( ( chr ) == INSTRUMENTATION_CHAR1 || ( chr ) == INSTRUMENTATION_CHAR2 )

/*****************************************
 * Parsing Strings
 *****************************************/

#define MANUAL_COUNTING_MACROS_STRING \
    "ABS  "                           \
    "BRANCH  "                        \
    "SQRT TRANS POWER LOG  "          \
    "LOOP  "                          \
    "FUNC  "                          \
    "TEST  "                          \
    "ADD MULT MAC  "                  \
    "MOVE STORE  "                    \
    "LOGIC SHIFT  "                   \
    "DIV  "                           \
    "INDIRECT PTR_INIT  "             \
    "MISC  "                          

#define AUTO_CASE_COUNTING_MACRO_STRING \
    "cost" WORD_INSTRUMENT_STRING

#define AUTO_DEFAULT_COUNTING_MACRO_STRING \
    INSTRUMENTATION_STRING2

#define AUTO_LEFT_COUNTING_MACRO_STRING \
    AUTO_DEFAULT_COUNTING_MACRO_STRING " "
#define AUTO_RIGHT_COUNTING_MACRO_STRING \
    AUTO_CASE_COUNTING_MACRO_STRING " "

#define AUTO_COUNTING_MACROS_STRING \
    AUTO_LEFT_COUNTING_MACRO_STRING \
    AUTO_RIGHT_COUNTING_MACRO_STRING

#define COUNTING_MACROS_STRING    \
    MANUAL_COUNTING_MACROS_STRING \
    AUTO_COUNTING_MACROS_STRING

#define WMOPS_FUNCTS_STRING \
    "reset_wmops push_wmops pop_wmops update_wmops print_wmops print_mem " \
    "rsize Get_Const_Data_Size Print_Const_Data_Size "

#define COUNTING_CODE_STRING \
    COUNTING_MACROS_STRING   \
    WMOPS_FUNCTS_STRING

#define SYSTEM_ALLOC_FUNCTS_STRING \
    "malloc calloc free "

#define SYSTEM_FUNCTS_STRING                           \
    "printf fprintf  "                                 \
    "fopen fclose fwrite fread  "                      \
    "assert exit  "                                       

#define MATH_FUNCTS_STRING                             \
    "abs fabs fabsf labs "                             \
    "floor floorf "                                    \
    "sqrt sqrtf inv_sqrt inv_sqrtf "                   \
    "pow powf exp expf "                               \
    "log logf log10 log10f log_base_2 log2 log2_f "    \
    "cos cosf sin sinf tan tanf "                      \
    "acos acosf asin asinf atan atanf atan2 atan2f "   \
    "cosh coshf sinh sinhf tanh tanhf "                \
    "fmod fmodf "                                      \
    "min max Min Max MIN MAX "                         \
    "sqr Sqr SQR square Square SQUARE "                \
    "sign Sign SIGN "                                  \
    "_round round_f "                                  \
    "set_min set_max "        

#define BASOP_FUNCTS_STRING      \
    "add sub abs_s shl shr extract_h extract_l mult L_mult negate round " \
    "L_mac L_msu L_macNs L_msuNs L_add L_sub L_add_c L_sub_c L_negate L_shl L_shr " \
    "mult_r shr_r mac_r msu_r L_deposit_h L_deposit_l L_shr_r L_abs L_sat norm_s div_s " \
    "norm_l move16 move32 Logic16 Logic32 Test s_max s_min L_max L_min L40_max " \
    "L40_min shl_r L_shl_r L40_shr_r L40_shl_r norm_L40 L40_shl L40_shr L40_negate " \
    "L40_add L40_sub L40_abs L40_mult L40_mac mac_r40 L40_msu msu_r40 Mpy_32_16_ss " \
    "Mpy_32_32_ss L_mult0 L_mac0 L_msu0 lshl lshr L_lshl L_lshr L40_lshl L40_lshr " \
    "s_and s_or s_xor L_and L_or L_xor rotl rotr L_rotl L_rotr L40_set L40_deposit_h " \
    "L40_deposit_l L40_deposit32 Extract40_H Extract40_L L_Extract40 L40_round " \
    "L_saturate40 round40 If Goto Break Switch For While Continue L_mls div_l i_mult "

#define WMOPS_LIB_INCLUDE_STRING \
    "wmc_auto.h"

/*****************************************
 * Parsing & Instrumentation Strings
 *****************************************/

#define FUNC_COUNTING_MACRO_STRING "func" WORD_INSTRUMENT_STRING

/*****************************************
 * Additional Instrumentation Strings
 *****************************************/

#define ADDED_TOOL_INFO_STRING "/*AddedByWMC_Tool*/"
#define WMC_TOOL_INCLUDE_STRING "wmc_auto.h"
#define WMC_TOOL_VERSION_STRING "WMC_TOOL_VERSION"
#define WMC_TOOL_SKIP_STRING "WMC_TOOL_SKIP"
#define CONST_DATA_SIZE_FUNC_PREFIX_STRING "Const_Data_Size"
#define PROM_SIZE_FUNC_PREFIX_STRING "PROM_Size"

/*****************************************
 * Function Entry
 *****************************************/

#define FUNC_START_STRING FUNC_COUNTING_MACRO_STRING "start" WORD_INSTRUMENT_STRING

/*****************************************
 * Function Exit
 *****************************************/

#define RETURN_KW_STRING "return"


/*-------------------------------------------------------------------*
 * Local Typedefs
 *-------------------------------------------------------------------*/

typedef char *( *search_fct_ptr )( const char *str1, const char *str2 );

typedef struct
{
    Item_Type op;
    const char *instrumentation_string;
    int prom_cost;
    int op_rules;
} op_rules_def;

/* Instruction Sizes in # of Words (Instructions) */
typedef struct
{
    int add;
    int abs;
    int mult;
    int mac;
    int move;
    int store;
    int logic;
    int shift;
    int branch;
    int div;
    int sqrt;
    int trans;
    int func;
    int args;
    int loop;
    int indirect;
    int ptr_init;
    int misc;
} PROM_Ops_Weights_defs;

/*-------------------------------------------------------------------*
 * Local Data
 *-------------------------------------------------------------------*/

static const struct
{
    const char *kw_name; /* Keyword Name                    */
    bool kw_ni;          /* Keyword was Never Instrumented  */
    char kw_delim;       /* Keyword Name Delimiter          */
    Item_Type kw_type;   /* Keyword Type                    */
    Item_Type expr_type; /* Keyword Expression Type         */
} keywords[] = {    /* capital keyword names added as they appear in the BASOP code */
    { "if", false, NUL_CHAR, ITEM_KEYWORD_IF, ITEM_IF },
    { "else", true, NUL_CHAR, ITEM_KEYWORD_ELSE | ITEM_SKIPPED, 0 },
    { "for", false, NUL_CHAR, ITEM_KEYWORD_FOR, ITEM_FOR },
    { "do", false, NUL_CHAR, ITEM_KEYWORD_DO | ITEM_SKIPPED, 0 },
    { "while", false, NUL_CHAR, ITEM_KEYWORD_WHILE, ITEM_WHILE },
    { "switch", false, NUL_CHAR, ITEM_KEYWORD_SWITCH, ITEM_SWITCH },
    { "case", true, NUL_CHAR, ITEM_KEYWORD_CASE | ITEM_SKIPPED, ITEM_CONSTANT | ITEM_SKIPPED },
    { "default", true, ':', ITEM_KEYWORD_DEFAULT | ITEM_SKIPPED, ITEM_DEFAULT },
    { "sizeof", true, NUL_CHAR, ITEM_KEYWORD_SIZEOF | /* sizeof 'expr' is not supported*/
                                    ITEM_CONSTANT | ITEM_SKIPPED, ITEM_CONSTANT | ITEM_SKIPPED },
    { "goto", false, NUL_CHAR, ITEM_KEYWORD_GOTO | ITEM_WARNING, ITEM_SKIPPED | ITEM_LABEL },
    { "break", false, NUL_CHAR, ITEM_KEYWORD_BREAK, 0 },
    { "continue", false, NUL_CHAR, ITEM_KEYWORD_CONTINUE | ITEM_WARNING, 0 },
    /* 'return' now Instrumented for enter/leave function Mechanism */
    { RETURN_KW_STRING, false, NUL_CHAR, ITEM_KEYWORD_RETURN | ITEM_RETURN }
};

static const char * const Conditionnal_Directives[] = { "ifdef", "ifndef", "if", "else", "elif", "endif", NULL };
static char Undefine_Directive[] = "undef";
static const char * const Other_Directives[] = { "error", "pragma", "line", "warning", NULL };

static const PROM_Ops_Weights_defs PROM_Ops_Weights[] = {
    {
        1 /* ADD_P      */,
        1 /* ABS_P      */,
        1 /* MULT_P     */,
        1 /* MAC_P      */, /* Used in Manually Instrumented Section (Unused Otherwise)*/
        1 /* MOVE_P     */,
        0 * 1 /* STORE_P    */, /* Unused*/
        1 /* LOGIC_P    */,
        1 /* SHIFT_P    */,
        2 /* BRANCH_P   */,
        2 /* DIV_P      */,
        2 /* SQRT_P     */,
        2 /* TRANS_P    */,
        2 /* FUNC_P     */, /* need to add number of arguments */
        1 /* ARGS_P     */,
        1 /* LOOP_P     */,
        2 /* INDIRECT_P */,
        1 /* PTR_INIT_P */, /* Unused*/
        1 /* MISC_P     */  /* Used for: floor, squant, ...*/
    },
    {
        1 /* ADD_P      */,
        1 /* ABS_P      */,
        1 /* MULT_P     */,
        1 /* MAC_P      */, /* Used in Manually Instrumented Section (Unused Otherwise)*/
        1 /* MOVE_P     */,
        0 * 1 /* STORE_P    */, /* Unused*/
        1 /* LOGIC_P    */,
        1 /* SHIFT_P    */,
        1 /* BRANCH_P   */,
        1 /* DIV_P      */,
        1 /* SQRT_P     */,
        1 /* TRANS_P    */,
        1 /* FUNC_P     */, /* need to add number of arguments */
        0 /* ARGS_P     */,
        1 /* LOOP_P     */,
        1 /* INDIRECT_P */,
        1 /* PTR_INIT_P */, /* Unused*/
        1 /* MISC_P     */  /* Used for: floor, squant, ...*/
    }
};

/*-------------------------------------------------------------------*
 * LOCAL VARIABLES
 *-------------------------------------------------------------------*/

static int dummy_idx_ptr;
static int abses, miscs, loops, whiles, jumps, funcs, args, transes, sqrts, divs, mults, minmaxs, logicals;
static int adds, cases, moves, ptr_inits, indirects, shifts, logics;
static int macs;

/*-------------------------------------------------------------------*
 * LOCAL FUNCTIONS
 *-------------------------------------------------------------------*/

static void count_ops( Item_Type item_type )
{
    switch ( item_type & ~ITEM_OPERATOR_COMPOUND )
    {
        case ITEM_OPERATOR_PLUS:
        case ITEM_OPERATOR_MINUS:
        case ITEM_OPERATOR_INC:
        case ITEM_OPERATOR_DEC:
        case ITEM_OPERATOR_EE:
        case ITEM_OPERATOR_NE:
        case ITEM_OPERATOR_LT:
        case ITEM_OPERATOR_LE:
        case ITEM_OPERATOR_GT:
        case ITEM_OPERATOR_GE:
            adds++;
            break;
        case ITEM_OPERATOR_MULT:
            mults++;
            break;
        case ITEM_OPERATOR_DIV:
        case ITEM_OPERATOR_MOD:
            divs++;
            break;
        case ITEM_OPERATOR_AND:
        case ITEM_OPERATOR_OR:
        case ITEM_OPERATOR_XOR:
        case ITEM_OPERATOR_NOT:
            logics++;
            break;
        case ITEM_OPERATOR_SHL:
        case ITEM_OPERATOR_SHR:
            shifts++;
            break;
        case ITEM_OPERATOR_NOTL:
            miscs++;
            break;
        case ITEM_OPERATOR_EQ:
            moves++;
            break;
        case ITEM_OPERATOR_MEMBER:
        case ITEM_OPERATOR_MEMBER_PTR:
            indirects++;
            break;
    }
}

/*-------------------------------------------------------------------*
 * Blank
 *-------------------------------------------------------------------*/
static inline void Blank( char *ptr )
{
    *ptr = BLANK_CHAR;
}
static inline void Blank( char *start, char *end )
{
    while ( start != end )
        Blank( start++ );
}

/*-------------------------------------------------------------------*
 * Delete
 *-------------------------------------------------------------------*/
static inline void Delete( char *ptr )
{
    *ptr = DEL_CHAR;
}
static inline void Delete( char *start, char *end )
{
    while ( start != end )
        Delete( start++ );
}


/*-------------------------------------------------------------------*
 * Instrument_Words
 *
 * Instrument all Words in "Space Separated String".
 *-------------------------------------------------------------------*/
static void Instrument_Words( char *dst, const char *src )
{
    char buf[2] = "", *ptr = buf;
    char* ptr_src = (char*)src;
    char chr;

    do
    {
        chr = *ptr_src++;

        /* Separator? */
        if ( chr == SPACE_CHAR || chr == NUL_CHAR )
        { /* Yes */
            /* Add Instrumentation String */
            while ( *ptr != NUL_CHAR )
            {
                *dst++ = *ptr++;
            }
        }
        else
        { /* No */
            /* Enable Add Instrumentation String on Next Separator */
            strcpy(buf, WORD_INSTRUMENT_STRING);
            ptr = buf;
        }

        /* Copy Character */
        *dst++ = chr;
    } while ( chr != NUL_CHAR );
}

/*-------------------------------------------------------------------*
 * DesInstrument_Word (Keyword, Math Functions, ...)
 *-------------------------------------------------------------------*/
static void DesInstrument_Word( char **start, char **end )
{
    char *ptr;
    /* Setup & Update Pointers */
    ptr = *end;
    *end -= STRING_LENGTH( WORD_INSTRUMENT_STRING );
    /* Delete Instrumentation String */
    Delete( *end, ptr );
}

/*-------------------------------------------------------------------*
 * Instrument_Words_Front (Keyword, Math Functions, ...)
 *-------------------------------------------------------------------*/
static void Instrument_Words_Front( char *dst, const char *src )
{
    char buf[2] = WORD_INSTRUMENT_STRING, *ptr = buf;
    char* ptr_src = (char*)src;
    char chr;

    do
    {
        chr = *ptr_src++;
        /* Separator? */
        if ( chr == SPACE_CHAR || chr == NUL_CHAR )
        { /* Yes */
            /* Enable Add Instrumentation String on Next Non-Separator */
            strcpy(buf, WORD_INSTRUMENT_STRING);
            ptr = buf;
        }
        else
        { /* No */
            /* Add Instrumentation String */
            while ( *ptr != NUL_CHAR )
            {
                *dst++ = *ptr++;
            }
        }

        /* Copy Character */
        *dst++ = chr;
    } while ( chr != NUL_CHAR );
}

/*-------------------------------------------------------------------*
 * DesInstrument_Word_Front (Keyword, Math Functions, ...)
 *-------------------------------------------------------------------*/
static void DesInstrument_Word_Front( char **start, char **end )
{
    char *ptr;
    /* Setup & Update Pointers */
    ptr = *start;
    *start += STRING_LENGTH( WORD_INSTRUMENT_STRING );
    /* Delete Instrumentation String */
    Delete( ptr, *start );
}

/*-------------------------------------------------------------------*
 * Make_Prj_Fct_Call_Instrumentation_String
 *-------------------------------------------------------------------*/
static char *Make_Prj_Fct_Call_Instrumentation_String( int NameOcc )
{
#define WORD_INSTRUMENT_STRING_LEN STRING_LENGTH( WORD_INSTRUMENT_STRING )

    static char function_call_instrumentation[WORD_INSTRUMENT_STRING_LEN * FCTCALL_MAX_NAME_OCC + 1] = ""; /* +1 for NUL Char*/

    char *ptr;
    int i;

    ptr = function_call_instrumentation;

    /* Empty Vector? */
    if ( *ptr == NUL_CHAR )
    { /* Yes */
        /* Fill Vector */
        for ( i = 0; i < FCTCALL_MAX_NAME_OCC; i++ )
        {
            strcpy( ptr, WORD_INSTRUMENT_STRING );
            ptr += WORD_INSTRUMENT_STRING_LEN;
        }
    }

    return function_call_instrumentation +
           sizeof( function_call_instrumentation ) - 1 -  /*-1 for NUL Char*/
           WORD_INSTRUMENT_STRING_LEN * NameOcc;
}

/*-------------------------------------------------------------------*
 * DesInstrument_Prj_Fct_Call
 *-------------------------------------------------------------------*/
static void DesInstrument_Prj_Fct_Call( char **start, char **end )
{
    char *ptr;

    /* Setup & Update Pointers */
    ptr = *end;
    while ( IS_INSTRUMENTATION_CHAR( PREV_CHAR( *end ) ) )
    {
        ( *end )--;
    }

    /* Delete Instrumentation String */
    Delete( *end, ptr );
}


/*-------------------------------------------------------------------*
 * Make_Position_String
 *-------------------------------------------------------------------*/
static char *Make_Position_String( const char *ptr, char *dest )
{
    sprintf( dest, "Ln%5i, Col%3i -", strlno( ptr ), strcno( ptr ) );

    return dest;
}

/*-------------------------------------------------------------------*
 * Parentheses_Error
 *-------------------------------------------------------------------*/
static TOOL_ERROR Parentheses_Error( char *ptr, char chr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = ERR_UNBALANCED_PARENTHESES;

    Error( "%s Cannot Find Matching Parenthesis '%c'",
           ErrCode, Make_Position_String( ptr, temp ), chr );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Brackets_Error
 *-------------------------------------------------------------------*/
static TOOL_ERROR Brackets_Error( char *ptr, char chr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = ERR_UNBALANCED_BRACKETS;

    Error( "%s Cannot Find Matching Bracket '%c'",
           ErrCode, Make_Position_String( ptr, temp ), chr );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Symbol_Missing_Error
 *-------------------------------------------------------------------*/
static TOOL_ERROR Symbol_Missing_Error( char *ptr, char chr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = ERR_MISSING_SYMBOL;

    Error( "%s '%c' was Expected",
           ErrCode, Make_Position_String( ptr, temp ), chr );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Internal_Error
 *-------------------------------------------------------------------*/
static TOOL_ERROR Internal_Error( const char *func_name )
{
    TOOL_ERROR ErrCode = ERR_INTERNAL;

    Error( "Internal Error in Function " DQUOTE( "%s" ), ErrCode, func_name );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Bad_Preproc_Syntax
 *-------------------------------------------------------------------*/
static TOOL_ERROR Bad_Preproc_Syntax( char *ptr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = ERR_CMD_BAD_SYNTAX;

    Error( "%s Bad Preprocessor Command Syntax",
           ErrCode, Make_Position_String( ptr, temp ) );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Expected_Expression
 *-------------------------------------------------------------------*/
static TOOL_ERROR Expected_Expression( char *ptr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = ERR_EXPECTED_VALUE;

    Error( "%s Value or Label was Expected",
           ErrCode, Make_Position_String( ptr, temp ) );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Expected_EOS
 *-------------------------------------------------------------------*/
static TOOL_ERROR Expected_EOS( char *ptr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = ERR_EXPECTED_EOS;

    Error( "%s End of Statement '%c' was Expected",
           ErrCode, Make_Position_String( ptr, temp ), ';' );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Invalid_KW_Expression
 *-------------------------------------------------------------------*/
static TOOL_ERROR Invalid_KW_Expression( const char *ptr, const char *info )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = ERR_INVALID_EXPRESSION;

    Error( "%s Invalid Keyword Expression; Check # of %s",
           ErrCode, Make_Position_String( ptr, temp ), info );

    return ErrCode;
}


/*-------------------------------------------------------------------*
 * Allocate_Region
 *-------------------------------------------------------------------*/
static TOOL_ERROR Allocate_Region( Parse_Tbl_def *ParseTbl_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    Parse_Rec_def *ptr;
    int size, nbytes;

    char temp[10 + 3 + 1]; /* 10 Digits for 32 Bits Int, 3 Commas, 1 for NUL*/

    /* Need more Room? */
    if ( ParseTbl_ptr->MaxSize == ParseTbl_ptr->Size )
    { /* Yes */
        /* Calculate Bigger Size */
        size = ParseTbl_ptr->MaxSize + PARSE_TBL_MEM_INCREMENT;
        /* Allocate Memory */
        if ( ( ptr = (Parse_Rec_def *) malloc( nbytes = PARSE_REC_SIZE * size ) ) == NULL )
        {
            ErrCode = ERR_MEM_PARSING;
            Error( ERROR_MSG_MEM_PARSING, ErrCode, itos( temp, nbytes ) );
            goto ret;
        }
        /* Copy Current Table in Newly Allocated Memory */
        memmove( ptr, ParseTbl_ptr->Data, PARSE_REC_SIZE * ParseTbl_ptr->Size );
        /* Free Currently Allocated Parse Table Memory */
        Free_Parse_Table_Memory( ParseTbl_ptr );
        /* Set Base Pointer in Parse Table Context */
        ParseTbl_ptr->Data = ptr;
        /* Set Parse Table Max Size in Parse Table Context */
        ParseTbl_ptr->MaxSize = size;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Add_Region
 *-------------------------------------------------------------------*/
static TOOL_ERROR Add_Region(
    Parse_Tbl_def *ParseTbl_ptr,
    Parse_Rec_def *ParseRec_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    if ( ( ErrCode = Allocate_Region( ParseTbl_ptr ) ) == NO_ERR )
    {
        /* Copy Record */
        memmove( ParseTbl_ptr->Data + ParseTbl_ptr->Size, ParseRec_ptr,
                 PARSE_REC_SIZE );
        /* Update # of Entries */
        ParseTbl_ptr->Size++;
    }

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Add_Region
 *-------------------------------------------------------------------*/
static TOOL_ERROR Add_Region(
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type item_type,
    char *item_start,
    char *item_end,
    char *item_link = NULL )
{
    TOOL_ERROR ErrCode = NO_ERR;

    Parse_Rec_def *ParseRec;

    if ( ( ErrCode = Allocate_Region( ParseTbl_ptr ) ) == NO_ERR )
    {
        ParseRec = ParseTbl_ptr->Data + ParseTbl_ptr->Size;
        /* Fill Record */
        ParseRec->item_type = item_type;
        ParseRec->item_start = item_start;
        ParseRec->item_end = item_end;
        ParseRec->item_link = item_link;
        /* Update # of Entries */
        ParseTbl_ptr->Size++;
    }

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Add_Region
 *-------------------------------------------------------------------*/
static TOOL_ERROR Add_Region(
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type item_type,
    char *item_start,
    int size = 1,
    char *item_link = NULL )
{
    return Add_Region( ParseTbl_ptr, item_type,
                       item_start, item_start + size,
                       item_link );
}

/*-------------------------------------------------------------------*
 * Parse_Table_Entries_Cmp
 *-------------------------------------------------------------------*/
static int Parse_Table_Entries_Cmp( const void *elem1, const void *elem2 )
{
    Parse_Rec_def *ptr1 = (Parse_Rec_def *) elem1;
    Parse_Rec_def *ptr2 = (Parse_Rec_def *) elem2;

    if ( ptr1->item_start < ptr2->item_start )
        return -1;
    if ( ptr1->item_start > ptr2->item_start )
        return 1;

    if ( ptr1->item_end > ptr2->item_end )
        return -1;  /*same start but larger region is before*/
    if ( ptr1->item_end < ptr2->item_end )
        return 1; /* same start but smaller region is after*/

    return 0; /* identical regions (should not happen)*/
}

/*-------------------------------------------------------------------*
 * Sort_Parse_Table
 *-------------------------------------------------------------------*/
static void Sort_Parse_Table( Parse_Tbl_def *ParseTbl_ptr )
{
    qsort( ParseTbl_ptr->Data,
           ParseTbl_ptr->Size,
           PARSE_REC_SIZE,
           Parse_Table_Entries_Cmp );
}


/*-------------------------------------------------------------------*
 * Allocate_Insertion
 *-------------------------------------------------------------------*/
static TOOL_ERROR Allocate_Insertion( Insert_Tbl_def *InsertTbl_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    Insert_Rec_def *ptr;
    int size, nbytes;

    char temp[10 + 3 + 1]; /* 10 Digits for 32 Bits Int, 3 Commas, 1 for NUL*/

    /* Need more Room? */
    if ( InsertTbl_ptr->MaxSize == InsertTbl_ptr->Size )
    { /* Yes */
        /* Calculate Bigger Size */
        size = InsertTbl_ptr->MaxSize + INSERT_TBL_MEM_INCREMENT;
        /* Allocate Memory */
        if ( ( ptr = (Insert_Rec_def *) malloc( nbytes = INSERT_REC_SIZE * size ) ) == NULL )
        {
            ErrCode = ERR_MEM_PARSING;
            Error( ERROR_MSG_MEM_PARSING, ErrCode, itos( temp, nbytes ) );
            goto ret;
        }
        /* Copy Current Table in Newly Allocated Memory */
        memmove( ptr, InsertTbl_ptr->Data, INSERT_REC_SIZE * InsertTbl_ptr->Size );
        /* Free Currently Allocated Insertion Table Memory */
        Free_Insert_Table_Memory( InsertTbl_ptr );
        /* Set Base Pointer in Insertion Table Context */
        InsertTbl_ptr->Data = ptr;
        /* Set Insert Table Max Size in Insertion Table Context */
        InsertTbl_ptr->MaxSize = size;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Add_Insertion
 *-------------------------------------------------------------------*/
static TOOL_ERROR Add_Insertion(
    Insert_Tbl_def *InsertTbl_ptr,
    const char *ptr,
    const char *string,
    int niters = 1 )
{
#define MAX_CHARS ( sizeof( InsertRec_ptr->String ) - 1 )

    TOOL_ERROR ErrCode = NO_ERR;

    int idx;
    size_t nChars, string_len, size;
    char *string2;

    Insert_Rec_def *InsertRec_ptr;

    /* Set # of Chars Stored to Full */
    nChars = MAX_CHARS;

    /* Start at End (To Find Last Duplicated Pointer) */
    InsertRec_ptr = InsertTbl_ptr->Data + InsertTbl_ptr->Size;

    /* Look for Entry in Table
       (Start from End to Potentially Find Partially Full Entry First) */
    for ( idx = 0; idx < InsertTbl_ptr->Size; idx++ )
    {
        InsertRec_ptr--;
        /* Same Pointer? */
        if ( ptr == InsertRec_ptr->Ptr )
        { /* Yes */
            /* Set # of Chars Stored */
            nChars = strlen( InsertRec_ptr->String );
            /* Stop */
            break;
        }
    }

    while ( niters-- )
    {
        /* Get String Length */
        string2 = (char *)string;
        string_len = strlen( string );
        do
        {
            /* Full? */
            if ( nChars == MAX_CHARS )
            { /* Yes */
                /* Allocate an Entry */
                if ( ( ErrCode = Allocate_Insertion( InsertTbl_ptr ) ) != NO_ERR )
                {
                    goto ret;
                }
                InsertRec_ptr = InsertTbl_ptr->Data + InsertTbl_ptr->Size;
                /* Fill Record */
                InsertRec_ptr->Ptr = (char *)ptr;
                /* Reset # of Chars Stored */
                nChars = 0;
                /* Update # of Entries */
                InsertTbl_ptr->Size++;
                /* Add Sequence # */
                InsertRec_ptr->Seq = InsertTbl_ptr->Size;
            }
            /* Calculate Length to Copy */
            size = MAX_CHARS - nChars;
            if ( size > string_len )
                size = string_len;
            /* Copy String */
            memcpy( InsertRec_ptr->String + nChars, string2, size );
            /* Update # of Chars Stored */
            nChars += size;
            /* Append NUL Char */
            InsertRec_ptr->String[nChars] = NUL_CHAR;
            /* Advance */
            string2 += size;
            string_len -= size;
        } while ( string_len != 0 );
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Insert_Table_Entries_Cmp
 *-------------------------------------------------------------------*/
static int Insert_Table_Entries_Cmp( const void *elem1, const void *elem2 )
{
    Insert_Rec_def *ptr1 = (Insert_Rec_def *) elem1;
    Insert_Rec_def *ptr2 = (Insert_Rec_def *) elem2;

    /* Compare Address */
    if ( ptr1->Ptr < ptr2->Ptr )
        return -1;
    if ( ptr1->Ptr > ptr2->Ptr )
        return 1;

    /* Compare Sequence Number */
    if ( ptr1->Seq < ptr2->Seq )
        return -1;
    if ( ptr1->Seq > ptr2->Seq )
        return 1;

    return 0;
}

/*-------------------------------------------------------------------*
 * Sort_Insert_Table
 *-------------------------------------------------------------------*/
static void Sort_Insert_Table( Insert_Tbl_def *InsertTbl_ptr )
{
    qsort( InsertTbl_ptr->Data,
           InsertTbl_ptr->Size,
           INSERT_REC_SIZE,
           Insert_Table_Entries_Cmp );
}



/*-------------------------------------------------------------------*
 * Allocate_Function_Call
 *-------------------------------------------------------------------*/
static TOOL_ERROR Allocate_Function_Call(
    FctCall_Tbl_def *FctCallTbl_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    FctCall_Rec_def *ptr;
    int size, nbytes;

    char temp[10 + 3 + 1]; /* 10 Digits for 32 Bits Int, 3 Commas, 1 for NUL*/

    /* Need more Room? */
    if ( FctCallTbl_ptr->MaxSize == FctCallTbl_ptr->Size )
    { /* Yes */
        /* Calculate Bigger Size */
        size = FctCallTbl_ptr->MaxSize + FCTCALL_TBL_MEM_INCREMENT;
        /* Allocate Memory */
        if ( ( ptr = (FctCall_Rec_def *) malloc( nbytes = FCTCALL_REC_SIZE * size ) ) == NULL )
        {
            ErrCode = ERR_MEM_PARSING;
            Error( ERROR_MSG_MEM_PARSING, ErrCode, itos( temp, nbytes ) );
            goto ret;
        }
        /* Copy Current Table in Newly Allocated Memory */
        memmove( ptr, FctCallTbl_ptr->Data, FCTCALL_REC_SIZE * FctCallTbl_ptr->Size );
        /* Free Currently Allocated Function Call Table Memory */
        Free_FctCall_Table_Memory( FctCallTbl_ptr );
        /* Set Base Pointer in Function Call Table Context */
        FctCallTbl_ptr->Data = ptr;
        /* Set Insert Table Max Size in Function Call Table Context */
        FctCallTbl_ptr->MaxSize = size;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Add_Function_Call
 *-------------------------------------------------------------------*/
static TOOL_ERROR Add_Function_Call(
    FctCall_Tbl_def *FctCallTbl_ptr,
    char *Ptr,
    int NameLength,
    int nArgs,
    int *NameOcc )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = NO_ERR;

    int idx;

    FctCall_Rec_def *FctCallRec_ptr;

    /* Start at Beginning */
    FctCallRec_ptr = FctCallTbl_ptr->Data;

    /* Set Function Name Count */
    *NameOcc = 1;

    /* Look for Entry in Table */
    for ( idx = 0; idx < FctCallTbl_ptr->Size; idx++ )
    {
        /* Function Name has Same Lenght? */
        if ( FctCallRec_ptr->NameLength == NameLength )
        { /* Yes */
            /* Same Name? */
            if ( memcmp( FctCallRec_ptr->Ptr, Ptr, NameLength ) == 0 )
            { /* Yes */
                /* Same # of Arguments? */
                if ( FctCallRec_ptr->nArgs == nArgs )
                { /* Yes */
                    /* Done */
                    goto ret;
                }
                /* Max Function Name Count Reached? */
                if ( *NameOcc == FCTCALL_MAX_NAME_OCC )
                { /* Yes */
                    ErrCode = ERR_TOO_MANY_VAR_ARGS_FUNC;
                    Error( "%s Call of " DQUOTE( "%s" ) " has more than %i Variants",
                           ErrCode, Make_Position_String( Ptr, temp ),
                           memstr( Ptr, NameLength ), FCTCALL_MAX_NAME_OCC );
                    goto ret;
                }
                /* Increment Function Name Count */
                ( *NameOcc )++;
            }
        }
        /* Advance */
        FctCallRec_ptr++;
    }

    if ( ( ErrCode = Allocate_Function_Call( FctCallTbl_ptr ) ) == NO_ERR )
    {
        /* Get Record Address */
        FctCallRec_ptr = FctCallTbl_ptr->Data + FctCallTbl_ptr->Size;
        /* Fill Record */
        FctCallRec_ptr->Ptr = Ptr;
        FctCallRec_ptr->NameLength = NameLength;
        FctCallRec_ptr->nArgs = nArgs;
        FctCallRec_ptr->NameOcc = *NameOcc;
        /* Update # of Entries */
        FctCallTbl_ptr->Size++;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * FctCall_Table_Entries_Cmp
 *-------------------------------------------------------------------*/
static int FctCall_Table_Entries_Cmp( const void *elem1, const void *elem2 )
{
    int cmp, len_cmp;

    FctCall_Rec_def *ptr1 = (FctCall_Rec_def *) elem1;
    FctCall_Rec_def *ptr2 = (FctCall_Rec_def *) elem2;

    len_cmp = ptr1->NameLength < ptr2->NameLength ? ptr1->NameLength : ptr2->NameLength;

    /* Case Insensitive Compare First )To Group Same Letters Together) */
    if ((cmp = _strnicmp(ptr1->Ptr, ptr2->Ptr, len_cmp)) == 0)
    {
        /* Case Sensitive Compare Now (Lowercase Letters First) */
        if ((cmp = strncmp(ptr2->Ptr, ptr1->Ptr, len_cmp)) == 0)
        {
            if (ptr1->NameLength < ptr2->NameLength)
            {
                return -1;
            }
            else if (ptr1->NameLength > ptr2->NameLength)
            {
                return 1;
            }
            else
            {
                /* Compare Name Occurance */
                return (int)(ptr1->NameOcc - ptr2->NameOcc);
            }
        }
    }

    return cmp;
}

/*-------------------------------------------------------------------*
 * Sort_FctCall_Table
 *-------------------------------------------------------------------*/
static void Sort_FctCall_Table( FctCall_Tbl_def *FctCallTbl_ptr )
{
    qsort( FctCallTbl_ptr->Data,
           FctCallTbl_ptr->Size,
           FCTCALL_REC_SIZE,
           FctCall_Table_Entries_Cmp );
}


/*-------------------------------------------------------------------*
 * Allocate_Pointer
 *-------------------------------------------------------------------*/
static TOOL_ERROR Allocate_Pointer(
    Pointer_Tbl_def *PointerTbl_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    Pointer_Rec_def *ptr;
    int size, nbytes;

    char temp[10 + 3 + 1]; /* 10 Digits for 32 Bits Int, 3 Commas, 1 for NUL*/

    /* Need more Room? */
    if ( PointerTbl_ptr->MaxSize == PointerTbl_ptr->Size )
    { /* Yes */
        /* Calculate Bigger Size */
        size = PointerTbl_ptr->MaxSize + POINTER_TBL_MEM_INCREMENT;
        /* Allocate Memory */
        if ( ( ptr = (Pointer_Rec_def *) malloc( nbytes = POINTER_REC_SIZE * size ) ) == NULL )
        {
            ErrCode = ERR_MEM_PARSING;
            Error( ERROR_MSG_MEM_PARSING, ErrCode, itos( temp, nbytes ) );
            goto ret;
        }
        /* Copy Current Table in Newly Allocated Memory */
        memmove( ptr, PointerTbl_ptr->Data, POINTER_REC_SIZE * PointerTbl_ptr->Size );
        /* Free Currently Allocated Pointer Table Memory */
        Free_Pointer_Table_Memory( PointerTbl_ptr );
        /* Set Base Pointer in Pointer Table Context */
        PointerTbl_ptr->Data = ptr;
        /* Set Insert Table Max Size in Pointer Table Context */
        PointerTbl_ptr->MaxSize = size;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Add_Pointer
 *-------------------------------------------------------------------*/
static TOOL_ERROR Add_Pointer(
    Pointer_Tbl_def *PointerTbl_ptr,
    Pointer_Rec_def *PointerRec_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    if ( ( ErrCode = Allocate_Pointer( PointerTbl_ptr ) ) == NO_ERR )
    {
        /* Copy Record */
        memmove( PointerTbl_ptr->Data + PointerTbl_ptr->Size, PointerRec_ptr,
                 POINTER_REC_SIZE );
        /* Update # of Entries */
        PointerTbl_ptr->Size++;
    }

    return ErrCode;
}


/*-------------------------------------------------------------------*
 * Find_Region
 *-------------------------------------------------------------------*/
static int Find_Region(
    const char *ptr,
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type item_spec,
    int idx = 0, /* Search Start Index in Parse Table */
    MOVE_DIRECTION step = FORWARD )
{
    int i, size;
    Parse_Rec_def *ParseRec_ptr;

    /* Get Table Size */
    size = ParseTbl_ptr->Size;

    /* Any Region Specified? */
    if ( item_spec == ITEM_ANY )
    { /* Yes */
        return size;
    }

    /* Type Specified? */
    if ( item_spec != ITEM_NONE )
    { /* Yes */
        Item_Type item_class = item_spec & ITEM_CLASS;
        item_spec -= item_class;

        /* Start at Entry 'idx' */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Go through the Parse Table */
        /* 'i' and 'size' are Treated as unsigned ints. */
        /* This allows to have a single comparision for */
        /* both up and down searching. When 'i' hits -1 */
        /* it becomes FFFFFFFF and is > than 'size' and */
        /* that stops the loop.                         */
        for ( i = idx; (unsigned int) i < (unsigned int) size; i += step )
        {
            /* Region Matches Specification? */
            if ( ParseRec_ptr->item_type & item_class && ( ParseRec_ptr->item_type & item_spec ) == item_spec )
            { /* Yes */
                /* In a Region? */
                if ( (ParseRec_ptr->item_start <= ptr && ptr < ParseRec_ptr->item_end) || ptr == NULL )
                { /* Yes */
                    /* Done */
                    return i;
                }
            }
            /* Next Parse Table Record */
            ParseRec_ptr += step;
        }
    }

    return -1; /* not found*/
}



/*-------------------------------------------------------------------*
 * Fix_Item_Type
 *-------------------------------------------------------------------*/
static Item_Type Fix_Item_Type( Item_Type included_types, Item_Type excluded_types )
{
    Item_Type item_exc_subclass = excluded_types & ITEM_SUB_CLASS;

    /* No Sub-Class or Same Sub-Class? */
    if ( item_exc_subclass == 0 ||
         item_exc_subclass == ( included_types & ITEM_SUB_CLASS ) )
    { /* Yes */
        /* Remove 'included_types' from 'excluded_types' */
        excluded_types &= ~included_types;
        /* Some Class Left? */
        unless( excluded_types & ITEM_CLASS )
        { /* No */
            /* Clear it all */
            excluded_types = ITEM_NONE;
        }
    }

    return excluded_types;
}

/*-------------------------------------------------------------------*
 * Move_on_Chars
 *-------------------------------------------------------------------*/
static char *Move_on_Chars(
    search_fct_ptr strpbrk_func,
    const char *ptr_data,   /* Data */
    const char *chars, /* Chars to Search */
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types,
    Item_Type excluded_types,
    MOVE_DIRECTION step,
    int *idx_ptr = &dummy_idx_ptr )
{
    int i;
    char* ptr = (char*)ptr_data;

    excluded_types = Fix_Item_Type( included_types, excluded_types );

    /* Search Specified Chars */
    while ( ( ptr = strpbrk_func( ptr, chars ) ) != NULL )
    {
        /* Check if Chars are IN an Included Region? */
        if ( ( i = Find_Region( ptr, ParseTbl_ptr, included_types ) ) >= 0 )
        { /* Yes */
            /* Save Index */
            *idx_ptr = i;
            /* Check if Chars are IN an Excluded Region? */
            if ( ( i = Find_Region( ptr, ParseTbl_ptr, excluded_types ) ) < 0 )
            { /* No */
                /* Done */
                break;
            }
            /* Move to Edge of Region (either Start or End) */
            if ( step < 0 )
                ptr = ParseTbl_ptr->Data[i].item_start;
            else
                ptr = ParseTbl_ptr->Data[i].item_end - 1;
        }
        /* Move to Next Char */
        ptr += step;
    }

    return ptr;
}

/*-------------------------------------------------------------------*
 * Skip_Chars
 *-------------------------------------------------------------------*/
static char *Skip_Chars(
    const char *ptr_data,   /* Data */
    const char *chars, /* Chars to Search */
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types = ITEM_ANY,
    Item_Type excluded_types = ITEM_MOVED_OVER,
    MOVE_DIRECTION step = FORWARD,
    int *idx_ptr = &dummy_idx_ptr )
{
    return Move_on_Chars( step < 0 ? r_strpbrkn : strpbrkn,
                          ptr_data, chars, ParseTbl_ptr,
                          included_types, excluded_types,
                          step, idx_ptr );
}

/*-------------------------------------------------------------------*
 * Goto_Chars
 *-------------------------------------------------------------------*/
static char *Goto_Chars(
    const char *ptr_data,   /* Data */
    const char *chars, /* Chars to Search */
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types = ITEM_ANY,
    Item_Type excluded_types = ITEM_NOT_SEARCHED,
    MOVE_DIRECTION step = FORWARD,
    int *idx_ptr = &dummy_idx_ptr )
{
    return Move_on_Chars( step < 0 ? r_strpbrk2 : strpbrk2,
                          ptr_data, chars, ParseTbl_ptr,
                          included_types, excluded_types,
                          step, idx_ptr );
}

/*-------------------------------------------------------------------*
 * Find
 *-------------------------------------------------------------------*/
static char *Find(
    const char *ptr_data, /* Data */
    const char *ptr_data_to_search, /* Data to Search */
    search_fct_ptr str_func,
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types,
    Item_Type excluded_types,
    int *idx_ptr = &dummy_idx_ptr )
{
    int i;
    char* ptr1 = (char*)ptr_data;

    excluded_types = Fix_Item_Type( included_types, excluded_types );

    /* Search for the Specified Data */
    while ( ( ptr1 = str_func( ptr1, ptr_data_to_search) ) != NULL )
    {
        /* Check if String is IN an Included Region? */
        if ( ( i = Find_Region( ptr1, ParseTbl_ptr, included_types ) ) >= 0 )
        { /* Yes */
            /* Save Index */
            *idx_ptr = i;
            /* Check if String is IN an Excluded Region? */
            if ( ( i = Find_Region( ptr1, ParseTbl_ptr, excluded_types ) ) < 0 )
            { /* No */
                /* Done */
                break;
            }
            /* Move to End of Region */
            ptr1 = ParseTbl_ptr->Data[i].item_end - 1;
        }
        /* Advance to Next Char */
        ptr1++;
    }

    return ptr1;
}

/*-------------------------------------------------------------------*
 * Find_String
 *-------------------------------------------------------------------*/
static char *Find_String(
    const char *ptr,    /* Data */
    const char *string, /* String to Search */
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types = ITEM_FUNC_BLOCK,
    Item_Type excluded_types = ITEM_NOT_SEARCHED,
    int *idx_ptr = &dummy_idx_ptr )
{
    return Find( ptr, string, strstr2, ParseTbl_ptr,
                 included_types, excluded_types,
                 idx_ptr );
}

/*-------------------------------------------------------------------*
 * Find_Chars
 *-------------------------------------------------------------------*/
static char *Find_Chars(
    const char *ptr,   /* Data */
    const char *chars, /* Characters to Search */
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types = ITEM_FUNC_BLOCK,
    Item_Type excluded_types = ITEM_NOT_SEARCHED,
    int *idx_ptr = &dummy_idx_ptr )
{
    return Find( ptr, chars, strpbrk2, ParseTbl_ptr,
                 included_types, excluded_types,
                 idx_ptr );
}

/*-------------------------------------------------------------------*
 * Find_Identifier
 *-------------------------------------------------------------------*/
static char *Find_Identifier(
    const char *ptr_data,        /* Data */
    const char *identifier, /* Identifier to Search */
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types = ITEM_FUNC_BLOCK,
    Item_Type excluded_types = ITEM_NOT_SEARCHED,
    int *idx_ptr = &dummy_idx_ptr )
{
    char* ptr = (char*)ptr_data;

    /* Get Identifier Length */
    size_t len = strlen( identifier );

    /* Search for the 'identifier' that is in the Specified Region */
    while ( ( ptr = Find_String( ptr, identifier, ParseTbl_ptr,
                                 included_types, excluded_types, idx_ptr ) ) != NULL )
    {
        if ( !IS_IDENTIFIER_CHAR( PREV_CHAR( ptr ) ) &&
             !IS_IDENTIFIER_CHAR( ptr[len] ) )
        {
            break;
        }
        /* Advance */
        ptr += len + 1;
    }

    return ptr;
}

/*-------------------------------------------------------------------*
 * Goto_Match
 *-------------------------------------------------------------------*/
static char *Goto_Match(
    const char *ptr_data, /* Data (point to start char) */
    Parse_Tbl_def *ParseTbl_ptr,
    Item_Type included_types = ITEM_ANY,
    Item_Type excluded_types = ITEM_NOT_SEARCHED
)
{
    char chr, *ptr = (char *)ptr_data;
    char chars[5] = "";
    MOVE_DIRECTION step;
    int level;

    /* Save Character */
    chr = *ptr;

    /* Set Chars to Search */
    switch ( chr )
    {
        case '{':
        case '}':
            strcpy(chars, "{}");
            break;
        /* No Blocks Chars Allowed between Parentheses or Brackets */
        case '(':
        case ')':
            strcpy(chars, "(){}");
            break;
        case '[':
        case ']':
            strcpy(chars, "[]{}");
            break;
        default:
        {
            /* Problem! */
            Internal_Error( __FUNCTION__ );
        }
    }

    /* Set Direction */
    step = chr == chars[0] ? FORWARD : BACKWARDS;

    /* Set Level to 1 */
    level = 1;
    /* Find Matching Character */
    do
    {
        /* Find Start or End Chars */
        if ( ( ptr = Goto_Chars( ptr + step, chars, ParseTbl_ptr,
                                 included_types, excluded_types, step ) ) == NULL )
        {
            break;
        }
        /* Opening Character? */
        if ( *ptr == chars[0] )
        { /* Yes */
            /* Adjust Level */
            level += step;
        }
        /* Closing Character */
        else if ( *ptr == chars[1] )
        { /* Yes */
            /* Adjust Level */
            level -= step;
        }
        /* Block Characters? */
        else
        { /* Yes */
            return NULL;
        }
    } while ( level != 0 );

    return ptr;
}

/*-------------------------------------------------------------------*
 * Goto_EOS
 *-------------------------------------------------------------------*/
static char *Goto_EOS(
    char *ptr, /* Data */
    Parse_Tbl_def *ParseTbl_ptr )
{
    /* Search for End of Statement and Block Start or End */
    while ( *( ptr = Goto_Chars( ptr, ";{}", ParseTbl_ptr ) ) != ';' )
    {
        /* End of Block? */
        if ( *ptr == '}' )
        { /* Yes */
            return NULL;
        }
        /* Start of Block */
        /* Proceed to End of Block (this cannot fail, all blocks are balanced) */
        ptr = Goto_Match( ptr, ParseTbl_ptr ) + 1;
    }

    return ptr;
}

/*-------------------------------------------------------------------*
 * Is_Empty_Region
 *-------------------------------------------------------------------*/
static bool Is_Empty_Region(
    char *start,
    char *end,
    Parse_Tbl_def *ParseTbl_ptr )
{
    char *ptr;

    /* Skip Blanks to the Right */
    ptr = Skip_Chars( start, BLANK_CHARS, ParseTbl_ptr );

    /* Found Something? */
    if ( ptr >= end || ptr == NULL )
    { /* No */
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------*
 * Count_Args
 *-------------------------------------------------------------------*/
static int Count_Args(
    char *ptr, /* Data (point to start of Group) */
    char *end, /* Data (point to end of Group) */
    Parse_Tbl_def *ParseTbl_ptr )
{
    int nargs;

    /* Empty Region? */
    if ( Is_Empty_Region( ptr + 1, end, ParseTbl_ptr ) )
    { /* Yes */
        return 0;
    }

    /* Reset # of Arguments */
    nargs = 1;

    /* Find Parentheses and Comma (stop on End of Group) */
    while ( ( ptr = Goto_Chars( ptr + 1, "(),", ParseTbl_ptr ) ) != end )
    {
        /* Opening Parenthesis? */
        if ( *ptr == '(' )
        { /* Yes */
            /* Proceed to End of Group (this cannot fail, all Parentheses are balanced) */
            ptr = Goto_Match( ptr, ParseTbl_ptr );
        }
        /* Comma at Level 0? */
        else if ( *ptr == ',' )
        { /* Yes */
            /* Increase # of Arguments */
            nargs++;
        }
    }

    return nargs;
}

/*-------------------------------------------------------------------*
 * Is_Cast
 *-------------------------------------------------------------------*/
static char *Is_Cast(
    char *ptr1, /* start of Expression */
    char *ptr2, /*   end of Expression */
    Parse_Tbl_def *ParseTbl_ptr )
{
    /* Skip Blanks (this cannot fail) */
    ptr1 = Skip_Chars( ptr1 + 1, BLANK_CHARS, ParseTbl_ptr );

    /* Valid Identifier Character? */
    if ( IS_IDENTIFIER_START_CHAR( *ptr1 ) )
    { /* Yes */
        /* Any of the Known Data Types, Type Definition
           Type Modifier or Storage Class? */
        if ( memwordcmp( ptr1, DATA_DEF_STRING ) != NULL )
        { /* Yes */
            /* Set End of Cast */
            ptr1 = ptr2;
        }
        else
        { /* No */
            /* Skip Identifier Chars to the Right (there is at least one) */
            ptr1 = Skip_Identifier( ptr1 );
            /* Skip Blanks (this cannot fail) */
            ptr1 = Skip_Chars( ptr1, BLANK_CHARS, ParseTbl_ptr );
            /* Hit the Closing Parenthesis? */
            if ( ptr1 != ptr2 )
            { /* No */
                /* Star Character? */
                if ( *ptr1 == '*' )
                { /* Yes */
                    /* Skip Blanks (this cannot fail) */
                    ptr1 = Skip_Chars( ptr1 + 1, BLANK_CHARS, ParseTbl_ptr );
                }
            }
            else
            { /* Yes */
                /* Unknown Identifier between Parentheses (could be a cast) */
                ptr1 = NULL;
            }
        }
    }

    /* Hit Closing Parenthesis (Found a Cast)? */
    return ptr1;
}

/*-------------------------------------------------------------------*
 * Is_Constant
 *-------------------------------------------------------------------*/
static bool Is_Constant(
    char *ptr /* start of Identifier */
)
{
    unsigned long UpperChar, Total;

    UpperChar = Total = 0;
    do
    {
        if ( IS_UPPER_CHAR( *ptr ) )
            UpperChar += 100;
        Total++;
        ptr++;
    } while ( IS_IDENTIFIER_CHAR( *ptr ) );

    /* >= 70% of UpperCase? */
    return ( UpperChar / Total ) >= 70;
}


/*-------------------------------------------------------------------*
 * Find_Enclosed_Text
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Enclosed_Text(
    Parse_Context_def *ParseCtx_ptr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;
    char *last_ptr;
    Item_Type item_type;
    char chr;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Search for Quote, Double Quote & Comment Start */
    while ( ( ptr = strpbrk( ptr, "'" DQUOTE_STRING "/" ) ) != NULL )
    {
        /* Save Last Position */
        last_ptr = ptr;

        switch ( *ptr )
        {
            case QUOTE_CHAR:
            case STRING_CHAR:
            {
                /* Get Current Char */
                chr = *ptr;
                /* Set Item Type */
                item_type = chr == STRING_CHAR ? ITEM_STRING : ITEM_CHAR;
                /* Escaped? */
                if ( PREV_CHAR( ptr ) != ESCAPE_CHAR )
                { /* No */
                    /* Find End */
                    if ( ( ptr = strnechr( ptr, chr ) ) == NULL )
                    {
                        ErrCode = ERR_UNBALANCED_QUOTES;
                        Error( "%s EOF Reached Looking for Ending Quote (%c)",
                               ErrCode, Make_Position_String( last_ptr, temp ), chr );
                        goto ret;
                    }
                    /* Check for EOL in String (adjusted to scan only enclosed text) */
                    if ( strpbrk( memstr( last_ptr + 1, ptr ), EOL_CHARS ) != NULL )
                    { /* Yes */
                        ErrCode = ERR_MULTI_LINE_CSTE;
                        Error( "%s Multiple Line Constants are not Supported",
                               ErrCode, Make_Position_String( last_ptr, temp ) );
                        goto ret;
                    }
                }
                break;
            }
            case START_COMMENT_CHAR:
            {
                /* Get Next Char */
                chr = NEXT_CHAR( ptr );
                /* Comment Start? */
                if ( chr == LONG_COMMENT_CHAR || chr == LINE_COMMENT_CHAR )
                {
                    /* Find End (ptr+2 to skip Comment Start Chars) */
                    if ( chr == LONG_COMMENT_CHAR )
                    {
                        ptr = strstr( ptr + 2, END_COMM_STRING );
                    }
                    else
                    { /* Search for CR or LF */
                        ptr = strpbrk( ptr + 2, EOL_CHARS );
                        /* Escaped? */
                        if ( PREV_CHAR( ptr ) == ESCAPE_CHAR )
                        { /* Yes */
                            /* Line Continuation is Treated as if it was
                               not Part of the Single Line Comment. */
                            ptr--;
                        }
                    }
                    /* End Found? */
                    if ( ptr == NULL )
                    { /* No */
                        ErrCode = ERR_UNENDING_COMMENT;
                        Error( "%s EOF Reached Inside " DQUOTE( "%c%c" ) " Comment",
                               ErrCode, Make_Position_String( last_ptr, temp ), START_COMMENT_CHAR, chr );
                        goto ret;
                    }
                    /* Set Item Type */
                    item_type = ITEM_COMMENT_LINE | ITEM_WARNING;
                    /* Set Pointer to Last Char of Comment End */
                    if ( chr == LONG_COMMENT_CHAR )
                    {
                        ptr += STRING_LENGTH( END_COMM_STRING );
                        /* Set Item Type */
                        item_type = ITEM_COMMENT_BLOCK;
                    }
                    ptr--;
                }
                break;
            }
            default:
            {
                ErrCode = Internal_Error( __FUNCTION__ );
                goto ret;
            }
        }

        /**************************************************
        * Here, either
        * - 'ptr' hasn't moved or
        * - 'ptr' points to the char that ends the region
        **************************************************/

        /* Valid Region Found? */
        if ( ptr != last_ptr )
        { /* Yes */
            /* Advance to Next Char (get outside of region) */
            ptr++;
            /* Add Region to Table */
            if ( ( ErrCode = Add_Region( &ParseCtx_ptr->ParseTbl,
                                         item_type, last_ptr, ptr ) ) != NO_ERR )
            {
                goto ret;
            }
        }
        else
        { /* No */
            /* Advance to Next Char */
            ptr++;
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Line_Continuation
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Line_Continuation(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/

    char *ptr;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Search for an Escape Character that is not
       in a Comment or Text Region */
    while ( ( ptr = Find_String( ptr, "\\", &ParseCtx_ptr->ParseTbl, ITEM_ANY ) ) != NULL )
    {
        /* Advance to Next Char */
        ptr++;

        /* Is Character CR or LF? */
        if ( IS_EOL_CHAR( *ptr ) )
        { /* Yes */
            ErrCode = ERR_LINE_CONTINUATION;
            Error( "%s Line Continuation '%c' is not Supported",
                   ErrCode, Make_Position_String( ptr - 1, temp ), ESCAPE_CHAR );
            goto ret;
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Do_Preproc_Command
 *-------------------------------------------------------------------*/
static TOOL_ERROR Do_Preproc_Command(
    Parse_Tbl_def *ParseTbl_ptr,
    char **ptr,
    size_t cmd_len,
    Item_Type item_type,
    char **ptr_arg_start,
    char **ptr_arg_end )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr2, *start, *tmp;

    /* Save Start (Search Hit Position) */
    start = *ptr;

    /* Skip Spaces to the Left */
    ptr2 = Skip_Chars( start - 1, SPACE_CHARS, ParseTbl_ptr,
                       ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
    /* is it the Preprocessor Character? */
    if ( *ptr2 != PREPROC_CHAR )
    { /* No */
        /* Reset Arguments Pointers */
        *ptr_arg_start = *ptr_arg_end = NULL;
        goto ret;
    }

    /* Save Preprocessor Character Position */
    tmp = ptr2;

    /* Skip Spaces to the Left */
    ptr2 = Skip_Chars( ptr2 - 1, SPACE_CHARS, ParseTbl_ptr,
                       ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
    /* Directive Starts as First Non Whitespace Character? */
    if ( !IS_EOL_CHAR( *ptr2 ) )
    { /* No */
        ErrCode = ERR_CMD_NOT_FIRST;
        Error( "%s Command Must be First non whitespace Char",
               ErrCode, Make_Position_String( tmp, temp ) );
        goto ret;
    }

    /* Save Preprocessor Command Start Position (Long Comments are Included) */
    tmp = ptr2 + 1;

    /* Skip Matched String */
    ptr2 = start + cmd_len;
    /* Valid Identifier Character after Match? */
    if ( IS_IDENTIFIER_CHAR( *ptr2 ) )
    { /* Yes */
        ErrCode = ERR_CMD_INVALID;
        Error( "%s Invalid Preprocessor Command",
               ErrCode, Make_Position_String( start, temp ) );
        goto ret;
    }

    /* Add a 'Preprocessor Directive' Region
       (Includes the Preproc Char and any Long Comments before the '#') */
    if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_PREPROC_CMD | item_type,
                                 tmp, ptr2 ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Skip Spaces to the Right (this cannot fail) */
    *ptr = Skip_Chars( ptr2, SPACE_CHARS, ParseTbl_ptr );

    /* Save Argument Start Position */
    *ptr_arg_start = ptr2;
    do
    {
        /* Go to End of Line (Skip Comments) */
        ptr2 = Goto_Chars( ptr2, EOL_CHARS, ParseTbl_ptr );
        /* Line Continuation? */
        if ( PREV_CHAR( ptr2 ) != ESCAPE_CHAR )
        { /* No */
            /* Done */
            break;
        }
        /* Skip the LF if it is a CR+LF */
        if ( IS_EOL_SEQ( ptr2 ) )
        {
            /* One Extra Skip for 2nd Char */
            ptr2++;
        }
        ptr2++;
    } while ( *ptr2 != NUL_CHAR ); /* Stop if End of File Reached */

    /* Save Argument End Position */
    *ptr_arg_end = ptr2;

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Defines
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Defines(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    static char directive[] = "define";
    char *ptr;
    char *start;
    char *ptr_arg_start, *ptr_arg_end;
    Item_Type item_type;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Search for the "define" String that is not in a Comment or Text Region */
    while ( ( ptr = Find_String( ptr, directive, ParseTbl_ptr, ITEM_ANY ) ) != NULL )
    {
        if ( ( ErrCode = Do_Preproc_Command( ParseTbl_ptr, &ptr,
                                             STRING_LENGTH( directive ), ITEM_PREPROC_DEF,
                                             &ptr_arg_start, &ptr_arg_end ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Preprocessor Command? */
        if ( ptr_arg_start == NULL || ptr_arg_end == NULL )
        { /* No */
            /* Set Next Position */
            ptr++;
            /* Search Again */
            continue;
        }

        /* Valid Identifier Character? */
        if ( !IS_IDENTIFIER_START_CHAR( *ptr ) )
        { /* No */
            ErrCode = Bad_Preproc_Syntax( ptr );
            goto ret;
        }

        /* Skip Identifier Chars to the Right (there is at least one) */
        ptr = Skip_Identifier( ptr );

        /* Set Item Type (a Macro has its Parenthesis Right after its Name (no Spaces or Comment) */
        item_type = *ptr == '(' ? ITEM_MACRO_NAME : ITEM_CSTE_NAME;

        /* Add a Name Region (Includes any Long Comments) */
        if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type,
                                     ptr_arg_start, ptr ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Get Parameters */
        if ( item_type == ITEM_MACRO_NAME )
        {
            /* Save Start */
            start = ptr;
            /* Find Closing Parenthesis */
            ptr = Goto_Match( ptr, ParseTbl_ptr );
            /* Found one and Inside Preprocessor Definition? */
            if ( ptr == NULL || ptr >= ptr_arg_end )
            { /* No */
                ErrCode = Parentheses_Error( start, ')' );
                goto ret;
            }
            /* Advance */
            ptr++;
            /* Add "Macro Parameters" Region to Table */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_MACRO_PROTO, start, ptr ) ) != NO_ERR )
            {
                goto ret;
            }
            /* Set Next Item Type */
            item_type = ITEM_MACRO_CODE;
        }
        else
        {
            /* Is there a Space, Newline or Comment (empty defs are accepted) */
            if ( !IS_BLANK_CHAR( *ptr ) && Find_Region( ptr, ParseTbl_ptr, ITEM_COMMENT ) < 0 )
            { /* No */
                ErrCode = Bad_Preproc_Syntax( ptr );
                goto ret;
            }
            /* Set Next Item Type */
            item_type = ITEM_CSTE_VALUE;
        }

        /* Is Definition Empty? */
        if ( Is_Empty_Region( ptr, ptr_arg_end, ParseTbl_ptr ) )
        { /* Yes */
            /* Will Warn */
            item_type |= ITEM_WARNING;
        }
        /* Add Last Region to Table */
        if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type, ptr, ptr_arg_end ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Set Next Position */
        ptr = ptr_arg_end;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Includes
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Includes(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    static char directive[] = "include";
    char *ptr;
    char *start;
    char *ptr_arg_start, *ptr_arg_end;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Search for the "include" String that is not in a Comment or Text Region */
    while ( ( ptr = Find_String( ptr, directive, ParseTbl_ptr, ITEM_ANY ) ) != NULL )
    {
        if ( ( ErrCode = Do_Preproc_Command( ParseTbl_ptr, &ptr,
                                             STRING_LENGTH( directive ), ITEM_PREPROC_INC,
                                             &ptr_arg_start, &ptr_arg_end ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Preprocessor Command? */
        if ( ptr_arg_start == NULL || ptr_arg_end == NULL )
        { /* No */
            /* Set Next Position */
            ptr++;
            /* Search Again */
            continue;
        }

        /* Local or System Include Start? */
        if ( *ptr != STRING_CHAR && *ptr != '<' )
        { /* No */
            ErrCode = Bad_Preproc_Syntax( ptr );
            goto ret;
        }

        /* System Include Start? */
        if ( *ptr == '<' )
        { /* Yes */
            /* Save Start */
            start = ptr;

            /* Skip Spaces to the Left (from Argument End) */
            ptr = Skip_Chars( ptr_arg_end - 1, SPACE_CHARS, ParseTbl_ptr,
                              ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
            /* System Include End? */
            if ( *ptr != '>' )
            { /* No */
                ErrCode = Bad_Preproc_Syntax( start );
                goto ret;
            }
        }

        /* Add Last Region to Table */
        if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_PREPROC_ARGS | ITEM_PREPROC_INC,
                                     ptr_arg_start, ptr_arg_end ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Set Next Position */
        ptr = ptr_arg_end;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Conditionnals
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Conditionnals(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;
    char *ptr_arg_start, *ptr_arg_end;
    char **directive;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    directive = (char **)Conditionnal_Directives;

    while ( *directive != NULL )
    {
        /* Start at Beginning */
        ptr = ParseCtx_ptr->File.Data;

        /* Search for the directive String that is not in a Comment or Text Region */
        while ( ( ptr = Find_String( ptr, *directive, ParseTbl_ptr, ITEM_ANY ) ) != NULL )
        {
            if ( ( ErrCode = Do_Preproc_Command( ParseTbl_ptr, &ptr,
                                                 strlen( *directive ), ITEM_PREPROC_COND,
                                                 &ptr_arg_start, &ptr_arg_end ) ) != NO_ERR )
            {
                goto ret;
            }

            /* Preprocessor Command? */
            if ( ptr_arg_start == NULL || ptr_arg_end == NULL )
            { /* No */
                /* Set Next Position */
                ptr++;
                /* Search Again */
                continue;
            }

            /* Add Last Region to Table */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_PREPROC_ARGS | ITEM_PREPROC_COND,
                                         ptr_arg_start, ptr_arg_end ) ) != NO_ERR )
            {
                goto ret;
            }

            /* Set Next Position (Skip whole Line) */
            ptr = ptr_arg_end;
        }
        /* Next Directive */
        directive++;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Undefines
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Undefines(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;
    char *ptr_arg_start, *ptr_arg_end;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Search for the directive String that is not in a Comment or Text Region */
    while ( ( ptr = Find_String( ptr, Undefine_Directive, ParseTbl_ptr, ITEM_ANY ) ) != NULL )
    {
        if ( ( ErrCode = Do_Preproc_Command( ParseTbl_ptr, &ptr,
                                             STRING_LENGTH( Undefine_Directive ), ITEM_PREPROC_UNDEF,
                                             &ptr_arg_start, &ptr_arg_end ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Preprocessor Command? */
        if ( ptr_arg_start == NULL || ptr_arg_end == NULL )
        { /* No */
            /* Set Next Position */
            ptr++;
            /* Search Again */
            continue;
        }

        /* Valid Identifier Character? */
        if ( !IS_IDENTIFIER_START_CHAR( *ptr ) )
        { /* No */
            ErrCode = Bad_Preproc_Syntax( ptr );
            goto ret;
        }

        /* Skip Identifier Chars to the Right (there is at least one) */
        ptr = Skip_Identifier( ptr );
        /* The name of the Identifier is not Put in the Parsing Table*/
        /* Because we have runned out of Types*/
        /* Add Last Region to Table */
        if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_PREPROC_ARGS | ITEM_PREPROC_UNDEF,
                                     ptr_arg_start, ptr_arg_end ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Set Next Position (Skip whole Line) */
        ptr = ptr_arg_end;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Others
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Others(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;
    char *ptr_arg_start, *ptr_arg_end;
    char **directive;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    directive = (char **)Other_Directives;

    while ( *directive != NULL )
    {
        /* Start at Beginning */
        ptr = ParseCtx_ptr->File.Data;

        /* Search for the directive String that is not in a Comment or Text Region */
        while ( ( ptr = Find_String( ptr, *directive, ParseTbl_ptr, ITEM_ANY ) ) != NULL )
        {
            if ( ( ErrCode = Do_Preproc_Command( ParseTbl_ptr, &ptr,
                                                 strlen( *directive ), ITEM_PREPROC_OTHER,
                                                 &ptr_arg_start, &ptr_arg_end ) ) != NO_ERR )
            {
                goto ret;
            }

            /* Preprocessor Command? */
            if ( ptr_arg_start == NULL || ptr_arg_end == NULL )
            { /* No */
                /* Set Next Position */
                ptr++;
                /* Search Again */
                continue;
            }

            /* Add Last Region to Table */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_PREPROC_ARGS | ITEM_PREPROC_OTHER,
                                         ptr_arg_start, ptr_arg_end ) ) != NO_ERR )
            {
                goto ret;
            }

            /* Set Next Position (Skip whole Line) */
            ptr = ptr_arg_end;
        }
        /* Next Directive */
        directive++;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Functions
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Functions(
    Parse_Context_def *ParseCtx_ptr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;
    char *ns, *ne; /* Name   Start/End */
    char *bs, *be; /* Block  Start/End */
    char *ps, *pe; /* Params Start/End */
    Item_Type item_type;

    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Search for a Block Start that is not in a Comment, Text Region or PreProc Line */
    while ( ( ptr = Find_String( ptr, "{", ParseTbl_ptr, ITEM_ANY ) ) != NULL )
    {
        /* Save Block Start */
        bs = ptr;
        /* Find End of Block */
        if ( ( ptr = Goto_Match( ptr, ParseTbl_ptr ) ) == NULL )
        {
            ErrCode = ERR_UNBALANCED_BRACES;
            Error( "%s Cannot Find Matching Brace '%c'",
                   ErrCode, Make_Position_String( bs, temp ), '}' );
            goto ret;
        }
        /* Save Block End */
        be = ptr + 1;

        /* Used as a Flag */
        item_type = ITEM_NONE;

        /* Move Up to Next Non Blank Character that is not in a Comment or PreProc Line
           (from Block Start) */
        if ( ( ptr = Skip_Chars( bs - 1, BLANK_CHARS, ParseTbl_ptr,
                                 ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS ) ) != NULL )
        {
            /* Is it a Closing Parenthesis? */
            if ( *ptr == ')' )
            { /* Yes */
                /* Save Prototype End */
                pe = ptr + 1;
                /* Find Opening Parenthesis */
                if ( ( ptr = Goto_Match( ptr, ParseTbl_ptr ) ) == NULL )
                {
                    ErrCode = Parentheses_Error( pe - 1, '(' );
                    goto ret;
                }
                /* Save Prototype Start */
                ps = ptr;

                /* Move Up to Next Non Blank Character that is not in a Comment or PreProc Line
                   (from Block Parameters Start) */
                if ( ( ptr = Skip_Chars( ps - 1, BLANK_CHARS, ParseTbl_ptr,
                                         ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS ) ) != NULL )
                {
                    /* Valid Identifier? */
                    if ( IS_IDENTIFIER_CHAR( *ptr ) )
                    { /* Yes */
                        /* Save Name End */
                        ne = ptr + 1;
                        /* Move Up to Next Non Identifier Character (from Name End) */
                        ptr = Skip_Identifier( ptr, BACKWARDS );
                        /* Save Name Start */
                        ns = ptr + 1;
                        if ( memwordcmp( ns, MATH_FUNCTS_STRING ) != NULL )
                            item_type = ITEM_FUNC_MATH;
                        else if ( memwordcmp( ns, SYSTEM_FUNCTS_STRING ) != NULL )
                            item_type = ITEM_FUNC_SYSTEM;
                        else
                        { /* No */
                            item_type = ITEM_FUNC_PROJECT;
                            /* Add "Function Name" Region to Table */
                            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_FUNC_NAME | item_type, ns, ne ) ) != NO_ERR )
                            {
                                goto ret;
                            }
                            /* Add "Function Prototype" Region to Table */
                            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_FUNC_PROTO | item_type, ps, pe ) ) != NO_ERR )
                            {
                                goto ret;
                            }
                            /* Add "Function Block" Region to Table */
                            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_FUNC_BLOCK | item_type, bs, be ) ) != NO_ERR )
                            {
                                goto ret;
                            }
                        }
                    }
                }
            }
        }

        /* Was a Function Block? */
        if ( item_type == ITEM_NONE )
        { /* No */
            /* Add "Data" Region to Table */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_SKIPPED | ITEM_DATA,
                                         bs, be ) ) != NO_ERR )
            {
                goto ret;
            }
        }

        /* Continue after End of Block */
        ptr = be;
    }
ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Token_Pasting
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Token_Pasting(
    Parse_Context_def *ParseCtx_ptr )
{
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    TOOL_ERROR ErrCode = NO_ERR;

    static char token_pasting[] = "##";
    char *ptr;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Search for a Token Pasting Sequence */
    if ( ( ptr = Find_String( ptr, token_pasting, &ParseCtx_ptr->ParseTbl ) ) != NULL )
    {
        ErrCode = ERR_TOKEN_PASTING;
        Error( "%s Token Pasting '%s' is not Supported",
               ErrCode, Make_Position_String( ptr, temp ), token_pasting );
        goto ret;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Keywords
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Keywords(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char kw_name[16];
    size_t kw_name_len;
    char *ns, *ne; /* Name Start/End */
    char *ps, *pe; /* Params Start/End */
    char *dp;
    Item_Type item_type;
    Parse_Tbl_def *ParseTbl_ptr;
    int ncomma, neos;
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/

    int i, run, finished;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* For all the Keywords */
    for ( i = 0; i < (int)(nitems( keywords )); i++ )
    {
        /* Get UnInstrumented Keyword Name */
        strcpy( kw_name, keywords[i].kw_name );

        finished = 0;
        run = 0;
        while (!finished)
        {
            item_type = ITEM_NONE;
            if (run == 1)
            { /* Yes */
                /* Try Uppercase Version */
                for (ns = kw_name; *ns != NUL_CHAR; *ns = toupper(*ns), ns++);

                /* Uppercase version is recognized as BASOP operation -> do not instrument */
                item_type = ITEM_INSTRUMENTATION_OFF;
            }
            else if (run == 2)
            {
                /*  Skip Runs for Keywords that are Never Intrumented */
                if (keywords[i].kw_ni)
                {
                    run = 4;
                    break;
                }
                else
                {
                    /* Try Instrumented Lowercase Version */
                    Instrument_Words(kw_name, keywords[i].kw_name);
                    item_type = ITEM_INSTRUMENTED;
                }
            }
            else if (run == 3)
            {
                /* Try Instrumented Uppercase Version */
                for (ns = kw_name; *ns != NUL_CHAR; *ns = toupper(*ns), ns++);
                item_type = ITEM_INSTRUMENTED;
            }
            else if (run == 4)
            {
                if (keywords[i].kw_type == ITEM_KEYWORD_WHILE)
                { /* Yes */
                    /* Try (Front) Instrumented Version */
                    Instrument_Words_Front(kw_name, keywords[i].kw_name);
                    item_type = ITEM_INSTRUMENTED;
                }
                else
                {
                    /* Skip this Run */
                    run++;
                    continue;
                }
            }
            else if (run == 5)
            {
                if (keywords[i].kw_type == ITEM_KEYWORD_SWITCH)
                { /* Yes */
                    /* Try '__' Instrumented Version */
                    Instrument_Words(kw_name, keywords[i].kw_name);
                    Instrument_Words(kw_name, kw_name);
                    item_type = ITEM_INSTRUMENTED;
                }
                else
                {
                    /* Skip this Run */
                    run++;
                    continue;
                }
            }
            else if (run > 5)
            {
                finished = 1;
                break;
            }


            /* Set Name Length */
            kw_name_len = strlen( kw_name );

            /* Start at Beginning */
            ne = ParseCtx_ptr->File.Data;

            /* Find Keyword */
            while ( ( ns = Find_Identifier( ne, kw_name, ParseTbl_ptr ) ) != NULL )
            {
                /* Set Name End */
                ne = ns + kw_name_len;

                /* Add Keyword Region */
                if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type +
                    /*  Keywords that Compute to Constants are stripped of KW Attribute. */
                    /* Added because when 'sizeof' is marked as a KW, instrumentation of something */
                    /* like 'sizeof(float) * 2' would be instrumented between the ')' and the '2'.*/
                      ( ~( ( keywords[i].kw_type & ITEM_CONSTANT ) ? ( ITEM_KEYWORD | ITEM_SUB_CLASS ) : 0 ) & keywords[i].kw_type ), ns, ne ) ) != NO_ERR )
                {
                    goto ret;
                }
                /* Reset Parameter End */
                pe = NULL;
                /* Reset Delimiter Position */
                dp = NULL;
                /* Keyword has an Argument? */
                if ( keywords[i].kw_type & ITEM_KEYWORD_HAS_ARG )
                { /* Yes */
                    /* Skip Blanks to the Right (this cannot fail) */
                    ps = Skip_Chars( ne, BLANK_CHARS, ParseTbl_ptr );
                    /* Is it an Opening Parenthesis? */
                    if ( *ps != '(' )
                    { /* No */
                        ErrCode = Symbol_Missing_Error( ps, '(' );
                        goto ret;
                    }
                    /* Find Closing Parenthesis */
                    if ( ( pe = Goto_Match( ps, ParseTbl_ptr ) ) == NULL )
                    {
                        ErrCode = Parentheses_Error( ps, ')' );
                        goto ret;
                    }
                }
                /* Keyword has a Value? */
                if ( keywords[i].kw_type & ITEM_KEYWORD_HAS_VAL )
                { /* Yes */
                    /* Skip Blanks to the Right (this cannot fail) */
                    ps = Skip_Chars( ne, BLANK_CHARS, ParseTbl_ptr );
                    /* Is it a Jump */
                    if ( keywords[i].kw_type & ITEM_KEYWORD_IS_JUMP )
                    { /* Yes */
                        /* Is it the End of Statement Character? */
                        if ( *ps == ';' )
                        {   /* Yes */
                            /*  Added because 'return' Can Have Empty Value*/
                            /* Is it a 'return'? */
                            /* 'return' is Now Instrumented */
                            if ( keywords[i].kw_type != ITEM_KEYWORD_RETURN )
                            { /* No */
                                ErrCode = Expected_Expression( ps );
                                goto ret;
                            }
                        }
                        /* Go to End (this cannot fail) */
                        pe = Goto_Chars( ps, ";{}", ParseTbl_ptr );
                        /* Is it the End of Statement Character? */
                        if ( *pe != ';' )
                        { /* No */
                            ErrCode = Expected_EOS( pe );
                            goto ret;
                        }
                    }
                    else
                    { /* No */
                        /* Is it the Colon Character? */
                        if ( *ps == ':' )
                        { /* Yes */
                            ErrCode = Expected_Expression( ps );
                            goto ret;
                        }
                        /* Go to End (this cannot fail) */
                        pe = Goto_Chars( ps, "?:{}", ParseTbl_ptr );
                        /* Is it the Ternary Operator? */
                        if ( pe != NULL && *pe == '?' )
                        { /* Yes */
                            /* A ? enclosed between () would be Valid but it is not Supported */
                            ErrCode = ERR_OPERATOR_NOT_ALLOWED;
                            Error( "%s Unsupported Operator '%c' in Context",
                                   ErrCode, Make_Position_String( pe, temp ), *pe );
                            goto ret;
                        }
                        /* Is it the Colon Character? */
                        if ( pe != NULL && *pe != ':' )
                        { /* No */
                            ErrCode = Symbol_Missing_Error( pe, ':' );
                            goto ret;
                        }
                        /* Set Delimiter Position */
                        dp = pe;
                    }
                    /* Skip Blanks to the Left (this cannot fail) */
                    pe = Skip_Chars( pe - 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                }
                /* Keyword Name has a Delimiter? */
                if ( keywords[i].kw_delim != NUL_CHAR )
                { /* Yes */
                    /* Skip Blanks to the Right (this cannot fail) */
                    dp = Skip_Chars( ne, BLANK_CHARS, ParseTbl_ptr );
                    /* Is it the Delimiter? */
                    if ( dp != NULL && *dp != keywords[i].kw_delim )
                    { /* No */
                        ErrCode = Symbol_Missing_Error( dp, keywords[i].kw_delim );
                        goto ret;
                    }
                }

                /* Keyword has Arguments or Value Associated? */
                if ( pe != NULL )
                { /* Yes */
                    /*  Changed parsing_defs.h so that 'return' is now like 'goto'*/
                    /* Non Empty Args? */
                    if ( ps <= pe )
                    { /* Yes */
                        /* Verify Keyword Expression */
                        ncomma = 0;
                        neos = 0;
                        /* Set # of Commas and EOS */
                        switch ( keywords[i].kw_type )
                        {
                            case ITEM_KEYWORD_FOR:
                                neos = 2;
                            case ITEM_KEYWORD_IF:
                            case ITEM_KEYWORD_WHILE:
                            case ITEM_KEYWORD_SWITCH:
                            /* Changed parsing_defs.h so that 'return' is now like 'goto'*/
                            /* 'return' is Now Instrumented */
                            case ITEM_KEYWORD_RETURN:
                                ncomma = 99999L;
                        }
                        /* Count Commas, EOS, ... */
                        /* Go to End of Keyword Expression or Separator */
                        ne = ps;
                        while ( ( ne = Goto_Chars( ne + 1, ",;):", ParseTbl_ptr ) ) < pe )
                        {
                            /* Comma? */
                            if ( *ne == ',' )
                                if ( --ncomma < 0 )
                                    break;
                            /* EOS? */
                            if ( *ne == ';' )
                                if ( --neos < 0 )
                                    break;
                        }
                        /* Error at Start of Keyword Expression if All Expression Parsed */
                        if ( ne == pe )
                            ne = ps;
                        /*
                        * Must have less than # of Comma Allowed but
                        * # of EOS must Exactly Match
                        */
                        if ( ncomma < 0 || neos != 0 )
                        {
                            ErrCode = Invalid_KW_Expression( ne, ncomma < 0 ? "Comma ','" : "EOS ';'" );
                            goto ret;
                        }

                        /* Add Keyword Expression Region */
                        /* !!! VM: added ITEM_INSTRUMENTATION_OFF to mark a non-instrumented BASOP keyword expression region !!! */
                        if ( ( ErrCode = Add_Region( ParseTbl_ptr, (item_type == ITEM_INSTRUMENTATION_OFF ? ITEM_INSTRUMENTATION_OFF : 0) +
                            /* Keywords Expressions that Compute to Constants are stripped of KW_EXPR Attribute */
                            /* Added because when 'sizeof' is marked as a KW Expresion, instrumentation of something */
                            /* like 'sizeof(float) * 2' would be instrumented between the ')' and the '2'.*/
                           (( ( keywords[i].kw_type & ITEM_CONSTANT ) ? 0 : ITEM_KW_EXPR ) | keywords[i].expr_type), ps, pe + 1 ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                    }

                    /* Continue after Arguments or Value End */
                    ne = pe + 1;
                }

                /* Keyword has a Delimiter? */
                if ( dp != NULL )
                { /* Yes */
                    /* Add Skipped Region (for the Delimiter) */
                    if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_SKIPPED | ITEM_OPERATOR_DELIMITER, dp ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                    /* Continue after Delimiter End */
                    ne = dp + 1;
                }
                /* Continue after Keyword Name End */
            }

            /* Increase the Run Iterator */
            run++;
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Operators_Sub
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Operators_Sub(
    Parse_Context_def *ParseCtx_ptr,
    char *ptr,
    Item_Type *item_type1, /* Double Operand Operator Item Type (Default)*/
    Item_Type item_type2,  /* Single Operand Operator Item Type*/
    bool ignore_leading_kw = false
)
{
    TOOL_ERROR ErrCode = NO_ERR;

    Parse_Tbl_def *ParseTbl_ptr;
    int idx;
    char *start, *ptr2;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Save Start */
    start = ptr;

    /* Skip Blanks to the Left */
    ptr = Skip_Chars( ptr - 1, BLANK_CHARS, ParseTbl_ptr,
                      ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
    /* Comma, Opening Parenthesis, ... */
    switch ( *ptr )
    {
        /* Single/Double Operand Operator Part */
        case ')': /* Cast, Call Arguments, Instrumentation or Expression between '()'*/
        {
            /* Instrumentation or Call Arguments? */
            if ( ( idx = Find_Region( ptr, ParseTbl_ptr, ITEM_INSTRUMENTATION | ITEM_CALL_ARGS ) ) >= 0 )
            { /* Yes */
                /* Call Arguments and not Instrumentation? */
                if ( !( ParseTbl_ptr->Data[idx].item_type & ITEM_INSTRUMENTATION ) )
                { /* Yes */
                    /* Double Operand Operator (default) */
                    break;
                }
                /* Fall Through */
            }
            /* Cast or Expression between '()'? */
            else
            {     /* Yes */
                /* Is it a Constant? */
                if ( ignore_leading_kw && ( idx = Find_Region( ptr, ParseTbl_ptr, ITEM_CONSTANT ) ) >= 0 )
                { /* Yes */
                    /* Double Operand Operator (default) */
                    break;
                }

                /* Find Opening Parenthesis */
                if ( ( ptr2 = Goto_Match( ptr, ParseTbl_ptr ) ) == NULL )
                {
                    ErrCode = Parentheses_Error( ptr, '(' );
                    goto ret;
                }
                /* Check for Cast */
                ptr2 = Is_Cast( ptr2, ptr, ParseTbl_ptr );
                /* Cast? */
                if ( ptr2 == ptr )
                { /* Yes */
                    /* Fall Through */
                }
                /* Expression between '()' or Undertermined Cast */
                else
                {
                    /* Undertermined Cast? */
                    if ( ptr2 == NULL )
                    { /* Yes */
                        /* Add Warning Region to Table */
                        if ( ( ErrCode = Add_Region( ParseTbl_ptr,
                                                     ITEM_WARNING | *item_type1,
                                                     start ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                    }
                    /* Double Operand Operator (default) */
                    break;
                }
            }
        }
        /* Single Operand Operator Part */
        case ',': /* Comma       (in Argument List)*/
        case '(': /* Parenthesis (in Start of Argument List)*/
        case '{': /* Block Start*/
        case '}': /* Block End*/
        case ';': /* End of Statement*/
        {
            /* Single Operand Operator */
            *item_type1 = item_type2;
            break;
        }
        default:
        {
            /* Operator? */
            if ( ( idx = Find_Region( ptr, ParseTbl_ptr, ITEM_OPERATOR ) ) >= 0 )
            { /* Yes */
                /* Operator? */
                switch ( ParseTbl_ptr->Data[idx].item_type )
                { /* Yes */
                    case ITEM_OPERATOR_INC:
                    case ITEM_OPERATOR_DEC:
                    case ITEM_OPERATOR_BRACKET_CLOSE:
                    {
                        /* Double Operand Operator (default) */
                        break;
                    }
                    default:
                    {
                        /* Single Operand Operator */
                        *item_type1 = item_type2;
                    }
                }
            }
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Operators (All of Them)
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Operators(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

#define NOT_COMPOUNDABLE    0x0
#define COMPOUNDABLE_SINGLE 0x1
#define COMPOUNDABLE_DOUBLE 0x2

    static char operators[] = "+-*/%&^|~<>!=?:.["; /* Don't Search for ']'*/
    static const struct
    {
        Item_Type first_item_type;
        char second_char;
        Item_Type second_item_type;

        Item_Type doubled_item_type;
        int compoundable;
    } operators_parsing[] = {
        { ITEM_OPERATOR_PLUS, NUL_CHAR, ITEM_NONE, ITEM_OPERATOR_INC, COMPOUNDABLE_SINGLE },
        { ITEM_OPERATOR_MINUS, '>', ITEM_OPERATOR_MEMBER_PTR, ITEM_OPERATOR_DEC, COMPOUNDABLE_SINGLE },

        { ITEM_OPERATOR_MULT, NUL_CHAR, ITEM_NONE, ITEM_NONE, COMPOUNDABLE_SINGLE },
        { ITEM_OPERATOR_DIV, NUL_CHAR, ITEM_NONE, ITEM_NONE, COMPOUNDABLE_SINGLE },

        { ITEM_OPERATOR_MOD, NUL_CHAR, ITEM_NONE, ITEM_NONE, COMPOUNDABLE_SINGLE },
        { ITEM_OPERATOR_AND, NUL_CHAR, ITEM_NONE, ITEM_OPERATOR_ANDL, COMPOUNDABLE_SINGLE },
        { ITEM_OPERATOR_XOR, NUL_CHAR, ITEM_NONE, ITEM_NONE, COMPOUNDABLE_SINGLE },
        { ITEM_OPERATOR_OR, NUL_CHAR, ITEM_NONE, ITEM_OPERATOR_ORL, COMPOUNDABLE_SINGLE },
        { ITEM_OPERATOR_NOT, NUL_CHAR, ITEM_NONE, ITEM_NONE, NOT_COMPOUNDABLE },

        { ITEM_OPERATOR_LT, '=', ITEM_OPERATOR_LE, ITEM_OPERATOR_SHL, COMPOUNDABLE_DOUBLE },
        { ITEM_OPERATOR_GT, '=', ITEM_OPERATOR_GE, ITEM_OPERATOR_SHR, COMPOUNDABLE_DOUBLE },

        { ITEM_OPERATOR_NOTL, '=', ITEM_OPERATOR_NE, ITEM_NONE, NOT_COMPOUNDABLE },

        { ITEM_OPERATOR_EQ, NUL_CHAR, ITEM_NONE, ITEM_OPERATOR_EE, NOT_COMPOUNDABLE },

        { ITEM_OPERATOR_TERNARY, NUL_CHAR, ITEM_NONE, ITEM_NONE, NOT_COMPOUNDABLE },
        { ITEM_NONE, 0, 0, 0, 0 }, /* Use Same Definitions as Previous Entry*/

        { ITEM_OPERATOR_MEMBER, NUL_CHAR, ITEM_NONE, ITEM_NONE, NOT_COMPOUNDABLE },

        { ITEM_OPERATOR_BRACKET_OPEN, NUL_CHAR, ITEM_NONE, ITEM_NONE, NOT_COMPOUNDABLE },
        { ITEM_NONE, 0, 0, 0, 0 }, /* Use Same Definitions as Previous Entry*/
    };
    char *ptr, *start, *tmp;
    Item_Type item_type;
    Parse_Tbl_def *ParseTbl_ptr;
    int i;
    int compoundable;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /*
    * Search for the Following Operators:
    *   Logical: &, &&, | and ||
    *   Ternary: ?:
    *   Assignment: =
    *   Comparision: ==
    *   ...
    */
    while ( ( start = Goto_Chars( ptr, operators, ParseTbl_ptr, ITEM_FUNC_BLOCK ) ) != NULL )
    {
        /* Get Start */
        ptr = start;

        /* Get Operator Entry */
        for ( i = 0; operators[i] != *ptr; i++ )
            ;

        /* Set Initial Item Type */
        while ( item_type = operators_parsing[i].first_item_type, item_type == ITEM_NONE )
            i--;

        /* Advance */
        ptr++;

        /* Set Compoundability Flag */
        compoundable = operators_parsing[i].compoundable;

        /* Two Chars Operator? */
        if ( *ptr == operators_parsing[i].second_char )
        { /* Yes */
            /* Set Item Type */
            item_type = operators_parsing[i].second_item_type;
            /* Advance */
            ptr++;
            /* Done (Two Different Chars Operators are not Compoundable) */
        }
        /* Double Operator? */
        else if ( *ptr == *start && operators_parsing[i].doubled_item_type != ITEM_NONE )
        { /* Yes */
            /* Set Item Type */
            item_type = operators_parsing[i].doubled_item_type;
            /* Advance */
            ptr++;
            /* Set Compoundability Flag */
            compoundable &= COMPOUNDABLE_DOUBLE;
        }
        /* Normal Operator? */
        else
        { /* Yes */
            /* Set Compoundability Flag */
            compoundable &= COMPOUNDABLE_SINGLE;
        }

        /* Compoundable? */
        if ( compoundable )
        { /* Yes */
            /* Followed by Equal? */
            if ( *ptr == '=' )
            { /* Yes */
                /* Set Item Type */
                item_type |= ITEM_OPERATOR_COMPOUND;
                /* Advance */
                ptr++;
            }
        }

        /* Distinguish 'Address Of' vrss 'Bitwise And' */
        if ( item_type == ITEM_OPERATOR_AND )
        {
            ErrCode = Find_Operators_Sub( ParseCtx_ptr, start, &item_type, ITEM_OPERATOR_ADDR );
        }
        /* Distinguish 'Multiply' vrss 'Dereference' and Pointer Cast */
        else if ( item_type == ITEM_OPERATOR_MULT )
        {
            /* Skip Blanks (this cannot fail) */
            tmp = Skip_Chars( ptr, BLANK_CHARS, ParseTbl_ptr );
            /* Closing Parenthesis? */
            if ( *tmp == ')' )
            { /* Yes */
                /* Looks like a Pointer Cast */
                /* Continue After Closing Parenthesis */
                ptr = tmp + 1;
                continue;
            }
            ErrCode = Find_Operators_Sub( ParseCtx_ptr, start, &item_type, ITEM_OPERATOR_DEREF, true );
        }
        if ( ErrCode != NO_ERR )
            goto ret;
        /*  Moved before "Opening Bracket Operator is a Pair" so Operators have Proper Ordering*/
        /* Add Region */
        if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type, start, ptr ) ) != NO_ERR )
        {
            goto ret;
        }

        /* Opening Bracket Operator is a Pair */
        if ( item_type == ITEM_OPERATOR_BRACKET_OPEN )
        {
            /* Go To Closing Bracket */
            if ( ( tmp = Goto_Match( start, ParseTbl_ptr ) ) == NULL )
            {
                ErrCode = Brackets_Error( start, ']' );
                goto ret;
            }
            /* Add Region (for Closing Bracket) */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_OPERATOR_BRACKET_CLOSE, tmp ) ) != NO_ERR )
            {
                goto ret;
            }
            /* Add Region (for Index) */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_INDEX_EXPR, ptr, tmp ) ) != NO_ERR )
            {
                goto ret;
            }
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Data_Declarations
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Data_Declarations(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    int idx;
    char *ptr, *tmp;
    char *start, *end;

    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Record to Fill */
    Parse_Rec_def ParseRecord;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Set Region Link */
    ParseRecord.item_link = NULL;

    /* for all Functions */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_FUNC_BLOCK, idx ) ) >= 0; idx++ )
    {
        /* Get Function Block Region Pointer */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        /* Save Block Range */
        start = ParseRec_ptr->item_start;
        end = ParseRec_ptr->item_end - 1;

        /* Set Region Type */
        ParseRecord.item_type = ITEM_DATA_DECL_1ST;

        do
        {
            /* Skip Blanks to the Right (start after Block Start) */
            ptr = Skip_Chars( start + 1, BLANK_CHARS, ParseTbl_ptr );

            /* Set Region Start */
            ParseRecord.item_start = ptr;

            /* Scan until End of Block Reached */
            while ( ptr != end )
            {
                /* Function Start Macro? */
                if ( ( start = memwordcmp( ptr, FUNC_START_STRING ) ) != NULL )
                { /* Yes */
                    /* Add Instrumentation Region */
                    if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_DATA_DECL | ITEM_INSTRUMENTATION, ptr, start ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                    /* Skip Blanks to the Right */
                    ptr = Skip_Chars( start, BLANK_CHARS, ParseTbl_ptr );
                }
                /* Any of the Known Data Types, Type Definition
                   Type Modifier or Storage Class? */
                if ( ( start = memwordcmp( ptr, DATA_DEF_STRING ) ) != NULL )
                { /* Yes */
                    /* Will Proceed to Next Statement */
                }
                /* Any of the Known Keywords? */
                else if ( Find_Region( ptr, ParseTbl_ptr, ITEM_KEYWORD ) >= 0 )
                { /* Yes */
                    /* Done */
                    break;
                }
                /* Any of the Counting Macros or Functions? */
                else if ( ( start = memwordcmp( ptr, COUNTING_CODE_STRING ) ) != NULL )
                { /* Yes */
                    /* Done */
                    break;
                }
                /* Is it an Identifier? */
                else if ( IS_IDENTIFIER_START_CHAR( *ptr ) )
                { /* Yes */
                    /* Move Down to Next Non Identifier Character (this cannot fail) */
                    start = Skip_Identifier( ptr );
                    /* Move Down to Next Non Blank Character that is not in a Comment Region */
                    start = Skip_Chars( start, BLANK_CHARS, ParseTbl_ptr );
                    /* Is it an Identifier? */
                    if ( IS_IDENTIFIER_START_CHAR( *start ) )
                    {   /* Yes */
                        /***********************************************
                        * We have: Label Label
                        ***********************************************/
                        /* Will Proceed to Next Statement */
                    }
                    /* Is it a Function Call Symbol? */
                    else if ( *start == '(' )
                    {   /* Yes */
                        /***********************************************
                        * Now, either we have:
                        *     type (*func_ptr)...    OR
                        *     func (*arg)
                        * So, proceed to end of Group to check
                        * for another Group.
                        * Label ()() cannot be valid code; unless
                        * Label is a macro that hides a cast or function
                        * which is not supported.
                        ***********************************************/

                        /* Find Closing Parenthesis */
                        tmp = Goto_Match( start, ParseTbl_ptr );
                        /* Valid End of Group? */
                        if ( tmp == NULL || tmp >= end )
                        { /* No */
                            ErrCode = Parentheses_Error( start, ')' );
                            goto ret;
                        }
                        /* Move Down to Next Non Blank Character that is not in a Comment */
                        start = Skip_Chars( tmp + 1, BLANK_CHARS, ParseTbl_ptr );
                        /* Is it a Function Call Symbol? */
                        if ( *start == '(' )
                        {   /* Yes */
                            /********************************************************
                            * Looks like a Prototype (it is a func ptr declaration)
                            ********************************************************/
                            /* Will Proceed to Next Statement */
                        }
                        else
                        {   /* No */
                            /***********************************
                            * Looks looks like a Function Call
                            ***********************************/
                            /* Done */
                            break;
                        }
                    }
                    /* Is it a Pointer Symbol? */
                    else if ( *start == '*' )
                    { /* Yes */
                        /* Move Down to Next Non Blank Character that is not in a Comment */
                        start = Skip_Chars( start + 1, BLANK_CHARS, ParseTbl_ptr );
                        /* Is it an Identifier? */
                        if ( IS_IDENTIFIER_START_CHAR( *start ) )
                        {   /* Yes */
                            /****************************
                            * We have: Label * Label...
                            ****************************/
                            /* Is it the 'const' Modifier? */
                            if ( memwordcmp( start, CONST_STRING ) )
                            {
                                /****************************
                                * We have: Label * const...
                                ****************************/
                                /* Will Proceed to Next Statement */
                            }
                            else
                            {
                                /* Move Down to Next Non Identifier Character (this cannot fail) */
                                start = Skip_Identifier( start );
                                /* Move Down to Next Non Blank Character that is not in a Comment Region */
                                start = Skip_Chars( start, BLANK_CHARS, ParseTbl_ptr );
                                /* End of Statement or Separator? */
                                if ( *start == ';' || *start == ',' || *start == '=' )
                                {   /* Yes */
                                    /*************************
                                    * We have: type * label
                                    *************************/
                                    /* Will Proceed to Next Statement */
                                }
                                else
                                {
                                    /* Array Start? */
                                    if ( *start == '[' )
                                    {
                                        /* Find Closing Bracket */
                                        if ( ( tmp = Goto_Match( start, ParseTbl_ptr ) ) == NULL )
                                        {
                                            ErrCode = Brackets_Error( start, '[' );
                                            goto ret;
                                        }
                                        /* Move Down to Next Non Blank Character that is not in a Comment */
                                        start = Skip_Chars( tmp + 1, BLANK_CHARS, ParseTbl_ptr );
                                        /* End of Statement or Separator? */
                                        if ( *start == ';' || *start == ',' || *start == '=' )
                                        { /* Yes */
                                            /*************************
                                            * We have: type * label[]
                                            *************************/
                                            /* Will Proceed to Next Statement */
                                        }
                                        else
                                        {
                                            /******************
                                            * Looks like Code
                                            ******************/
                                            /* Done */
                                            break;
                                        }
                                    }
                                    else
                                        /******************
                                    * Looks like Code
                                    ******************/
                                        /* Done */
                                        break;
                                }
                            }
                        }
                        /* Is it a Function Call Symbol? */
                        else if ( *start == '(' )
                        {   /* Yes */
                            /***********************************************
                            * Now, either we have:
                            *     type * (*func_ptr)...    OR
                            *     label * (...)
                            * So, proceed to end of Group to check
                            * for another Group.
                            ***********************************************/
                            /* Find Closing Parenthesis */
                            tmp = Goto_Match( start, ParseTbl_ptr );
                            /* Valid End of Group? */
                            if ( tmp == NULL || tmp >= end )
                            { /* No */
                                ErrCode = Parentheses_Error( start, ')' );
                                goto ret;
                            }
                            /* Move Down to Next Non Blank Character that is not in a Comment */
                            start = Skip_Chars( tmp + 1, BLANK_CHARS, ParseTbl_ptr );
                            /* Is it a Function Call Symbol? */
                            if ( *start == '(' )
                            {   /* Yes */
                                /********************************************************
                                * Looks like a Prototype (it is a func ptr declaration)
                                ********************************************************/
                                /* Will Proceed to Next Statement */
                            }
                            else
                            {   /* No */
                                /*****************************************************
                                * Looks looks like an Expression between Parentheses
                                *****************************************************/
                                /* Done */
                                break;
                            }
                        }
                        else
                        {
                            /******************
                            * Looks like Code
                            ******************/
                            /* Done */
                            break;
                        }
                    }
                    else
                    {
                        /******************
                        * Looks like Code
                        ******************/
                        /* Done */
                        break;
                    }
                }
                else /* Is it an Identifier? */
                {    /* No */
                    /* Done */
                    break;
                }

                /* Go to End of Statement */
                /* !!!!! Incorrect when have 'int a' then 'BRANCH(1)', 'a += 2', ... on Next Line (Known Limitation)*/
                if ( ( start = Goto_EOS( start, ParseTbl_ptr ) ) == NULL )
                {
                    /* Goto Block Start or End (this cannot fail, all blocks are balanced) */
                    ptr = Goto_Chars( ptr, "{}", ParseTbl_ptr );
                    ErrCode = Expected_EOS( ptr );
                    goto ret;
                }

                /* Skip Blanks to the Right */
                ptr = Skip_Chars( start + 1, BLANK_CHARS, ParseTbl_ptr );
            }

            /* Set Region End */
            ParseRecord.item_end = ptr;

            /* Is Region Empty? */
            if ( !Is_Empty_Region( ParseRecord.item_start, ptr, ParseTbl_ptr ) )
            {
                /* Add Data Definition Region */
                if ( ( ErrCode = Add_Region( ParseTbl_ptr, &ParseRecord ) ) != NO_ERR )
                {
                    goto ret;
                }
            }

            /* Change Data Declaration Region Type */
            ParseRecord.item_type = ITEM_DATA_DECL_SUB | ITEM_WARNING;

            /* Go to Next Code Block */
            start = Find_String( ptr, "{", ParseTbl_ptr, ITEM_ANY );
        } while ( start != NULL && start < end );
    }

ret:
    return ErrCode;
}


/*-------------------------------------------------------------------*
 * Find_End_Preproc_Block
 *-------------------------------------------------------------------*/

static char* Find_End_Preproc_Block(
    const char* start,
    Parse_Tbl_def* ParseTbl_ptr
)
{
    int idx1, idx2, cnt_ifdef;
    Parse_Rec_def* ParseRec_ptr;
    char* ptr, * ptr2;

    ptr = NULL;
    ptr2 = (char*)start;
    cnt_ifdef = 0;
    do
    {
        /* get the next region */
        idx1 = Find_Region(ptr2, ParseTbl_ptr, ITEM_PREPROC_CMD | ITEM_PREPROC_INC);
        idx2 = Find_Region(ptr2, ParseTbl_ptr, ITEM_PREPROC_CMD | ITEM_PREPROC_COND);

        if (idx1 >= 0)
        {
            /* We're in #include region */
            /* Get the next record - should be Preprocessor Directive Arguments */
            ParseRec_ptr = &ParseTbl_ptr->Data[idx1 + 1];

            if (cnt_ifdef == 0)
            {
                /* Go To End of Preprocessor Directive */
                ptr = ParseRec_ptr->item_end;
            }

            /* Advance pointer by skipping spaces and comments-only sections */
            ptr2 = Skip_Chars(ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_COMMENT);
        }
        else if (idx2 >= 0)
        {
            /* We're in #if, #ifdef, #ifndef, #else, #elif and #endif region */

            /* Get the current record */
            ParseRec_ptr = &ParseTbl_ptr->Data[idx2];

            /* check which type of preproc macro we have found */
            if (memwordcmp(ptr2, "#endif") != NULL)
            {
                /* this is #endif */
                if (cnt_ifdef > 0)
                {
                    /* decrement the counter */
                    cnt_ifdef--;

                    if (cnt_ifdef == 0)
                    {
                        /* the top-most #if .. #endif section is finished */
                        /* Move the Pointer to the end of #endif */
                        ptr = ParseRec_ptr->item_end;
                    }
                }
                else
                {
                    /* this should not happen */
                    Error("Error locating the end of the pre-processor section!\n");
                    return NULL;
                }

                /* Advance pointer by skipping spaces and comments-only sections */
                ptr2 = Skip_Chars(ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_COMMENT);
            }
            else if (memwordcmp(ptr2, "#if #ifdef #ifndef #elif") != NULL)
            {
                /* this is one of #if, #ifdef, #ifndef, #elif */
                if (memwordcmp(ptr2, "#if #ifdef #ifndef") != NULL)
                {
                    cnt_ifdef++;
                }

                /* Get the next record - should be Preprocessor Directive Arguments */
                ParseRec_ptr = &ParseTbl_ptr->Data[idx2 + 1];

                /* Advance pointer to the end of Preprocessor Arguments (Skip Spaces and Comments Only) */
                ptr2 = Skip_Chars(ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_COMMENT);
            }
            else if (memwordcmp(ptr2, "#else") != NULL)
            {
                /* this is #else */

                /* Advance pointer by skipping spaces and comments-only sections */
                ptr2 = Skip_Chars(ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_COMMENT);
            }
            else
            {
                /* it must be something else */
                Error("Error locating the end of the pre-processor section!\n");
                return NULL;
            }
        }

    } while (idx1 >= 0 || idx2 >= 0);

    return ptr;
}


/*-------------------------------------------------------------------*
 * Find_Preproc_Instrumentation
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Preproc_Instrumentation(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    static struct
    {
        const char *identifier;
        Item_Type included_types;
        Item_Type excluded_types;
    } preproc_instr[] = {
        { FUNC_COUNTING_MACRO_STRING, ITEM_CSTE_VALUE, ITEM_ENCLOSED },
        { "rsize", ITEM_CSTE_VALUE, ITEM_COMMENT },
        { WMC_TOOL_INCLUDE_STRING, ITEM_PREPROC_ARGS, ITEM_COMMENT },
        { WMC_TOOL_VERSION_STRING, ITEM_CSTE_NAME, ITEM_ENCLOSED },
        { WMC_TOOL_SKIP_STRING, ITEM_PREPROC_ARGS | ITEM_PREPROC_OTHER, ITEM_COMMENT }
    };

    int idx;
    char *ptr, *end;
    int i;

    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    for ( i = 0; i < (int)(nitems( preproc_instr )); i++ )
    {
        /* Start at Beginning */
        ptr = ParseCtx_ptr->File.Data;

        /* Search for Function Instrumentation Macro in Definitions (and some other defines/macros) */
        while ( ( ptr = Find_Identifier( ptr, preproc_instr[i].identifier, ParseTbl_ptr,
                                         preproc_instr[i].included_types,
                                         preproc_instr[i].excluded_types, &idx ) ) != NULL )
        {
            /* Go to Preprocessor Command Start */
            idx = Find_Region( NULL, ParseTbl_ptr, ITEM_PREPROC_CMD, idx, BACKWARDS );
            /* Get Record */
            ParseRec_ptr = &ParseTbl_ptr->Data[idx];
            /* Mark as Instrumentation */
            ParseRec_ptr->item_type |= ITEM_INSTRUMENTATION;
            /* for All Item on Preprocessor Command Line */
            do
            {
                /* Next Parse Table Record */
                ParseRec_ptr++;
                /* Mark as Instrumentation */
                ParseRec_ptr->item_type |= ITEM_INSTRUMENTATION | ITEM_FUNC_COUNTERS_AUTO;
                /* Get Item End */
                end = ParseRec_ptr->item_end;
            } while ( !IS_EOL_CHAR( *end ) );

            /* Go to End of Line (this cannot fail) */
            ptr = strlend( end ) + 1;

            /* Continue at 'ptr' Position */
        }
    }

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_ROM_Counting_Instrumentation
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_ROM_Counting_Instrumentation(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    int idx, ln, i;
    char *ptr, *ptr_start, *ptr_end;
    const char* keywords[3] = { CONST_DATA_SIZE_FUNC_PREFIX_STRING, PROM_SIZE_FUNC_PREFIX_STRING, "Const_Data_PROM_Table"};

    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Find all ROM instrumentation functions, e.g. 'Const_Data_Size', 'PROM_Size', 'Const_Data_PROM_Table' */
    for (i = 0; i < (int)(sizeof(keywords) / sizeof(char*)); i++)
    {
        /* Start at Beginning */
        ptr = ParseCtx_ptr->File.Data;

        while ((ptr = Find_String(ptr, keywords[i], ParseTbl_ptr, ITEM_ANY, ITEM_ENCLOSED | ITEM_PREPROC_LINE | ITEM_COMMENT, &idx)) != NULL)
        {
            if ((idx = Find_Region(ptr, ParseTbl_ptr, ITEM_FUNC_NAME)) >= 0)
            {
                /* Found in Function Region */
                ParseRec_ptr = &ParseTbl_ptr->Data[idx];
                /* Mark Function Name as Instrumentation */
                ParseRec_ptr++->item_type |= (ITEM_INSTRUMENTATION | ITEM_SKIPPED);
                /* Mark Function Proto as Instrumentation */
                ParseRec_ptr++->item_type |= (ITEM_INSTRUMENTATION | ITEM_SKIPPED);
                /* Mark Function Block as Instrumentation */
                ParseRec_ptr->item_type |= (ITEM_INSTRUMENTATION | ITEM_SKIPPED);
                /* Advance */
                ptr = ParseRec_ptr->item_end;

                continue;
            }
            else if (strcmp(keywords[i], "Const_Data_PROM_Table") == 0)
            {
                /* special case - ROM_Size_Lookup_Table Const_Data_PROM_Table[] = {} */
                /* include all white space to the left */
                ptr_start = Skip_Chars(ptr-1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER | ITEM_FUNC_COUNTERS_AUTO, BACKWARDS) + 1;

                /* check if it's preceded by 'ROM_Size_Lookup_Table' */
                ln = (int)strlen("ROM_Size_Lookup_Table");
                if (strncmp(ptr_start - ln, "ROM_Size_Lookup_Table", ln) == 0)
                {
                    ptr_start -= ln;
                    ptr_start = Skip_Chars(ptr_start-1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER | ITEM_FUNC_COUNTERS_AUTO, BACKWARDS) + 1;
                }

                /* Find the end of function name */
                ptr_end = ptr;
                while (!IS_CHAR_IN_STRING(*ptr_end, " ,;()[]{}"))
                {
                    ptr_end++;
                }
                ptr_end = Skip_Chars(ptr_end, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);

                /* check if it's succeeded by [] */
                if (*ptr_end == '[')
                {
                    /* Find Closing Bracket -> this must succeed */
                    ptr_end = Goto_Match(ptr_end, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);

                    if (ptr_end == NULL)
                    { /* No */
                        ErrCode = Brackets_Error(ptr_end, ']');
                        goto ret;
                    }
                    ptr_end++;      /* Go past the closing ']' */
                    ptr_end = Skip_Chars(ptr_end, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);
                }

                /* check if it's succeeded by = */
                if (*ptr_end == '=')
                {
                    ptr_end++;
                    ptr_end = Skip_Chars(ptr_end, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);

                    /* check if it's succeeded by {...} */
                    if (*ptr_end == '{')
                    {
                        /* Find Closing Parenthesis -> this must succeed */
                        ptr_end = Goto_Match(ptr_end, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);

                        if (ptr_end == NULL)
                        { /* No */
                            ErrCode = Parentheses_Error(ptr_end, '}');
                            goto ret;
                        }
                        ptr_end++;      /* Go past the closing '}' */
                        ptr_end = Skip_Chars(ptr_end, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);

                        /* Check for terminating ';' */
                        if (*ptr_end == ';')
                        {
                            ptr_end++;
                            ptr_end = Skip_Chars(ptr_end, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);
                        }

                        /* Skip EOL character(s), if present */
                        if (IS_EOL_SEQ(ptr_end))
                        {
                            ptr_end += 2;
                        }
                        else if (IS_EOL_CHAR(*ptr_end))
                        {
                            ptr_end++;
                        }
                    }
                }


                /* Add a New Instrumentation Region */
                if ((ErrCode = Add_Region(ParseTbl_ptr, ITEM_INSTRUMENTATION | ITEM_SKIPPED | ITEM_DATA_DECL, ptr_start, ptr_end)) != NO_ERR)
                {
                    goto ret;
                }

                /* Advance pointer */
                ptr = ptr_end++;

                continue;
            }

            /* check if it's preceded by 'Get_' */
            ln = (int)strlen("Get_");
            if (strncmp(ptr - ln, "Get_", ln) == 0)
            {
                ptr -= strlen("Get_");
            }

            /* Find the end of function name */
            ptr_end = ptr;
            while (!IS_CHAR_IN_STRING(*ptr_end, " ,;()[]{}"))
            {
                ptr_end++;
            }

            /* Add a New Function Name Region */
            if ((ErrCode = Add_Region(ParseTbl_ptr, ITEM_INSTRUMENTATION | ITEM_SKIPPED | ITEM_FUNC_NAME, ptr, ptr_end)) != NO_ERR)
            {
                goto ret;
            }

            /* check if it's succeeded by (void) */
            ptr = Skip_Chars(ptr_end, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);

            if (*ptr == '(')
            {
                /* Find Closing Parenthesis -> this must succeed */
                ptr_end = Goto_Match(ptr, ParseTbl_ptr, ITEM_ANY, ITEM_COMMENT);

                if (ptr_end == NULL)
                { /* No */
                    ErrCode = Parentheses_Error(ptr_end, ')');
                    goto ret;
                }
                ptr_end++;      /* Go past the closing ')' */

                /* Check for terminating ';' */
                ptr_end = Skip_Chars(ptr_end, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);
                if (*ptr_end == ';')
                {
                    ptr_end++;
                    ptr_end = Skip_Chars(ptr_end, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);
                }

                /* Advance pointer to the end of line */
                if (!IS_EOL_SEQ(ptr_end))
                { /* No */
                    /* Either CR or LF Char */
                    ptr_end++;
                }
                else
                {
                    /* A Pair of CR and LF Chars */
                    ptr_end += 2;
                }

                /* Add a New Function Prototype Region */
                if ((ErrCode = Add_Region(ParseTbl_ptr, ITEM_INSTRUMENTATION | ITEM_SKIPPED | ITEM_FUNC_PROTO, ptr, ptr_end)) != NO_ERR)
                {
                    goto ret;
                }
            }

            ptr = ptr_end;
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Constants_Sub
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Constants_Sub(
    Parse_Context_def *ParseCtx_ptr,
    const char *identifier,
    Item_Type item_type )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr, *ptr2;

    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Get Identifier Length */
    size_t len = strlen( identifier );

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Find Identifier */
    while ( ( ptr = Find_Identifier( ptr, identifier, ParseTbl_ptr ) ) != NULL )
    {
        /* Set End */
        ptr2 = ptr + len;
        /* Add Region */
        if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type | ITEM_SKIPPED | ITEM_CONSTANT | ITEM_CSTE_ZERO,
                                     ptr, ptr2 ) ) != NO_ERR )
        {
            goto ret;
        }
        /* Advance */
        ptr = ptr2 + 1;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Find_Constants
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Constants(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr, *ptr2, *ptr3, *ptr4;
    char temp_str[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/

    Parse_Tbl_def *ParseTbl_ptr;

    char max_digit;
    int base;
    long temp;

    Item_Type item_type;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /*******************
    *   Find Labels   *
    *******************/

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Find Colon Character */
    while ( ( ptr = Find_String( ptr, ":", ParseTbl_ptr ) ) != NULL )
    {
        /* Skip Blanks to the Left */
        ptr2 = Skip_Chars( ptr - 1, BLANK_CHARS, ParseTbl_ptr,
                           ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
        /* Valid Identifier Character? */
        if ( IS_IDENTIFIER_CHAR( *ptr2 ) )
        { /* Yes */
            /* Move Up to Next Non Identifier Character (from Name End) */
            ptr3 = Skip_Identifier( ptr2, BACKWARDS );
            /* Skip Blanks to the Left */
            ptr4 = Skip_Chars( ptr3, BLANK_CHARS, ParseTbl_ptr,
                               ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
            /* End of Statement or Block Start/End? */
            if ( IS_CHAR_IN_STRING( *ptr4, ";{}" ) )
            { /* Yes */
                /* Add Skipped Region (for the Label Delimiter) */
                if ( ( ErrCode = Add_Region( ParseTbl_ptr,
                                             ITEM_SKIPPED | ITEM_OPERATOR_DELIMITER, ptr ) ) != NO_ERR )
                {
                    goto ret;
                }
                /* Add Label Region (Colon is not Part of the Label) */
                if ( ( ErrCode = Add_Region( ParseTbl_ptr,
                                             ITEM_SKIPPED | ITEM_LABEL,
                                             ptr3 + 1, ptr2 + 1 ) ) != NO_ERR )
                {
                    goto ret;
                }
            }
        }
        /* Advance to Next Char */
        ptr++;
    }

    /**************************
    * Find Special Constants *
    **************************/

    if ( ( ErrCode = Find_Constants_Sub( ParseCtx_ptr, "NULL", ITEM_NONE ) ) != NO_ERR )
    {
        goto ret;
    }

    /****************
    * Find Numbers *
    ****************/

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Find Numbers */
    while ( ( ptr = Goto_Chars( ptr, "0123456789", ParseTbl_ptr, ITEM_FUNC_BLOCK,
                                ITEM_NOT_SEARCHED | ITEM_INSTRUMENTATION ) ) != NULL )
    {
        /* Previous Character is Identifier? */
        if ( IS_IDENTIFIER_CHAR( PREV_CHAR( ptr ) ) )
        { /* Yes */
            /* Get Out of Identifier */
            ptr = Skip_Identifier( ptr );
        }
        else
        { /* No */
            max_digit = 0;
            /* Set Default Numner Base */
            base = 10;
            /* Set Item Type */
            item_type = ITEM_SKIPPED | ITEM_CONSTANT | ITEM_CSTE_ZERO;
            /* Back up if There is a Dot in Front of the Digit */
            if ( PREV_CHAR( ptr ) == '.' )
                ptr--;
            /* Save Start */
            ptr2 = ptr;
            /* Process Number */
            for ( ;; )
            {
                char chr = toupper( *ptr );
                /* Dot? */
                if ( chr == '.' )
                { /* Yes */
                    /* Already Seen '.' or in Hex or Octal Number? */
                    if ( item_type & ( ITEM_CSTE_FLOAT | ITEM_CSTE_HEX | ITEM_CSTE_OCTAL ) )
                    { /* Yes */
                        item_type |= ITEM_CSTE_ERROR;
                        /* Stop */
                        break;
                    }
                    item_type |= ITEM_CSTE_FLOAT;
                }
                /* Hexadecimal? */
                else if ( chr == 'X' )
                { /* Yes */
                    /* Float, Octal, Already Hex Number or Previous Non-Zero Digits? */
                    if ( item_type & ( ITEM_CSTE_FLOAT | ITEM_CSTE_OCTAL | ITEM_CSTE_HEX | ITEM_CSTE_NZ ) )
                    { /* Yes */
                        item_type |= ITEM_CSTE_ERROR;
                        /* Stop */
                        break;
                    }
                    item_type |= ITEM_CSTE_HEX;
                    /* Set Default Numner Base */
                    base = 16;
                }
                /* Digit? */
                else if ( IS_DIGIT_CHAR( chr ) )
                { /* Yes */
                    /* Not Zero? */
                    if ( chr != '0' )
                    { /* Yes */
                        item_type |= ITEM_CSTE_NZ;
                        item_type &= ~ITEM_CSTE_ZERO;
                    }
                    /* Keep Highest Digit */
                    if ( max_digit < chr )
                        max_digit = chr;
                    /* Octal Numbers are Validated Later */
                }
                /* Exponent and not Hex Number? */
                else if ( chr == 'E' && !( item_type & ITEM_CSTE_HEX ) )
                { /* Yes */
                    item_type |= ITEM_CSTE_FLOAT;
                    /* Advance */
                    ptr++;
                    /* Valid Exponent? */
                    if ( *ptr != '+' && *ptr != '-' && !IS_DIGIT_CHAR( *ptr ) )
                    { /* No */
                        item_type |= ITEM_CSTE_ERROR;
                        /* Stop */
                        break;
                    }
                    /* Skip Exponent */
                    do
                    {
                        ptr++;
                    } while ( IS_DIGIT_CHAR( *ptr ) );
                    /* Done */
                    break;
                }
                /* Float Suffix and not Hex or Octal Number? */
                else if ( chr == 'F' && ( item_type & ITEM_CSTE_FLOAT || !( item_type & ( ITEM_CSTE_HEX | ITEM_CSTE_OCTAL ) ) ) )
                {
                    /* Already Suffixed */
                    if ( item_type & ITEM_CSTE_SUFFIX )
                    { /* Yes */
                        item_type |= ITEM_CSTE_ERROR;
                        /* Stop */
                        break;
                    }
                    item_type |= ITEM_CSTE_FLOAT | ITEM_CSTE_SUFFIX;
                }
                /* Long Suffix and not Float Number? */
                else if ( chr == 'L' && !( item_type & ITEM_CSTE_FLOAT ) )
                {
                    /* Already Suffixed */
                    if ( item_type & ITEM_CSTE_SUFFIX )
                        if ( toupper( PREV_CHAR( ptr ) ) != 'L' )
                            if ( toupper( PREV_CHAR( ptr ) ) != 'U' )
                            { /* Yes */
                                item_type |= ITEM_CSTE_ERROR;
                                /* Stop */
                                break;
                            }
                    item_type |= ITEM_CSTE_SUFFIX;
                }
                /* Unsigned Suffix and not Float Number? */
                else if ( chr == 'U' && !( item_type & ITEM_CSTE_FLOAT ) )
                {
                    /* Already Suffixed */
                    if ( item_type & ITEM_CSTE_SUFFIX )
                    { /* Yes */
                        item_type |= ITEM_CSTE_ERROR;
                        /* Stop */
                        break;
                    }
                    item_type |= ITEM_CSTE_SUFFIX;
                }
                /* Hex Letter Character? */
                else if ( chr >= 'A' && chr <= 'F' )
                {
                    /* Hex Number in Progress? */
                    if ( !( item_type & ITEM_CSTE_HEX ) )
                    { /* No */
                        item_type |= ITEM_CSTE_ERROR;
                        /* Stop */
                        break;
                    }
                    item_type |= ITEM_CSTE_NZ;
                    item_type &= ~ITEM_CSTE_ZERO;
                }
                /* Identifier? */
                else if ( IS_IDENTIFIER_CHAR( *ptr ) )
                {
                    item_type |= ITEM_CSTE_ERROR;
                    /* Stop */
                    break;
                }
                else
                {
                    /* Done */
                    break;
                }
                /* Advance */
                ptr++;
            }
            /* Not Float nor Hex Number? */
            if ( !( item_type & ( ITEM_CSTE_FLOAT | ITEM_CSTE_HEX ) ) )
            { /* Yes */
                /* Octal Number? (Starts with 0 and is Non-Zero Value) */
                if ( *ptr2 == '0' && item_type & ITEM_CSTE_NZ )
                { /* Yes */
                    item_type |= ITEM_CSTE_OCTAL;
                    /* Set Default Number Base */
                    base = 8;
                    /* One Digit Greater then 7? */
                    if ( max_digit > '7' )
                    { /* Yes */
                        item_type |= ITEM_CSTE_ERROR;
                    }
                }
            }

            /* Number Ok? */
            if ( item_type & ITEM_CSTE_ERROR )
            { /* No */
                ErrCode = ERR_BAD_NUMBER;
                Error( "%s Invalid Number",
                       ErrCode, Make_Position_String( ptr2, temp_str ) );
                goto ret;
            }

            /* Not Float nor Zero Number? */
            if ( !( item_type & ( ITEM_CSTE_FLOAT | ITEM_CSTE_ZERO ) ) )
            { /* Yes */
                /* Convert to Binary */
                temp = strtol( ptr2, NULL, base );
                /* Number is Power of 2? */
                if ( ( ( temp - 1 ) & temp ) == 0 )
                { /* Yes */
                    item_type |= ITEM_CSTE_POWEROF2;
                }
            }

            /* Leading Sign? */
            if ( ptr3 = ptr2 - 1, *ptr3 == '+' || *ptr3 == '-' )
            {
#ifdef DEBUG_PRINT
                ptr4 = ptr2;
#endif
                /* Move Left (This Cannot Fail) */
                ptr3 = Skip_Chars( ptr3 - 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                /* Closing Operator? (These Output a Value; The Leading Sign is then an Operation) */
                if ( *ptr3 != ')' && *ptr3 != ']' )
                { /* No */
                    /* Not an Identifier or Keyword? */
                    if ( !IS_IDENTIFIER_CHAR( *ptr3 ) || Find_Region( ptr3, ParseTbl_ptr, ITEM_KEYWORD ) >= 0 )
                    { /* No (Then an Operator) */
                        /* Leading Sign is Part of Numeric Constant */
                        ptr2--;
                    }
                }
#ifdef DEBUG_PRINT
                {
                    char str[40];
#define LEN 15
                    memset( str, 0, sizeof( str ) );
                    memmove( str, ptr2 - LEN, LEN );
                    memmove( str + LEN + 1, ptr, LEN );

                    for ( temp = 0; temp < sizeof( str ); temp++ )
                    {
                        if ( IS_EOL_CHAR( str[temp] ) || IS_RESERVED_CHAR( str[temp] ) )
                        {
                            str[temp] = ' ';
                        }
                    }

                    fprintf( stdout, "%s: Number=%s#%s#\n", ptr2 == ptr4 ? "+/- is Oper" : "+/- is Cste", str, memstr( ptr2, ptr ) /*, str+LEN+1*/ );
                }
#endif
            }

            /* Add Region */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type, ptr2, ptr ) ) != NO_ERR )
            {
                goto ret;
            }
        }
        /* Advance */
        ptr++;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Get_Call_Type
 *-------------------------------------------------------------------*/
static Item_Type Get_Call_Type(
    char *name_start )
{
    static char ins_math_funcs_string[STRING_LENGTH( MATH_FUNCTS_STRING ) * 2] = { NUL_CHAR };
    static char ins_sys_alloc_funcs_string[STRING_LENGTH( SYSTEM_ALLOC_FUNCTS_STRING ) * 2] = { NUL_CHAR };

    /* First Call? */
    if ( *ins_sys_alloc_funcs_string == NUL_CHAR )
    { /* Yes */
        /* Make Instrumented Math Functions Names */
        Instrument_Words( ins_sys_alloc_funcs_string, SYSTEM_ALLOC_FUNCTS_STRING );
    }

    /* First Call? */
    if ( *ins_math_funcs_string == NUL_CHAR )
    { /* Yes */
        /* Make Instrumented Math Functions Names */
        Instrument_Words( ins_math_funcs_string, MATH_FUNCTS_STRING );
    }

    /* Any of the Data Types? */
    if ( memwordcmp( name_start, TYPES_STRING ) != NULL )
    { /* Yes */
        /* Not a Function Call */
        return ITEM_NONE;
    }

    /* Any of the System Functions? */
    if ( memwordcmp( name_start, SYSTEM_FUNCTS_STRING ) != NULL )
    { /* Yes */
        return ITEM_FUNC_SYSTEM | ITEM_SKIPPED;
    }

    /* Any of the System Allocation Functions? */
    if ( memwordcmp( name_start, SYSTEM_ALLOC_FUNCTS_STRING ) != NULL )
    { /* Yes */
        return ITEM_FUNC_SYSTEM;
    }

    /* Any of the Instrumented System Allocation Functions? */
    if ( memwordcmp( name_start, ins_sys_alloc_funcs_string ) != NULL )
    { /* Yes */
        return ITEM_FUNC_SYSTEM | ITEM_INSTRUMENTED;
    }

    /* Any of the Math Functions? */
    if ( memwordcmp( name_start, MATH_FUNCTS_STRING ) != NULL )
    { /* Yes */
        return ITEM_FUNC_MATH;
    }

    /* Any of the Instrumented Math Functions? */
    if ( memwordcmp( name_start, ins_math_funcs_string ) != NULL )
    { /* Yes */
        return ITEM_FUNC_MATH | ITEM_INSTRUMENTED;
    }

    /* Any of the Manual Counting Macros? */
    if ( memwordcmp( name_start, MANUAL_COUNTING_MACROS_STRING ) != NULL )
    { /* Yes */
        return ITEM_FUNC_COUNTERS_MAN | ITEM_INSTRUMENTATION | ITEM_SKIPPED;
    }

    /* Any of the Left Automatic Counting Macros? */
    if ( memwordcmp( name_start, AUTO_LEFT_COUNTING_MACRO_STRING ) != NULL )
    { /* Yes */
        return ITEM_FUNC_COUNTERS_AUTO_L | ITEM_INSTRUMENTATION | ITEM_SKIPPED;
    }

    /* Any of the Right Automatic Counting Macros? */
    if ( memwordcmp( name_start, AUTO_RIGHT_COUNTING_MACRO_STRING ) != NULL )
    { /* Yes */
        return ITEM_FUNC_COUNTERS_AUTO_R | ITEM_INSTRUMENTATION | ITEM_SKIPPED;
    }

    /* Any of the WMOPS Library Functions or Macros? */
    if ( memwordcmp( name_start, WMOPS_FUNCTS_STRING) != NULL )
    { /* Yes */
        return ITEM_FUNC_COUNT_LIB | ITEM_SKIPPED;
    }

    /* Any of the BASOP Functions or Macros? */
    if (memwordcmp(name_start, BASOP_FUNCTS_STRING) != NULL)
    { /* Yes */
        return ITEM_FUNC_BASOP | ITEM_INSTRUMENTATION_OFF;
    }

    return ITEM_FUNC_PROJECT;
}

/*-------------------------------------------------------------------*
 * Find_Calls
 *-------------------------------------------------------------------*/
static TOOL_ERROR Find_Calls(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ns, *ne; /* Name Start/End */
    char *ps, *pe; /* Params Start/End */
    Item_Type item_type;
    Parse_Tbl_def *ParseTbl_ptr;
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ps = ParseCtx_ptr->File.Data;

    /* Find Opening Parenthesis */
    while ( ( ps = Find_String( ps, "(", ParseTbl_ptr, ITEM_FUNC_BLOCK | ITEM_DATA_DECL ) ) != NULL )
    {
        /* Skip Blanks to the Left */
        ne = Skip_Chars( ps - 1, BLANK_CHARS, ParseTbl_ptr,
                         ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );

        /* Closing Bracket? */
        if ( *ne == ']' )
        { /* Yes */
            /* Go To Opening Bracket */
            if ( ( ne = Goto_Match( ne, ParseTbl_ptr ) ) == NULL )
            {
                ErrCode = Brackets_Error( ps, '[' );
                goto ret;
            }
            /* Skip Blanks to the Left */
            ne = Skip_Chars( ne - 1, BLANK_CHARS, ParseTbl_ptr,
                             ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
        }

        /* Is it an Identifier? */
        if ( IS_IDENTIFIER_CHAR( *ne ) )
        { /* Yes */
            /* Skip Identifier Chars to the Left (there is at least one) */
            ns = Skip_Identifier( ne, BACKWARDS );
            ns++;

            /* Any Keywords? */
            if ( Find_Region( ns, ParseTbl_ptr, ITEM_KEYWORD ) >= 0 )
            { /* Yes */
                /* Not a Function Call */
                item_type = ITEM_NONE;
            }
            /* Function Start Macro? */
            else if ( memwordcmp( ns, FUNC_START_STRING ) != NULL )
            { /* Yes */
                /* Not a Function Call */
                item_type = ITEM_NONE;
            }
            else
            { /* No */
                /* Function Name is Instrumented? */
                if ( Find_Identifier( ParseCtx_ptr->File.Data, memstr( ns, ne + 1 ), ParseTbl_ptr,
                                      ITEM_NAME | ITEM_FUNC_COUNTERS_AUTO, ITEM_ENCLOSED ) != NULL )
                { /* Yes */
                    item_type = ITEM_FUNC_PROJECT | ITEM_INSTRUMENTED;
                }
                else
                { /* No */
                    /* Get Call Type */
                    item_type = Get_Call_Type( ns );
                    /* Is it a System Allocation Function (All Other System Functions are Skipped) */
                    if ( ( item_type & ~ITEM_INSTRUMENTED ) == ITEM_FUNC_SYSTEM )
                    { /* Yes */
                        /* Add Function Call Name Region */
                        if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type | ITEM_CALL,
                                                     ns, ne + 1 ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                        /* Not a Function Call (Params Region not Needed) */
                        item_type = ITEM_NONE;
                    }
                    /* Is it in a Data Declaration */
                    if ( Find_Region( ns, ParseTbl_ptr, ITEM_DATA_DECL ) >= 0 )
                    { /* Yes */
                        /* Not a Function Call (Ignore Calls in Data Declaration) */
                        item_type = ITEM_NONE;
                    }

                    /* Project Function Call? */
                    if ( item_type == ITEM_FUNC_PROJECT )
                    {     /* Yes */
                        { /* No */
                            /* Function Name Ends with an Instrumentation Character */
                            if ( IS_INSTRUMENTATION_CHAR( *ne ) )
                            { /* Yes */
                                ErrCode = ERR_FUNCTION_NAME;
                                Error( "%s Name cannot End with an Instrumentation Character '%c'",
                                       ErrCode, Make_Position_String( ne, temp ), *ne );
                                goto ret;
                            }
                        }
                    }
                }

                /* Valid Function Call? */
                if ( item_type != ITEM_NONE )
                {
                    /* Add Function Call Name Region */
                    if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type | ITEM_CALL,
                                                 ns, ne + 1 ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                    /* Find Closing Parenthesis */
                    if ( ( pe = Goto_Match( ps, ParseTbl_ptr ) ) == NULL )
                    {
                        ErrCode = Parentheses_Error( ps, ')' );
                        goto ret;
                    }
                    /* Add Function Arguments Region (Arguments are never Marked as Instrumented) */
                    if ( ( ErrCode = Add_Region( ParseTbl_ptr, ( item_type | ITEM_CALL_ARGS ) & ~ITEM_INSTRUMENTED,
                                                 ps, pe + 1 ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                    /* Counting Macro, Couting Function or BASOP Function? */
                    if ( item_type & ( ITEM_FUNC_COUNTERS | ITEM_FUNC_COUNT_LIB | ITEM_FUNC_BASOP ) )
                    { /* Yes */
                        /*
                           Get Optional End of Statement
                           Blanks & Comments are Skipped to Find the End of Statement but
                           not Preprocessor Lines
                        */
                        /* Skip Blanks to the Right (this cannot fail) */
                        ps = Skip_Chars( pe + 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_COMMENT );
                        /* It is the End of Statement? */
                        if ( *ps == ';' )
                        { /* Yes */
                            /* Not Any of the Left Automatic Counting Macros
                               OR
                               End of Block? */
                            if ( item_type != ( ITEM_FUNC_COUNTERS_AUTO_L | ITEM_INSTRUMENTATION | ITEM_SKIPPED ) ||
                                 /* Go Forward (Skip Comments, Preproc Lines, Spaces & CR/LF) */
                                 *Skip_Chars( ps + 1, BLANK_CHARS, ParseTbl_ptr ) == '}' )
                            { /* Yes */
                                /* Add End of Statement Region */
                                if ( ( ErrCode = Add_Region( ParseTbl_ptr, item_type | ITEM_CALL_ARGS, ps ) ) != NO_ERR )
                                {
                                    goto ret;
                                }
                            }
                        }
                    }
                }
            }
        }
        /* Advance to Next Char */
        ps++;
    }

ret:
    return ErrCode;
}


/*-------------------------------------------------------------------*
 * Validate_Keywords
 *-------------------------------------------------------------------*/
static TOOL_ERROR Validate_Keywords(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    int idx, idx2;
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/

    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;

    char *ptr, *ptr2;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Search for 'do' Keyword */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_KEYWORD_DO, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Is it really a 'do'? */
        if ( ParseRec_ptr->item_type == ( ITEM_SKIPPED | ITEM_KEYWORD_DO ) )
        { /* Yes */
            /* In Skipped Region? ('do'/'while' are still Validated in Skipped Regions) */
            if ( Find_Region( ParseRec_ptr->item_start
                /*  need to check for Skipped Region outside of already Skipped 'do' Keyword */
                                  + ( ParseRec_ptr->item_end - ParseRec_ptr->item_start ) , ParseTbl_ptr, ITEM_SKIPPED ) < 0 )
            { /* No */
                /* Skip Blanks (this cannot fail) */
                ptr = Skip_Chars( ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr );
                /* Is it a Block Start? */
                if ( *ptr != '{' )
                { /* No */
                    ErrCode = ERR_DO_REQUIRES_BLOCK;
                    Error( "%s Unsupported do/while without Block",
                           ErrCode, Make_Position_String( ParseRec_ptr->item_start, temp ) );
                    goto ret;
                }
                /* Proceed to End of Block (this cannot fail, all blocks are balanced) */
                ptr = Goto_Match( ptr, ParseTbl_ptr ) + 1;

                /* Skip Blanks (this cannot fail) */
                ptr2 = Skip_Chars( ptr, BLANK_CHARS, ParseTbl_ptr );
                /* Is it a 'keyword'? */
                if ( ( idx2 = Find_Region( ptr2, ParseTbl_ptr, ITEM_KEYWORD ) ) >= 0 )
                { /* Yes */
                    /* Get Record */
                    ParseRec_ptr = &ParseTbl_ptr->Data[idx2];
                    /* Is it a 'while'? */
                    if ( ParseRec_ptr->item_type == ITEM_KEYWORD_WHILE )
                    { /* Yes */
                        /* Change Item Type */
                        ParseRec_ptr->item_type = ITEM_KEYWORD_WHILE2;
                    }
                }

                /* Is it a 'while' that is part of a 'do' Block? */
                if ( ParseRec_ptr->item_type != ITEM_KEYWORD_WHILE2 )
                {
                    ErrCode = ERR_MISSING_WHILE;
                    Error( "%s while Expected",
                           ErrCode, Make_Position_String( ptr, temp ) );
                    goto ret;
                }
            }
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Assignations
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_Assignations(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Check for Assignations in Data Declaration Region (those are in a "Not Searched" Region) */
    while ( ( ptr = Find_String( ptr, "=", ParseTbl_ptr, ITEM_DATA_DECL
             /* Added ITEM_INSTRUMENTATION_OFF to avoid Warning in Non-Instrumented Region*/
                                 , ITEM_NOT_SEARCHED | ITEM_INSTRUMENTATION_OFF ) ) != NULL )
    {
        /* Is it Equal Equal? */
        if ( NEXT_CHAR( ptr ) != *ptr )
        { /* No */
            /* Add Warning Region to Table */
            if ( ( ErrCode = Add_Region( ParseTbl_ptr,
                                         ITEM_WARNING | ITEM_SKIPPED | ITEM_DATA_DECL | ITEM_OPERATOR_EQ,
                                         ptr ) ) != NO_ERR )
            {
                goto ret;
            }
        }
        else
        { /* Yes */
            /* Advance to Next Char */
            ptr++;
        }
        /* Advance to Next Char */
        ptr++;
    }

    /* Add Code... */

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Switch
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_Switch_Sub(
    Parse_Tbl_def *ParseTbl_ptr,
    int idx,
    char **bs, /* Switch Block Start*/
    char **be  /* Switch Block End*/
)
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;

    Parse_Rec_def *ParseRec_ptr;

    /* Get Record */
    ParseRec_ptr = &ParseTbl_ptr->Data[idx];

    /* Advance to Parameters */
    ParseRec_ptr++;

    /* Start after Parameters */
    ptr = ParseRec_ptr->item_end;
    /* Skip Blanks to the Right */
    *bs = Skip_Chars( ptr, BLANK_CHARS, ParseTbl_ptr );

    /* Is it a Block Start? */
    if ( **bs != '{' )
    { /* No */
        ErrCode = Symbol_Missing_Error( ptr, '{' );
        goto ret;
    }

    /* Find End of Block (This Cannot Fail) */
    *be = Goto_Match( *bs, ParseTbl_ptr );

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Switch
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_Switch(
    Parse_Context_def *ParseCtx_ptr,
    int idx,
    int *prom_size_ptr,
    const PROM_Ops_Weights_defs *prom_ops_weights_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;
    char temp_str[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/

    char *ptr, *ptr2;
    char *start, *end;

    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;

    short n;

#define CASE_COUNTING_STRLEN STRING_LENGTH( AUTO_CASE_COUNTING_MACRO_STRING )

    /* +2 for '()', +10 for Digits, +1 for NUL*/
    char temp[CASE_COUNTING_STRLEN + 2 + 10 + 1] = AUTO_CASE_COUNTING_MACRO_STRING;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Validate & Get Switch Block Start/End */
    if ( ( ErrCode = Instrument_Switch_Sub( ParseTbl_ptr, idx, &ptr, &end ) ) != NO_ERR )
    {
        goto ret;
    }

    ptr2 = end; /* Pointer for Fall Through or default*/

    n = 0; /* First 'case' Hit has a Cost of 1*/
    /* Parse Through Keywords */
    while ( ( ptr = Skip_Chars( ptr + 1, BLANK_CHARS, ParseTbl_ptr,
                                ITEM_KEYWORD, ITEM_DATA_DECL,
                                FORWARD, &idx ) ) != NULL &&
            ptr < end )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        switch ( ParseRec_ptr->item_type & ~ITEM_SKIPPED )
        {
            /* Must Skip Nested 'switch'es */
            case ITEM_KEYWORD_SWITCH:
            {
                /* Validate & Get Switch Block Start/End */
                if ( ( ErrCode = Instrument_Switch_Sub( ParseTbl_ptr, idx, &start, &ptr ) ) != NO_ERR )
                {
                    goto ret;
                }
                break;
            }
            /* Instrument 'case' */
            case ITEM_KEYWORD_CASE:
            {
                /* Advance to Delimiter */
                ParseRec_ptr += 2;
                ptr = ParseRec_ptr->item_end - 1;
                /* Make Instrumentation String */
                sprintf( temp + CASE_COUNTING_STRLEN, "(%i)", ++n );
                /* Insert Cost Right After Delimiter */
                if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                                ptr + 1, temp ) ) != NO_ERR )
                {
                    goto ret;
                }
                break;
            }
            /* Instrument 'case' */
            case ITEM_KEYWORD_DEFAULT:
            {
                /* Already have a 'default:'? */
                if ( ptr2 != end )
                {
                    ErrCode = ERR_MORE_THAN_1_DEFAULT;
                    Error( "%s More than one default", ErrCode, Make_Position_String( ptr, temp_str ) );
                    goto ret;
                }
                /* Advance to Delimiter */
                ParseRec_ptr++;
                ptr = ParseRec_ptr->item_end - 1;
                /* Set Last Cost Instrumentation Location */
                ptr2 = ptr;
                /* 'default:' has no Program Memory Cost */
                break;
            }
        }
        /* Continue at 'ptr' Position */
    }

    /* 'default:' or End of 'switch' Block Alone? */
    if ( n > 0 )
    { /* No */
        /* Make Instrumentation String */
        sprintf( temp + CASE_COUNTING_STRLEN, "(%i)", n );
        /* Insert Last Cost Right After Switch or Default */
        if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, ptr2 + 1, temp ) ) != NO_ERR )
        {
            goto ret;
        }
        /* Count Program Memory */
        *prom_size_ptr += ( prom_ops_weights_ptr->add +
                            prom_ops_weights_ptr->branch ) *
                          n;
        cases++;
        /* 'default:' has no Program Memory Cost */
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Keywords
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_Keywords(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    int idx;
    Item_Type item_type;

    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;
    const PROM_Ops_Weights_defs *prom_ops_weights_ptr;

    char *ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Get PROM Ops Weights (for clarity) */
    prom_ops_weights_ptr = &PROM_Ops_Weights[ParseCtx_ptr->PROMOpsWeightsSet];

    /* Search for Keywords */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_KEYWORD, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        /* Get Item Type */
        item_type = ParseRec_ptr->item_type;

        /* Skipped? */
        unless( item_type & ITEM_SKIPPED )
        { /* No */
            /* In Skipped or Non-Instrumented Region? */
            if ( Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_SKIPPED | ITEM_INSTRUMENTATION_OFF ) < 0
                 /* 'return' always gets Instrumented (even in Manual Regions) */
                 || ( item_type == ITEM_KEYWORD_RETURN
                      /* 'return not' Instrumented in Skipped Regions */
                      && Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_SKIPPED ) < 0
                      && Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_FUNC_BLOCK ) < 0 && Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_FUNC_BLOCK | ITEM_FUNC_MATH ) < 0 && Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_FUNC_BLOCK | ITEM_FUNC_SYSTEM ) < 0 ) )
            { /* No */
                /* Instrument After (by default) */
                ptr = ParseRec_ptr->item_end;

                /* Is it a 'while' that is part of a 'do' Block? */
                /* Insert Instrumentation Character */
                if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, ptr, WORD_INSTRUMENT_STRING ) ) != NO_ERR )
                {
                    goto ret;
                }

                /* Is it a 'while'?
                   Auto Instrumentation Macro for 'while' in a 'do' Block
                   can handle multiple Condition. */
                if ( item_type == ITEM_KEYWORD_WHILE
                      /* 'while' from 'do/while' now Needs Extra () for Multiple Condition*/
                     || item_type == ITEM_KEYWORD_WHILE2
                )
                { /* Yes */
                    /* Advance to Parameters Record */
                    ParseRec_ptr++;
                    /* More than one Condition? */
                    if ( Count_Args( ParseRec_ptr->item_start,
                                     ParseRec_ptr->item_end - 1,
                                     ParseTbl_ptr ) > 1 )
                    { /* Yes */
                        /* Must Add Extra () */
                        if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                                        ParseRec_ptr->item_start,
                                                        "(" ) ) != NO_ERR )
                        {
                            goto ret;
                        }

                        if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                                        ParseRec_ptr->item_end,
                                                        ")" ) ) != NO_ERR )
                        {
                            goto ret;
                        }

                        /* Add Warning */
                        ParseRec_ptr->item_type |= ITEM_WARNING;
                    }
                }
                /* Is it a switch? */
                else if ( item_type == ITEM_KEYWORD_SWITCH )
                { /* Yes */
                    if ( ( ErrCode = Instrument_Switch( ParseCtx_ptr, idx,
                                                        &ParseCtx_ptr->PROMSize,
                                                        prom_ops_weights_ptr ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                }

                /* Count Program Memory */
                //if ( item_type == ITEM_KEYWORD_FOR )
                //    loops++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->loop;
                //else if ( item_type == ITEM_KEYWORD_WHILE )
                //    whiles++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->branch * 2;
                //else if ( item_type & ( ITEM_KEYWORD_CONTROL | ITEM_KEYWORD_IS_JUMP ) )
                //    jumps++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->branch;
            }

            /* Count Program Memory */   /* !!! VM: Moved to this place to count PROM size even for non-instrumented keywords */
            if ( Find_Region(ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_SKIPPED) < 0 )
            {
                if (item_type == ITEM_KEYWORD_FOR)
                    loops++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->loop;
                else if (item_type == ITEM_KEYWORD_WHILE)
                    whiles++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->branch * 2;
                else if (item_type & (ITEM_KEYWORD_CONTROL | ITEM_KEYWORD_IS_JUMP))
                    jumps++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->branch;
            }
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Operators_Sub2
 *-------------------------------------------------------------------*/
static char *Instrument_Operators_Sub2(
    Parse_Tbl_def *ParseTbl_ptr,
    char *ptr,
    char *ptr_dflt )
{
    char *ptr2;

    /* Search Instrumentation Insertion Location
    *
    * Stop for
    *      ')' Because it Can be: Keyword Control Expression
    *                             Instrumentation Macro
    *
    *      ':' Because it Can be: Case, Label End or Ternary
    *
    *      ';' Because it Can be: Previous statement Delimitter or
    *                             Data Decl Statement End
    *
    *      '{' Because it Can be: Start of Function or Block
    *      '}' Because it Can be: End of a Previous Block
    *
    *      'e' Because it Can be: 'e' of else
    */
    while ( ptr = Goto_Chars( ptr - 1, ");:{}e", ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS ),
            *ptr == ')' || *ptr == ':' || *ptr == 'e' )
    {
        if ( *ptr == ':' )
        {
            /* Is it the Ternary Operator? */
            if ( Find_Region( ptr, ParseTbl_ptr, ITEM_OPERATOR_TERNARY ) < 0 )
            { /* No */
                /* Done */
                break;
            }
        }
        else if ( *ptr == ')' )
        {
            /* Is it a Keyword Control Expression or Instrumentation? */
            if ( Find_Region( ptr, ParseTbl_ptr,
                              ITEM_KW_EXPR | ITEM_INSTRUMENTATION ) >= 0 )
            { /* Yes */
                /* Done */
                break;
            }
        }
        else /* 'e' of 'else' */
        {
            /* Is it an 'else'? */
            if ( Find_Region( ptr, ParseTbl_ptr, ITEM_KEYWORD_ELSE ) >= 0 )
            { /* Yes */
                /* Done */
                break;
            }
        }
    }

    /* Go Forward (Skip Comments, Preproc Lines, Instrumentation, Data Declaration, Spaces & CR/LF) */
    ptr = Skip_Chars( ptr + 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER | ITEM_INSTRUMENTATION | ITEM_DATA_DECL );

    /* Go Backwards Again (Do not Skip Instrumentation and CR/LF Now) */
    ptr2 = Skip_Chars( ptr - 1, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS ) + 1;

    /*
    * Is Insertion Location at Start of Line
    * OR
    * After Manual Instrumentation
    * OR
    * After Counting Library Function Call
    *
    * Note: ~97% of the Time, 1st Condition is False because
    *       1) It is the Start of Line
    *       2) Manual Instrumentation is Removed by Default
    *       3) Push/Pop WMops Happen Not that Often Relative to Code Lines
    */
    if ( !IS_EOL_CHAR( PREV_CHAR( ptr2 ) ) &&
         Find_Region( ptr2 - 1, ParseTbl_ptr, ITEM_INSTRUMENTATION ) < 0 &&
         Find_Region( ptr2 - 1, ParseTbl_ptr, ITEM_CALL | ITEM_CALL_ARGS | ITEM_FUNC_COUNT_LIB ) < 0
    )
    { /* No */
        /* Instrument in Front of Code */
        ptr2 = ptr;
        /* Get Default Pointer (for Better Instrumentation Location) */
        if ( ptr_dflt != NULL )
            ptr2 = ptr_dflt;
    }

    return ptr2;
}

/*-------------------------------------------------------------------*
 * Instrument_Operators_Sub3
 *-------------------------------------------------------------------*/
static int Instrument_Operators_Sub3(
    Item_Type item_type,
    const op_rules_def *op_rules_ptr,
    int op_rules_size,
    bool *value_mod_flag,
    char *instrumentation_string,
    int *prom_cost )
{
    Item_Type item_type2;
    unsigned int op_rules;
    const op_rules_def *op_rules_ptr2;
    char *ptr;
    int temp;

    /* Get Item Type (without the Compound Flag) */
    item_type2 = item_type & ~ITEM_OPERATOR_COMPOUND;

    /* Search for Operator Rules */
    while ( op_rules_size-- )
    {
        /* Found it? */
        if ( op_rules_ptr->op == item_type2 )
        { /* Yes */
            /* Save Cost */
            *prom_cost = op_rules_ptr->prom_cost;
            /* Save Operator Rules Pointer */
            op_rules_ptr2 = op_rules_ptr;
            /* Get Operator Rules */
            op_rules = RULES_NONE;
            do
            {
                op_rules |= ( temp = op_rules_ptr--->op_rules );
            } while ( temp & RULES_SAME );
            /* Get Instrumentation String */
            while ( ( ptr = (char *)(op_rules_ptr2--->instrumentation_string) ) == NULL )
                ;
            strcpy( instrumentation_string, ptr );
            /* Set Value Modifying Operator Flag */
            *value_mod_flag = ( item_type & ITEM_OPERATOR_COMPOUND ) != 0 ||
                              item_type == ITEM_OPERATOR_INC ||
                              item_type == ITEM_OPERATOR_DEC;
            /* Compounded Operator? */
            if ( item_type & ITEM_OPERATOR_COMPOUND )
            { /* Yes */
                /* Add '=' */
                instrumentation_string = strcat( instrumentation_string, "=" );
            }
            /* Never Instrumented? */
            if ( op_rules & RULE_NI )
            { /* Yes */
                /* Operator Not Instrumented */
                break;
            }
            /* Done */
            return op_rules;
        }
        op_rules_ptr++;
    }

    return -1;
}

/*-------------------------------------------------------------------*
 * Instrument_Operators_Sub
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_Operators_Sub(
    Parse_Tbl_def *ParseTbl_ptr,
    int idx,
    Insert_Tbl_def *OperInsTbl_ptr,
    int *prom_size_ptr,
    const PROM_Ops_Weights_defs *pw_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

/* NI = Not Instrumented */
#define RULE_NI_COMPARED_TO_ZERO          0x00000010
#define RULE_NI_1ST_IN_FOR_CTRL           0x00000020
#define RULE_NI_COMPOUNDED_1ST_IN_FOR_RPT 0x00000040
#define RULE_NI_1ST_IN_FOR_INIT           0x00000080
#define RULE_NI_OUTSIDE_OF_FOR            0x00000100
#define RULE_USE_MULT_W_CSTE              0x00000200
#define RULE_USE_AND_W_POWER_OF2          0x00000400
#define RULE_NI_IN_FCT_ARGS               0x00000800

    static const op_rules_def operators_rules[] = {
        /* ++, --, not instrumented outside of 'for'*/
        { ITEM_OPERATOR_PLUS, "+", pw_ptr->add, RULE_NI_COMPOUNDED_1ST_IN_FOR_RPT },
        { ITEM_OPERATOR_MINUS, "-", pw_ptr->add, RULES_SAME },
        { ITEM_OPERATOR_INC, "U", pw_ptr->add, RULES_SAME | RULE_NI_OUTSIDE_OF_FOR },
        { ITEM_OPERATOR_DEC, "D", pw_ptr->add, RULES_SAME },
        { ITEM_OPERATOR_MULT, "*", pw_ptr->mult, RULES_NONE },
        { ITEM_OPERATOR_DIV, "/", pw_ptr->div, RULE_USE_MULT_W_CSTE },
        { ITEM_OPERATOR_MOD, "%", pw_ptr->div, RULE_USE_AND_W_POWER_OF2 },
        { ITEM_OPERATOR_AND, "&", pw_ptr->logic, RULES_NONE },
        { ITEM_OPERATOR_OR, "|", pw_ptr->logic, RULES_NONE },
        { ITEM_OPERATOR_XOR, "^", pw_ptr->logic, RULES_NONE },
        { ITEM_OPERATOR_NOT, "~", pw_ptr->logic, RULES_NONE },
        /*        { ITEM_OPERATOR_SHL,           "«",  pw_ptr->shift,    RULES_NONE     },*/
        /*        { ITEM_OPERATOR_SHR,           "»",  pw_ptr->shift,    RULES_NONE     },*/
        { ITEM_OPERATOR_SHL, "<<", pw_ptr->shift, RULES_NONE },
        { ITEM_OPERATOR_SHR, ">>", pw_ptr->shift, RULES_NONE },
        { ITEM_OPERATOR_NOTL, "!", pw_ptr->misc, RULES_NONE },
        { ITEM_OPERATOR_EQ, "M", pw_ptr->move, RULE_NI_1ST_IN_FOR_INIT},

        { ITEM_OPERATOR_MEMBER, ".", pw_ptr->indirect, RULE_NI_IN_FCT_ARGS },
        { ITEM_OPERATOR_MEMBER_PTR, "->", pw_ptr->indirect, RULE_NI_IN_FCT_ARGS },
        { ITEM_OPERATOR_BRACKET_OPEN, "[", 0, RULES_NONE },
        { ITEM_OPERATOR_BRACKET_CLOSE, "]", 0, RULES_NONE },

        { ITEM_OPERATOR_EE, "Ee", pw_ptr->add, RULE_NI_COMPARED_TO_ZERO | RULE_NI_1ST_IN_FOR_CTRL },
        { ITEM_OPERATOR_NE, "Ne", pw_ptr->add, RULES_SAME },
        { ITEM_OPERATOR_LT, "Lt", pw_ptr->add, RULES_SAME },
        { ITEM_OPERATOR_LE, "Le", pw_ptr->add, RULES_SAME },
        { ITEM_OPERATOR_GT, "Gt", pw_ptr->add, RULES_SAME },
        { ITEM_OPERATOR_GE, "Ge", pw_ptr->add, RULES_SAME },
    };

    Item_Type item_type;
    int op_rules;
    char instrumentation_string[10];
    bool value_mod_flag = 0;
    int prom_cost = 0;

    Parse_Rec_def *ParseRec_ptr;
    Parse_Rec_def *ParseRec_ptr2;

    char *start; /* Operator Start*/
    char *ptr, *ptr2, *ptr3;
    int idx2; /* Keyword Expression */
#ifdef DEBUG_PRINT
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
#endif

    /* Get Record */
    ParseRec_ptr = &ParseTbl_ptr->Data[idx];

    /* Get Item Type */
    item_type = ParseRec_ptr->item_type;

    /* Get Item Rules */
    op_rules = Instrument_Operators_Sub3( item_type, operators_rules, nitems( operators_rules ),
                                          &value_mod_flag, instrumentation_string, &prom_cost );

    /* In an Indirection? */
    if ( Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_INDEX_EXPR ) >= 0 )
    { /* Yes */
        /* Operator Not Instrumented */
        goto ret;
    }

    /* Found it? */
    if ( op_rules >= 0 )
    { /* Yes */
        /* Get Item Start */
        start = ParseRec_ptr->item_start;

        ptr = start;

        /* Disable Default Pointer */
        ptr2 = NULL;

        /* Plus or Minus operator? */
        if ( item_type == ITEM_OPERATOR_PLUS || item_type == ITEM_OPERATOR_MINUS )
        { /* Yes */
            /* Move After Operator */
            ptr3 = Skip_Chars( ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr );
            /* Constant? */
            if ( Find_Region( ptr3, ParseTbl_ptr, ITEM_CONSTANT ) >= 0 )
            { /* Yes */
                /* Move Before Operator */
                ptr3 = Skip_Chars( ptr - 1, BLANK_CHARS, ParseTbl_ptr,
                                   ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS, &idx2 );
                /* Operator? */
                if ( ( idx2 = Find_Region( ptr3, ParseTbl_ptr, ITEM_OPERATOR ) ) >= 0 )
                { /* Yes */
                    /* Get Operator Record */
                    ParseRec_ptr2 = &ParseTbl_ptr->Data[idx2];
                    /* Instrument only if Preceded by Post Inc/Dec and Subscript */
                    if ( ParseRec_ptr2->item_type != ITEM_OPERATOR_INC &&
                         ParseRec_ptr2->item_type != ITEM_OPERATOR_DEC &&
                         ParseRec_ptr2->item_type != ITEM_OPERATOR_BRACKET_CLOSE )
                    {
                        /* Operator Not Instrumented */
                        goto ret;
                    }
                }
            }
        }

        /* Instrumented as a 'Bit And' if Modulo with a Power of 2 */
        if ( op_rules & RULE_USE_AND_W_POWER_OF2 )
        {
            /* Move After Operator */
            ptr3 = Skip_Chars( ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr );
            /* Power of 2 Constant? */
            if ( ( idx2 = Find_Region( ptr3, ParseTbl_ptr, ITEM_CONSTANT | ITEM_CSTE_POWEROF2 ) ) >= 0 )
            { /* Yes */
                /* Instrumentent as a 'Bit And' */
                /* Get Rules */
                op_rules = Instrument_Operators_Sub3( ITEM_OPERATOR_AND | ( item_type & ITEM_OPERATOR_COMPOUND ),
                                                      operators_rules, nitems( operators_rules ),
                                                      &value_mod_flag, instrumentation_string, &prom_cost );
                /* Found it? */
                if ( op_rules < 0 )
                { /* No */
                    goto ret;
                }
                /* Add Warning Region to Table */
                if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_WARNING | item_type,
                                             ParseRec_ptr->item_start,
                                             ParseRec_ptr->item_end ) ) != NO_ERR )
                {
                    goto ret;
                }
            }
        }

        /* Not Instrumented if Compared to Zero */
        if ( op_rules & RULE_NI_COMPARED_TO_ZERO )
        {
            /* Move After Operator */
            ptr3 = Skip_Chars( ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr );
            /* Constant is Zero? */
            if ( ( idx2 = Find_Region( ptr3, ParseTbl_ptr, ITEM_CONSTANT | ITEM_CSTE_ZERO ) ) >= 0 )
            { /* Yes */
                /* Compare with 0 Not Instrumented */
                goto ret;
            }
        }

        /* Instrumented as a Multiply if Division by a Constant */
        if ( op_rules & RULE_USE_MULT_W_CSTE )
        {
            /* Move After Operator */
            ptr3 = Skip_Chars( ParseRec_ptr->item_end, BLANK_CHARS, ParseTbl_ptr );
            /* Look for Constant */
            idx2 = Find_Region( ptr3, ParseTbl_ptr, ITEM_CONSTANT );
            /* Constant? */
            if ( idx2 < 0 )
            { /* No */
                /* Plus or Minus Operator? */
                if ( ( idx2 = Find_Region( ptr3, ParseTbl_ptr, ITEM_OPERATOR_ADD ) ) >= 0 )
                { /* Yes */
                    /* Get Operator Record */
                    ParseRec_ptr2 = &ParseTbl_ptr->Data[idx2];
                    /* Move After Operator */
                    ptr3 = Skip_Chars( ParseRec_ptr2->item_end, BLANK_CHARS, ParseTbl_ptr );
                    /* Look for Constant */
                    idx2 = Find_Region( ptr3, ParseTbl_ptr, ITEM_CONSTANT );
                }
            }
            /* Constant? */
            if ( idx2 >= 0 )
            { /* Yes */
                /* Instrumented as a Multiply */
                /* Get Multiply Rules */
                op_rules = Instrument_Operators_Sub3( ITEM_OPERATOR_MULT | ( item_type & ITEM_OPERATOR_COMPOUND ),
                                                      operators_rules, nitems( operators_rules ), &value_mod_flag,
                                                      instrumentation_string, &prom_cost );
                /* Found it? */
                if ( op_rules < 0 )
                { /* No */
                    goto ret;
                }
                /* Add Warning Region to Table */
                if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_WARNING | item_type,
                                             ParseRec_ptr->item_start,
                                             ParseRec_ptr->item_end ) ) != NO_ERR )
                {
                    goto ret;
                }
            }
        }

        /* in a Keyword Expression? */
        if ( ( idx2 = Find_Region( ptr, ParseTbl_ptr, ITEM_KW_EXPR ) ) >= 0 )
        { /* Yes */
            /* Get Keyword Record */
            ParseRec_ptr2 = &ParseTbl_ptr->Data[idx2 - 1];
            /* Move to Keyword */
            ptr = ParseRec_ptr2->item_start;
            /* Is it a for? */
            if ( ParseRec_ptr2->item_type == ITEM_KEYWORD_FOR )
            {   /* Yes */
                /*
                * Check if the Operator is
                *   1st in Init Section    or
                *   1st in Control Section or
                *   1st and Value Modifying in Repeat Section
                */
                if ( (op_rules & RULE_NI_1ST_IN_FOR_INIT) ||
                     (op_rules & RULE_NI_1ST_IN_FOR_CTRL) ||
                     (op_rules & RULE_NI_COMPOUNDED_1ST_IN_FOR_RPT && value_mod_flag) )
                { /* Yes */
                    /* Skip Spaces to the Left (start at Operator) */
                    start = Skip_Chars( start - 1, SPACE_CHARS, ParseTbl_ptr,
                                        ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                    /* Valid Identifier? */
                    if ( IS_IDENTIFIER_CHAR( *start ) )
                    { /* Yes */
                        /* Skip Identifier Chars to the Left (there is at least one) */
                        start = Skip_Identifier( start, BACKWARDS );
                    }
                    /* Skip Spaces to the Left */
                    start = Skip_Chars( start, SPACE_CHARS, ParseTbl_ptr,
                                        ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                    /* Operator 1st in Init Section Rule? */
                    if ( op_rules & RULE_NI_1ST_IN_FOR_INIT )
                    {   /* Yes */
                        /*
                        * The only form that is not Instrumented is (identifier operator
                        * ex: for (a=0; ...
                        *     for (cnt=1; ...
                        */
                        /* Start of Keyword Expression? */
                        if ( start == ParseTbl_ptr->Data[idx2].item_start )
                        { /* Yes */
                            /* 1st Operator of Init Section Not Instrumented */
                            goto ret;
                        }
                    }
                    /* Operator 1st in Control Section Rule? */
                    else if ( op_rules & RULE_NI_1ST_IN_FOR_CTRL )
                    {
                        /*
                        * The only form that is not Instrumented is ;identifier operator
                        * The 1st operator after the first ';'
                        * ex: for (a=0; a <= 10; ...)
                        *     for (cnt=10; cnt != max_cnt; ...)
                        */
                        /* Expression Separator? */
                        if ( *start == ';' )
                        { /* Yes */
                            ptr3 = start;
                            /* Go to Start of Keyword Expression or Separator */
                            while ( *( ptr3 = Goto_Chars( ptr3 - 1, ";(", ParseTbl_ptr, ITEM_ANY,
                                                          ITEM_MOVED_OVER, BACKWARDS ) ) == '(' &&
                                    ptr3 != ParseTbl_ptr->Data[idx2].item_start )
                                ;
                            /* Expression Separator? */
                            if ( *ptr3 != ';' )
                            { /* No */
                                /* 1st Operator of Ending Condition Section Not Instrumented */
                                goto ret;
                            }
                        }
                    }
                    /* Operator 1st & Compounded in Repeat Section */
                    else
                    {
                        /* Expression Separator? */
                        if ( *start == ';' )
                        { /* Yes */
                            ptr3 = start;
                            /* Go to End of Keyword Expression or Separator */
                            while ( *( ptr3 = Goto_Chars( ptr3 + 1, ";)", ParseTbl_ptr ) ) == ')' &&
                                    ptr3 != ParseTbl_ptr->Data[idx2].item_end - 1 )
                                ;
                            /* Expression Separator? */
                            if ( *ptr3 != ';' )
                            { /* No */
                                /* 1st Operator of Repeat Section Not Instrumented */
                                goto ret;
                            }
                        }
                    }
                }
                /* Go to Start of Keyword Expression or Separator */
                while ( *( start = Goto_Chars( start, ";(", ParseTbl_ptr, ITEM_ANY,
                                               ITEM_MOVED_OVER, BACKWARDS ) ) == '(' &&
                        start != ParseTbl_ptr->Data[idx2].item_start )
                    start--;
            }
            /* Not a 'for' */
            else
            { /* No */
                /* Not Instrumented if Outside of 'for'? */
                if ( op_rules & RULE_NI_OUTSIDE_OF_FOR )
                { /* Yes */
                    /* Operator Not Instrumented */
                    goto ret;
                }
            }
            /* Is it a 'while' or in 'for' Ending Condition or Repeated Section? */
            if ( ParseRec_ptr2->item_type == ITEM_KEYWORD_WHILE || *start == ';' )
            { /* Yes */
                /* Instrument Repetition */
                /* Move After Keyword Expression */
                ptr3 = Skip_Chars( ParseRec_ptr2[1].item_end, BLANK_CHARS, ParseTbl_ptr );
                /* Get Instrumentation Insertion Location */
                ptr2 = Instrument_Operators_Sub2( ParseTbl_ptr, ptr3, ptr );
                /* Is it the Best Location? */
                if ( ptr2 == ptr )
                { /* No */
                    /* Set Default Pointer */
                    ptr2 = ptr3;
                    /* Is it a Start of Block? */
                    if ( *ptr3 == '{' )
                    { /* Yes */
                        /* Move Right Again (Past Start of Block) */
                        ptr2 = Skip_Chars( ptr3 + 1, BLANK_CHARS, ParseTbl_ptr );
                        /* Get Instrumentation Insertion Location */
                        ptr2 = Instrument_Operators_Sub2( ParseTbl_ptr, ptr2, ptr3 );
                    }
                }

                /* Add Operator for this Line */
                if ( ( ErrCode = Add_Insertion( OperInsTbl_ptr,
                                                ptr2,
                                                instrumentation_string ) ) != NO_ERR )
                {
                    goto ret;
                }
                /* Disable Default Pointer */
                ptr2 = NULL;
                /* Count Program Memory */
                *prom_size_ptr += prom_cost;
                /* Assignation? */
                if ( value_mod_flag )
                { /* Yes */
                    *prom_size_ptr += pw_ptr->move;
                }
                count_ops( item_type );
                /* in 'for' Ending Condition or Repeated Section? */
                if ( *start == ';' )
                { /* Yes */
                    /* Go to End of Keyword Expression or Separator (This cannot Fail) */
                    while ( *( start = Goto_Chars( start + 1, ";)", ParseTbl_ptr ) ) == ')' &&
                            start != ParseTbl_ptr->Data[idx2].item_end - 1 )
                        ;
                    /* Expression Separator? */
                    if ( *start != ';' )
                    { /* No */
                        /* Operator in Repeat Section is Not Instrumented in Front of Keyword (On Initialization of 'for') */
                        ptr = NULL;
                    }
                }
            }
            /* Is it a 'while' that is part of a 'do' Block? */
            if ( ParseRec_ptr2->item_type == ITEM_KEYWORD_WHILE2 )
            { /* Yes */
                /* Save Keyword Start */
                ptr2 = ptr;
                /* Go Backwards to End of Block (Skip Comments, Preproc Lines & Blanks) */
                ptr = Skip_Chars( ptr - 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                /* 20101025: Must Instrument Inside Block because using Standard Macro that has no {}*/
                /* Set Default Instrumentation Location */
                ptr2 = ptr;
                /* Instrument inside Block */
            }
            /* Otherwise we're ok.*/
        }
        else
        { /* No */
            /* Not Instrumented if Outside of 'for'? */
            if ( op_rules & RULE_NI_OUTSIDE_OF_FOR )
            { /* Yes */
                /* Operator Not Instrumented */
                goto ret;
            }
            /* in Function Call Arguments? */
            if ( ( idx2 = Find_Region( ptr, ParseTbl_ptr, ITEM_CALL_ARGS ) ) >= 0 )
            { /* Yes */
                /* Not Instrumented if in Function Argument List? */
                if ( op_rules & RULE_NI_IN_FCT_ARGS )
                { /* Yes */
                    /* Operator Not Instrumented */
                    goto ret;
                }
                /* Get Function Name Record */
                ParseRec_ptr2 = &ParseTbl_ptr->Data[idx2 - 1];
                /* Move to Function Name */
                ptr = ParseRec_ptr2->item_start;
            }
        }

        /* Default Instrumentation Disabled for this Operator */
        if ( ptr != NULL )
        { /* No */
            /* Get Instrumentation Insertion Location */
            ptr = Instrument_Operators_Sub2( ParseTbl_ptr, ptr, ptr2 );

            /* Add Operator for this Line */
            if ( ( ErrCode = Add_Insertion( OperInsTbl_ptr, ptr, instrumentation_string ) ) != NO_ERR )
            {
                goto ret;
            }
            /* Count Program Memory */
            *prom_size_ptr += prom_cost;
            /* Assignation? */
            if ( value_mod_flag )
            { /* Yes */
                *prom_size_ptr += pw_ptr->move;
            }
            count_ops( item_type );
        }
#ifdef DEBUG_PRINT
        if ( item_type == ITEM_OPERATOR_DIV )
        {
            char *pt1, *pt2;
            int sz;
            pt1 = ParseRec_ptr->item_start;
            sz = 20;
            while ( !IS_EOL_CHAR( pt1[-1] ) && --sz > 0 )
                pt1--;
            pt2 = ParseRec_ptr->item_start;
            sz = 20;
            while ( !IS_EOL_CHAR( pt2[+1] ) && --sz > 0 )
                pt2++;
            Debug_Print( "%s '%s'\n", Make_Position_String( ParseRec_ptr->item_start, temp ), memstr( pt1, pt2 ) );
        }
#endif
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Operators
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_Operators(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

#define MACRO_START AUTO_DEFAULT_COUNTING_MACRO_STRING "(" DQUOTE_STRING
#define MACRO_END   DQUOTE_STRING ")"
#define MACRO_TOTAL MACRO_START MACRO_END
    int idx, idx2;
    Item_Type item_type;
    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;
    const PROM_Ops_Weights_defs *prom_ops_weights_ptr;
    Insert_Tbl_def OperInsTbl;
    Insert_Rec_def *OperInsRec_ptr;

    static const char * const operator_set1_strings[] = {
        LOGICAL_INSTRUMENT_STRING,
        LOGICAL_INSTRUMENT_STRING " ",
        TERNARY_INSTRUMENT_STRING,
        TERNARY_INSTRUMENT_STRING " ",
    };
    static const char * const operator_set2_strings[] = {
        MACRO_END, MACRO_END "{};"
    };
    char **instrumentation_string;

    int nChars, max_nChars;
    char *ptr, *ptr2;
    char *ptr3, *ptr4;
    char *end;

    /* Erase Operator Insert Table */
    OperInsTbl.Size = 0;

    /* Initialize (No Memory Allocated by Default) */
    OperInsTbl.MaxSize = 0;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Get PROM Ops Weights (for clarity) */
    prom_ops_weights_ptr = &PROM_Ops_Weights[ParseCtx_ptr->PROMOpsWeightsSet];

    if ( ( ErrCode = Instrument_Assignations( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Search for Operators */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_OPERATOR, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Get Item Type */
        item_type = ParseRec_ptr->item_type;
        /* Skipped? */
        unless( item_type & ITEM_SKIPPED )
        { /* No */
            /* In Skipped or Non-Instrumented Region? */
            if ( Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr,
                              ITEM_SKIPPED | ITEM_INSTRUMENTATION_OFF ) < 0 )
            { /* No */
                /* Logical or Ternary Operator? */
                if ( item_type == ITEM_OPERATOR_ANDL ||
                     item_type == ITEM_OPERATOR_ORL ||
                     item_type == ITEM_OPERATOR_TERNARY )
                { /* Yes */
                    /* Set Instrumentation String */
                    instrumentation_string = (char **)operator_set1_strings;
                    if ( item_type == ITEM_OPERATOR_TERNARY )
                        instrumentation_string += 2;
                    /* Valid Identifier Character after Match? */
                    if ( IS_IDENTIFIER_CHAR( *ParseRec_ptr->item_end ) )
                    { /* Yes */
                        /* Add Space */
                        instrumentation_string++;
                    }
                    /* Insert Instrumentation String */
                    if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                                    ParseRec_ptr->item_end,
                                                    *instrumentation_string ) ) != NO_ERR )
                    {
                        goto ret;
                    }

                    /* Logical Operator? */
                    if ( item_type != ITEM_OPERATOR_TERNARY )
                    { /* Yes */
                        /* In a for Control Expression? */
                        if ( ( idx2 = Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_KW_EXPR_FOR ) ) >= 0 )
                        { /* Yes */
                            /* Get Record */
                            ParseRec_ptr = &ParseTbl_ptr->Data[idx2];
                            /* Add Warning */
                            ParseRec_ptr->item_type |= ITEM_WARNING;
                        }
                    }
                    /* Count Program Memory */
                    ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->branch;
                    logicals++;
                }
                /* Other Operators */
                else
                { /* Yes */
                    /* Do Margin Instrumentation */
                    if ( ( ErrCode = Instrument_Operators_Sub( ParseTbl_ptr, idx, &OperInsTbl,
                                                               &ParseCtx_ptr->PROMSize,
                                                               prom_ops_weights_ptr ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                }
            }
        }
    }

    /* Sort Operator Insert Table */
    Sort_Insert_Table( &OperInsTbl );

    /* Add Last Insertion */
    if ( ( ErrCode = Add_Insertion( &OperInsTbl,
                                    ParseCtx_ptr->File.Data + ParseCtx_ptr->File.Size,
                                    "" ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Initialize Maximum # of Chars to Insert */
    max_nChars = 0;
    /* Reset # of Instrumentation Chars */
    nChars = 0;
    /* Start at First Entry */
    OperInsRec_ptr = &OperInsTbl.Data[0];
    /* Get First Insertion Location */
    ptr = OperInsRec_ptr->Ptr;
    /* Check Maximum Spaces to Insert */
    for ( idx = 0; idx < OperInsTbl.Size - 1; idx++ )
    {
        /* New Instrumentation Location? */
        if ( nChars == 0 )
        { /* Yes */
            /* Insert Instrumentation Macro Start */
            if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                            ptr, MACRO_START ) ) != NO_ERR )
            {
                goto ret;
            }
            /* Set # of Chars Inserted */
            nChars = STRING_LENGTH( MACRO_START );
        }
        /* Insert Operator Instrumentation String */
        if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                        ptr, OperInsRec_ptr->String ) ) != NO_ERR )
        {
            goto ret;
        }
        /* Add Length of Operator Instrumentation String */
        nChars += (int) strlen( OperInsRec_ptr->String );
        /* Advance to Next Record */
        OperInsRec_ptr++;
        /* Last Insertion at the Same Location? */
        if ( ptr != OperInsRec_ptr->Ptr )
        { /* Yes */
            /* Go Forward (Skip Comments, Preproc Lines, Spaces & CR/LF) */
            ptr2 = Skip_Chars( ptr, BLANK_CHARS, ParseTbl_ptr );
            /* Set Instrumentation String */
            instrumentation_string = (char **)operator_set2_strings;
            /* Is it an End of Block? */
            if ( *ptr2 == '}' )
            { /* Yes */
                /* Need to Add a ';'? */
                instrumentation_string++;
                /* Update # of Chars Inserted */
                nChars++;
            }
            /* Insert Instrumentation Macro End (with optional End of Statement) */
            if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                            ptr, *instrumentation_string ) ) != NO_ERR )
            {
                goto ret;
            }
            /* Update # of Chars Inserted */
            nChars += STRING_LENGTH( MACRO_END );
            /*
            * Is Insertion Location at Start of Line
            * OR
            * After Manual Instrumentation
            * OR
            * After Counting Library Function Call
            *
            * Otherwise, it will be inserted so Space is not Counted
            *
            * Note: ~96% of the Time, 1st Condition is True because
            *       1) It is the Start of Line
            *       2) Manual Instrumentation is Removed by Default
            *       3) Push/Pop WMops Happen Not that Often Relative to Code Lines
           */
            if ( IS_EOL_CHAR( PREV_CHAR( ptr ) ) ||
                 Find_Region( ptr - 1, ParseTbl_ptr, ITEM_INSTRUMENTATION ) >= 0 ||
                 Find_Region( ptr - 1, ParseTbl_ptr, ITEM_CALL | ITEM_CALL_ARGS | ITEM_FUNC_COUNT_LIB ) >= 0 )
            { /* Yes */
                /* Save # of Chars Inserted */
                memmove( OperInsRec_ptr[-1].String, &nChars, sizeof( nChars ) );
                /* Move to First Non Space */
                for ( ptr2 = ptr; IS_SPACE_CHAR( *ptr2 ); ptr2++ )
                {
                    /* Deleted Character? */
                    if ( *ptr2 == DEL_CHAR )
                    { /* Yes */
                        /* Increase Required Space (Deleted Chars are not Available Space) */
                        nChars++;
                    }
                }
                /* Calculate Spare Space */
                nChars -= (int) ( ptr2 - ptr );
                /* Enough Space? */
                if ( nChars >= 0 )
                { /* No */
                    /* At Least one Space (Only if Right Next to Code) */
                    if ( Find_Region( ptr2, ParseTbl_ptr, ITEM_COMMENT ) < 0 )
                        nChars++;
                    /* Keep Maximum */
                    if ( max_nChars < nChars )
                        max_nChars = nChars;
                }
            }
            else
            {
                /* Reset # of Chars Inserted */
                memset( OperInsRec_ptr[-1].String, 0, sizeof( nChars ) );
            }

            /* Get Next Insertion Location */
            ptr = OperInsRec_ptr->Ptr;
            /* Reset # of Instrumentation Chars */
            nChars = 0;
        }
    }
    /* Adjust # of Chars to Add to Fix Alignment */
    max_nChars += ( TAB_SIZE * 10000L - ParseCtx_ptr->nChars - max_nChars ) % TAB_SIZE;
    /* Clear # of Chars to Remove to Restore Alignment */
    ParseCtx_ptr->nChars = 0;

    /* Start at First Entry */
    OperInsRec_ptr = &OperInsTbl.Data[0];
    /* Get First Insertion Location */
    ptr = OperInsRec_ptr->Ptr;
    /* Now Add Spaces */
    for ( idx = 0; idx < OperInsTbl.Size - 1; idx++ )
    {
        /* Advance to Next Record */
        OperInsRec_ptr++;
        /* More Insertions at the Same Location? */
        if ( ptr != OperInsRec_ptr->Ptr )
        { /* No */
            /* Get # of Chars Inserted */
            memmove( &nChars, OperInsRec_ptr[-1].String, sizeof( nChars ) );
            /* Insertion Location at Start of Line? */
            if ( nChars != 0 )
            { /* Yes */
                /* Adjust Spaces */
                nChars -= max_nChars;
                /* Remove Spaces? */
                if ( nChars > 0 )
                { /* Yes */
                    Delete( ptr, ptr + nChars );
                }
                /* Add Spaces? */
                if ( nChars < 0 )
                { /* Yes */
                    if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                                    ptr, " ", -nChars ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                }
            }

            /* Get Next Insertion Location */
            ptr = OperInsRec_ptr->Ptr;
        }
    }

    /* Start at First Entry */
    OperInsRec_ptr = &OperInsTbl.Data[0];
    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;
    /* Set End */
    end = ParseCtx_ptr->File.Data + ParseCtx_ptr->File.Size;
    ParseCtx_ptr->nChars = max_nChars;

    /* Now Add Spaces For All Uninstrumented Lines */
    while ( ptr < end )
    {
        nChars = 0;
        /* Find EOL */
        ptr2 = strpbrk( ptr, EOL_CHARS );
        /* Instrumentation on Line? */
        if ( OperInsRec_ptr->Ptr < ptr2 )
        { /* Yes */
            /* Skip 1st Instrumentation */
            do
            {
                OperInsRec_ptr++;
            } while ( OperInsRec_ptr[-1].Ptr == OperInsRec_ptr->Ptr );

            /* Get # of Chars Inserted */
            memmove( &nChars, OperInsRec_ptr[-1].String, sizeof( nChars ) );

            /* Skip Other Instrumentation */
            while ( OperInsRec_ptr->Ptr < ptr2 )
                OperInsRec_ptr++;
        }

        /* Must Add Spaces? */
        if ( nChars == 0 )
        { /* No */
            ptr4 = NULL;
            do
            {
                /* Set Start Point */
                ptr3 = ptr4;
                /* Skip Intrumentation */
                ptr4 = Skip_Chars( ptr, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_INSTRUMENTATION );
                /* Has Pointer Moved? (or 1st Pass with ptr3 set to NULL) */
                if ( ptr4 > ptr3 )
                {
                    /* Set Start Point */
                    ptr3 = ptr4;
                }
                /* Set Continuation Point */
                ptr = ptr4;
            } while ( ptr4 > ptr3 );

            /* Something else than Spaces */
            if ( strpbrkn( memstr( ptr, ptr2 ), SPACE_CHARS ) != NULL )
            { /* Yes */
                /* Add Spaces */
                if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                                ptr, " ", max_nChars ) ) != NO_ERR )
                {
                    goto ret;
                }
            }
        }

        /* Go to Start of Next Line */
        ptr = strlend( ptr2 ) + 1;
    }

ret:
    Free_Insert_Table_Memory( &OperInsTbl );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Calls
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_Calls(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    int idx;
    char *name, *end;
    int name_length;
    Item_Type item_type;
    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;
    const PROM_Ops_Weights_defs *prom_ops_weights_ptr;
    int NameOcc;
    int nArgs;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;
    /* Get PROM Ops Weights (for clarity) */
    prom_ops_weights_ptr = &PROM_Ops_Weights[ParseCtx_ptr->PROMOpsWeightsSet];

    /* Search for Function Calls */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_CALL, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Get Item Type */
        item_type = ParseRec_ptr->item_type;
        /* Skipped? */
        unless( item_type & ITEM_SKIPPED )
        { /* No */
            /* Not In Skipped or Non-Instrumented Region or a System Function? */
            if ( item_type == ( ITEM_CALL | ITEM_FUNC_SYSTEM ) || Find_Region( ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_SKIPPED | ITEM_INSTRUMENTATION_OFF ) < 0 )
            { /* Yes */
                /* Get Name Information */
                name = ParseRec_ptr->item_start;
                end = ParseRec_ptr->item_end;

                /* Project Function? */
                if ( item_type & ITEM_FUNC_PROJECT )
                { /* Yes */
                    /* Local Macro? */
                    if ( !Is_Constant( name ) &&
                         Find_Identifier( ParseCtx_ptr->File.Data, memstr( name, end ), ParseTbl_ptr, ITEM_MACRO_NAME ) == NULL )
                    { /* No */
                        /* Get Function Name Length */
                        name_length = (int) ( end - name ); /* name will never exceed this limit*/

                        /* Advance to Parameters */
                        idx++;
                        /* Get Record */
                        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

                        /* Get # of Arguments */
                        nArgs = Count_Args( ParseRec_ptr->item_start,
                                            ParseRec_ptr->item_end - 1,
                                            ParseTbl_ptr );

                        /* Add Function Call to Table */
                        if ( ( ErrCode = Add_Function_Call( &ParseCtx_ptr->FctCallTbl,
                                                            name, name_length, nArgs,
                                                            &NameOcc ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                        /* Insert Instrumentation */
                        if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, end,
                                                        Make_Prj_Fct_Call_Instrumentation_String( NameOcc ) ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                        /* Count Program Memory */
                        ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->func +
                                                  prom_ops_weights_ptr->args * nArgs;
                        funcs++;
                        args += nArgs;
                    }
                    else /* Local Macro */
                    {    /* Yes */
                        /* Local Macros are not Instrumented, External Macros Will not compile */
                        /* Add Warning Region to Table */
                        if ( ( ErrCode = Add_Region( ParseTbl_ptr,
                                                     ITEM_WARNING | ITEM_SKIPPED | ITEM_CALL,
                                                     name, end ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                    }
                }

                /* Math Function? */
                if ( item_type & ITEM_FUNC_MATH )
                { /* Yes */
                    /* Insert Instrumentation */
                    if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, end,
                                                    WORD_INSTRUMENT_STRING ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                    /* Absolute? */
                    if ( stristr( memstr( name, end ), "abs" ) )
                        /* Count Program Memory */
                        abses++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->abs;
                    else if ( stristr( memstr( name, end ), "sqrt" ) )
                        /* Count Program Memory */
                        sqrts++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->sqrt;
                    else if ( stristr( memstr( name, end ), "mod" ) )
                        /* Count Program Memory */
                        divs++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->div;
                    else if ( stristr( memstr( name, end ), "mac" ) )
                        /* Count Program Memory */
                        macs++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->mac;
                    else if ( memwordicmp( name, "sqr square" ) )
                        /* Count Program Memory */
                        mults++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->mult;
                    else if ( memwordicmp( name, "min max" ) )
                        /* Count Program Memory */
                        minmaxs++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->branch;
                    else if ( strwordistr( memstr( name, end ), "cos sin tan log exp pow" ) )
                        /* Count Program Memory */
                        transes++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->trans;
                    else
                        /* Count Program Memory */
                        miscs++, ParseCtx_ptr->PROMSize += prom_ops_weights_ptr->misc;
                }
                /* System Allocation Function */
                /* !! VM: Added && Find_Region(.) to avoid instrumentation of malloc() calls within WMC_TOOL_SKIP segments */
                if ( (item_type & ITEM_FUNC_SYSTEM) && Find_Region(ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_SKIPPED | ITEM_INSTRUMENTATION_OFF) < 0 )
                { /* Yes */
                    /* Insert Instrumentation */
                    if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, end, WORD_INSTRUMENT_STRING ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                }
            }
        }
    }

ret:
    return ErrCode;
}


/*-------------------------------------------------------------------*
 * Delete_Identifier
 *-------------------------------------------------------------------*/
static void Delete_Identifier(
    char *ptr,        /* Data */
    const char *identifier, /* Identifier to Search */
    Parse_Tbl_def *ParseTbl_ptr )
{
    char *ptr2;

    /* Get Identifier Length */
    size_t len = strlen( identifier );

    /* Find Identifier */
    while ( ( ptr2 = Find_Identifier( ptr, identifier, ParseTbl_ptr ) ) != NULL )
    {
        /* Advance */
        ptr = ptr2 + len;
        /* Delete it */
        Delete( ptr2, ptr );
    }
}

/*-------------------------------------------------------------------*
 * Insert_Line
 *-------------------------------------------------------------------*/
static TOOL_ERROR Insert_Line( /* Insertion Done after Current Line */
                               Insert_Tbl_def *InsertTbl_ptr,
                               char *ptr, /* Pointer to 1st EOL Character in Line */
                               const char *string )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr2;

    /* EOL Character? */
    if ( !IS_EOL_CHAR( *ptr ) )
    { /* No */
        /* Problem! */
        ErrCode = Internal_Error( __FUNCTION__ );
        goto ret;
    }

    /* Go to Start of Next Line */
    ptr2 = strlend( ptr ) + 1;

    /* Insert String */
    if ( ( ErrCode = Add_Insertion( InsertTbl_ptr, ptr2, string ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Insert Newline */
    ErrCode = Add_Insertion( InsertTbl_ptr, ptr2, memstr( ptr, ptr2 ) );

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Remove_Superfluous_EOS
 *-------------------------------------------------------------------*/
static TOOL_ERROR Remove_Superfluous_EOS(
    Parse_Context_def *ParseCtx_ptr,
    bool Warn = false )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr, *ptr2;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Find End of Statement */
    while ( ( ptr = Find_String( ptr, ";", ParseTbl_ptr ) ) != NULL )
    {
        /* Skip Blanks to the Left (this cannot fail) */
        ptr2 = Skip_Chars( ptr - 1, BLANK_CHARS, ParseTbl_ptr,
                           ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
        /* Is it a Start or End of Block? */
        if ( *ptr2 == '{' || *ptr2 == '}' )
        { /* Yes */
            bool remove = true;
            /* Is it an End of Block? */
            if ( *ptr2 == '}' )
            { /* Yes */
                /* Go To Matching Start of Block */
                ptr2 = Goto_Match( ptr2, ParseTbl_ptr );
                /* Skip Blanks to the Left from Initial Opening Brace */
                ptr2 = Skip_Chars( ptr2 - 1, BLANK_CHARS, ParseTbl_ptr,
                                   ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                /* Assignation? */
                if ( *ptr2 == '=' )
                { /* Yes */
                    /* Then we have an Initialized array in an undetected Data Declaration Section*/
                    remove = false;
                }
            }
            if ( remove )
            {
                /* Warning for it? */
                if ( Warn )
                { /* Yes */
                    /* Add Warning */
                    if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_WARNING | ';', ptr ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                }
                /* Remove EOS */
                *ptr = SPACE_CHAR;
            }
        }

        do
        {
            /* Skip Blanks to the Right (this cannot fail) */
            ptr = Skip_Chars( ptr + 1, BLANK_CHARS, ParseTbl_ptr );

            /* Is it an End of Statement? */
            if ( *ptr == ';' )
            {
                /* It is in a Keyword Expression? */
                if ( Find_Region( ptr, ParseTbl_ptr, ITEM_KW_EXPR ) < 0 )
                { /* No */
                    /* Warning for it? */
                    if ( Warn )
                    { /* Yes */
                        /* Add Warning */
                        if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_WARNING | ';', ptr ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                    }
                    /* Remove EOS */
                    *ptr = SPACE_CHAR;
                }
            }
            /* Continue at Replace Location */
        } while ( *ptr == SPACE_CHAR );

        /* Advance to Next Char */
        ptr++;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Remove_Superfluous_Spaces
 *-------------------------------------------------------------------*/
static TOOL_ERROR Remove_Superfluous_Spaces(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr, *ptr2;
    char *last_ptr;
    bool flag;
    char chr;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Find End of Lines */
    while ( ( ptr2 = strpbrk( ptr, EOL_CHARS ) ) != NULL )
    {
        /* Keep Last Start of Line */
        last_ptr = ptr;
        /* Reset Flag */
        flag = false;
        /* Scan whole Line */
        for ( ptr = ptr2; ptr > last_ptr; ptr-- )
        {
            /* Get Previous Character */
            chr = PREV_CHAR( ptr );

            /* Non Space? */
            if ( !IS_SPACE_CHAR( chr ) )
            { /* Yes */
                /* Stop */
                break;
            }

            /* Something About Instrumentation was Deleted on this Line? */
            if ( chr == BLANK_CHAR || chr == DEL_CHAR )
            { /* Yes */
                /* Set Flag */
                flag = true;
            }
        }

        /* Remove Spaces */
        Delete( ptr, ptr2 );

        /* Completely Blank Line? */
        if ( ptr != last_ptr )
        { /* No */
            /* Reset Flag */
            flag = false;
        }

        /* Convert Blanks to Spaces */
        while ( ptr > last_ptr )
        {
            ptr--;
            /* It is a Blank Character */
            if ( *ptr == BLANK_CHAR )
            { /* Yes */
                /* Convert to Space */
                *ptr = SPACE_CHAR;
            }
        }

        /* Go to End of Line */
        ptr = strlend( ptr2 );
        /* Advance */
        ptr++;

        /* Something About Instrumentation was Deleted on this Line
           AND
           Completely Blank Line? */
        if ( flag )
        { /* Yes */
            /* Remove Newline too */
            Delete( ptr2, ptr );
        }
    }

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Remove_Extra_Parentheses
 *-------------------------------------------------------------------*/
static TOOL_ERROR Remove_Extra_Parentheses(
    Parse_Context_def *ParseCtx_ptr,
    bool Warn = false )
{
    TOOL_ERROR ErrCode = NO_ERR;

    int idx;
    char *ptr, *start, *tmp;
    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;
    /* for All Keyword Expressions (only while for now) */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_KW_EXPR_WHILE, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Get Parameters Start */
        start = ParseRec_ptr->item_start;
        /* Skip Blanks to the Right */
        start = Skip_Chars( start + 1, BLANK_CHARS, ParseTbl_ptr );
        /* Opening Parenthesis? */
        if ( *start == '(' )
        { /* Yes */
            /* Find Closing Parenthesis (this cannot fail) */
            ptr = Goto_Match( start, ParseTbl_ptr );
            /* Skip Blanks to the Right */
            tmp = Skip_Chars( ptr + 1, BLANK_CHARS, ParseTbl_ptr );
            /* Extra ()? */
            if ( tmp == ParseRec_ptr->item_end - 1 )
            {
                /* Warning for it? */
                if ( Warn )
                { /* Yes */
                    /* Add Warning */
                    if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_WARNING | '(', start ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                }
                /* Delete Them (the inner) */
                Delete( start, start + 1 );
                Delete( ptr, ptr + 1 );
            }
        }
    }
ret:
    return ErrCode;
}


/*-------------------------------------------------------------------*
 * GLOBAL FUNCTIONS
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 * Free_Parse_Table_Memory
 *-------------------------------------------------------------------*/
void Free_Parse_Table_Memory(
    Parse_Tbl_def *ParseTbl_ptr )
{
    if ( ParseTbl_ptr->MaxSize != 0 )
    {
        free( ParseTbl_ptr->Data );
        /* Mark as Free */
        ParseTbl_ptr->MaxSize = 0;
    }
}

/*-------------------------------------------------------------------*
 * Free_Insert_Table_Memory
 *-------------------------------------------------------------------*/
void Free_Insert_Table_Memory(
    Insert_Tbl_def *InsertTbl_ptr )
{
    if ( InsertTbl_ptr->MaxSize != 0 )
    {
        free( InsertTbl_ptr->Data );
        /* Mark as Free */
        InsertTbl_ptr->MaxSize = 0;
    }
}

/*-------------------------------------------------------------------*
 * Free_FctCall_Table_Memory
 *-------------------------------------------------------------------*/
void Free_FctCall_Table_Memory(
    FctCall_Tbl_def *FctCallTbl_ptr )
{
    if ( FctCallTbl_ptr->MaxSize != 0 )
    {
        free( FctCallTbl_ptr->Data );
        /* Mark as Free */
        FctCallTbl_ptr->MaxSize = 0;
    }
}

/*-------------------------------------------------------------------*
 * Free_Pointer_Table_Memory
 *-------------------------------------------------------------------*/
void Free_Pointer_Table_Memory(
    Pointer_Tbl_def *PointerTbl_ptr )
{
    if ( PointerTbl_ptr->MaxSize != 0 )
    {
        free( PointerTbl_ptr->Data );
        /* Mark as Free */
        PointerTbl_ptr->MaxSize = 0;
    }
}

/*-------------------------------------------------------------------*
 * Setup_Regions
 *-------------------------------------------------------------------*/
TOOL_ERROR Setup_Regions(
    Parse_Context_def *ParseCtx_ptr,
    bool verbose
)
{
    TOOL_ERROR ErrCode = NO_ERR;
    char* ptr, * end;
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/
    File_def* file_ptr;
    Parse_Tbl_def* ParseTbl_ptr;
    Parse_Rec_def* ParseRec_ptr;
    int idx;

    /* Check Previous State */
    if ( ParseCtx_ptr->State != FILE_OK )
    {
        ErrCode = Internal_Error( __FUNCTION__ );
        goto ret;
    }

    /* Set State Failure (by default) */
    ParseCtx_ptr->State = SETUP_FAILED;

    /* Get File Address (for clarity) */
    file_ptr = &ParseCtx_ptr->File;
    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Set File Pointers */
    ptr = file_ptr->Data;
    end = ptr + file_ptr->Size;

    /* Prefix Data with NUL & LF Chars */
    ptr[-2] = NUL_CHAR;
    ptr[-1] = LF_CHAR;
    /* Append CR & NUL Chars to Data */
    end[0] = CR_CHAR;
    end[1] = NUL_CHAR;

    /* Erase Parse Table */
    ParseCtx_ptr->ParseTbl.Size = 0;

    /* Check for Invalid Chars */
    while ( ptr != end )
    {
        /* Contains a Forbidden Character (Blank, DEL or NUL) */
        if ( IS_RESERVED_CHAR( *ptr ) || *ptr == NUL_CHAR )
        { /* Yes */
            ErrCode = ERR_FILE_INVALID_CHAR;
            Error( "%s Invalid Character '0x%02x' in " DQUOTE( "%s" ), ErrCode,
                   Make_Position_String( ptr, temp ), (unsigned char) *ptr,
                   ParseCtx_ptr->File.Name );
            goto ret;
        }
        /* Advance */
        ptr++;
    }

    /* Find Quote, Double Quote & Comment */
    if ( ( ErrCode = Find_Enclosed_Text( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Defines */
    if ( ( ErrCode = Find_Defines( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find '\' Newline */
    if ( ( ErrCode = Find_Line_Continuation( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Includes */
    if ( ( ErrCode = Find_Includes( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Conditionnals */
    if ( ( ErrCode = Find_Conditionnals( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Undefines */
    if ( ( ErrCode = Find_Undefines( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Others */
    if ( ( ErrCode = Find_Others( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Preprocessor Instrumentation */
    if ( ( ErrCode = Find_Preproc_Instrumentation( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Functions */
    if ( ( ErrCode = Find_Functions( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Token Pasting */
    if ( ( ErrCode = Find_Token_Pasting( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Search for the Preprocessor Character that is in a Function Block but
       not in a Comment, Text or on a Definition Line Command Region */
    if ( ( ptr = Find_String( ParseCtx_ptr->File.Data, "#",
                              &ParseCtx_ptr->ParseTbl ) ) != NULL )
    {
        ErrCode = ERR_POUND;
        Error( "%s " DQUOTE( "%c" ) " not allowed within functions",
               ErrCode, Make_Position_String( ptr, temp ), *ptr );
        goto ret;
    }

    /* Find Keywords */
    if ( ( ErrCode = Find_Keywords( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Data Declarations */
    if ( ( ErrCode = Find_Data_Declarations( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Remove Superfluous End of Statement */
    if ( ( ErrCode = Remove_Superfluous_EOS( ParseCtx_ptr, true ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Remove Extra Parentheses */
    if ( ( ErrCode = Remove_Extra_Parentheses( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Delete Instrumentation Strings (To Avoid being Misinterpreted as Functions if next to a '(') */
    Delete_Identifier( ParseCtx_ptr->File.Data, LOGICAL_INSTRUMENT_STRING, &ParseCtx_ptr->ParseTbl );
    Delete_Identifier( ParseCtx_ptr->File.Data, TERNARY_INSTRUMENT_STRING, &ParseCtx_ptr->ParseTbl );

    /* Find Calls (Functions, Macros, ...) */
    if ( ( ErrCode = Find_Calls( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Constants (Values, Labels, ...) */
    if ( ( ErrCode = Find_Constants( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Operators */
    if ( ( ErrCode = Find_Operators( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Find Non-Instrumented Regions marked with #ifdef WMC_TOOL_SKIP ... #undef WMC_TOOL_SKIP */
    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;
    /* Find the Beginning of Non-Instrumented Regions */
    while ( ( ptr = Find_Identifier( ptr, WMC_TOOL_SKIP_STRING, ParseTbl_ptr, ITEM_CSTE_NAME, ITEM_ENCLOSED, &idx ) ) != NULL )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        /* Go to Start of Next Line */
        ptr = strlend( ParseRec_ptr->item_end ) + 1;

        /* Find the End of Non-Instrumented Regions */
        /* Found it? */
        if ( ( end = Find_Identifier( ptr, WMC_TOOL_SKIP_STRING, ParseTbl_ptr,
                                      ITEM_PREPROC_ARGS | ITEM_PREPROC_UNDEF, ITEM_ENCLOSED, &idx ) ) == NULL )
        { /* No */
            
            /* Error - #undef WMC_TOOL_SKIP missing ! */
            ErrCode = ERR_EXPECTED_EOS;
            Error("Unable to find matching #undef %s!", ErrCode, WMC_TOOL_SKIP_STRING);
            goto ret;

            /* Skipped Region will End at EOF */
            //end = file_ptr->Data + file_ptr->Size;
        }
        else
        { /* Yes */
            /* Go to Preprocessor Command */
            idx--;

            /* Get Record */
            ParseRec_ptr = &ParseTbl_ptr->Data[idx];

            /* Non-Instrumented Region will End before #undef */
            end = ParseRec_ptr->item_start;
        }

        /* Add Non-Instrumented Region */
        if ( ( ErrCode = Add_Region( ParseTbl_ptr, ITEM_INSTRUMENTATION_OFF, ptr, end ) ) != NO_ERR )
        {
            goto ret;
        }
    }

    /* Check, if Manual Instrumentation Macro is Found in a Non-Skipped Region */
    if (verbose)
    {
        if ((ptr = Find_String(ParseCtx_ptr->File.Data, "(", ParseTbl_ptr,
            ITEM_SKIPPED | ITEM_INSTRUMENTATION | ITEM_CALL_ARGS | ITEM_FUNC_COUNTERS_MAN,
            ITEM_INSTRUMENTATION_OFF)) != NULL)
        {
            Print_Warning("\nManual Instrumentation Macro in a Non-Skipped Region", WARN_MANUAL_MACRO_IN_NONSKIPPED_REGION);
        }
    }

    /* Find ROM Counting Instrumentation */
    if ( ( ErrCode = Find_ROM_Counting_Instrumentation( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;

    /* Find Instrumentation Comments */
    while ( ( ptr = Find_String( ptr, "WMC_Tool", ParseTbl_ptr,
                                 ITEM_COMMENT, ITEM_PREPROC_LINE, &idx ) ) != NULL )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        /* Mark Comment as Instrumentation */
        ParseRec_ptr->item_type |= ITEM_INSTRUMENTATION;

        /* continue After Comment */
        ptr = ParseRec_ptr->item_end;
    }

    /* Set Program Memory Size */
    ParseCtx_ptr->PROMSize = 0;

    /* Set State Success */
    ParseCtx_ptr->State = SETUP_OK;

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Unindent_sub
 *-------------------------------------------------------------------*/
static void Unindent_sub(
    Pointer_Rec_def *PointerRec_ptr,
    int Size,
    int max_nChars )
{
    int nChars;
    char *ptr;
    char *ptr2;

    char chr;

    int idx;

    /* Now Delete Spaces */
    for ( idx = 0; idx < Size; idx++ )
    {
        /* Get 2nd Location */
        ptr2 = PointerRec_ptr->Ptr2;
        /* Get Location */
        ptr = PointerRec_ptr++->Ptr;
        /* Set Maximum # of Spaces to Delete */
        nChars = max_nChars;
        do
        {
            /* Rewind Through Spaces */
            while ( chr = PREV_CHAR( ptr ), IS_SPACE_CHAR( chr ) )
            {
                /* Back Up (Pointer Retrieved is at 1st Non-Space Location) */
                ptr--;
                /* Deleted Character? */
                if ( chr != DEL_CHAR )
                { /* No */
                    /* Remove Space */
                    Delete( ptr, ptr + 1 );
                    /* Decrease Spaces Counter */
                    if ( --nChars == 0 )
                    { /* Don't Do 2nd Location */
                        ptr2 = NULL;
                        break;
                    }
                }
            }
            /* Do 2nd Location */
            ptr = ptr2;
            ptr2 = NULL;
        } while ( ptr != NULL );
    }
}

/*-------------------------------------------------------------------*
 * Unindent
 *-------------------------------------------------------------------*/
TOOL_ERROR Unindent(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;
    /* Check Previous State */
    if ( ParseCtx_ptr->State != DESINSTRUMENTED )
    {
        ErrCode = Internal_Error( __FUNCTION__ );
        goto ret;
    }

    int nChars, max_nChars;
    char *ptr, *end, *tmp;
    char *ptr2;
    char chr;
    int del_min, del_step, del_size;

    File_def *file_ptr;
    Parse_Tbl_def *ParseTbl_ptr;
    Parse_Rec_def *ParseRec_ptr;
    Pointer_Tbl_def *PointerTbl_ptr;
    Pointer_Rec_def PointerRecord;

    int idx;

    Item_Type item_type;

    typedef struct
    {
        enum
        {
            HIT_NONE,
            HIT_1ST_COMMENT,
            HIT_NORMAL_CODE
        } hit_type;
        int nChars;
        int idx;
    } unindent_info_def;
    unindent_info_def unindent_info[2];
    int unindent_info_size;

    short braces_alignments[TAB_SIZE];

    /* Get File Address (for clarity) */
    file_ptr = &ParseCtx_ptr->File;
    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;
    /* Get Pointer Table Address (for clarity) */
    PointerTbl_ptr = &ParseCtx_ptr->PointerTbl;

    /* Set File Start Pointer */
    ptr = file_ptr->Data;

    /* Reset Braces Alignment Occurances Vector */
    memset( braces_alignments, 0, sizeof( braces_alignments ) );
    /* Count Braces Occurences per TAB_SIZE Modulo of the Column Index */
    for ( end = ptr; ( end = Find_Chars( end, "{}", ParseTbl_ptr ) ) != NULL; end++ )
    {
        /* Skip Spaces to the Left (this cannot fail) */
        tmp = Skip_Chars( end - 1, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER | ITEM_INSTRUMENTATION, BACKWARDS );
        /* Start of Line? */
        if ( IS_EOL_CHAR( *tmp ) )
        { /* Yes */
            /* Calculate Column # */
            idx = (int) ( end - tmp );
            /* Convert Column # to Column Index */
            idx--;
            /* Convert Column Index to Alignement (0..TAB_SIZE-1) */
            idx %= TAB_SIZE;
            /* Count Alignement */
            braces_alignments[idx]++;
        }
    }
    /* Get Highest Deletion Size to Keep Alignment */
    del_min = 0; /* Start with 0 which Represents TAB_SIZE Removal (Efficient when Already Aligned)*/
    for ( idx = TAB_SIZE; --idx > 0; )
    {
        /* Greater # of Occurances? */
        if ( braces_alignments[idx] > braces_alignments[del_min] )
        { /* Yes */
            del_min = idx;
        }
    }
    /* Get # of Occurances */
    nChars = braces_alignments[del_min];
    /* Is it > 0? */
    if ( nChars > 0 )
    { /* Yes (Braces Present) */
        /* Remove Multiples of TAB_SIZE */
        del_step = TAB_SIZE;
        /* Set # of Chars to Remove to Restore Alignment (In Case cannot Remove Spaces) */
        ParseCtx_ptr->nChars = del_min;
        /* Misalignment is 0? */
        if ( del_min == 0 )
        { /* Yes */
            /* Will Try to Remove at Least the Delete Step */
            del_min = del_step;
        }
        /* Get Smallest Deletion Size to Reach Alignment (That is as Good: Same # of Occurances) */
        for ( idx = del_min; --idx > 0; )
        {
            /* Same # of Occurances? */
            if ( braces_alignments[idx] == nChars )
            { /* Yes */
                del_min = idx;
            }
        }
    }
    else
    { /* No (No Braces Information) */
        /* Will Try to Remove at Least 1 Char  */
        del_min = 1;
        /* Can Delete by 1 Char Steps */
        del_step = 1;
        /* No Chars Removal Required to Restore Alignment */
        ParseCtx_ptr->nChars = 0;
    }

    /* Set File End Pointer */
    end = ptr + file_ptr->Size;

    /* Reset # of Unindent Entries */
    unindent_info_size = 0;

    /* Erase Pointer Table */
    PointerTbl_ptr->Size = 0;
    do
    {
        /* Full? */
        if ( unindent_info_size >= (int)(nitems( unindent_info )) )
        { /* Yes */
            /* Problem! */
            ErrCode = Internal_Error( __FUNCTION__ );
            goto ret;
        }
        /* Set Hit Type */
        unindent_info[unindent_info_size].hit_type = unindent_info_def::HIT_NONE;
        /* Save Start Index */
        unindent_info[unindent_info_size].idx = PointerTbl_ptr->Size;

        /* Set Default Delete Size */
        del_size = del_min;
        /* Initialize Maximum # of Spaces that Could be Deleted */
        max_nChars = 999999L;
        /* Get End (for Current Pass) */
        tmp = end;
        /* Check whole File */
        while ( ptr < tmp )
        {
            /* Reset 2nd Pointer Location */
            PointerRecord.Ptr2 = NULL;

            /* Reset Spaces Counter */
            nChars = 0;
            do
            {
                /* Advance Through Spaces */
                while ( chr = *ptr, IS_SPACE_CHAR( chr ) )
                {
                    /* Space or Blank? */
                    if ( chr == SPACE_CHAR || chr == BLANK_CHAR )
                    { /* Yes */
                        /* Increment Spaces Counter */
                        nChars++;
                    }
                    /* Advance */
                    ptr++;
                }
                /* Start of Line? */
                if ( IS_EOL_CHAR( chr ) )
                { /* Yes */
                    /* Empty Line (Do Not Count Spaces) */
                    break;
                }
                /* Manual Instrumentation? (All are Packed to the Left) */
                else if ( ( idx = Find_Region( ptr, ParseTbl_ptr, ITEM_CALL | ITEM_FUNC_COUNTERS_MAN ) ) >= 0 )
                { /* Yes */
                    /* Skip Call */
                    idx++;
                    /* Get Call Arguments Record */
                    ParseRec_ptr = &ParseTbl_ptr->Data[idx];
                    /* Get Call Arguments Record Type */
                    item_type = ParseRec_ptr->item_type;
                    /* Continue After Call Arguments */
                    ptr = ParseRec_ptr->item_end;
                    /* Skip Call Arguments Record */
                    ParseRec_ptr++;
                    /* Optional End of Statement? */
                    if ( ParseRec_ptr->item_type == item_type )
                    { /* Yes */
                        /* Continue After End of Statement */
                        ptr = ParseRec_ptr->item_end;
                    }
                    /* Reset Spaces Counter (Keep at Least one Space After Manual Macro) */
                    nChars = -1;
                }
                else
                { /* No */
                    /* Preprocessor Line? */
                    if ( Find_Region( ptr, ParseTbl_ptr, ITEM_PREPROC_LINE ) >= 0 )
                    { /* Yes */
                        /* Set Hit Type */
                        unindent_info[unindent_info_size].hit_type = unindent_info_def::HIT_NORMAL_CODE;
                        /* (Do Not Count Spaces) */
                    }
                    /* Counting Library function Call? */
                    else if ( ( idx = Find_Region( ptr, ParseTbl_ptr, ITEM_CALL | ITEM_CALL_ARGS | ITEM_FUNC_COUNT_LIB ) ) >= 0 )
                    {
                        /* Skip Call */
                        idx++;
                        /* Get Call Arguments Record */
                        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
                        /* Get Call Arguments Record Type */
                        item_type = ParseRec_ptr->item_type;
                        /* Continue After Call Arguments */
                        ptr2 = ParseRec_ptr->item_end;
                        /* Skip Call Arguments Record */
                        ParseRec_ptr++;
                        /* Optional End of Statement? */
                        if ( ParseRec_ptr->item_type == item_type )
                        { /* Yes */
                            /* Continue After End of Statement */
                            ptr2 = ParseRec_ptr->item_end;
                        }
                        /* Advance Through Spaces */
                        while ( chr = *ptr2, IS_SPACE_CHAR( chr ) )
                        {
                            /* Advance */
                            ptr2++;
                        }
                        /* Save 2nd Location */
                        PointerRecord.Ptr2 = ptr2;
                        /* Set Hit Type */
                        unindent_info[unindent_info_size].hit_type = unindent_info_def::HIT_NORMAL_CODE;
                        /* (Do Not Count Spaces) */
                    }
                    else
                    { /* No */
                        /* 1st Hit? */
                        if ( unindent_info[0].hit_type == unindent_info_def::HIT_NONE )
                        { /* Yes */
                            /* Set Hit Type (by Default) */
                            unindent_info[unindent_info_size].hit_type = unindent_info_def::HIT_NORMAL_CODE;
                            /* Long Comment? */
                            if ( ( idx = Find_Region( ptr, ParseTbl_ptr, ITEM_COMMENT_BLOCK ) ) >= 0 )
                            { /* Yes */
                                /* Set Hit Type */
                                unindent_info[unindent_info_size].hit_type = unindent_info_def::HIT_1ST_COMMENT;
                                /* Get Comment Record */
                                ParseRec_ptr = &ParseTbl_ptr->Data[idx];
                                /* Set End to End of Comment (for Current Pass) */
                                tmp = ParseRec_ptr->item_end;
                                /* Allow Smaller Delete Size */
                                del_size = 1;
                            }
                        }
                        /* Stop if Available Space is Smaller than Delete Size */
                        if ( nChars < del_size )
                        {
                            /* Go To End */
                            ptr = tmp;
                            goto next;
                        }
                        /* Keep Maximum # of Spaces that Could be Deleted */
                        if ( max_nChars > nChars )
                            max_nChars = nChars;
                    }
                    /* Save Location */
                    PointerRecord.Ptr = ptr;
                    if ( ( ErrCode = Add_Pointer( PointerTbl_ptr, &PointerRecord ) ) != NO_ERR )
                    {
                        goto ret;
                    }
                    /* Advance to End of Line */
                    while ( ++ptr, !IS_EOL_CHAR( *ptr ) )
                        ;
                }
                /* Get Character */
                chr = *ptr;
            }
            /* Continue Until End of Line Reached? */
            while ( !IS_EOL_CHAR( chr ) );

            /* Advance to Start of Next Line */
            while ( ++ptr, IS_EOL_CHAR( *ptr ) )
                ;
        }

        /* Do not Unindent Now, Only Save Info */
        unindent_info[unindent_info_size++].nChars = max_nChars;
    next:;
    } while ( ptr < end );

    /* Now, Space Removal is Done:*/
    /* Either while Preserving Alignment (del_step == TAB_SIZE) or*/
    /* without Alignment Requirement (del_step == 1)*/
    /* Otherwise, it is not Done (No Space at All)*/

    /* Look for Normal Code */
    for ( idx = 0; idx < unindent_info_size; idx++ )
    {
        /* Found Normal Code? */
        if ( unindent_info[idx].hit_type == unindent_info_def::HIT_NORMAL_CODE )
        { /* Yes (Space Removal can be Done) */
            int temp;

            /* Get Maximum # of Chars to Unindent */
            max_nChars = unindent_info[idx].nChars;
            /* Calculate Remainder of Delete Step Size */
            nChars = ( max_nChars - del_min ) % del_step;
            /* Minimize Remainder (Maximize Deletion Size) */
            for ( temp = nChars; --temp >= 0; )
            {
                /* Increase Deletion (and Take Care of Wraparound) */
                ++del_min %= TAB_SIZE;
                /* Same # of Occurances (That is a Suitable Alignment Too) */
                if ( braces_alignments[del_min] == braces_alignments[ParseCtx_ptr->nChars] )
                { /* Yes */
                    nChars = temp;
                }
            }
            /* Substract Characters that Must Stay to Keep Alignment (nChars can be 0) */
            max_nChars -= nChars;
            /* Update # of Chars to Remove to Restore Alignment */
            ParseCtx_ptr->nChars = 0;

            /* Unindent */
            temp = PointerTbl_ptr->Size;
            while ( --unindent_info_size >= 0 )
            { /* Yes */
                idx = unindent_info[unindent_info_size].idx;
                /* Get # of Chars to Unindent */
                nChars = unindent_info[unindent_info_size].nChars;
                /* Use Minimum */
                if ( nChars > max_nChars )
                    nChars = max_nChars;
                /* Unindent */
                Unindent_sub( &PointerTbl_ptr->Data[idx], temp - idx, nChars );
                temp = idx;
            }
            /* Stop */
            break;
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * DesInstrument
 *-------------------------------------------------------------------*/
TOOL_ERROR DesInstrument(
    Parse_Context_def *ParseCtx_ptr,
    bool remove_manual )
{
    TOOL_ERROR ErrCode = NO_ERR;
    /* Check Previous State */
    if ( ParseCtx_ptr->State != SETUP_OK )
    {
        ErrCode = Internal_Error( __FUNCTION__ );
        goto ret;
    }
    /* Set State Failure (by default) */
    ParseCtx_ptr->State = DI_FAILED;

    int idx, idx2;
    char *ptr, *start, *tmp;
    char *ptr2;
    Item_Type item_type;
    Parse_Tbl_def *ParseTbl_ptr;
    Parse_Rec_def *ParseRec_ptr;
    size_t size, n;
    char chr;
    bool blank;

    /* Set Item Type */
    item_type = ITEM_FUNC_COUNTERS_AUTO;
    if ( remove_manual )
        item_type |= ITEM_FUNC_COUNTERS_MAN;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* for All Stuff that is Instrumentation */
    for (idx = 0; (idx = Find_Region(NULL, ParseTbl_ptr, ITEM_INSTRUMENTATION, idx)) >= 0; idx++)
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        /* Get Range */
        start = ParseRec_ptr->item_start;
        tmp = ParseRec_ptr->item_end;

        /* check if we're in WMC_TOOL_SKIP region */
        if (Find_Region(ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_INSTRUMENTATION_OFF) > 0)
        {
            continue;
        }

        /* Instrumentation Definition? */
        if (ParseRec_ptr->item_type & ITEM_PREPROC_LINE)
        { /* Yes */
            if (Find_Region(start, ParseTbl_ptr, ITEM_FUNC_BLOCK) < 0)
            {
                /* Check if this is "#include wmc_auto.h", i.e. not preceded by AddedByWMC_Tool -> do not delete it */
                ptr = start; while (*ptr == ' ') ptr++;    /* skip all spaces to the left */
                if (strncmp(ptr, "#include", strlen("#include")) == 0)
                {
                    ptr = ptr + strlen("#include"); while (*ptr == ' ') ptr++;    /* skip all spaces to the left */
                    if (strncmp(ptr, DQUOTE(WMC_TOOL_INCLUDE_STRING), strlen(DQUOTE(WMC_TOOL_INCLUDE_STRING))) == 0)
                    {
                        /* Item is no Longer Instrumented */
                        ParseRec_ptr->item_type ^= ITEM_INSTRUMENTATION;

                        /* Get Next Item */
                        ParseRec_ptr = &ParseTbl_ptr->Data[++idx];
                        ParseRec_ptr->item_type ^= ITEM_INSTRUMENTATION;

                        continue;
                    }
                }

                /* Delete it */
                Delete(start, tmp);
            }
        }

        /* Instrumentation Code? */
        else if (ParseRec_ptr->item_type & item_type && ((ParseRec_ptr->item_type & ITEM_FUNC_COUNTERS_MAN) == 0 ||
            Find_Region(start, ParseTbl_ptr, ITEM_INSTRUMENTATION_OFF) < 0))
        { /* Yes */
            /* Blank Instrumentation by Default */
            blank = true;
            /* Right Instrumentation? */
            if (ParseRec_ptr->item_type & ITEM_FUNC_COUNTERS_AUTO_R)
            { /* Yes */
                /* Delete */
                blank = false;
            }
            else if (ParseRec_ptr->item_type & ITEM_FUNC_COUNTERS_AUTO_L)
            { /* No */
                /* Skips Spaces to the Left (this cannot fail) */
                ptr = Skip_Chars(start - 1, SPACE_CHARS, ParseTbl_ptr,
                    ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS,
                    &idx2);
                /* If it is Possible to Get to a Start of Line
                   or to an Instrumentation Macro or Function by
                   Skipping Blanks, Comments and Preprocessor Lines,
                   then it is not Auto Instrumentation Trapped between
                   Statements */

                   /* Start of Line? */
                if (!IS_EOL_CHAR(*ptr))
                { /* No */
                    /* Instrumentation or Counting Library Call? */
                    if (Find_Region(ptr, ParseTbl_ptr, ITEM_INSTRUMENTATION) < 0 &&
                        Find_Region(ptr, ParseTbl_ptr, ITEM_CALL | ITEM_CALL_ARGS | ITEM_FUNC_COUNT_LIB) < 0)
                    { /* No */
                        /* Delete */
                        blank = false;
                    }
                }
            }
            /* Blank? */
            if (blank)
            { /* Yes */
                /* Blank it */
                Blank(start, tmp);
            }
            else
            { /* No */
                /* Delete it */
                Delete(start, tmp);
            }
        }
        /* Function Start? */
        else if (ParseRec_ptr->item_type & ITEM_DATA_DECL)
        { /* Yes */
            /* Delete Leading Spaces */
            while (IS_SPACE_CHAR(PREV_CHAR(start)))
                start--;
            /* Trailing Space? */
            if (IS_SPACE_CHAR(*tmp))
            { /* Yes */
                /* Delete it too */
                tmp++;
            }
            /* Delete it */
            Delete(start, tmp);
        }
        /* Manual Macro? (Automatic Macros are Always Blanked or Deleted) */
        else if (ParseRec_ptr->item_type & ITEM_FUNC_COUNTERS_MAN)
        { /* Yes (This gets executed only in Keep Macros Mode; to compact them) */
            /* Get Manual Macro Length */
            size = tmp - start;
            /* Find where Spaces End */
            for (ptr2 = start; chr = PREV_CHAR(ptr2), IS_SPACE_CHAR(chr); ptr2--)
                ;
            /* Not EOS or EOS of a Counting Library Function Call? */
            if (chr != ';' || Find_Region(ptr2 - 1, ParseTbl_ptr, ITEM_CALL | ITEM_CALL_ARGS | ITEM_FUNC_COUNTERS_MAN) >= 0)
            {
                /* Then we have the Case were
                   1) Alone on one Line
                   2) On same Line as Block Start
                   3) Misplaced
                   All this is can only occur for the 1st Manual Macro on the Line
                */
                /* Do not Do it */
                /*                break; see if break works*/
                /* Do not Do it */
                ptr2 = start;
            }
            else
            {
                /* Do not Do it */
                ptr2 = start;
            }
            /* Stop when there is no Room to Move Left */
            while (start > ptr2)
            {
                /* Get Character */
                chr = PREV_CHAR(start);
                /*
                  Manual Macros will not be Moved over Comments, Code, Line Change, ...
                  Only over Blanks on Same Line
                  However, they will be moved if they are after Statements
                    ex: "i++;   ADD(1)" becomes "i++;ADD(1)"
                  Which should not happen with Manual Instrumentation.
                */
                /* Save Start Pointer */
                ptr = start;
                /* Move Left */
                while (chr == PREV_CHAR(--start))
                    ;
                /* Move Manual Macro Left */
                memcpy(start, ptr, size);
                /* Calculate Move Offset */
                n = ptr - start;
                /* Get Minimal Length to Restore */
                if (size < n)
                    n = size;
                /* Restore Space Chars */
                memset(tmp - n, chr, n);
                /* Save End Pointer */
                tmp = start + size;
            }
            /* Adjust Locations */
            ParseRec_ptr->item_start = start;
            ParseRec_ptr->item_end = tmp;
            /* Item will Stay Instrumentation */
            ParseRec_ptr->item_type ^= ITEM_INSTRUMENTATION;
        }
        /* ROM Counting Function Definition? */
        else if (ParseRec_ptr->item_type & ITEM_FUNC_DEF)
        { /* Yes */
            /* Delete it */
            Delete(start, tmp);

            /* Desintrumenting Function Name? -> Check for preceding return type */
            if (ParseRec_ptr->item_type & ITEM_FUNC_NAME)
            {
                /* Find Return Type (this can fail) */
                tmp = Skip_Chars(start - 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS);

                /* Identifier? */
                if (tmp != NULL && IS_IDENTIFIER_CHAR(*tmp))
                { /* Yes */
                    /* Find Start */
                    start = Skip_Identifier(tmp, BACKWARDS) + 1;
                    /* Is it the 'int' Data Type? */
                    if (memwordcmp(start, INT_STRING))
                    { /* Yes */
                        /* Delete it */
                        Delete(start, tmp + 2);

                        /* Check if it's preceded by 'extern' keyword */
                        tmp = Skip_Chars(start - 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS);
                        /* Identifier? */
                        if (tmp != NULL && IS_IDENTIFIER_CHAR(*tmp))
                        { /* Yes */
                            /* Find Start */
                            start = Skip_Identifier(tmp, BACKWARDS) + 1;
                            /* Is it the 'extern' Data Type? */
                            if (memwordcmp(start, STORAGE_STRING))
                            { /* Yes */
                                /* Delete it */
                                Delete(start, tmp + 2);
                            }
                        }
                    }
                }
            }

            /* Item is no Longer a Function Name/Proto/Block */
            ParseRec_ptr->item_type &= ~ITEM_FUNC_DEF;
        }
        /* Item is no Longer Instrumentation */
        ParseRec_ptr->item_type ^= ITEM_INSTRUMENTATION;
    }

    /* for All Stuff that is Instrumented */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_INSTRUMENTED, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Get Item Type */
        item_type = ParseRec_ptr->item_type;

        /* Will be DesInstrumented */
        item_type ^= ITEM_INSTRUMENTED;

        /* Is it a Project Function Call? */
        if ( item_type == ( ITEM_CALL | ITEM_FUNC_PROJECT ) )
        { /* Yes */
            /* DesInstrument Project Function Call */
            DesInstrument_Prj_Fct_Call( &ParseRec_ptr->item_start, &ParseRec_ptr->item_end );
            /* Recheck Project Function Call Type */
            item_type = Get_Call_Type( ParseRec_ptr->item_start );
            /* Stil a Project Function Call? */
            if ( item_type != ITEM_FUNC_PROJECT )
            { /* No */
                /* Change Function Call Arguments Type */
                ParseRec_ptr[1].item_type = item_type | ITEM_CALL_ARGS;
            }
            /* Set Function Call Type */
            item_type |= ITEM_CALL;
        }
        /* Is it a 'while' that is part of a 'do'/'while' Block Keyword? */
        else if ( item_type == ITEM_KEYWORD_WHILE2 )
        { /* Yes */
            /* DeInstrument Item */
            DesInstrument_Word_Front( &ParseRec_ptr->item_start, &ParseRec_ptr->item_end );
            /* 2nd 'while' type now determined by Validation of 'do'*/
            /* Change Item Type */
            item_type = ITEM_KEYWORD_WHILE;
        }
        else
        { /* No */
            /* DeInstrument Item */
            DesInstrument_Word( &ParseRec_ptr->item_start, &ParseRec_ptr->item_end );
            /* Is it a 'return' that was added by the tool? */
            if ( item_type == ITEM_KEYWORD_RETURN )
            { /* Yes */
                /* Go To EOS */
                if ( ( ptr = Goto_EOS( ParseRec_ptr->item_end, ParseTbl_ptr ) ) != NULL )
                {
                    /* Go Back by 1 */
                    ptr--;
                    /* Something Erased? */
                    if ( *ptr == DEL_CHAR )
                    { /* Yes */
                        /* Was it a Comment? */
                        if ( ( idx2 = Find_Region( ptr, ParseTbl_ptr, ITEM_COMMENT_BLOCK ) ) >= 0 )
                        { /* Yes */
                            /* Go To Start of Comment */
                            tmp = ParseTbl_ptr->Data[idx2].item_start;
                            /* Go Back */
                            tmp = Skip_Chars( tmp - 1, BLANK_CHARS, ParseTbl_ptr,
                                              ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                            /* It is the 'return' Keyword */
                            if ( tmp == ParseRec_ptr->item_end - 1 )
                            { /* Yes */
                                /* Go To Start of Line */
                                tmp = ParseRec_ptr->item_start;
                                while ( --tmp, !IS_EOL_CHAR( *tmp ) )
                                    ;
                                /* Advance (Past EOS) */
                                ptr += 2;
                                /* End of Line Char? */
                                if ( IS_EOL_CHAR( *ptr ) )
                                {
                                    ptr = strlend( ptr ) + 1;
                                }
                                /* Erase from Start of Line to EOS and Line Feed */
                                Delete( tmp + 1, ptr );
                                /* Destroy Item */
                                item_type = ITEM_NONE;
                            }
                        }
                    }
                }
            }
        }

        /* Item is no Longer Instrumented */
        ParseRec_ptr->item_type = item_type;
    }

    if ( ( ErrCode = Remove_Superfluous_EOS( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* for All Function */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_FUNC_NAME, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Is it a Debug/Math or System Function? */
        if ( ParseRec_ptr->item_type & ( ITEM_FUNC_MATH | ITEM_FUNC_SYSTEM ) )
        { /* Yes */
            /* Function will be Skipped w Warning (not Instrumented) */
            ParseRec_ptr++->item_type |= ITEM_SKIPPED | ITEM_WARNING;
            /* Function Prototype will be Skipped (not Instrumented) */
            ParseRec_ptr++->item_type |= ITEM_SKIPPED;
            /* Function Block will be Skipped (not Instrumented) */
            ParseRec_ptr->item_type |= ITEM_SKIPPED;
        }
        /* Advance to Function Block */
        idx += 2;
    }

    /* Remove Footer Spaces */
    /* Start at End */
    tmp = ParseCtx_ptr->File.Data + ParseCtx_ptr->File.Size;
    /* Skip Blanks to the Left */
    if ( ( ptr = r_strpbrkn( tmp - 1, BLANK_CHARS ) ) == NULL )
    {
        ErrCode = ERR_FILE_EMPTY;
        Error( ERROR_MSG_FILE_EMPTY, ErrCode, ParseCtx_ptr->File.Name );
        goto ret;
    }
    /* Go to Start of Next Line */
    ptr = strlend( ptr ) + 1;
    /* File has Proper CR/LF Before End? */
    if ( *ptr == NUL_CHAR )
    { /* No (The only EOL Char Found was the one Appended) */
        /* Append PC CR/LF (There is Enough Room) */
        strcpy( tmp, EOL_CHARS );
        /* Update File Size */
        ParseCtx_ptr->File.Size += STRING_LENGTH( EOL_CHARS );
    }
    else
    { /* Yes */
        /* Set End There */
        *ptr = NUL_CHAR;
        /* Update File Size */
        ParseCtx_ptr->File.Size = (int) ( ptr - ParseCtx_ptr->File.Data );
    }

    /* Erase Insert Table */
    ParseCtx_ptr->InsertTbl.Size = 0;

    /* Set State Success */
    ParseCtx_ptr->State = DESINSTRUMENTED;

    /* Unindent File */
    ErrCode = Unindent( ParseCtx_ptr );

ret:
    return ErrCode;
}



/*-------------------------------------------------------------------*
 * DesInstrument_ROM
 *-------------------------------------------------------------------*/

TOOL_ERROR DesInstrument_ROM(
    Parse_Context_def* ParseCtx_ptr)
{
    TOOL_ERROR ErrCode = NO_ERR;

    int idx;
    char *start, *end, * tmp, *start_const_data_prom_table, *end_const_data_prom_table;
    Parse_Tbl_def* ParseTbl_ptr;
    Parse_Rec_def* ParseRec_ptr;

    start_const_data_prom_table = NULL;
    end_const_data_prom_table = NULL;

    /* Check Previous State */
    if (ParseCtx_ptr->State != SETUP_OK)
    {
        ErrCode = Internal_Error(__FUNCTION__);
        goto ret;
    }

    /* Set State Failure (by default) */
    ParseCtx_ptr->State = DI_FAILED;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Go through all stuff that is instrumentation */
    for (idx = 0; (idx = Find_Region(NULL, ParseTbl_ptr, ITEM_INSTRUMENTATION, idx)) >= 0; idx++)
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        /* Get Range */
        start = ParseRec_ptr->item_start;
        tmp = ParseRec_ptr->item_end;

        /* Instrumentation Code? E.g. 'AddedByWMC_Tool' */
        if (ParseRec_ptr->item_type & ITEM_FUNC_COUNTERS_AUTO && ((ParseRec_ptr->item_type & ITEM_FUNC_COUNTERS_MAN) == 0 ||
            Find_Region(start, ParseTbl_ptr, ITEM_INSTRUMENTATION_OFF) < 0))
        { /* Yes */
            if (!(ParseRec_ptr->item_type & ITEM_PREPROC_LINE))
            {
                /* Delete it */
                Delete(start, tmp);
            }
        }

        /* 'Const_Data_PROM_Table' Declaration? */
        else if (ParseRec_ptr->item_type & ITEM_DATA_DECL)
        { /* Yes */

            /* Check if this ROM_Size_Lookup_Table declaration */
            if (strnistr(start, "ROM_Size_Lookup_Table", strlen("ROM_Size_Lookup_Table")) != NULL)
            {
                /* Save the pointers - will be used later to check, if it was enclosed in #ifdef WMOPS .. #endif pair */
                start_const_data_prom_table = start;
                end_const_data_prom_table = tmp;
            }

            /* Delete it */
            Delete(start, tmp);

        }

        /* One of the ROM counting functions, e.g. 'Const_Data_Size_Func(...)'? */
        else if (ParseRec_ptr->item_type & ITEM_FUNC_DEF)
        { /* Yes */
            /* Is function prototype or block followed by some blank chars? -> delete them as well */
            if (ParseRec_ptr->item_type & (ITEM_FUNC_PROTO | ITEM_FUNC_BLOCK))
            {
                tmp = Skip_Chars(tmp, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_COMMENT, FORWARD);
            }

            /* Delete it */
            Delete(start, tmp);

            /* Desintrumenting Function Name? -> Check if it's preceded by 'extern int' or 'static int' return type */
            if (ParseRec_ptr->item_type & ITEM_FUNC_NAME)
            {
                /* Find the end of the preceding keyword */
                while ((tmp = Skip_Chars(start - 1, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS)) != NULL && IS_IDENTIFIER_CHAR(*tmp))
                {
                    /* Find the beginning of the preceding keyword */
                    start = Skip_Identifier(tmp, BACKWARDS) + 1;

                    /* Check if the found keyword matches one of 'extern', 'static' or 'int' */
                    if (memwordcmp(start, STORAGE_STRING) || memwordcmp(start, INT_STRING))
                    { /* Yes */
                        /* Delete it */
                        Delete(start, tmp + 2);
                    }
                }
            }

            /* Item is no Longer a Function Name/Proto/Block */
            ParseRec_ptr->item_type &= ~ITEM_FUNC_DEF;
        }

        /* Item is no Longer Instrumentation */
        ParseRec_ptr->item_type ^= ITEM_INSTRUMENTATION;
    }

    /* Check, if Const_Data_PROM_Table[] was enclosed in #ifdef WMOPS .. #endif pair */
    if (start_const_data_prom_table != NULL && end_const_data_prom_table != NULL)
    {
        /* Go back to the first word preceding Const_Data_PROM_Table[] */
        start = Skip_Chars(start_const_data_prom_table - 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_NONE, BACKWARDS);

        /* Proceed to the beginning of the line */
        while (!IS_EOL_CHAR(PREV_CHAR(start)))
        {
            start--;
        }

        /* Check if line contains "#ifdef WMOPS" */
        if ( (tmp = stristr(memstr(start, start_const_data_prom_table), "#ifdef")) != NULL)
        {
            tmp = Skip_Identifier(tmp, FORWARD);
            tmp = Skip_Chars(tmp + 1, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_NONE);
            if (strncmp(tmp, "WMOPS", strlen("WMOPS")) == 0)
            {
                /* Delete the whole line */
                Delete(start, start_const_data_prom_table);
            }
        }

        /* Go to the first word after Const_Data_PROM_Table[]  */
        end = Skip_Chars(end_const_data_prom_table, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_NONE);

        /* Check if line contains "#endif" */
        while (!IS_EOL_CHAR(NEXT_CHAR(end)))
        {
            if (*end == '#')
            {
                /* Found '#' */
                if (strncmp(end, "#endif", strlen("#endif")) == 0)
                {
                    /* Go to EOL */
                    while (*end != LF_CHAR)
                    {
                        end++;
                    }

                    /* Delete the whole line */
                    Delete(end_const_data_prom_table, end);

                    break;
                }
            }

            end++;
        }
    }

    /* Find the print_mem() function */
    start = ParseCtx_ptr->File.Data;
    if ((start = Find_String(start, "print_mem", ParseTbl_ptr, ITEM_ANY, ITEM_NONE)) != NULL)
    {
        /* Advance poiner */
        start += strlen("print_mem");

        /* Search for '(' (Stop at EOS or Block start/end) */
        start = Skip_Chars(start, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_NONE);

        /* Found it? */
        if (*start == '(')
        { /* Yes */
            /* Find the matching Parentheses */
            end = Goto_Match(start, ParseTbl_ptr, ITEM_ANY, ITEM_NONE);

            /* Delete the contents */
            Delete(start + 1, end);
        }
    }

    /* Erase Insert Table */
    ParseCtx_ptr->InsertTbl.Size = 0;

    /* Set State Success */
    ParseCtx_ptr->State = DESINSTRUMENTED;

ret:
    return ErrCode;
}



/*-------------------------------------------------------------------*
 * Instrument_ROM_sub2
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_ROM_sub2(
    Parse_Context_def *ParseCtx_ptr,
    char **end,
    char **end2 )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;

#define FUNCTION_RETURN_TYPE "int"
#define FUNCTION_NAME_PREFIX CONST_DATA_SIZE_FUNC_PREFIX_STRING "_Func("
#define FUNCTION_NAME_LENGTH 60
#define FUNCTION_START ") { return"

    char func_1st_line[STRING_LENGTH( ADDED_TOOL_INFO_STRING ) +
                       STRING_LENGTH( FUNCTION_RETURN_TYPE ) + 1 + /* +1 for Space*/
                       STRING_LENGTH( FUNCTION_NAME_PREFIX ) +
                       FUNCTION_NAME_LENGTH +
                       STRING_LENGTH( FUNCTION_START ) +
                       1];                                                     /* +1 for NUL*/

    /* Build 1st Line */
    strcpy( func_1st_line, ADDED_TOOL_INFO_STRING FUNCTION_RETURN_TYPE " " FUNCTION_NAME_PREFIX );
    /* Add Name */
    ptr = func_1st_line + STRING_LENGTH( ADDED_TOOL_INFO_STRING ) +
          STRING_LENGTH( FUNCTION_RETURN_TYPE ) + 1 + /* +1 for Space*/
          STRING_LENGTH( FUNCTION_NAME_PREFIX );
    strncpy( ptr, ParseCtx_ptr->File.Name, FUNCTION_NAME_LENGTH );
    ptr[FUNCTION_NAME_LENGTH] = NUL_CHAR;
    /* Build Function Name (Add Function Start after FileName) */
    strcpy( Fix_FileName( ptr ), FUNCTION_START );

    /* Start at End */
    ptr = ParseCtx_ptr->File.Data + ParseCtx_ptr->File.Size;
    *end2 = ptr;

    /* File Ends with only one CR/LF Combination */
    do
    {
        ptr--;
    } while ( IS_EOL_CHAR( PREV_CHAR( ptr ) ) );

    *end = ptr;

    /* Insert ROM Counting Instrumentation */
    if ( ( ErrCode = Insert_Line( &ParseCtx_ptr->InsertTbl, ptr, func_1st_line ) ) != NO_ERR )
    {
        goto ret;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_ROM_Find_Identifier
 *-------------------------------------------------------------------*/
static char *Instrument_ROM_Find_Identifier(
    char *ptr,
    Parse_Tbl_def *ParseTbl_ptr )
{
    bool parentheses = false;

    /* Move Left until Identifier Character Found (this cannot fail because 'const' was Found) */
    while ( --ptr, !IS_IDENTIFIER_CHAR( *ptr ) )
    {
        /* Subscript? */
        if ( *ptr == ']' )
        { /* Yes */
            /* Goto Opening Bracket */
            if ( ( ptr = Goto_Match( ptr, ParseTbl_ptr ) ) == NULL )
            {
                goto ret;
            }
        }
        /* Closing Parenthesis? */
        else if ( *ptr == ')' )
        { /* Yes */
            /* 1st Time */
            if ( !parentheses )
            { /* Yes */
                /* Goto Opening Bracket */
                if ( ( ptr = Goto_Match( ptr, ParseTbl_ptr ) ) == NULL )
                {
                    goto ret;
                }
                /* Not 1st Time */
                parentheses = true;
            }
        }
    }

ret:
    return ptr;
}

/*-------------------------------------------------------------------*
 * Instrument_ROM
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_ROM
(
    Parse_Context_def *ParseCtx_ptr
)
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr, *ptr2, *ptr3;
    char *end = NULL, *end2 = NULL;
    Parse_Tbl_def *ParseTbl_ptr;

    char temp1[255 + 1]; /* +1 for NUL*/
    char temp2[255 + 1]; /* +1 for NUL*/
    char temp3[255 + 1]; /* +1 for NUL*/

    char const_data_names[13 * 1024 * 4] = "";
    size_t const_data_names_size = 0;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Start at Beginning */
    ptr = ParseCtx_ptr->File.Data;
    /* Find 'const' Identifier */
    while ( ( ptr = Find_Identifier( ptr, CONST_STRING, ParseTbl_ptr, ITEM_ANY, ITEM_FUNC_DEF | ITEM_NOT_SEARCHED | ITEM_INSTRUMENTATION_OFF) ) != NULL )
    {
        /* Advance (after 'const') */
        ptr += STRING_LENGTH( CONST_STRING );
        /* Search for '=' (Stop at EOS or Block start/end) */
        ptr2 = Goto_Chars( ptr, "=;{}", ParseTbl_ptr, ITEM_ANY, ITEM_ENCLOSED | ITEM_PREPROC_LINE );

        /* Found it? */
        if ( ptr2 != NULL && *ptr2 == '=' )
        { /* Yes */
            /* Is it Equal Equal? */
            if ( NEXT_CHAR( ptr2 ) != *ptr2 )
            { /* No */
                /* Find Identifier */
                if ( ( ptr = Instrument_ROM_Find_Identifier( ptr2, ParseTbl_ptr ) ) != NULL )
                {
                    /* 1st Time? */
                    if ( const_data_names_size == 0 )
                    { /* Yes */
                        if ( ( ErrCode = Instrument_ROM_sub2( ParseCtx_ptr, &end, &end2 ) ) != NO_ERR )
                        {
                            goto ret;
                        }
                        strupr( temp1, CONST_DATA_SIZE_FUNC_PREFIX_STRING "_" );
                    }

                    /* Keep Pointer to Last Character of Identifier */
                    ptr3 = Skip_Identifier(ptr, BACKWARDS);
                    /* Copy Constant Name */
                    strcpy(temp3, memstr(ptr3 + 1, ptr + 1));
                    /* Build Uppercase (and Prefixed) Constant Name */
                    strupr(temp1 + STRING_LENGTH(CONST_DATA_SIZE_FUNC_PREFIX_STRING) + 1, temp3);
                    /* Add '#define' to the Constant Data Size */
                    strcpy(temp2, ADDED_TOOL_INFO_STRING "#define ");
                    strcat(temp2, temp1);
                    strcat(temp2, " +rsize(");
                    strcat(temp2, temp3);
                    strcat(temp2, ")");
                    /* Skip spaces and EOL chars after the '=' symbol */
                    ptr = Skip_Chars(ptr2+1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER);
                    /* Insert the string after the end of line */
                    if ((ErrCode = Insert_Line(&ParseCtx_ptr->InsertTbl, strlend(ptr), temp2)) != NO_ERR)
                    {
                        goto ret;
                    }
                    /* New Entry? */
                    if (memwordcmp(temp3, const_data_names) == NULL)
                    {                                                             /* Yes */
                        size_t idx = const_data_names_size + strlen(temp3) + 1; /* +1 for SPACE_CHAR*/
                        /* Enough Room? */
                        if (idx >= sizeof(const_data_names))
                        { /* Yes */
                            ErrCode = Internal_Error(__FUNCTION__);
                            goto ret;
                        }
                        /* Add Name to List */
                        strcpy(&const_data_names[const_data_names_size], temp3);
                        strcpy(&const_data_names[idx - 1], " ");
                        const_data_names_size = idx;

                        /* Instrument it */
                        if ((ErrCode = Add_Insertion(&ParseCtx_ptr->InsertTbl, end2,
                            ADDED_TOOL_INFO_STRING "#ifdef ")) != NO_ERR)
                        {
                            goto ret;
                        }
                        /* Add Constant Name */
                        if ((ErrCode = Insert_Line(&ParseCtx_ptr->InsertTbl, end, temp1)) != NO_ERR)
                        {
                            goto ret;
                        }
                        if ((ErrCode = Add_Insertion(&ParseCtx_ptr->InsertTbl, end2,
                            ADDED_TOOL_INFO_STRING "       ")) != NO_ERR)
                        {
                            goto ret;
                        }
                        if ((ErrCode = Insert_Line(&ParseCtx_ptr->InsertTbl, end, temp1)) != NO_ERR)
                        {
                            goto ret;
                        }
                        if ((ErrCode = Insert_Line(&ParseCtx_ptr->InsertTbl, end,
                            ADDED_TOOL_INFO_STRING "#endif")) != NO_ERR)
                        {
                            goto ret;
                        }
                    }
                }
                /* Advance (after '=') */
                ptr = ptr2 + 1;
            }
            else
            { /* Yes */
                /* Advance (after '==') */
                ptr = ptr2 + 2;
            }
        }
    }

    /* At Least one 'const' was Found? */
    if ( const_data_names_size != 0 )
    { /* Yes */
        if ( ( ErrCode = Insert_Line( &ParseCtx_ptr->InsertTbl, end, ADDED_TOOL_INFO_STRING "+0;}" ) ) != NO_ERR )
        {
            goto ret;
        }
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_PROM
 *-------------------------------------------------------------------*/
static TOOL_ERROR Instrument_PROM(
    Parse_Context_def* ParseCtx_ptr
)
{
    TOOL_ERROR ErrCode = NO_ERR;

#define FUNCTION_RETURN_TYPE "int"
#undef FUNCTION_NAME_PREFIX
#define FUNCTION_NAME_PREFIX PROM_SIZE_FUNC_PREFIX_STRING "_Func( "
#undef FUNCTION_NAME_LENGTH
#define FUNCTION_NAME_LENGTH 60
#undef FUNCTION_START
#define FUNCTION_START " ) { return"
#define FUNCTION_END "; }"

    char* end;
    char func_name[FUNCTION_NAME_LENGTH];

    char func_string[STRING_LENGTH(ADDED_TOOL_INFO_STRING) +
        STRING_LENGTH(FUNCTION_RETURN_TYPE) + 1 +               /* +1 for Space*/
        STRING_LENGTH(FUNCTION_NAME_PREFIX) +
        FUNCTION_NAME_LENGTH +
        STRING_LENGTH(FUNCTION_START) +
        10 +                                                    /* +10 for the number of digits in PROM Size value */
        STRING_LENGTH(FUNCTION_END) +
        1];                                                     /* +1 for NUL*/

    /* Build the string */
    strncpy(func_name, ParseCtx_ptr->File.Name, FUNCTION_NAME_LENGTH);
    strcpy(Fix_FileName(func_name), "\0");
    sprintf(func_string, "%s int PROM_Size_Func( %s ) { return %d; }", ADDED_TOOL_INFO_STRING, func_name, ParseCtx_ptr->PROMSize);

    /* Find suitable location at the end of file to insert PROM Size Counting Instrumentation */
    /* Start at End */
    end = ParseCtx_ptr->File.Data + ParseCtx_ptr->File.Size;
    do
    {
        end--;
    } while (IS_EOL_CHAR(PREV_CHAR(end)));      /* File Ends with only one CR/LF Combination */

    /* Insert PROM Size Counting Instrumentation */
    if ((ErrCode = Insert_Line(&ParseCtx_ptr->InsertTbl, end, func_string)) != NO_ERR)
    {
        goto ret;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Instrument_Const_Data_PROM_Table
 *-------------------------------------------------------------------*/

TOOL_ERROR Instrument_Const_Data_PROM_Table(
    Parse_Context_def* ParseCtx_ptr,
    T_FILE_BOOK file_book[],
    int nRecords
)
{
    TOOL_ERROR ErrCode = NO_ERR;

    char* ptr, *start = NULL, *ptr_tmp;
    char** list_Const_Data_Files = NULL, **array_of_insertions = NULL;
    int nConst_Data_Files = 0, dummy_int;
    Parse_Tbl_def* ParseTbl_ptr;
    Insert_Tbl_def* InsertTbl_ptr;
    int i, j, prom_size = 0, prom_size_dir = 0;
    char tmp_str[MAX_PATH + 100];  /* 100 is added to account for the text preceding function name */
    char cnst_data_size_func_name[MAX_PATH] = "", dir_name[MAX_PATH];

    if (nRecords == 0)
    {
        return ErrCode;
    }

    /* Get Parse Table Address and Insert Table Address */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;
    InsertTbl_ptr = &ParseCtx_ptr->InsertTbl;

    /* Find the End of the Last Contiguous Preprocessor Directive Block (#include) */
    ptr = Find_End_Preproc_Block(NULL, ParseTbl_ptr);

    /* Insert #ifdef WMOPS */
    strcpy(tmp_str, "/*AddedByWMC_Tool*/#ifdef WMOPS");
    if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
    {
        goto ret;
    }

    /* Process all records enetered on the command line */
    list_Const_Data_Files = NULL;
    for (i = 0; i < nRecords; i++)
    {
        prom_size_dir = 0;
        list_Const_Data_Files = NULL;
        nConst_Data_Files = 0;

        /* Parse all files in the record -> calculate PROM size and get Const_Data_Size_Func() name */
        for (j = 0; j < file_book[i].nFiles; j++)
        {
            /* extract PROM size of the current file and update the total PROM size */
            strcpy(tmp_str, file_book[i].pathname);
            strcat(tmp_str, "/");
            strcat(tmp_str, file_book[i].file_list[j]);
            prom_size = Extract_Value_From_File(tmp_str, "PROM_Size_Func(");
            if (prom_size > 0)
            {
                prom_size_dir += prom_size;
            }

            /* extract Const_Data_Size_Func() name from the current file */
            Extract_String_From_File(tmp_str, "Const_Data_Size_Func(", cnst_data_size_func_name);
            if (strcmp(cnst_data_size_func_name, "") != 0)
            {
                /* add to list, if present */
                list_Const_Data_Files = add_file_to_list(cnst_data_size_func_name, list_Const_Data_Files, &nConst_Data_Files);
            }
        }

        /* insert auxiliary Get_Const_Data_Size_xxx() function in case we found more than one Const_Data_Size_Func() */
        if (nConst_Data_Files > 1)
        {
            /* make string from the command-line spec -> replace '.' and '/' with '_' and discard '*' */
            strcpy(dir_name, file_book[i].cmd_line_spec);
            ptr_tmp = dir_name;
            for (j = 0; j < (int)strlen(dir_name); j++)
            {
                if (dir_name[j] == '*')
                {
                    continue;
                }
                else if ( !IS_ALPHA_CHAR(dir_name[j]) && !IS_DIGIT_CHAR(dir_name[j]) )
                {
                    *ptr_tmp++ = '_';
                }
                else
                {
                    *ptr_tmp++ = dir_name[j];
                }
            }
            *ptr_tmp = NUL_CHAR;
            
            /* insert function declaration */
            sprintf(tmp_str, "/*AddedByWMC_Tool*/static int Get_Const_Data_Size_%s(void)", dir_name);

            if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
            {
                goto ret;
            }

            /* insert function body*/
            strcpy(tmp_str, "/*AddedByWMC_Tool*/{");
            if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
            {
                goto ret;
            }

            strcpy(tmp_str, "/*AddedByWMC_Tool*/    int total_size = 0, sz;");
            if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
            {
                goto ret;
            }

            /* insert Get_Const_Data_Size() for each entry */
            for (j = 0; j < nConst_Data_Files; j++)
            {
                sprintf(tmp_str, "/*AddedByWMC_Tool*/    Get_Const_Data_Size(%s, &sz); total_size += sz;", list_Const_Data_Files[j]);
                if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
                {
                    goto ret;
                }
            }

            /* insert return string */
            sprintf(tmp_str, "/*AddedByWMC_Tool*/    return total_size;");
            if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
            {
                goto ret;
            }

            /* close the function body */
            strcpy(tmp_str, "/*AddedByWMC_Tool*/}");
            if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
            {
                goto ret;
            }

            /* generate entry string for the table */
            sprintf(tmp_str, "/*AddedByWMC_Tool*/    { \"%s\", %d, Get_Const_Data_Size_%s },", file_book[i].cmd_line_spec, prom_size_dir, dir_name);

            /* add new entry to the array of entries */
            array_of_insertions = add_file_to_list(tmp_str, array_of_insertions, &dummy_int);

        }
        else 
        {
            if (nConst_Data_Files == 1)
            {
                /* insert extern function declaration */
                sprintf(tmp_str, "/*AddedByWMC_Tool*/extern int Const_Data_Size_%s(void);", list_Const_Data_Files[0]);

                if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
                {
                    goto ret;
                }

                /* generate entry string for the table */
                sprintf(tmp_str, "/*AddedByWMC_Tool*/    { \"%s\", %d, Const_Data_Size_%s },", file_book[i].cmd_line_spec, prom_size_dir, list_Const_Data_Files[0]);
            }
            else
            {
                /* generate entry string for the table */
                sprintf(tmp_str, "/*AddedByWMC_Tool*/    { \"%s\", %d, NULL },", file_book[i].cmd_line_spec, prom_size_dir);
            }

            /* add new entry to the array of entries */
            array_of_insertions = add_file_to_list(tmp_str, array_of_insertions, &dummy_int);
        }

        /* de-allocate list */
        list_Const_Data_Files = free_list_of_strings(list_Const_Data_Files, nConst_Data_Files);
    }

    /* Insert Const_Data_PROM_Table[] = {...} */
    strcpy(tmp_str, "/*AddedByWMC_Tool*/ROM_Size_Lookup_Table Const_Data_PROM_Table[] =");
    if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
    {
        goto ret;
    }

    strcpy(tmp_str, "/*AddedByWMC_Tool*/{");
    if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
    {
        goto ret;
    }

    /* insert all entry strings into the table */
    for (i = 0; i < nRecords; i++)
    {
        if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, array_of_insertions[i])) != NO_ERR)
        {
            goto ret;
        }
    }

    /* terminate the table */
    sprintf(tmp_str, "/*AddedByWMC_Tool*/    { \"\", -1, NULL }");
    if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
    {
        goto ret;
    }

    strcpy(tmp_str, "/*AddedByWMC_Tool*/};");
    if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
    {
        goto ret;
    }

    /* insert #endif */
    strcpy(tmp_str, "/*AddedByWMC_Tool*/#endif");
    if ((ErrCode = Insert_Line(InsertTbl_ptr, ptr, tmp_str)) != NO_ERR)
    {
        goto ret;
    }

    /* Locate the print_mem() function and write 'Const_Data_PROM_Table' into it */
    start = ParseCtx_ptr->File.Data;
    if ((start = Find_String(start, "print_mem", ParseTbl_ptr, ITEM_ANY, ITEM_NONE)) != NULL)
    {
        /* Advance pointer */
        start += strlen("print_mem");

        /* Search for '(' (Stop at EOS or Block start/end) */
        start = Skip_Chars(start, SPACE_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_NONE);

        /* Found it? */
        if (*start == '(')
        { /* Yes */
            /* Insert 'Const_Data_PROM_Table' */
            Add_Insertion(InsertTbl_ptr, start + 1, " Const_Data_PROM_Table ");
        }
    }

    /* Set State Success */
    ParseCtx_ptr->State = INSTRUMENTED;

ret:
    free_list_of_strings(list_Const_Data_Files, nConst_Data_Files);
    free_list_of_strings(array_of_insertions, nRecords);

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Include_Header
 *-------------------------------------------------------------------*/

TOOL_ERROR Include_Header(
    Parse_Context_def* ParseCtx_ptr,
    char **ptr_end_preproc_block
)
{
    char* ptr, *ptr_end;
    int idx;
    TOOL_ERROR ErrCode = NO_ERR;
    Parse_Tbl_def* ParseTbl_ptr;

    /* Get Parse Table Address*/
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Find the End of the Last Contiguous Preprocessor Directive Block (#include) */
    ptr_end = Find_End_Preproc_Block(NULL, ParseTbl_ptr);

    /* if no #include is present in the file set the pointer to the beginning of the file */
    if (ptr_end == NULL)
    {
        ptr_end = ParseCtx_ptr->File.Data;
    }

    /* store the pointer for later use */
    if (ptr_end_preproc_block != NULL)
    {
        *ptr_end_preproc_block = ptr_end;
    }

    /* Check, if #include "wmc_auto.h" is present */
    if ((ptr = Find_String(ParseCtx_ptr->File.Data, WMOPS_LIB_INCLUDE_STRING, ParseTbl_ptr, ITEM_PREPROC_ARGS | ITEM_PREPROC_INC, ITEM_COMMENT, &idx)) == NULL)
    {
        /* Insert #include "wmc_auto.h" */
        if ((ErrCode = Insert_Line(&ParseCtx_ptr->InsertTbl, ptr_end, ADDED_TOOL_INFO_STRING "#include " DQUOTE(WMC_TOOL_INCLUDE_STRING))) != NO_ERR)
        {
            return ErrCode;
        }
    }

    return NO_ERR;
}

/*-------------------------------------------------------------------*
 * Instrument
 *-------------------------------------------------------------------*/

TOOL_ERROR Instrument(
    Parse_Context_def *ParseCtx_ptr,
    bool instrument_ROM )
{
    TOOL_ERROR ErrCode = NO_ERR;

    /* Check Previous State */
    if ( ParseCtx_ptr->State != DESINSTRUMENTED )
    {
        ErrCode = Internal_Error( __FUNCTION__ );
        goto ret;
    }
    /* Set State Failure (by default) */
    ParseCtx_ptr->State = INS_FAILED;

    int idx;

    char *ptr, *ptr2, *ptr_end_preproc_block;
    char *ptr3;
    char temp[552];
    int nChars, is_function_present, is_cnst_data_present;
    int name_length, max_name_length;
    int name_occ, max_name_occ;
    int align;

    Parse_Rec_def *ParseRec_ptr;
    Parse_Tbl_def *ParseTbl_ptr;
    FctCall_Rec_def *FctCallRec_ptr;
    FctCall_Tbl_def *FctCallTbl_ptr;
    const PROM_Ops_Weights_defs *prom_ops_weights_ptr;

    if ( ( ErrCode = Validate_Keywords( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Erase Function Call Table */
    ParseCtx_ptr->FctCallTbl.Size = 0;

    if ( ( ErrCode = Instrument_Calls( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Find the first function in the file which is not in non-instrumented region */
    for (idx = 0; (idx = Find_Region(NULL, ParseTbl_ptr, ITEM_FUNC_BLOCK | ITEM_FUNC_PROJECT, idx)) >= 0; idx++)
    {
        /* Get Function Block Region Pointer */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        if (Find_Region(ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_INSTRUMENTATION_OFF) < 0)
        {
            break;
        }
    }

    is_function_present = 0;
    if (idx > 0)
    {
        is_function_present = 1;
    }

    /* Check, if there are any const xx[] arrays */
    is_cnst_data_present = 0;
    if ((Find_Identifier(ParseCtx_ptr->File.Data, CONST_STRING, ParseTbl_ptr, ITEM_ANY, ITEM_FUNC_DEF | ITEM_NOT_SEARCHED | ITEM_INSTRUMENTATION_OFF)) != NULL)
    {
        is_cnst_data_present = 1;
    }

    /* Found something to instrument? */
    if (is_function_present || is_cnst_data_present)
    { /* Yes */

        /* Include "wmc_auto.h", if not present */
        if ((ErrCode = Include_Header(ParseCtx_ptr, &ptr_end_preproc_block)) != NO_ERR)
        {
            goto ret;
        }

        /* Get Function Call Table Address (for clarity) */
        FctCallTbl_ptr = &ParseCtx_ptr->FctCallTbl;

        if (FctCallTbl_ptr->Size > 0)       /* VM: added to avoid running Sort_FctCall_Table(FctCall_Tbl_def*) when FctCallTbl_ptr->Data is uninitialized */
        {
            /* Sort Table (to Have Ordered Function Names) */
            Sort_FctCall_Table(FctCallTbl_ptr);

            /* Reset Max Function Name Length */
            max_name_length = 0;

            /* Reset # of Occurances */
            max_name_occ = 0;

            /* Start at Beginning */
            FctCallRec_ptr = FctCallTbl_ptr->Data;

            /* Process all Project Function Calls */
            for (idx = 0; idx < FctCallTbl_ptr->Size; idx++)
            {
                /* Get Max Name Lenght */
                name_length = FctCallRec_ptr->NameLength;
                if (max_name_length < name_length)
                    max_name_length = name_length;

                /* Get Max Name Occurances */
                name_occ = FctCallRec_ptr->NameOcc;
                if (max_name_occ < name_occ)
                    max_name_occ = name_occ;

                /* Advance */
                FctCallRec_ptr++;
            }

            align = (int)strlen(Make_Prj_Fct_Call_Instrumentation_String(max_name_occ));

            /* Start at Beginning */
            FctCallRec_ptr = FctCallTbl_ptr->Data;

            /* Process all Project Function Calls */
            for (idx = 0; idx < FctCallTbl_ptr->Size; idx++)
            {
                /* Write to Buffer */
                nChars = snprintf(temp, sizeof(temp), ADDED_TOOL_INFO_STRING "#define %*s%-*s %s(%s,%i)",
                    max_name_length, memstr(FctCallRec_ptr->Ptr, FctCallRec_ptr->NameLength),
                    align, Make_Prj_Fct_Call_Instrumentation_String(FctCallRec_ptr->NameOcc),
                    FUNC_COUNTING_MACRO_STRING,
                    memstr(FctCallRec_ptr->Ptr, FctCallRec_ptr->NameLength),
                    FctCallRec_ptr->nArgs);

                /* All Written? */
                if (nChars < 0 || nChars >= (int)(sizeof(temp)))
                { /* No */
                    ErrCode = Internal_Error(__FUNCTION__);
                    goto ret;
                }

                /* Insert Macro for Function Call */
                if ((ErrCode = Insert_Line(&ParseCtx_ptr->InsertTbl, ptr_end_preproc_block, temp)) != NO_ERR)
                {
                    goto ret;
                }

                /* Advance */
                FctCallRec_ptr++;
            }
        }
        /*  Moved before 'Instrument_Operators' to Fix Instrumentation Problem \*/
        /*                     $(...) inserted on same Line as #pragma message for Skipped Region */
    }

    if ( ( ErrCode = Instrument_Operators( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    if ( ( ErrCode = Instrument_Keywords( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    if ( instrument_ROM && is_cnst_data_present)
    {
        if ( ( ErrCode = Instrument_ROM( ParseCtx_ptr) ) != NO_ERR )
        {
            goto ret;
        }
    }

    /* Get PROM Ops Weights (for clarity) */
    prom_ops_weights_ptr = &PROM_Ops_Weights[ParseCtx_ptr->PROMOpsWeightsSet];

    /* Find Manual Instrumentation Macros and Count their PROM size */
    for (idx = 0; (idx = Find_Region(NULL, ParseTbl_ptr, ITEM_CALL | ITEM_FUNC_COUNTERS_MAN, idx)) >= 0; idx++)
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Start Inside Macro Arguments */
        ptr = ParseRec_ptr[1].item_start + 1; /* +1 to Skip '('*/
        /* Skip Blanks to the Right */
        ptr = Skip_Chars(ptr, BLANK_CHARS, ParseTbl_ptr);
        /* By Default, it is assumed that this is a repeated Op and should take only one PROM Location */
        name_occ = 1;
        /* Digit Character or Leading Sign? */
        if (IS_DIGIT_CHAR(*ptr) || *ptr == '-' || *ptr == '+')
        { /* Yes */
            /* Preserve Location (In case of an Error) */
            ptr2 = ptr;
            /* Skip all Digits */
            while (ptr2++, IS_DIGIT_CHAR(*ptr2))
                ;
            /* Skip Blanks to the Right */
            ptr2 = Skip_Chars(ptr2, BLANK_CHARS, ParseTbl_ptr);
            /* Reached End of Manual Macro Params? */
            if (ptr2 == ParseRec_ptr[1].item_end - 1)
            { /* Yes */
                /* Pure Number, Take that */
                name_occ = atoi(ptr);
            }
            /* No, then it is assumed that this is an expression (probably for a repeated Op)
                   and that should take only one PROM Location */
        }

        /* Invalid or Missing Number? */
        if (name_occ < 0 || ptr == ParseRec_ptr[1].item_end - 1)
        { /* Yes */
            ErrCode = ERR_COUNTING_MACRO_INVALID_NUMBER;
            Error("%s Invalid Number for Manual Counting Macro", ErrCode, Make_Position_String(ptr, temp), *ptr);
            goto ret;
        }

        /* Get Name */
        ptr = ParseRec_ptr->item_start;

        /* Calc Weight */
        if (memwordcmp(ptr, "ABS"))
            /* Count Program Memory */
            abses += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->abs;
        else if (memwordcmp(ptr, "BRANCH"))
            /* Count Program Memory */
            jumps += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->branch;
        else if (memwordcmp(ptr, "TEST"))
            /* Count Program Memory */
            logicals += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->branch;
        else if (memwordcmp(ptr, "SQRT TRANS POWER LOG"))
            /* Count Program Memory */
            transes += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->trans;
        else if (memwordcmp(ptr, "LOOP"))
            /* Count Program Memory */
            loops += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->loop;
        else if (memwordcmp(ptr, "FUNC"))
            /* Count Program Memory */
            funcs++, ParseCtx_ptr->PROMSize += 1 * prom_ops_weights_ptr->func,
            args += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->args;
        else if (memwordcmp(ptr, "ADD"))
            /* Count Program Memory */
            adds += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->add;
        else if (memwordcmp(ptr, "MULT"))
            /* Count Program Memory */
            mults += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->mult;
        else if (memwordcmp(ptr, "MAC"))
            /* Count Program Memory */
            macs += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->mac;
        else if (memwordcmp(ptr, "MOVE"))
            /* Count Program Memory */
            moves += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->move;
        else if (memwordcmp(ptr, "STORE"))
            /* Count Program Memory */
            moves += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->store;
        else if (memwordcmp(ptr, "LOGIC"))
            /* Count Program Memory */
            logics += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->logic;
        else if (memwordcmp(ptr, "SHIFT"))
            /* Count Program Memory */
            shifts += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->shift;
        else if (memwordcmp(ptr, "DIV"))
            /* Count Program Memory */
            divs += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->div;
        else if (memwordcmp(ptr, "INDIRECT"))
            /* Count Program Memory */
            indirects += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->indirect;
        else if (memwordcmp(ptr, "PTR_INIT"))
            /* Count Program Memory */
            ptr_inits += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->ptr_init;
        else if (memwordcmp(ptr, "MISC"))
            /* Count Program Memory */
            miscs += name_occ, ParseCtx_ptr->PROMSize += name_occ * prom_ops_weights_ptr->misc;
    }

    /* Find BASOP Functions and Count their PROM size */
    for (idx = 0; (idx = Find_Region(NULL, ParseTbl_ptr, ITEM_CALL | ITEM_FUNC_BASOP, idx)) >= 0; idx++)
    {
        /* By default it is assumed that all BASOP functions take one PROM word */
        ParseCtx_ptr->PROMSize++;
    }

    /* Insert PROM_Size_Func() function */
    //if (is_function_present)
    if (ParseCtx_ptr->PROMSize > 0)
    {
        if ((ErrCode = Instrument_PROM(ParseCtx_ptr)) != NO_ERR)
        {
            goto ret;
        }
    }

    /* Insert "func_start_" and "return_" in all functions */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_FUNC_BLOCK | ITEM_FUNC_PROJECT, idx ) ) >= 0; idx++ )
    {
        /* Get Function Block Region Pointer */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];

        if ( Find_Region(ParseRec_ptr->item_start, ParseTbl_ptr, ITEM_INSTRUMENTATION_OFF, idx) >= 0 )
        {
            /* Function in Skipped Region -> Do not Instrument */
            continue;
        }

        /* Get Block Start */
        ptr = ParseRec_ptr->item_start + 1;
        /* Get Block End */
        ptr2 = ParseRec_ptr->item_end - 1;

        /* Add Function Start Macro */
        strcpy( temp, " " FUNC_START_STRING " " );
        /* Skip Deleted Chars */
        while ( *ptr == DEL_CHAR )
            ptr++;
        /* End Line Char? */
        if ( IS_EOL_CHAR( *ptr ) )
        { /* Yes */
            /* Remove Trailing Space */
            temp[STRING_LENGTH( FUNC_START_STRING ) + 1] = NUL_CHAR;
        }
        if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, ptr, temp ) ) != NO_ERR )
        {
            goto ret;
        }
        ptr3 = NULL;
        /* Find Last 'return' keyword in this Function (this is used for the Function Leave Mechanism) */
        while ( ( ptr = Find_String( memstr( ptr, ptr2 ), RETURN_KW_STRING, ParseTbl_ptr, ITEM_KEYWORD_RETURN ) ) != NULL )
        {
            /* Copy and Advance */
            ptr3 = ptr++;
        }
        /* Found One? */
        if ( ptr3 != NULL )
        { /* Yes */
            /* Advance to EOS (This cannot Fail, Trapped at Keyword Parsing) */
            ptr3 = Goto_EOS( ptr3, ParseTbl_ptr );
            /* Advance */
            ptr3 = Skip_Chars( ptr3 + 1, BLANK_CHARS, ParseTbl_ptr );
            /* Reached End of Function? */
            if ( ptr3 != ptr2 )
            { /* No */
                /* Closing Brace? */
                if ( *ptr3 == '}' )
                { /* Yes */
                    /* Go to Opening Brace (This cannot Fail) */
                    ptr = Goto_Match( ptr3, ParseTbl_ptr );
                    /* Move Back */
                    ptr = Skip_Chars( ptr - 1, BLANK_CHARS, ParseTbl_ptr, ITEM_ANY, ITEM_MOVED_OVER, BACKWARDS );
                    /* Is it an 'else' or Opening Brace of Function Block? */
                    if ( Find_Region( ptr, ParseTbl_ptr, ITEM_KEYWORD_ELSE ) >= 0 || ptr == ParseRec_ptr->item_start )
                    { /* Yes */
                        /* We should check that there is also a 'return*/
                        /* in the first block of the if.*/
                        /* Move After Closing Brace */
                        ptr3 = Skip_Chars( ptr3 + 1, BLANK_CHARS, ParseTbl_ptr );
                    }
                }
            }
 
        }

        /* Function ends with a 'return' keyword? */
        if ( ptr3 != ptr2 )
        { /* No */
            /* Get Block Start */
            ptr = ParseRec_ptr->item_start + 1;
            ptr3 = NULL;
            /* Find Last 'exit' Function Call in this Function */
            while ( ( ptr = Find_String( memstr( ptr, ptr2 ), "exit", ParseTbl_ptr, ITEM_SKIPPED | ITEM_CALL | ITEM_FUNC_SYSTEM ) ) != NULL )
            {
                /* Copy and Advance */
                ptr3 = ptr++;
            }
            /* Found One? */
            if ( ptr3 != NULL )
            { /* Yes */
                /* Advance to EOS */
                ptr = Goto_EOS( ptr3, ParseTbl_ptr );
                /* Found it? */
                if ( ptr == NULL )
                { /* No */
                    ErrCode = Expected_EOS( ptr3 );
                    goto ret;
                }
                /* Advance (After EOS) */
                ptr3 = Skip_Chars( ptr + 1, BLANK_CHARS, ParseTbl_ptr );
            }
        }

        /* Function ends with a 'return' keyword? */
        if ( ptr3 != ptr2 )
        {   /* No */
            /****************************************
            * Need to add one Manually to the Code *
            ****************************************/
            /* Set Default Insertion Point (Right at Function End Brace) */
            ptr = ptr2;
            /* Find Better Insertion Point */
            while ( --ptr2, IS_BLANK_CHAR( *ptr2 ) )
            {
                /* End of Line Character? */
                if ( IS_EOL_CHAR( *ptr2 ) )
                { /* Yes */
                    /* Get Column Number of Block End Brace */
                    nChars = strcno( ptr );
                    /* Search for another closing brace */
                    if ( ( ptr3 = Goto_Chars( ptr - 1, "{}", ParseTbl_ptr, ITEM_ANY, ITEM_NOT_SEARCHED, BACKWARDS ) ) != NULL )
                    {
                        /* Found a Closing Brace? */
                        if ( *ptr3 == '}' )
                        { /* Yes */
                            /* Get Column Number of Block End Brace */
                            nChars = strcno( ptr3 ) - TAB_SIZE;
                        }
                    }

                    /* Go Back one Character */
                    ptr = ptr2 - 1;
                    /* is it is a CR+LF? */
                    if ( !IS_EOL_SEQ( ptr ) )
                    { /* No */
                        /* Only one CR/LF Char */
                        ptr++;
                    }

                    /* Add Line Change */
                    if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, ptr, memstr( ptr, ptr2 + 1 ) ) ) != NO_ERR )
                    {
                        goto ret;
                    }

                    /* Add Spaces for Alignement */
                    if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, ptr, " ", ParseCtx_ptr->nChars + TAB_SIZE + nChars - 1 ) ) != NO_ERR )
                    {
                        goto ret;
                    }

                    break;
                }
            }

            /* Add 'return' keyword */
            if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl, ptr, RETURN_KW_STRING WORD_INSTRUMENT_STRING ADDED_TOOL_INFO_STRING ";" ) ) != NO_ERR )
            {
                goto ret;
            }
        }
    }

    /* Set State Success */
    ParseCtx_ptr->State = INSTRUMENTED;

#ifdef DEBUG_PRINT
    Debug_Print( "abses     %i\n", abses );
    Debug_Print( "miscs     %i\n", miscs );
    Debug_Print( "loops     %i\n", loops );
    Debug_Print( "whiles    %i\n", whiles );
    Debug_Print( "jumps x2  %i\n", jumps );
    Debug_Print( "funcs     %i\n", funcs );
    Debug_Print( "args      %i\n", args );
    Debug_Print( "transes   %i\n", transes );
    Debug_Print( "sqrts     %i\n", sqrts );
    Debug_Print( "divs      %i\n", divs );
    Debug_Print( "mults     %i\n", mults );
    Debug_Print( "minmax    %i\n", minmaxs );
    Debug_Print( "logicalx2 %i\n", logicals );
    Debug_Print( "adds      %i\n", adds );
    Debug_Print( "cases     %i\n", cases );
    Debug_Print( "moves     %i\n", moves );
    Debug_Print( "indirects %i\n", indirects );
    Debug_Print( "ptr inits %i\n", ptr_inits );
    Debug_Print( "shifts    %i\n", shifts );
    Debug_Print( "logics    %i\n", logics );
    Debug_Print( "macs      %i\n", macs );
#endif
ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Finalize
 *-------------------------------------------------------------------*/
TOOL_ERROR Finalize(
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;
    /* Check Previous State */
    if ( ParseCtx_ptr->State != DESINSTRUMENTED && ParseCtx_ptr->State != INSTRUMENTED )
    {
        ErrCode = Internal_Error( __FUNCTION__ );
        goto ret;
    }
    /* Set State Failure (by default) */
    ParseCtx_ptr->State = FINAL_FAILED;

    /* Sort Insert Table */
    Sort_Insert_Table( &ParseCtx_ptr->InsertTbl );

    /* Remove Superfluous Spaces */
    if ( ( ErrCode = Remove_Superfluous_Spaces( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Add Last Insertion
       (This is Required by wmc_tool::Write_File, Do not Remove!) */
    if ( ( ErrCode = Add_Insertion( &ParseCtx_ptr->InsertTbl,
                                    ParseCtx_ptr->File.Data + ParseCtx_ptr->File.Size,
                                    "" ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Set State Success */
    ParseCtx_ptr->State = INSTRUMENTED;

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Check_Name
 *-------------------------------------------------------------------*/
bool Check_Name( Parse_Rec_def **NameRecTbl_ptr,
                 Parse_Rec_def *CurRecTbl_ptr,
                 int MaxSize )
{
    char *item_start;
    size_t item_length;
    char *start;
    size_t length;
    Parse_Rec_def *NameRec_ptr;

    /* Get Item Start & Length */
    item_start = CurRecTbl_ptr->item_start;
    item_length = CurRecTbl_ptr->item_end - item_start;

    /* Search for Name in Table? */
    while ( *NameRecTbl_ptr != NULL )
    {
        NameRec_ptr = *NameRecTbl_ptr++;
        /* Get Item Start & Length */
        start = NameRec_ptr->item_start;
        length = NameRec_ptr->item_end - start;
        /* Same Length? */
        if ( length == item_length )
        { /* Yes */
            /* Same Name? */
            if ( memcmp( start, item_start, length ) == 0 )
            { /* Yes */
                return true;
            }
        }

        MaxSize--;
    }

    /* Not Found, Add Name to Table (If Enough Room) */
    if ( MaxSize > 1 ) /* > 1 to Leave Room for NULL*/
    {
        *NameRecTbl_ptr++ = CurRecTbl_ptr;
        *NameRecTbl_ptr = NULL;
    }

    return false;
}

/*-------------------------------------------------------------------*
 * Print_Words
 *-------------------------------------------------------------------*/
static void Print_Words( const char *words, bool ds = false, int margin = 4 )
{
    char temp[MAX_CHARS_PER_LINE + 1]; /* +1 for NUL*/

    char *ptr, *ptr_words = (char *)words;
    size_t str_length, lin_length, mcib;

    /* Set Maximum Characters in Buffer */
    mcib = (size_t)((int)MAX_CHARS_PER_LINE - margin);

    /* Blank Margin */
    memset( temp, ' ', margin );

    for ( ;; )
    {
        /* Advance (Skip Spaces) */
        ptr_words += strspn( ptr_words, SPACE_CHARS );
        /* Stop on String End */
        if ( *ptr_words == NUL_CHAR )
            break;
        /* Save Start */
        ptr = ptr_words;
        /* Find String End or Separator */
        for ( ;; )
        {
            /* Go to Space or NUL Char */
            ptr_words += strcspn(ptr_words, SPACE_CHARS );
            /* Continue until String End or Space Separator */
            if ( ds || *ptr_words == NUL_CHAR || ( IS_SPACE_CHAR(ptr_words[1] ) || ptr_words[1] == NUL_CHAR ) )
                break;
            /* Advance (Skip Spaces) */
            ptr_words += strspn(ptr_words, SPACE_CHARS );
        }
        /* Calculate Length to Print */
        str_length = ptr_words - ptr;
        /* Print */
        do
        {
            /* Get Remaining Length to Print */
            lin_length = str_length;
            /* Set Maximum to Store */
            if ( lin_length > mcib )
                lin_length = mcib;
            /* Copy to Buffer */
            memcpy( temp + margin, ptr, lin_length );
            /* Terminate String */
            temp[margin + lin_length] = NUL_CHAR;
            /* Print */
            fprintf(stdout, "%s", temp);
            /* Advance */
            ptr += lin_length;
            /* Update Remaining Length */
            str_length -= lin_length;
        } while ( str_length > 0 );
        /* Print newline */
        fprintf(stdout, "\n" );
    }
}

/*-------------------------------------------------------------------*
 * Print_Information
 *-------------------------------------------------------------------*/
void Print_Information( void )
{
    int i;

    fprintf(stdout, "\n  Manual Macros Removed during Desintrumentation:\n" );
    Print_Words( MANUAL_COUNTING_MACROS_STRING );
    fprintf(stdout, "\n  BASOP Functions that are not Instrumented:\n" );
    Print_Words( BASOP_FUNCTS_STRING, true );
    fprintf(stdout, "\n  WMOPS Library Functions that are not Instrumented:\n");
    Print_Words(WMOPS_FUNCTS_STRING, true);
    fprintf(stdout, "\n  System Functions that are not Instrumented:\n" );
    Print_Words( SYSTEM_FUNCTS_STRING );
    fprintf(stdout, "\n  System Functions that are Instrumented:\n" );
    Print_Words( SYSTEM_ALLOC_FUNCTS_STRING );

    fprintf(stdout, "\n  Preprocessor Directives that are Ignored:\n" );
    for (i = 0; Conditionnal_Directives[i] != NULL; i++)
    {
        fprintf(stdout, "    %c%s\n", PREPROC_CHAR, Conditionnal_Directives[i]);
    }
    fprintf(stdout, "    %c%s\n", PREPROC_CHAR, Undefine_Directive );
    for (i = 0; Other_Directives[i] != NULL; i++)
    {
        fprintf(stdout, "    %c%s\n", PREPROC_CHAR, Other_Directives[i]);
    }

    fprintf(stdout, "\n  Math Functions that are Instrumented:\n" );
    Print_Words( MATH_FUNCTS_STRING );

    fprintf(stdout, "\n  Keywords that are Instrumented:\n" );
    for ( i = 0; i < (int)(nitems( keywords )); i++ )
    {
        if ( !( keywords[i].kw_type & ITEM_SKIPPED ) )
        {
            Print_Words( keywords[i].kw_name );
        }
    }

    fprintf(stdout, "\n  Keywords that are not Instrumented:\n" );
    for ( i = 0; i < (int)(nitems( keywords )); i++ )
    {
        if ( keywords[i].kw_type & ITEM_SKIPPED )
        {
            Print_Words( keywords[i].kw_name );
        }
    }

    fprintf(stdout, "\n  Recognized Data Types:\n" );
    Print_Words( TYPES_STRING );

    fprintf(stdout, "\n  Macros used to Instrument: \n" );
    fprintf(stdout, "    switch/case: %s(n)\n", AUTO_CASE_COUNTING_MACRO_STRING );
    fprintf(stdout, "    logical operators: %s\n", LOGICAL_INSTRUMENT_STRING );
    fprintf(stdout, "    ternary operators: %s\n", TERNARY_INSTRUMENT_STRING );
    fprintf(stdout, "    all other operators: %s(\"ops_string\")\n", AUTO_DEFAULT_COUNTING_MACRO_STRING );

    fprintf(stdout, "\n  Include File Required in Instrumented Source Files:\n" );
    fprintf(stdout, "    %s\n", WMOPS_LIB_INCLUDE_STRING );

    fprintf(stdout, "\n  Skip Instrumentation:\n" );
    fprintf(stdout, "    Begin Uninstrumented Segment: #define " WMC_TOOL_SKIP_STRING "\n" );
    fprintf(stdout, "    End of Uninstrumented Segment: #undef  " WMC_TOOL_SKIP_STRING "\n" );
    fprintf(stdout, "\n" );
}

/*-------------------------------------------------------------------*
 * Print_Warnings
 *-------------------------------------------------------------------*/
int Print_Warnings( Parse_Context_def *ParseCtx_ptr,
                    unsigned int tool_warning_mask,
                    bool warn_once,
                    bool di_only )
{
#define SKIP_ON_DISABLED_WARNING( w )           \
    if ( tool_warning_mask & 1L << ( (w) -1 ) ) \
    continue
#define DISABLE_WARNING( w ) ( tool_warning_mask |= 1L << ( (w) -1 ) )

#define WARN_MSG_LINE_COMMENT                    "%s Line Comment Detected."
#define WARN_MSG_EMPTY_DEF                       "%s Empty Definition " DQUOTE( "%s" ) "."
#define WARN_MSG_INITIALIZATION_SKIPPED          "%s Data Initialization was not Instrumented."
#define WARN_MSG_SUPERFLUOUS_EOS_REMOVED         "%s Superfluous ';' Removed."
#define WARN_MSG_CODE_BLOCK_DATA_DECL            "%s Data Declaration in Code Block."
#define WARN_MSG_FUNC_SKIPPED                    "%s Function " DQUOTE( "%s" ) " was not Instrumented."
#define WARN_MSG_AVOID_KEYWORD                   "%s Please Avoid Using " DQUOTE( "%s" ) "."
#define WARN_MSG_PARENTHESES_ADDED               "%s () Added for Multiple Values in Expression."
#define WARN_MSG_UNSPECIFIED                     "%s Unspecified Warning."
#define WARN_MSG_LOCAL_MACRO_SKIPPED             "%s Macro Call " DQUOTE( "%s" ) " was not Instrumented."
#define WARN_MSG_AMBIGUOUS_CONTEXT_4_OP          "%s Ambiguous Context for Operator '%c', '%s' choosen."
#define WARN_MSG_DIVIDE_AS_MULTIPLY              "%s Division by a Constant Treated as a Multiply."
#define WARN_MSG_MODULO_AS_BITAND                "%s Modulo with a Power of 2 Treated as a 'Bit And'."
#define WARN_MSG_FOR_COMPLEX_CONDITION           "%s 'for' has complex condition, use 'while' instead."
#define WARN_MSG_SUPERFLUOUS_PARENTHESES_REMOVED "%s Superfluous '()' Removed."

    int nwarnings;
    int idx, idx2;
    Parse_Rec_def *ParseRec_ptr, *ParseRec_ptr2;
    char *ptr;
    TOOL_WARNING WarnCode;
    bool warning_disable;
    bool is_goto, warned_goto, warned_continue;
    char temp[MAX_CHARS_PER_LINE + 1 + 1]; /* +1+1 for '\n' and NUL*/

    Parse_Rec_def *NameRecTbl[200];

    Parse_Tbl_def *ParseTbl_ptr;

    /* Get Parse Table Address (for clarity) */
    ParseTbl_ptr = &ParseCtx_ptr->ParseTbl;

    /* Sort Table (will make warnings appear Line Ordered) */
    Sort_Parse_Table( ParseTbl_ptr );
#ifdef DEBUG_PRINT
    /* Print Accumulated Debug Messages */
    Debug_Print( NULL );
#endif
    /* Initialize # of Warnings */
    nwarnings = 0;
    /* Initialize Name Indexes Table */
    NameRecTbl[0] = NULL;
    /* Initialize Warning Flags */
    warned_goto = warned_continue = false;

    /* DesInstrument Only? */
    if ( di_only )
    { /* Yes */
        /* No Warning for Skipped Functions */
        DISABLE_WARNING( WARN_FUNC_SKIPPED );
    }

    /* Search for Warning Regions */
    for ( idx = 0; ( idx = Find_Region( NULL, ParseTbl_ptr, ITEM_WARNING, idx ) ) >= 0; idx++ )
    {
        /* Get Record */
        ParseRec_ptr = &ParseTbl_ptr->Data[idx];
        /* Get Start of Item */
        ptr = ParseRec_ptr->item_start;

        /* Desintrumented Code? */
        if ( IS_RESERVED_CHAR( *ptr ) )
        { /* Yes */
            /* No Warning Printed */
            continue;
            /* To Fix Warnings for Line Comments that we sometimes put in Constant Data Functions */
        }

        /* Keep Warning (by default) */
        warning_disable = false;
        /* Not goto (by default) */
        is_goto = false;
        /* Print Warning */
        switch ( ParseRec_ptr->item_type & ~( ITEM_WARNING | ITEM_INSTRUMENTED ) )
        {
            /* Avoid Use of '//' for Comments*/
            case ITEM_COMMENT_LINE:
            {
                WarnCode = WARN_LINE_COMMENT;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_LINE_COMMENT, WarnCode,
                               Make_Position_String( ptr, temp ) );
                warning_disable = true;
                break;
            }
            /* Empty Definitions*/
            case ITEM_CSTE_VALUE:
                continue; /* Don'T Warn for it Anymore (#define EMPTY is ok)*/
            case ITEM_MACRO_CODE:
            {
                WarnCode = WARN_EMPTY_DEF;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Get to Definition Name */
                idx2 = Find_Region( NULL, ParseTbl_ptr, ITEM_NAME, idx, BACKWARDS );
                ParseRec_ptr2 = &ParseTbl_ptr->Data[idx2];
                /* Skip Spaces to the Right (this cannot fail) */
                ptr = Skip_Chars( ParseRec_ptr2->item_start, SPACE_CHARS, ParseTbl_ptr, ITEM_DEF );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_EMPTY_DEF, WarnCode,
                               Make_Position_String( ptr, temp ), ptr,
                               memstr( ptr, ParseRec_ptr2->item_end ) );
                break;
            }
            /* Assignation in Data Declaration*/
            case ITEM_SKIPPED | ITEM_DATA_DECL | ITEM_OPERATOR_EQ:
            {
                WarnCode = WARN_INITIALIZATION_SKIPPED;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_INITIALIZATION_SKIPPED, WarnCode,
                               Make_Position_String( ptr, temp ) );
                warning_disable = true;
                break;
            }
            /* Superfluous ';'*/
            case ';':
            {
                WarnCode = WARN_SUPERFLUOUS_EOS_REMOVED;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_SUPERFLUOUS_EOS_REMOVED, WarnCode,
                               Make_Position_String( ptr, temp ) );
                break;
            }
            /* Superfluous '()'*/
            case '(':
            {
                WarnCode = WARN_SUPERFLUOUS_PARENTHESES_REMOVED;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_SUPERFLUOUS_PARENTHESES_REMOVED, WarnCode,
                               Make_Position_String( ptr, temp ) );
                break;
            }
            /* Data Declaration in Sub Block*/
            case ITEM_DATA_DECL_SUB:
            {
                WarnCode = WARN_CODE_BLOCK_DATA_DECL;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_CODE_BLOCK_DATA_DECL, WarnCode,
                               Make_Position_String( ptr, temp ) );
                warning_disable = true;
                break;
            }
            /* Unprocessed Functions*/
            case ITEM_SKIPPED | ITEM_FUNC_NAME | ITEM_FUNC_MATH:
            case ITEM_SKIPPED | ITEM_FUNC_NAME | ITEM_FUNC_SYSTEM:
            {
                WarnCode = WARN_FUNC_SKIPPED;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_FUNC_SKIPPED, WarnCode,
                               Make_Position_String( ptr, temp ),
                               memstr( ptr, ParseRec_ptr->item_end ) );
                break;
            }
            /* 'goto' & 'continue'*/
            case ITEM_KEYWORD_GOTO:
                is_goto = true;
            case ITEM_KEYWORD_CONTINUE:
            {
                WarnCode = WARN_AVOID_KEYWORD;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Warn Only Once? */
                if ( warn_once )
                { /* Yes */
                    /* Already Warned for 'goto' or 'continue'? */
                    if ( (is_goto && warned_goto) || (!is_goto && warned_continue) )
                    { /* Yes */
                        /*  Don't Warn */
                        continue;
                    }
                    /* Set Warning Flags */
                    warned_goto |= is_goto;
                    warned_continue |= !is_goto;
                }
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_AVOID_KEYWORD, WarnCode,
                               Make_Position_String( ptr, temp ),
                               is_goto ? "goto" : "continue" );
                break;
            }
            /* Multiple Condition for Keyword Expression (only 'while' for Now)*/
            case ITEM_KW_EXPR_WHILE: /* do/while can handle multiple initialisation.*/
            {
                WarnCode = WARN_PARENTHESES_ADDED;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_PARENTHESES_ADDED, WarnCode,
                               Make_Position_String( ptr, temp ) );
                break;
            }
            /* Local Macro Not Instrumented*/
            case ITEM_SKIPPED | ITEM_CALL:
            {
                WarnCode = WARN_LOCAL_MACRO_SKIPPED;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                /* Warn Only Once? */
                if ( warn_once && Check_Name( NameRecTbl, ParseRec_ptr, nitems( NameRecTbl ) ) )
                { /* Yes */
                    /* Name Already Warned for */
                    continue;
                }
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_LOCAL_MACRO_SKIPPED, WarnCode,
                               Make_Position_String( ptr, temp ),
                               memstr( ptr, ParseRec_ptr->item_end ) );
                break;
            }
            /* Ambiguity between 'Bit And' and 'Address of'*/
            case ITEM_OPERATOR_AND:
            {
                WarnCode = WARN_AMBIGUOUS_CONTEXT_4_OP;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                Print_Warning( WARN_MSG_AMBIGUOUS_CONTEXT_4_OP, WarnCode,
                               Make_Position_String( ptr, temp ),
                               *ptr, "Bit AND" );
                break;
            }
            /* Ambiguity between 'Mutiply' and 'Dereference'*/
            case ITEM_OPERATOR_MULT:
            {
                WarnCode = WARN_AMBIGUOUS_CONTEXT_4_OP;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                Print_Warning( WARN_MSG_AMBIGUOUS_CONTEXT_4_OP, WarnCode,
                               Make_Position_String( ptr, temp ),
                               *ptr, "Multiply" );
                break;
            }
            /* Division by a Constant Treated as a Multiply*/
            case ITEM_OPERATOR_DIV:
            case ITEM_OPERATOR_DIV | ITEM_OPERATOR_COMPOUND:
            {
                WarnCode = WARN_DIVIDE_AS_MULTIPLY;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                Print_Warning( WARN_MSG_DIVIDE_AS_MULTIPLY, WarnCode,
                               Make_Position_String( ptr, temp ) );
                warning_disable = true;
                break;
            }
            /* Modulo with a Power of 2 Treated as a 'Bit And'*/
            case ITEM_OPERATOR_MOD:
            case ITEM_OPERATOR_MOD | ITEM_OPERATOR_COMPOUND:
            {
                WarnCode = WARN_MODULO_AS_BITAND;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                Print_Warning( WARN_MSG_MODULO_AS_BITAND, WarnCode,
                               Make_Position_String( ptr, temp ) );
                warning_disable = true;
                break;
            }
            /* 'for' Control Expression has Complex Condition*/
            case ITEM_KW_EXPR_FOR:
            {
                WarnCode = WARN_FOR_COMPLEX_CONDITION;
                SKIP_ON_DISABLED_WARNING( WarnCode );
                Print_Warning( WARN_MSG_FOR_COMPLEX_CONDITION, WarnCode,
                               Make_Position_String( ptr, temp ) );
                break;
            }
            default:
            {
                WarnCode = WARN_UNSPECIFIED;
                /* Make & Print Warning */
                Print_Warning( WARN_MSG_UNSPECIFIED, WarnCode,
                               Make_Position_String( ptr, temp ) );
                break;
            }
        }

        /* Disable Warning? */
        if ( warning_disable && warn_once )
        { /* Yes */
            DISABLE_WARNING( WarnCode );
        }

        /* Increment # of Warnings */
        nwarnings++;
    }

    return nwarnings;
}
