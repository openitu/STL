/*
 * (C) 2024 copyright VoiceAge Corporation. All Rights Reserved.
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

#ifndef WMOPS_H
#define WMOPS_H

#ifndef EXIT_FAILURE
#include <stdlib.h> /* stdlib is needed for exit() */
#endif

#ifndef EOF
#include <stdio.h> /* stdio is needed for fprintf() */
#endif

/* To Prevent "warning: '$' in identifier or number" message under GCC */
#ifdef __GNUC__
#pragma GCC system_header
#endif

#define ENH_32_BIT_OPERATOR
#define ENH_64_BIT_OPERATOR
#define ENH_U_32_BIT_OPERATOR
#define COMPLEX_OPERATOR
#define CONTROL_CODE_OPS                    /* enable control code operators such as LT_16, GT_16, ... */
/* #define WMOPS_DISABLE_FCN_CALL_PENALIZATION*/ /* do not count the complexity of function calls */

#ifdef WMOPS
enum instructions
{
    _ADD,
    _ABS,
    _MULT,
    _MAC,
    _MOVE,
    _STORE,
    _LOGIC,
    _SHIFT,
    _BRANCH,
    _DIV,
    _SQRT,
    _TRANS,
    _FUNC,
    _LOOP,
    _INDIRECT,
    _PTR_INIT,
    _TEST,
    _POWER,
    _LOG,
    _MISC,
    NUM_INST
};

extern double ops_cnt;
extern double inst_cnt[NUM_INST];

/******************************************************************/
/* NOTES:                                                         */
/*   The 'wmc_flag_' flag is global to avoid declaration in every */
/*   function and 'static' to avoid clashing with other modules   */
/*   that include this header file.                               */
/*                                                                */
/*   The declarations of 'wmc_flag_' and 'wops_' in this header   */
/*   file prevent the addition of a 'C' file to the Project.      */
/******************************************************************/

/* General Purpose Global int */
static int wmc_flag_ = 0;

#define push_wmops( ... ) push_wmops_fct( __VA_ARGS__, NULL )
void push_wmops_fct( const char *label, ... );
void pop_wmops( void );
void reset_wmops( void );
void print_wmops( void );
void update_wmops( void );
void update_mem( void );

#define _ADD_C      1
#define _ABS_C      1
#define _MULT_C     1
#define _MAC_C      1
#define _MOVE_C     1
#define _STORE_C    1
#define _LOGIC_C    1
#define _SHIFT_C    1
#define _BRANCH_C   4
#define _DIV_C      18
#define _SQRT_C     10
#define _TRANS_C    25
#define _FUNC_C     2 /* need to add number of arguments */
#define _LOOP_C     3
#define _INDIRECT_C 2
#define _PTR_INIT_C 1
#define _TEST_C     2
#define _POWER_C    25
#define _LOG_C      25
#define _MISC_C     1

#define ADD( x )                       \
    {                                  \
        ops_cnt += ( _ADD_C * ( x ) ); \
        inst_cnt[_ADD] += ( x );       \
    }
#define ABS( x )                       \
    {                                  \
        ops_cnt += ( _ABS_C * ( x ) ); \
        inst_cnt[_ABS] += ( x );       \
    }
#define MULT( x )                       \
    {                                   \
        ops_cnt += ( _MULT_C * ( x ) ); \
        inst_cnt[_MULT] += ( x );       \
    }
#define MAC( x )                       \
    {                                  \
        ops_cnt += ( _MAC_C * ( x ) ); \
        inst_cnt[_MAC] += ( x );       \
    }
#define MOVE( x )                       \
    {                                   \
        ops_cnt += ( _MOVE_C * ( x ) ); \
        inst_cnt[_MOVE] += ( x );       \
    }
#define STORE( x )                       \
    {                                    \
        ops_cnt += ( _STORE_C * ( x ) ); \
        inst_cnt[_STORE] += ( x );       \
    }
#define LOGIC( x )                       \
    {                                    \
        ops_cnt += ( _LOGIC_C * ( x ) ); \
        inst_cnt[_LOGIC] += ( x );       \
    }
#define SHIFT( x )                       \
    {                                    \
        ops_cnt += ( _SHIFT_C * ( x ) ); \
        inst_cnt[_SHIFT] += ( x );       \
    }
#define BRANCH( x )                       \
    {                                     \
        ops_cnt += ( _BRANCH_C * ( x ) ); \
        inst_cnt[_BRANCH] += ( x );       \
    }
#define DIV( x )                       \
    {                                  \
        ops_cnt += ( _DIV_C * ( x ) ); \
        inst_cnt[_DIV] += ( x );       \
    }
#define SQRT( x )                       \
    {                                   \
        ops_cnt += ( _SQRT_C * ( x ) ); \
        inst_cnt[_SQRT] += ( x );       \
    }
#define TRANS( x )                       \
    {                                    \
        ops_cnt += ( _TRANS_C * ( x ) ); \
        inst_cnt[_TRANS] += ( x );       \
    }
#define LOOP( x )                       \
    {                                   \
        ops_cnt += ( _LOOP_C * ( x ) ); \
        inst_cnt[_LOOP] += ( x );       \
    }
#define INDIRECT( x )                       \
    {                                       \
        ops_cnt += ( _INDIRECT_C * ( x ) ); \
        inst_cnt[_INDIRECT] += ( x );       \
    }
#define PTR_INIT( x )                       \
    {                                       \
        ops_cnt += ( _PTR_INIT_C * ( x ) ); \
        inst_cnt[_PTR_INIT] += ( x );       \
    }
#define TEST( x )                       \
    {                                   \
        ops_cnt += ( _TEST_C * ( x ) ); \
        inst_cnt[_TEST] += ( x );       \
    }
#define POWER( x )                       \
    {                                    \
        ops_cnt += ( _POWER_C * ( x ) ); \
        inst_cnt[_POWER] += ( x );       \
    }
#define LOG( x )                       \
    {                                  \
        ops_cnt += ( _LOG_C * ( x ) ); \
        inst_cnt[_LOG] += ( x );       \
    }
#define MISC( x )                       \
    {                                   \
        ops_cnt += ( _MISC_C * ( x ) ); \
        inst_cnt[_MISC] += ( x );       \
    }
