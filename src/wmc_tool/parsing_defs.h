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

#ifndef __PARSING_DEFS_H__
#define __PARSING_DEFS_H__

/*-------------------------------------------------------------------*
 * Typedefs
 *-------------------------------------------------------------------*/

#define ITEM_NONE 0x00000000
#define ITEM_ANY  0x00000001

#define ITEM_CLASS     0xFFFFFF00
#define ITEM_SUB_CLASS ( ~ITEM_CLASS )

#define ITEM_COMMENT       0x00000100
#define ITEM_COMMENT_LINE  ( 0x1 + ITEM_COMMENT )
#define ITEM_COMMENT_BLOCK ( 0x2 + ITEM_COMMENT )

#define ITEM_TEXT   0x00000200
#define ITEM_CHAR   ( 0x1 + ITEM_TEXT )
#define ITEM_STRING ( 0x2 + ITEM_TEXT )

#define ITEM_ENCLOSED ( ITEM_COMMENT | ITEM_TEXT )

#define ITEM_FUNC_NAME  0x00000400
#define ITEM_FUNC_PROTO 0x00000800
#define ITEM_FUNC_BLOCK 0x00001000
#define ITEM_FUNC_DEF   ( ITEM_FUNC_NAME | ITEM_FUNC_PROTO | ITEM_FUNC_BLOCK )

#define ITEM_CALL 0x00002000 /* Macro or Function*/

#define ITEM_MACRO_NAME  0x00004000
#define ITEM_MACRO_PROTO 0x00008000
#define ITEM_MACRO_CODE  0x00010000
#define ITEM_MACRO_DEF   ( ITEM_MACRO_NAME | ITEM_MACRO_PROTO | ITEM_MACRO_CODE )

#define ITEM_PREPROC_CMD 0x00020000

/* Attributes for ITEM_PREPROC_CMD & ITEM_PREPROC_ARGS */
#define ITEM_PREPROC_DEF   0x01 /* #define*/
#define ITEM_PREPROC_UNDEF 0x02 /* #undef*/
#define ITEM_PREPROC_COND  0x04 /* #if, #ifdef, #ifndef, #else, #elif and #endif*/
#define ITEM_PREPROC_INC   0x08 /* #include*/
#define ITEM_PREPROC_OTHER 0x10 /* #error, #pragma, #line, ...*/

#define ITEM_PREPROC_ARGS 0x08000000 /* for all except #define*/

#define ITEM_CSTE_NAME  0x00040000
#define ITEM_CSTE_VALUE 0x00080000
#define ITEM_CSTE_DEF   ( ITEM_CSTE_NAME | ITEM_CSTE_VALUE )

#define ITEM_DEF  ( ITEM_MACRO_DEF | ITEM_CSTE_DEF )
#define ITEM_NAME ( ITEM_FUNC_NAME | ITEM_MACRO_NAME | ITEM_CSTE_NAME )

#define ITEM_PREPROC_LINE ( ITEM_PREPROC_CMD | ITEM_DEF | ITEM_PREPROC_ARGS )

#define ITEM_DATA_DECL     0x00100000
#define ITEM_DATA_DECL_1ST ( 0x1 + ITEM_DATA_DECL )
#define ITEM_DATA_DECL_SUB ( 0x2 + ITEM_DATA_DECL )