#define FUNC( x )                                 \
    {                                             \
        ops_cnt += ( _FUNC_C + _MOVE_C * ( x ) ); \
        inst_cnt[_FUNC]++;                        \
        inst_cnt[_MOVE] += ( x );                 \
    }
#define DADD( x )                          \
    {                                      \
        ops_cnt += ( 2 * _ADD_C * ( x ) ); \
        inst_cnt[_ADD] += ( x );           \
    }
#define DMULT( x )                          \
    {                                       \
        ops_cnt += ( 2 * _MULT_C * ( x ) ); \
        inst_cnt[_MULT] += ( x );           \
    }
#define DMAC( x )                          \
    {                                      \
        ops_cnt += ( 2 * _MAC_C * ( x ) ); \
        inst_cnt[_MAC] += ( x );           \
    }
#define DMOVE( x )                          \
    {                                       \
        ops_cnt += ( 2 * _MOVE_C * ( x ) ); \
        inst_cnt[_MOVE] += ( x );           \
    }
#define DSTORE( x )                          \
    {                                        \
        ops_cnt += ( 2 * _STORE_C * ( x ) ); \
        inst_cnt[_STORE] += ( x );           \
    }
#define DLOGIC( x )                          \
    {                                        \
        ops_cnt += ( 2 * _LOGIC_C * ( x ) ); \
        inst_cnt[_LOGIC] += ( x );           \
    }
#define DSHIFT( x )                          \
    {                                        \
        ops_cnt += ( 2 * _SHIFT_C * ( x ) ); \
        inst_cnt[_SHIFT] += ( x );           \
    }
#define DDIV( x )                          \
    {                                      \
        ops_cnt += ( 2 * _DIV_C * ( x ) ); \
        inst_cnt[_DIV] += ( x );           \
    }
#define DSQRT( x )                          \
    {                                       \
        ops_cnt += ( 2 * _SQRT_C * ( x ) ); \
        inst_cnt[_SQRT] += ( x );           \
    }
#define DTRANS( x )                          \
    {                                        \
        ops_cnt += ( 2 * _TRANS_C * ( x ) ); \
        inst_cnt[_TRANS] += ( x );           \
    }

#else

extern int cntr_push_pop;
#define push_wmops( x ) ( cntr_push_pop++ )
#define pop_wmops()     ( cntr_push_pop-- )
#define reset_wmops()
#define print_wmops()
#define update_wmops() ( assert( cntr_push_pop == 0 ) )
#define update_mem()

#define ADD( x )
#define ABS( x )
#define MULT( x )
#define MAC( x )
#define MOVE( x )
#define STORE( x )
#define LOGIC( x )
#define SHIFT( x )
#define BRANCH( x )
#define DIV( x )
#define SQRT( x )
#define TRANS( x )
#define FUNC( x )
#define LOOP( x )
#define INDIRECT( x )
#define PTR_INIT( x )
#define TEST( x )
#define POWER( x )
#define LOG( x )
#define MISC( x )

#define DADD( x )
#define DMULT( x )
#define DMAC( x )
#define DMOVE( x )
#define DSTORE( x )
#define DLOGIC( x )
#define DSHIFT( x )
#define DDIV( x )
#define DSQRT( x )
#define DTRANS( x )

#endif

#ifndef WMOPS
/* DESACTIVATE the Counting Mechanism */
#define OP_COUNT_( op, n )

/* DESACTIVATE Operation Counter Wrappers */
#define OP_COUNT_WRAPPER1_( op, val ) ( val )
#define OP_COUNT_WRAPPER2_( expr )
#define OP_COUNT_WRAPPER3_( op, expr ) expr

/* DESACTIVATE Logical & Ternary Operators */
#define __
#define _

#else

/* Operation Counter Wrappers */
#define OP_COUNT_( op, x )            ( ops_cnt += ( op##_C * ( x ) ), inst_cnt[op] += ( x ) )
#define OP_COUNT_WRAPPER1_( op, val ) ( op, val )
#define OP_COUNT_WRAPPER2_( expr ) \
    if ( expr, 0 )                 \
        ;                          \
    else
#define OP_COUNT_WRAPPER3_( op, expr ) \
    if ( op, 0 )                       \
        ;                              \
    else                               \
        expr

#endif

/* Define all Macros without '{' & '}' (None of these should be called externally!) */
#define ABS_( x )      OP_COUNT_( _ABS, ( x ) )
#define ADD_( x )      OP_COUNT_( _ADD, ( x ) )
#define MULT_( x )     OP_COUNT_( _MULT, ( x ) )
#define MAC_( x )      OP_COUNT_( _MAC, ( x ) )
#define MOVE_( x )     OP_COUNT_( _MOVE, ( x ) )
#define STORE_( x )    OP_COUNT_( _STORE, ( x ) )
#define LOGIC_( x )    OP_COUNT_( _LOGIC, ( x ) )
#define SHIFT_( x )    OP_COUNT_( _SHIFT, ( x ) )
#define BRANCH_( x )   OP_COUNT_( _BRANCH, ( x ) )
#define DIV_( x )      OP_COUNT_( _DIV, ( x ) )
#define SQRT_( x )     OP_COUNT_( _SQRT, ( x ) )
#define TRANS_( x )    OP_COUNT_( _TRANS, ( x ) )
#define POWER_( x )    TRANS_( x )
#define LOG_( x )      TRANS_( x )
#define LOOP_( x )     OP_COUNT_( _LOOP, ( x ) )
#define INDIRECT_( x ) OP_COUNT_( _INDIRECT, ( x ) )
#define PTR_INIT_( x ) OP_COUNT_( _PTR_INIT, ( x ) )
#ifdef WMOPS_DISABLE_FCN_CALL_PENALIZATION
#define FUNC_( x ) ( x )
#else
#define FUNC_( x ) ( OP_COUNT_( _MOVE, ( x ) ), OP_COUNT_( _FUNC, 1 ) )
#endif
#define MISC_( x ) ABS_( x )

/* Math Operations */
#define abs_    OP_COUNT_WRAPPER1_( ABS_( 1 ), abs )
#define fabs_   OP_COUNT_WRAPPER1_( ABS_( 1 ), fabs )
#define fabsf_  OP_COUNT_WRAPPER1_( ABS_( 1 ), fabsf )
#define labs_   OP_COUNT_WRAPPER1_( ABS_( 1 ), labs )
#define floor_  OP_COUNT_WRAPPER1_( MISC_( 1 ), floor )
#define floorf_ OP_COUNT_WRAPPER1_( MISC_( 1 ), floorf )
#define sqrt_   OP_COUNT_WRAPPER1_( SQRT_( 1 ), sqrt )
#define sqrtf_  OP_COUNT_WRAPPER1_( SQRT_( 1 ), sqrtf )
#define pow_    OP_COUNT_WRAPPER1_( POWER_( 1 ), pow )
#define powf_   OP_COUNT_WRAPPER1_( POWER_( 1 ), powf )
#define exp_    OP_COUNT_WRAPPER1_( POWER_( 1 ), exp )
#define expf_   OP_COUNT_WRAPPER1_( POWER_( 1 ), expf )
#define log_    OP_COUNT_WRAPPER1_( LOG_( 1 ), log )
#define logf_   OP_COUNT_WRAPPER1_( LOG_( 1 ), logf )
#define log10_  OP_COUNT_WRAPPER1_( LOG_( 1 ), log10 )
#define log10f_ OP_COUNT_WRAPPER1_( LOG_( 1 ), log10f )
#define cos_    OP_COUNT_WRAPPER1_( TRANS_( 1 ), cos )
#define cosf_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), cosf )
#define sin_    OP_COUNT_WRAPPER1_( TRANS_( 1 ), sin )
#define sinf_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), sinf )
#define tan_    OP_COUNT_WRAPPER1_( TRANS_( 1 ), tan )
#define tanf_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), tanf )
#define acos_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), acos )
#define acosf_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), acosf )
#define asin_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), asin )
#define asinf_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), asinf )
#define atan_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), atan )
#define atanf_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), atanf )
#define atan2_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), atan2 )
#define atan2f_ OP_COUNT_WRAPPER1_( TRANS_( 1 ), atan2f )
#define cosh_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), cosh )
#define coshf_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), coshf )
#define sinh_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), sinh )
#define sinhf_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), sinhf )
#define tanh_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), tanh )
#define tanhf_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), tanhf )
#define fmod_   OP_COUNT_WRAPPER1_( DIV_( 1 ), fmod )
#define fmodf_  OP_COUNT_WRAPPER1_( DIV_( 1 ), fmodf )
#define frexp_  OP_COUNT_WRAPPER1_( MISC_( 2 ), frexp )
#define frexpf_ OP_COUNT_WRAPPER1_( MISC_( 2 ), frexpf )

/* the macros below are instrumented versions of user-defined macros that might be used in the source code 
   representing some well-known and recognized mathematical operations (that are not defined in math.h) 
   Note: the 'wmc_flag_=wmc_flag_' is used to avoid warning: left-hand operand of comma expression has no effect with gcc */

#define min_( a, b )     OP_COUNT_WRAPPER1_( MISC_( 1 ), min( ( a ), ( b ) ) )
#define max_( a, b )     OP_COUNT_WRAPPER1_( MISC_( 1 ), max( ( a ), ( b ) ) )
#define MIN_( a, b )     OP_COUNT_WRAPPER1_( MISC_( 1 ), MIN( ( a ), ( b ) ) )
#define MAX_( a, b )     OP_COUNT_WRAPPER1_( MISC_( 1 ), MAX( ( a ), ( b ) ) )
#define Min_( a, b )     OP_COUNT_WRAPPER1_( MISC_( 1 ), Min( ( a ), ( b ) ) )
#define Max_( a, b )     OP_COUNT_WRAPPER1_( MISC_( 1 ), Max( ( a ), ( b ) ) )
#define sqr_( x )        OP_COUNT_WRAPPER1_( MULT_( 1 ), sqr( ( x ) ) )
#define Sqr_( x )        OP_COUNT_WRAPPER1_( MULT_( 1 ), Sqr( ( x ) ) )
#define SQR_( x )        OP_COUNT_WRAPPER1_( MULT_( 1 ), SQR( ( x ) ) )
#define square_( x )     OP_COUNT_WRAPPER1_( MULT_( 1 ), square( ( x ) ) )
#define Square_( x )     OP_COUNT_WRAPPER1_( MULT_( 1 ), Square( ( x ) ) )
#define SQUARE_( x )     OP_COUNT_WRAPPER1_( MULT_( 1 ), SQUARE( ( x ) ) )
#define sign_( x )       OP_COUNT_WRAPPER1_( MOVE_( 1 ), sign( ( x ) ) )
#define Sign_( x )       OP_COUNT_WRAPPER1_( MOVE_( 1 ), Sign( ( x ) ) )
#define SIGN_( x )       OP_COUNT_WRAPPER1_( MOVE_( 1 ), SIGN( ( x ) ) )
#define inv_sqrt_( x )   OP_COUNT_WRAPPER1_( SQRT_( 1 ), inv_sqrt( ( x ) ) )
#define inv_sqrtf_( x )  OP_COUNT_WRAPPER1_( SQRT_( 1 ), inv_sqrtf( ( x ) ) )
#define log_base_2_( x ) OP_COUNT_WRAPPER1_( ( LOG_( 1 ), MULT_( 1 ) ), log_base_2( ( x ) ) )
#define log2_( x )       OP_COUNT_WRAPPER1_( ( LOG_( 1 ), MULT_( 1 ) ), log2( ( x ) ) )
#define log2f_( x )      OP_COUNT_WRAPPER1_( ( LOG_( 1 ), MULT_( 1 ) ), log2f( ( x ) ) )
#define log2_f_( x )     OP_COUNT_WRAPPER1_( ( LOG_( 1 ), MULT_( 1 ) ), log2_f( ( x ) ) )
#define _round_( x )     OP_COUNT_WRAPPER1_( wmc_flag_ = wmc_flag_, _round( ( x ) ) )
#define round_( x )      OP_COUNT_WRAPPER1_( wmc_flag_ = wmc_flag_, round( ( x ) ) )
#define round_f_( x )    OP_COUNT_WRAPPER1_( wmc_flag_ = wmc_flag_, round_f( ( x ) ) )
#define roundf_( x )     OP_COUNT_WRAPPER1_( wmc_flag_ = wmc_flag_, roundf( ( x ) ) )
#define set_min_( a, b ) OP_COUNT_WRAPPER3_( ( ADD_( 1 ), BRANCH_( 1 ), MOVE_( 1 ) ), set_min( ( a ), ( b ) ) )
#define set_max_( a, b ) OP_COUNT_WRAPPER3_( ( ADD_( 1 ), BRANCH_( 1 ), MOVE_( 1 ) ), set_max( ( a ), ( b ) ) )