#define ITEM_OPERATOR            0x00200000
#define ITEM_OPERATOR_MATH       0x08
#define ITEM_OPERATOR_BITWISE    0x10
#define ITEM_OPERATOR_RELATIONAL 0x20
#define ITEM_OPERATOR_LOGICAL    0x40
#define ITEM_OPERATOR_COMPOUND   0x80
#define ITEM_OPERATOR_OTHER      0x00
/* Math */
#define ITEM_OPERATOR_EQ    ( 1 + ITEM_OPERATOR + ITEM_OPERATOR_MATH )
#define ITEM_OPERATOR_MULT  ( 2 + ITEM_OPERATOR + ITEM_OPERATOR_MATH )
#define ITEM_OPERATOR_DIV   ( 3 + ITEM_OPERATOR + ITEM_OPERATOR_MATH )
#define ITEM_OPERATOR_INC   ( 4 + ITEM_OPERATOR + ITEM_OPERATOR_MATH )
#define ITEM_OPERATOR_DEC   ( 5 + ITEM_OPERATOR + ITEM_OPERATOR_MATH )
#define ITEM_OPERATOR_PLUS  ( 6 + ITEM_OPERATOR + ITEM_OPERATOR_MATH )
#define ITEM_OPERATOR_MINUS ( 7 + ITEM_OPERATOR + ITEM_OPERATOR_MATH )
#define ITEM_OPERATOR_ADD   ( ITEM_OPERATOR_PLUS & ITEM_OPERATOR_MINUS )
/* Bitwise */
#define ITEM_OPERATOR_MOD ( 1 + ITEM_OPERATOR + ITEM_OPERATOR_BITWISE )
#define ITEM_OPERATOR_AND ( 2 + ITEM_OPERATOR + ITEM_OPERATOR_BITWISE )
#define ITEM_OPERATOR_XOR ( 3 + ITEM_OPERATOR + ITEM_OPERATOR_BITWISE )
#define ITEM_OPERATOR_OR  ( 4 + ITEM_OPERATOR + ITEM_OPERATOR_BITWISE )
#define ITEM_OPERATOR_NOT ( 5 + ITEM_OPERATOR + ITEM_OPERATOR_BITWISE )
#define ITEM_OPERATOR_SHL ( 6 + ITEM_OPERATOR + ITEM_OPERATOR_BITWISE )
#define ITEM_OPERATOR_SHR ( 7 + ITEM_OPERATOR + ITEM_OPERATOR_BITWISE )
/* Relationnal */
#define ITEM_OPERATOR_EE ( 1 + ITEM_OPERATOR + ITEM_OPERATOR_RELATIONAL )
#define ITEM_OPERATOR_NE ( 2 + ITEM_OPERATOR + ITEM_OPERATOR_RELATIONAL )
#define ITEM_OPERATOR_GT ( 3 + ITEM_OPERATOR + ITEM_OPERATOR_RELATIONAL )
#define ITEM_OPERATOR_GE ( 4 + ITEM_OPERATOR + ITEM_OPERATOR_RELATIONAL )
#define ITEM_OPERATOR_LT ( 5 + ITEM_OPERATOR + ITEM_OPERATOR_RELATIONAL )
#define ITEM_OPERATOR_LE ( 6 + ITEM_OPERATOR + ITEM_OPERATOR_RELATIONAL )
/* Logical */
#define ITEM_OPERATOR_ANDL ( 1 + ITEM_OPERATOR + ITEM_OPERATOR_LOGICAL )
#define ITEM_OPERATOR_ORL  ( 2 + ITEM_OPERATOR + ITEM_OPERATOR_LOGICAL )
#define ITEM_OPERATOR_NOTL ( 3 + ITEM_OPERATOR + ITEM_OPERATOR_LOGICAL )
/* Ternary */
#define ITEM_OPERATOR_TERNARY ( 4 + ITEM_OPERATOR + ITEM_OPERATOR_LOGICAL )
/* Delimiter */
#define ITEM_OPERATOR_DELIMITER ( 1 + ITEM_OPERATOR + ITEM_OPERATOR_OTHER )
/* Memory */
#define ITEM_OPERATOR_ADDR          ( 2 + ITEM_OPERATOR + ITEM_OPERATOR_OTHER )
#define ITEM_OPERATOR_DEREF         ( 3 + ITEM_OPERATOR + ITEM_OPERATOR_OTHER )
#define ITEM_OPERATOR_BRACKET_OPEN  ( 4 + ITEM_OPERATOR + ITEM_OPERATOR_OTHER )
#define ITEM_OPERATOR_BRACKET_CLOSE ( 5 + ITEM_OPERATOR + ITEM_OPERATOR_OTHER )
#define ITEM_OPERATOR_MEMBER        ( 6 + ITEM_OPERATOR + ITEM_OPERATOR_OTHER )
#define ITEM_OPERATOR_MEMBER_PTR    ( 7 + ITEM_OPERATOR + ITEM_OPERATOR_OTHER )
/* Compounded */
#define ITEM_OPERATOR_PLUS_EQ  ( ITEM_OPERATOR_PLUS | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_MINUS_EQ ( ITEM_OPERATOR_MINUS | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_MULT_EQ  ( ITEM_OPERATOR_MULT | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_DIV_EQ   ( ITEM_OPERATOR_DIV | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_MOD_EQ   ( ITEM_OPERATOR_MOD | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_AND_EQ   ( ITEM_OPERATOR_AND | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_XOR_EQ   ( ITEM_OPERATOR_XOR | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_OR_EQ    ( ITEM_OPERATOR_OR | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_SHL_EQ   ( ITEM_OPERATOR_SHL | ITEM_OPERATOR_COMPOUND )
#define ITEM_OPERATOR_SHR_EQ   ( ITEM_OPERATOR_SHR | ITEM_OPERATOR_COMPOUND )