/* Functions */
#define func_( name, x ) OP_COUNT_WRAPPER1_( FUNC_( x ), name )

/* Logical Operators */
#ifndef __
#define __ ( BRANCH_( 1 ), 1 ) &&
#endif

/* Ternary Operators (? and :) */
#ifndef _
#define _ ( BRANCH_( 1 ), 0 ) ? 0:
#endif

/* Flow Control keywords */
#define if_                            \
    OP_COUNT_WRAPPER2_( BRANCH_( 1 ) ) \
    if
#define for_    OP_COUNT_WRAPPER2_(  LOOP_(1)) for
#define while_( c ) \
    while           \
    OP_COUNT_WRAPPER1_( BRANCH_( 1 ), ( c ) ) /* needs extra "()" if ',' encountered */
#define do_ \
    do      \
    {
#define _while    \
    BRANCH_( 1 ); \
    }             \
    while

#define goto_                          \
    OP_COUNT_WRAPPER2_( BRANCH_( 1 ) ) \
    goto
#define break_                         \
    OP_COUNT_WRAPPER2_( BRANCH_( 1 ) ) \
    break
#define continue_                      \
    OP_COUNT_WRAPPER2_( BRANCH_( 1 ) ) \
    continue
#define return_                                                                     \
    OP_COUNT_WRAPPER2_( ( wmc_flag_ = stack_tree_level_, STACK_DEPTH_FCT_RETURN ) ) \
    return
#define switch_                                           \
    OP_COUNT_WRAPPER2_( ( BRANCH_( 1 ), wmc_flag_ = 1 ) ) \
    switch
#define cost_( n ) OP_COUNT_WRAPPER2_( wmc_flag_ ? ( ADD_( n ), BRANCH_( n ), wmc_flag_ = 0 ) : 0 );

#ifdef WMOPS

#define ACC 2
#define MUL 1

/* Counting Function (should not be called externally!) */
static void wops_( const char *ops )
{
    char lm = 0;        /* lm: Last Operation is Math */
    static char lo = 0; /* Last Operation */

    void ( *fct )( const char *ops ) = wops_;

st:
    while ( *ops != '\0' )
    {
        switch ( *ops++ )
        {
            int cnt;
            case '-':
                for ( cnt = 0; ops[cnt] == '>'; cnt++ )
                    ;
                if ( cnt & 1 )
                    goto ind;
            case '+':
                lm = 2;
                if ( lo & MUL )
                {
                    MULT_( -1 );
                    MAC_( 1 );
                    break;
                }
                lo = ACC << 2;
            case 'U':
            case 'D':
                ADD_( 1 );
                break;
            case '*':
                lm = 2;
                if ( lo & ACC )
                {
                    ADD_( -1 );
                    MAC_( 1 );
                    break;
                }
                lo = MUL << 2;
                MULT_( 1 );
                break;
            case '/':
            case '%':
                lm = 2;
                DIV_( 1 );
                break;
            case '&':
            case '|':
            case '^':
                lm = 2;
            case '~':
                LOGIC_( 1 );
                break;
            case '<':
            case '>':
                if ( *ops != ops[-1] )
                    goto error;
                ops++;
            case -85:
            case -69:
                lm = 2;
                SHIFT_( 1 );
                break;
            case 'L':
            case 'G':
                if ( *ops == 't' )
                    goto comp;
            case 'E':
            case 'N':
                if ( *ops != 'e' )
                    goto error;
            comp:
                ops++;
                ADD_( 1 );
                break;
            case '!':
                MISC_( 2 );
                break;
            case 'M':
                MOVE_( 1 );
                break;
            case 'S':
                STORE_( 1 );
                break;
            case 'P':
                PTR_INIT_( 1 );
                break;
            case '[':
            case ']':
                goto st;
            ind:
                ops++;
            case 'I':
            case '.':
                INDIRECT_( 1 );
                break;
            case '=':
                if ( lm )
                    goto st;
            case '\0':
                /* This Shouldn't Happen */
                /* These are Used to Avoid: "warning: 'name' defined but not used" with Cygwin gcc Compiler */
                wmc_flag_ = wmc_flag_;
                fct( "" );
            error:
            default:
                fprintf( stderr, "\r wops: Invalid Counting Operation '%s'\n", ops - 1 );
                exit( -1 );
        }
        lm >>= 1;
        lo >>= 2;
    }

    return;
}

#endif

/* All Other Operations */
#define $( str ) OP_COUNT_WRAPPER2_( wops_( str ) )


/*-------------------------------------------------------------------*
 * Memory counting tool
 *-------------------------------------------------------------------*/

/* Enhanced Const Data Size Counting (Rounding Up to the Nearest 'Integer' Size) */
#define rsize( item ) ( ( sizeof( item ) + sizeof( int ) - 1 ) / sizeof( int ) * sizeof( int ) )

#ifdef _MSC_VER
/* Disable "warning C4210: nonstandard extension used : function given file scope" with Visual Studio Compiler */
#pragma warning( disable : 4210 )
#endif

/* Const Data Size and PROM Size Wrapper Functions */
#define Const_Data_Size_Func( file ) Const_Data_Size_##file( void )
#define Get_Const_Data_Size( file, val_ptr )       \
    {                                              \
        extern int Const_Data_Size_##file( void ); \
        *( val_ptr ) = Const_Data_Size_##file();   \
    }
#define PROM_Size_Func( file ) PROM_Size_##file( void )
#define Get_PROM_Size( file, val_ptr )     \
    {                                      \
        int PROM_Size_##file( void );      \
        *( val_ptr ) = PROM_Size_##file(); \
    }

/* ROM Size Lookup Table - contains information about PROM size and Const Data Size in all source files */
/* The print_mem() function looks for this table to print the results of Const Data usage and PROM usage */
typedef struct ROM_Size_Lookup_Table
{
    const char file_spec[255];
    int PROM_size;
    int ( *Get_Const_Data_Size_Func )( void );
} ROM_Size_Lookup_Table;

/* The WMC tool inserts the following declaration during the innstrumentation process in the .c file where the function print_mem() is located */
/* and modifies it to print_mem(Const_Data_PROM_Table) */

/* #ifdef WMOPS
 * ROM_Size_Lookup_Table Const_Data_PROM_Table[] =
 * {
 *   {"../lib_enc/rom_enc.c", 0, NULL},
 *   {"../lib_com/[star].c", 0, NULL},
 *   {"", -1, NULL}
 * };
 * #endif
 */

/*#define MEM_ALIGN_64BITS */ /* Define this when using 64 Bits values in the code (ex: double), otherwise it will align on 32 Bits */
/*#define MEM_COUNT_DETAILS*/

typedef enum
{
    USE_BYTES = 0,
    USE_16BITS = 1,
    USE_32BITS = 2
} Counting_Size;


#ifdef WMOPS

void *mem_alloc( const char *func_name, int func_lineno, size_t size, char *alloc_str );
void mem_free( const char *func_name, int func_lineno, void *ptr );

#define malloc_( size )    mem_alloc( __func__, __LINE__, size, "m:" #size )
#define calloc_( n, size ) mem_alloc( __func__, __LINE__, ( n ) * ( size ), "c:" #n ", " #size )
#define free_( ptr )       mem_free( __func__, __LINE__, ptr )

void reset_mem( Counting_Size cnt_size );
void print_mem( ROM_Size_Lookup_Table Const_Data_PROM_Table[] );

int push_stack( const char *filename, const char *fctname );
int pop_stack( const char *filename, const char *fctname );

#ifdef WMOPS_DETAIL
#define STACK_DEPTH_FCT_CALL   ( push_wmops( __func__, "[WMC_AUTO]" ), push_stack( __FILE__, __func__ ) ) /* add push_wmops() in all function calls */
#define STACK_DEPTH_FCT_RETURN ( pop_wmops(), pop_stack( __FILE__, __func__ ) )                           /* add pop_wmops() in all function returns */
#else
#define STACK_DEPTH_FCT_CALL   push_stack( __FILE__, __func__ )
#define STACK_DEPTH_FCT_RETURN pop_stack( __FILE__, __func__ )
#endif

void reset_stack( void );
#define func_start_ int stack_tree_level_ = STACK_DEPTH_FCT_CALL;

#else
#define malloc_( n1 )     malloc( n1 )
#define calloc_( n1, n2 ) calloc( n1, n2 )
#define free_( ptr )      free( ptr )
#define reset_mem( cnt_size )
#define print_mem( Const_Data_PROM_Table )

#define push_stack( file, fct )
#define pop_stack( file, fct )
#define reset_stack()
#define func_start_

#endif