#define ITEM_INDEX_EXPR ( ITEM_OPERATOR + ITEM_SUB_CLASS ) /* Expression Between Brackets*/

#define ITEM_CALL_ARGS 0x00400000

#define ITEM_KEYWORD 0x00800000

#define ITEM_KEYWORD_HAS_ARG 0x80
#define ITEM_KEYWORD_IS_JUMP 0x40
#define ITEM_KEYWORD_CONTROL 0x20
#define ITEM_KEYWORD_HAS_VAL 0x10

#define ITEM_IF       ( 1 + ITEM_KEYWORD_CONTROL + ITEM_KEYWORD_HAS_ARG )
#define ITEM_ELSE     ( 2 )
#define ITEM_FOR      ( 3 + ITEM_KEYWORD_CONTROL + ITEM_KEYWORD_HAS_ARG )
#define ITEM_DO       ( 4 )
#define ITEM_WHILE    ( 5 + ITEM_KEYWORD_CONTROL + ITEM_KEYWORD_HAS_ARG )
#define ITEM_WHILE2   ( 6 + ITEM_KEYWORD_CONTROL + ITEM_KEYWORD_HAS_ARG )
#define ITEM_SWITCH   ( 7 + ITEM_KEYWORD_CONTROL + ITEM_KEYWORD_HAS_ARG )
#define ITEM_CASE     ( 8 + ITEM_KEYWORD_HAS_VAL )
#define ITEM_DEFAULT  ( 9 )
#define ITEM_GOTO     ( 10 + ITEM_KEYWORD_IS_JUMP + ITEM_KEYWORD_HAS_VAL )
#define ITEM_BREAK    ( 11 + ITEM_KEYWORD_IS_JUMP )
#define ITEM_CONTINUE ( 12 + ITEM_KEYWORD_IS_JUMP )
#define ITEM_RETURN   ( 13 + ITEM_KEYWORD_IS_JUMP + ITEM_KEYWORD_HAS_VAL )
#define ITEM_SIZEOF   ( 14 + ITEM_KEYWORD_HAS_ARG )