/* Global counter variable for calculation of complexity weight */
typedef struct
{
    unsigned int add;   /* Complexity Weight of 1 */
    unsigned int sub;   /* Complexity Weight of 1 */
    unsigned int abs_s; /* Complexity Weight of 1 */
    unsigned int shl;   /* Complexity Weight of 1 */
    unsigned int shr;   /* Complexity Weight of 1 */

    unsigned int extract_h; /* Complexity Weight of 1 */
    unsigned int extract_l; /* Complexity Weight of 1 */
    unsigned int mult;      /* Complexity Weight of 1 */
    unsigned int L_mult;    /* Complexity Weight of 1 */
    unsigned int negate;    /* Complexity Weight of 1 */

    unsigned int round;   /* Complexity Weight of 1 */
    unsigned int L_mac;   /* Complexity Weight of 1 */
    unsigned int L_msu;   /* Complexity Weight of 1 */
    unsigned int L_macNs; /* Complexity Weight of 1 */
    unsigned int L_msuNs; /* Complexity Weight of 1 */

    unsigned int L_add;    /* Complexity Weight of 1 */
    unsigned int L_sub;    /* Complexity Weight of 1 */
    unsigned int L_add_c;  /* Complexity Weight of 2 */
    unsigned int L_sub_c;  /* Complexity Weight of 2 */
    unsigned int L_negate; /* Complexity Weight of 1 */

    unsigned int L_shl;  /* Complexity Weight of 1 */
    unsigned int L_shr;  /* Complexity Weight of 1 */
    unsigned int mult_r; /* Complexity Weight of 1 */
    unsigned int shr_r;  /* Complexity Weight of 3 */
    unsigned int mac_r;  /* Complexity Weight of 1 */

    unsigned int msu_r;       /* Complexity Weight of 1 */
    unsigned int L_deposit_h; /* Complexity Weight of 1 */
    unsigned int L_deposit_l; /* Complexity Weight of 1 */
    unsigned int L_shr_r;     /* Complexity Weight of 3 */
    unsigned int L_abs;       /* Complexity Weight of 1 */

    unsigned int L_sat;  /* Complexity Weight of 4 */
    unsigned int norm_s; /* Complexity Weight of 1 */
    unsigned int div_s;  /* Complexity Weight of 18 */
    unsigned int norm_l; /* Complexity Weight of 1 */
    unsigned int move16; /* Complexity Weight of 1 */

    unsigned int move32;  /* Complexity Weight of 2 */
    unsigned int Logic16; /* Complexity Weight of 1 */
    unsigned int Logic32; /* Complexity Weight of 2 */
    unsigned int Test;    /* Complexity Weight of 2 */
    unsigned int s_max;   /* Complexity Weight of 1 */

    unsigned int s_min;   /* Complexity Weight of 1 */
    unsigned int L_max;   /* Complexity Weight of 1 */
    unsigned int L_min;   /* Complexity Weight of 1 */
    unsigned int L40_max; /* Complexity Weight of 1 */
    unsigned int L40_min; /* Complexity Weight of 1 */

    unsigned int shl_r;     /* Complexity Weight of 2 */
    unsigned int L_shl_r;   /* Complexity Weight of 2 */
    unsigned int L40_shr_r; /* Complexity Weight of 2 */
    unsigned int L40_shl_r; /* Complexity Weight of 2 */
    unsigned int norm_L40;  /* Complexity Weight of 1 */

    unsigned int L40_shl;    /* Complexity Weight of 1 */
    unsigned int L40_shr;    /* Complexity Weight of 1 */
    unsigned int L40_negate; /* Complexity Weight of 1 */
    unsigned int L40_add;    /* Complexity Weight of 1 */
    unsigned int L40_sub;    /* Complexity Weight of 1 */

    unsigned int L40_abs;  /* Complexity Weight of 1 */
    unsigned int L40_mult; /* Complexity Weight of 1 */
    unsigned int L40_mac;  /* Complexity Weight of 1 */
    unsigned int mac_r40;  /* Complexity Weight of 2 */

    unsigned int L40_msu;      /* Complexity Weight of 1 */
    unsigned int msu_r40;      /* Complexity Weight of 2 */
    unsigned int Mpy_32_16_ss; /* Complexity Weight of 2 */
    unsigned int Mpy_32_32_ss; /* Complexity Weight of 2 */
    unsigned int L_mult0;      /* Complexity Weight of 1 */

    unsigned int L_mac0; /* Complexity Weight of 1 */
    unsigned int L_msu0; /* Complexity Weight of 1 */
    unsigned int lshl;   /* Complexity Weight of 1 */
    unsigned int lshr;   /* Complexity Weight of 1 */
    unsigned int L_lshl; /* Complexity Weight of 1 */

    unsigned int L_lshr;   /* Complexity Weight of 1 */
    unsigned int L40_lshl; /* Complexity Weight of 1 */
    unsigned int L40_lshr; /* Complexity Weight of 1 */
    unsigned int s_and;    /* Complexity Weight of 1 */
    unsigned int s_or;     /* Complexity Weight of 1 */

    unsigned int s_xor; /* Complexity Weight of 1 */
    unsigned int L_and; /* Complexity Weight of 1 */
    unsigned int L_or;  /* Complexity Weight of 1 */
    unsigned int L_xor; /* Complexity Weight of 1 */
    unsigned int rotl;  /* Complexity Weight of 3 */

    unsigned int rotr;          /* Complexity Weight of 3 */
    unsigned int L_rotl;        /* Complexity Weight of 3 */
    unsigned int L_rotr;        /* Complexity Weight of 3 */
    unsigned int L40_set;       /* Complexity Weight of 1 */
    unsigned int L40_deposit_h; /* Complexity Weight of 1 */

    unsigned int L40_deposit_l; /* Complexity Weight of 1 */
    unsigned int L40_deposit32; /* Complexity Weight of 1 */
    unsigned int Extract40_H;   /* Complexity Weight of 1 */
    unsigned int Extract40_L;   /* Complexity Weight of 1 */
    unsigned int L_Extract40;   /* Complexity Weight of 1 */

    unsigned int L40_round;    /* Complexity Weight of 1 */
    unsigned int L_saturate40; /* Complexity Weight of 1 */
    unsigned int round40;      /* Complexity Weight of 1 */
    unsigned int If;           /* Complexity Weight of 3 */
    unsigned int Goto;         /* Complexity Weight of 2 */

    unsigned int Break;    /* Complexity Weight of 2 */
    unsigned int Switch;   /* Complexity Weight of 6 */
    unsigned int For;      /* Complexity Weight of 3 */
    unsigned int While;    /* Complexity Weight of 3 */
    unsigned int Continue; /* Complexity Weight of 2 */

    unsigned int L_mls;  /* Complexity Weight of 1 */
    unsigned int div_l;  /* Complexity Weight of 32 */
    unsigned int i_mult; /* Complexity Weight of 1 */

/* New complex basic operators */
#ifdef COMPLEX_OPERATOR
    unsigned int CL_shr;            /* Complexity Weight of 1 */
    unsigned int CL_shl;            /* Complexity Weight of 1 */
    unsigned int CL_add;            /* Complexity Weight of 1 */
    unsigned int CL_sub;            /* Complexity Weight of 1 */
    unsigned int CL_scale;          /* Complexity Weight of 1 */
    unsigned int CL_dscale;         /* Complexity Weight of 1 */
    unsigned int CL_msu_j;          /* Complexity Weight of 1 */
    unsigned int CL_mac_j;          /* Complexity Weight of 1 */
    unsigned int CL_move;           /* Complexity Weight of 1 */
    unsigned int CL_Extract_real;   /* Complexity Weight of 1 */
    unsigned int CL_Extract_imag;   /* Complexity Weight of 1 */
    unsigned int CL_form;           /* Complexity Weight of 1 */
    unsigned int CL_multr_32x16;    /* Complexity Weight of 2 */
    unsigned int CL_negate;         /* Complexity Weight of 1 */
    unsigned int CL_conjugate;      /* Complexity Weight of 1 */
    unsigned int CL_mul_j;          /* Complexity Weight of 1 */
    unsigned int CL_swap_real_imag; /* Complexity Weight of 1 */
    unsigned int C_add;             /* Complexity Weight of 1 */
    unsigned int C_sub;             /* Complexity Weight of 1 */
    unsigned int C_mul_j;           /* Complexity Weight of 1 */
    unsigned int C_multr;           /* Complexity Weight of 2 */
    unsigned int C_form;            /* Complexity Weight of 1 */

    unsigned int C_scale;        /* Complexity Weight of 1 */
    unsigned int CL_round32_16;  /* Complexity Weight of 1 */
    unsigned int CL_scale_32;    /* Complexity Weight of 1 */
    unsigned int CL_dscale_32;   /* Complexity Weight of 1 */
    unsigned int CL_multr_32x32; /* Complexity Weight of 2 */
    unsigned int C_mac_r;        /* Complexity Weight of 2 */
    unsigned int C_msu_r;        /* Complexity Weight of 2 */
    unsigned int C_Extract_real; /* Complexity Weight of 1 */
    unsigned int C_Extract_imag; /* Complexity Weight of 1 */
    unsigned int C_negate;       /* Complexity Weight of 1 */
    unsigned int C_conjugate;    /* Complexity Weight of 1 */
    unsigned int C_shr;          /* Complexity Weight of 1 */
    unsigned int C_shl;          /* Complexity Weight of 1 */

#endif /* #ifdef COMPLEX_OPERATOR */

/* New 64 bit basops */
#ifdef ENH_64_BIT_OPERATOR
    unsigned int move64;        /* Complexity Weight of 1 */
    unsigned int W_add_nosat;   /* Complexity Weight of 1 */
    unsigned int W_sub_nosat;   /* Complexity Weight of 1 */
    unsigned int W_shl;         /* Complexity Weight of 1 */
    unsigned int W_shr;         /* Complexity Weight of 1 */
    unsigned int W_shl_nosat;   /* Complexity Weight of 1 */
    unsigned int W_shr_nosat;   /* Complexity Weight of 1 */
    unsigned int W_mac_32_16;   /* Complexity Weight of 1 */
    unsigned int W_msu_32_16;   /* Complexity Weight of 1 */
    unsigned int W_mult_32_16;  /* Complexity Weight of 1 */
    unsigned int W_mult0_16_16; /* Complexity Weight of 1 */
    unsigned int W_mac0_16_16;  /* Complexity Weight of 1 */
    unsigned int W_msu0_16_16;  /* Complexity Weight of 1 */
    unsigned int W_mult_16_16;  /* Complexity Weight of 1 */
    unsigned int W_mac_16_16;   /* Complexity Weight of 1 */
    unsigned int W_msu_16_16;   /* Complexity Weight of 1 */
    unsigned int W_shl_sat_l;   /* Complexity Weight of 1 */
    unsigned int W_sat_l;       /* Complexity Weight of 1 */
    unsigned int W_sat_m;       /* Complexity Weight of 1 */
    unsigned int W_deposit32_l; /* Complexity Weight of 1 */
    unsigned int W_deposit32_h; /* Complexity Weight of 1 */
    unsigned int W_extract_l;   /* Complexity Weight of 1 */
    unsigned int W_extract_h;   /* Complexity Weight of 1 */
    unsigned int W_round48_L;   /* Complexity Weight of 1 */
    unsigned int W_round32_s;   /* Complexity Weight of 1 */
    unsigned int W_norm;        /* Complexity Weight of 1 */

    unsigned int W_add;         /* Complexity Weight of 1 */
    unsigned int W_sub;         /* Complexity Weight of 1 */
    unsigned int W_neg;         /* Complexity Weight of 1 */
    unsigned int W_abs;         /* Complexity Weight of 1 */
    unsigned int W_mult_32_32;  /* Complexity Weight of 1 */
    unsigned int W_mult0_32_32; /* Complexity Weight of 1 */
    unsigned int W_lshl;        /* Complexity Weight of 1 */
    unsigned int W_lshr;        /* Complexity Weight of 1 */
    unsigned int W_round64_L;   /* Complexity Weight of 1 */

#endif /* #ifdef ENH_64_BIT_OPERATOR */

#ifdef ENH_32_BIT_OPERATOR
    unsigned int Mpy_32_16_1;  /* Complexity Weight of 1 */
    unsigned int Mpy_32_16_r;  /* Complexity Weight of 1 */
    unsigned int Mpy_32_32;    /* Complexity Weight of 1 */
    unsigned int Mpy_32_32_r;  /* Complexity Weight of 1 */
    unsigned int Madd_32_16;   /* Complexity Weight of 1 */
    unsigned int Madd_32_16_r; /* Complexity Weight of 1 */
    unsigned int Msub_32_16;   /* Complexity Weight of 1 */
    unsigned int Msub_32_16_r; /* Complexity Weight of 1 */
    unsigned int Madd_32_32;   /* Complexity Weight of 1 */
    unsigned int Madd_32_32_r; /* Complexity Weight of 1 */
    unsigned int Msub_32_32;   /* Complexity Weight of 1 */
    unsigned int Msub_32_32_r; /* Complexity Weight of 1 */
#endif                         /* #ifdef ENH_32_BIT_OPERATOR */

#ifdef ENH_U_32_BIT_OPERATOR
    unsigned int UL_addNs;      /* Complexity Weight of 1 */
    unsigned int UL_subNs;      /* Complexity Weight of 1 */
    unsigned int UL_Mpy_32_32;  /* Complexity Weight of 1 */
    unsigned int Mpy_32_32_uu;  /* Complexity Weight of 2 */
    unsigned int Mpy_32_16_uu;  /* Complexity Weight of 2 */
    unsigned int norm_ul_float; /* Complexity Weight of 1 */
    unsigned int UL_deposit_l;  /* Complexity Weight of 1 */
#endif                          /* #ifdef ENH_U_32_BIT_OPERATOR */

#ifdef CONTROL_CODE_OPS
    unsigned int LT_16; /* Complexity Weight of 1 */
    unsigned int GT_16; /* Complexity Weight of 1 */
    unsigned int LE_16; /* Complexity Weight of 1 */
    unsigned int GE_16; /* Complexity Weight of 1 */
    unsigned int EQ_16; /* Complexity Weight of 1 */
    unsigned int NE_16; /* Complexity Weight of 1 */
    unsigned int LT_32; /* Complexity Weight of 1 */
    unsigned int GT_32; /* Complexity Weight of 1 */
    unsigned int LE_32; /* Complexity Weight of 1 */
    unsigned int GE_32; /* Complexity Weight of 1 */
    unsigned int EQ_32; /* Complexity Weight of 1 */
    unsigned int NE_32; /* Complexity Weight of 1 */
    unsigned int LT_64; /* Complexity Weight of 1 */
    unsigned int GT_64; /* Complexity Weight of 1 */
    unsigned int LE_64; /* Complexity Weight of 1 */
    unsigned int GE_64; /* Complexity Weight of 1 */
    unsigned int EQ_64; /* Complexity Weight of 1 */
    unsigned int NE_64; /* Complexity Weight of 1 */

#endif /* #ifdef CONTROL_CODE_OPS */
} BASIC_OP;

#ifdef WMOPS
extern BASIC_OP *multiCounter;
extern unsigned int currCounter;
extern long funcid_total_wmops_at_last_call_to_else;
extern char func_name_where_last_call_to_else_occurred[];

long TotalWeightedOperation( unsigned int counterId );
long DeltaWeightedOperation( unsigned int counterId );
void Reset_BASOP_WMOPS_counter( unsigned int counterId );

#endif

/*****************************************************************************
 *
 *  Function Name : FOR
 *
 *  Purpose :
 *
 *    The macro FOR should be used instead of the 'for' C statement.
 *    The complexity is independent of the number of loop iterations that are
 *    performed.
 *
 *  Complexity weight : 3 (regardless of number of iterations).
 *
 *****************************************************************************/
#ifndef WMOPS
#define FOR( a ) for ( a )

#else /* ifndef WMOPS */
#define FOR( a )        \
    if ( incrFor(), 0 ) \
        ;               \
    else                \
        for ( a )

static __inline void incrFor( void )
{
    multiCounter[currCounter].For++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : WHILE
 *
 *  Purpose :
 *
 *    The macro WHILE should be used instead of the 'while' C statement.
 *    The complexity is proportional to the number of loop iterations that
 *    are performed.
 *
 *  Complexity weight : 4 x 'number of loop iterations'.
 *
 *****************************************************************************/
#ifndef WMOPS
#define WHILE( a ) while ( a )

#else /* ifndef WMOPS */
#define WHILE( a ) while ( incrWhile(), a )

static __inline void incrWhile( void )
{
    multiCounter[currCounter].While++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : DO
 *
 *  Purpose :
 *
 *    The macro DO should be used instead of the 'do' C statement.
 *
 *  Complexity weight : 0 (complexity counted by WHILE macro).
 *
 *****************************************************************************/
#ifndef WMOPS
#define DO do

#else /* ifndef WMOPS */
#define DO do

#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : IF
 *
 *  Purpose :
 *
 *    The macro IF should :
 *
 *    - not be used when :
 *      - the 'if' structure does not have any 'else if' nor 'else' statement
 *      - and it conditions only one DSP basic operations.
 *
 *    - be used instead of the 'if' C statement in every other case :
 *      - when there is an 'else' or 'else if' statement,
 *      - or when the 'if' conditions several DSP basic operations,
 *      - or when the 'if' conditions a function call.
 *
 *  Complexity weight : 3
 *
 *****************************************************************************/

#ifndef WMOPS
#define IF( a ) if ( a )
#else /* ifndef WMOPS */
#define IF( a ) if ( incrIf( __func__ ), a )
void incrIf( const char *func_name );
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : ELSE
 *
 *  Purpose :
 *
 *    The macro ELSE should be used instead of the 'else' C statement.
 *
 *  Complexity weight : 3
 *
 *****************************************************************************/

#ifndef WMOPS
#define ELSE else
#else /* ifndef WMOPS */
#define ELSE                             \
    else if ( incrElse( __func__ ), 0 ); \
    else
void incrElse( const char *func_name );
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : SWITCH
 *
 *  Purpose :
 *
 *    The macro SWITCH should be used instead of the 'switch' C statement.
 *
 *  Complexity weight : 6
 *
 *****************************************************************************/
#ifndef WMOPS
#define SWITCH( a ) switch ( a )

#else /* ifndef WMOPS */
#define SWITCH( a ) switch ( incrSwitch(), a )

static __inline void incrSwitch( void )
{
    multiCounter[currCounter].Switch++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : CONTINUE
 *
 *  Purpose :
 *
 *    The macro CONTINUE should be used instead of the 'continue' C statement.
 *
 *  Complexity weight : 2
 *
 *****************************************************************************/
#ifndef WMOPS
#define CONTINUE continue

#else /* ifndef WMOPS */
#define CONTINUE             \
    if ( incrContinue(), 0 ) \
        ;                    \
    else                     \
        continue

static __inline void incrContinue( void )
{
    multiCounter[currCounter].Continue++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : BREAK
 *
 *  Purpose :
 *
 *    The macro BREAK should be used instead of the 'break' C statement.
 *
 *  Complexity weight : 2
 *
 *****************************************************************************/
#ifndef WMOPS
#define BREAK break

#else /* ifndef WMOPS */
#define BREAK             \
    if ( incrBreak(), 0 ) \
        ;                 \
    else                  \
        break

static __inline void incrBreak( void )
{
    multiCounter[currCounter].Break++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : GOTO
 *
 *  Purpose :
 *
 *    The macro GOTO should be used instead of the 'goto' C statement.
 *
 *  Complexity weight : 2
 *
 *****************************************************************************/
#ifndef WMOPS
#define GOTO goto

#else /* ifndef WMOPS */
#define GOTO             \
    if ( incrGoto(), 0 ) \
        ;                \
    else                 \
        goto

static __inline void incrGoto( void )
{
    multiCounter[currCounter].Goto++;
}
#endif /* ifndef WMOPS */


#ifdef CONTROL_CODE_OPS

extern int LT_16( short var1, short var2 );
extern int GT_16( short var1, short var2 );
extern int LE_16( short var1, short var2 );
extern int GE_16( short var1, short var2 );
extern int EQ_16( short var1, short var2 );
extern int NE_16( short var1, short var2 );

extern int LT_32( int L_var1, int L_var2 );
extern int GT_32( int L_var1, int L_var2 );
extern int LE_32( int L_var1, int L_var2 );
extern int GE_32( int L_var1, int L_var2 );
extern int EQ_32( int L_var1, int L_var2 );
extern int NE_32( int L_var1, int L_var2 );

extern int LT_64( long long int L64_var1, long long int L64_var2 );
extern int GT_64( long long int L64_var1, long long int L64_var2 );
extern int LE_64( long long int L64_var1, long long int L64_var2 );
extern int GE_64( long long int L64_var1, long long int L64_var2 );
extern int EQ_64( long long int L64_var1, long long int L64_var2 );
extern int NE_64( long long int L64_var1, long long int L64_var2 );

#endif /* #ifdef CONTROL_CODE_OPS */


#endif /* WMOPS_H */