#define ITEM_KEYWORD_IF       ( ITEM_KEYWORD + ITEM_IF )
#define ITEM_KEYWORD_ELSE     ( ITEM_KEYWORD + ITEM_ELSE )
#define ITEM_KEYWORD_FOR      ( ITEM_KEYWORD + ITEM_FOR )
#define ITEM_KEYWORD_DO       ( ITEM_KEYWORD + ITEM_DO )
#define ITEM_KEYWORD_WHILE    ( ITEM_KEYWORD + ITEM_WHILE )
#define ITEM_KEYWORD_WHILE2   ( ITEM_KEYWORD + ITEM_WHILE2 )
#define ITEM_KEYWORD_SWITCH   ( ITEM_KEYWORD + ITEM_SWITCH )
#define ITEM_KEYWORD_CASE     ( ITEM_KEYWORD + ITEM_CASE )
#define ITEM_KEYWORD_DEFAULT  ( ITEM_KEYWORD + ITEM_DEFAULT )
#define ITEM_KEYWORD_GOTO     ( ITEM_KEYWORD + ITEM_GOTO )
#define ITEM_KEYWORD_BREAK    ( ITEM_KEYWORD + ITEM_BREAK )
#define ITEM_KEYWORD_CONTINUE ( ITEM_KEYWORD + ITEM_CONTINUE )
#define ITEM_KEYWORD_RETURN   ( ITEM_KEYWORD + ITEM_RETURN )
#define ITEM_KEYWORD_SIZEOF   ( ITEM_KEYWORD + ITEM_SIZEOF )

#define ITEM_KW_EXPR        0x01000000
#define ITEM_KW_EXPR_IF     ( ITEM_KW_EXPR + ITEM_IF )
#define ITEM_KW_EXPR_FOR    ( ITEM_KW_EXPR + ITEM_FOR )
#define ITEM_KW_EXPR_WHILE  ( ITEM_KW_EXPR + ITEM_WHILE )
#define ITEM_KW_EXPR_WHILE2 ( ITEM_KW_EXPR + ITEM_WHILE2 )
#define ITEM_KW_EXPR_SWITCH ( ITEM_KW_EXPR + ITEM_SWITCH )
#define ITEM_KW_EXPR_RETURN ( ITEM_KW_EXPR + ITEM_RETURN )
#define ITEM_KW_EXPR_SIZEOF ( ITEM_KW_EXPR + ITEM_SIZEOF )

/* Attributes for ITEM_CALL, ITEM_FUNC_NAME, ... */
#define ITEM_FUNC_MATH            0x04
#define ITEM_FUNC_PROJECT         0x08
#define ITEM_FUNC_COUNTERS_AUTO_L 0x02 /* Left*/
#define ITEM_FUNC_COUNTERS_AUTO_R 0x80 /* Right*/
#define ITEM_FUNC_COUNTERS_AUTO   ( ITEM_FUNC_COUNTERS_AUTO_L | ITEM_FUNC_COUNTERS_AUTO_R )
#define ITEM_FUNC_COUNTERS_MAN    0x01
#define ITEM_FUNC_COUNTERS        ( ITEM_FUNC_COUNTERS_MAN | ITEM_FUNC_COUNTERS_AUTO )
#define ITEM_FUNC_COUNT_LIB       0x20
#define ITEM_FUNC_SYSTEM          0x40

/* Attributes for ITEM_SKIPPED */
#define ITEM_CAST  0x01 /* Unused nor Now*/
#define ITEM_LABEL 0x02
#define ITEM_DATA  0x04 /* typedef or Declaration*/

#define ITEM_INSTRUMENTED    0x02000000
#define ITEM_INSTRUMENTATION 0x04000000

#define ITEM_MOVED_OVER   ( ITEM_COMMENT | ITEM_PREPROC_LINE )
#define ITEM_NOT_SEARCHED ( ITEM_SKIPPED | ITEM_ENCLOSED | ITEM_PREPROC_LINE | ITEM_DATA_DECL )

/* Attributes for ITEM_CONSTANT */
#define ITEM_CSTE_NZ       0x01
#define ITEM_CSTE_HEX      0x02
#define ITEM_CSTE_OCTAL    0x04
#define ITEM_CSTE_POWEROF2 0x08
#define ITEM_CSTE_ZERO     0x10
#define ITEM_CSTE_FLOAT    0x20
#define ITEM_CSTE_SUFFIX   0x40
#define ITEM_CSTE_ERROR    0x80

#define ITEM_INSTRUMENTATION_OFF 0x10000000

#define ITEM_CONSTANT 0x20000000
#define ITEM_SKIPPED  0x40000000
#define ITEM_WARNING  0x80000000

typedef unsigned int Item_Type;

typedef struct
{
    Item_Type item_type; /* What has been found */
    /* All Locations refer to the Original File */
    char *item_start;
    char *item_end;
    char *item_link;
} Parse_Rec_def;
#define PARSE_REC_SIZE sizeof( Parse_Rec_def )

typedef struct
{
    char *Name;  /* Name of the File */
    int Size;    /* Size of the File */
    char *Data;  /* Data of the File */
    int MaxSize; /* Allocated Memory (can be > than Size) */
} File_def;

typedef struct
{
    int Size;            /* # of Entries of the Parsing Table */
    Parse_Rec_def *Data; /* Pointer to the Record Table */
    int MaxSize;         /* # of Allocated Entries (can be > than Size) */
} Parse_Tbl_def;

typedef struct
{
    char *Ptr;       /* Insert Location */
    char String[24]; /* Data to Insert */
    int Seq;         /* Sequence # */
} Insert_Rec_def;
#define INSERT_REC_SIZE sizeof( Insert_Rec_def )

typedef struct
{
    int Size;             /* # of Entries of the Insert Table */
    Insert_Rec_def *Data; /* Pointer to the Insert Table */
    int MaxSize;          /* # of Allocated Entries (can be > than Size) */
} Insert_Tbl_def;

typedef struct
{
    char *Ptr;      /* Function Name Location */
    int NameLength; /* Function Name Length */
    int NameOcc;    /* Function Name Occurances */
    int nArgs;      /* Function's # of Args */
} FctCall_Rec_def;
#define FCTCALL_REC_SIZE     sizeof( FctCall_Rec_def )
#define FCTCALL_MAX_NAME_OCC 10 /* Arbitrary Number*/

typedef struct
{
    int Size;              /* # of Entries of the Function Call Table */
    FctCall_Rec_def *Data; /* Pointer to the Function Call Table */
    int MaxSize;           /* # of Allocated Entries (can be > than Size) */
} FctCall_Tbl_def;

typedef struct
{
    char *Ptr;             /* Some Pointer */
    char *Ptr2;            /* Another Pointer (for Unindenting around Counting Library Functions) */
} Pointer_Rec_def;
#define POINTER_REC_SIZE sizeof( Pointer_Rec_def )

typedef struct
{
    int Size;              /* # of Entries of the Pointer Table */
    Pointer_Rec_def *Data; /* Pointer to the Pointer Table */
    int MaxSize;           /* # of Allocated Entries (can be > than Size) */
} Pointer_Tbl_def;

typedef enum
{
    FILE_FAILED,
    FILE_OK,
    SETUP_FAILED,
    SETUP_OK,
    DI_FAILED,
    DESINSTRUMENTED,
    INS_FAILED,
    INSTRUMENTED,
    FINAL_FAILED,
    FINALIZED,
} PARSER_STATE;

typedef struct
{
    File_def File;
    Parse_Tbl_def ParseTbl;
    Insert_Tbl_def InsertTbl;
    FctCall_Tbl_def FctCallTbl;
    Pointer_Tbl_def PointerTbl;
    int State;
    int PROMSize;
    int PROMOpsWeightsSet;
    int nChars;
} Parse_Context_def;


typedef struct
{
    char* cmd_line_spec;
    char* pathname;
    char* filename_or_mask;
    char** file_list;
    int nFiles;
} T_FILE_BOOK;

#endif /* #ifndef __PARSING_DEFS_H__ */
