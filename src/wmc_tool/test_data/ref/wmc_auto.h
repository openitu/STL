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

/* Real-time relationships */
#define FRAMES_PER_SECOND 50.0
#define MILLION_CYCLES    1e6
#define WMOPS_BOOST_FAC   ( 1.0f ) /* scaling factor for equalizing the difference between automatic and manual instrumentation */
#define FAC               ( FRAMES_PER_SECOND / MILLION_CYCLES * WMOPS_BOOST_FAC )
#define NUM_INST          20 /* Total number of instruction types (in enum below) */


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
    _MISC
};

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

#define _ADD_P      1
#define _ABS_P      1
#define _MULT_P     1
#define _MAC_P      1
#define _MOVE_P     1
#define _STORE_P    0
#define _LOGIC_P    1
#define _SHIFT_P    1
#define _BRANCH_P   2
#define _DIV_P      2
#define _SQRT_P     2
#define _TRANS_P    2
#define _FUNC_P     2 /* need to add number of arguments */
#define _LOOP_P     1
#define _INDIRECT_P 2
#define _PTR_INIT_P 1
#define _TEST_P     1
#define _POWER_P    2
#define _LOG_P      2
#define _MISC_P     1

#define ADD( x )                                    \
    {                                               \
        {                                           \
            ops_cnt += ( _ADD_C * ( x ) );          \
            inst_cnt[_ADD] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _ADD_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define ABS( x )                                    \
    {                                               \
        {                                           \
            ops_cnt += ( _ABS_C * ( x ) );          \
            inst_cnt[_ABS] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _ABS_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define MULT( x )                                    \
    {                                                \
        {                                            \
            ops_cnt += ( _MULT_C * ( x ) );          \
            inst_cnt[_MULT] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _MULT_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define MAC( x )                                    \
    {                                               \
        {                                           \
            ops_cnt += ( _MAC_C * ( x ) );          \
            inst_cnt[_MAC] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _MAC_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define MOVE( x )                                    \
    {                                                \
        {                                            \
            ops_cnt += ( _MOVE_C * ( x ) );          \
            inst_cnt[_MOVE] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _MOVE_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define STORE( x )                                    \
    {                                                 \
        {                                             \
            ops_cnt += ( _STORE_C * ( x ) );          \
            inst_cnt[_STORE] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _STORE_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define LOGIC( x )                                    \
    {                                                 \
        {                                             \
            ops_cnt += ( _LOGIC_C * ( x ) );          \
            inst_cnt[_LOGIC] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _LOGIC_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define SHIFT( x )                                    \
    {                                                 \
        {                                             \
            ops_cnt += ( _SHIFT_C * ( x ) );          \
            inst_cnt[_SHIFT] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _SHIFT_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define BRANCH( x )                                    \
    {                                                  \
        {                                              \
            ops_cnt += ( _BRANCH_C * ( x ) );          \
            inst_cnt[_BRANCH] += ( x );                \
            {                                          \
                static int pcnt;                       \
                if ( !pcnt )                           \
                {                                      \
                    pcnt = 1;                          \
                    prom_cnt += ( _BRANCH_P * ( x ) ); \
                }                                      \
            }                                          \
        }                                              \
    }
#define DIV( x )                                    \
    {                                               \
        {                                           \
            ops_cnt += ( _DIV_C * ( x ) );          \
            inst_cnt[_DIV] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _DIV_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define SQRT( x )                                    \
    {                                                \
        {                                            \
            ops_cnt += ( _SQRT_C * ( x ) );          \
            inst_cnt[_SQRT] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _SQRT_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define TRANS( x )                                    \
    {                                                 \
        {                                             \
            ops_cnt += ( _TRANS_C * ( x ) );          \
            inst_cnt[_TRANS] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _TRANS_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define LOOP( x )                                    \
    {                                                \
        {                                            \
            ops_cnt += ( _LOOP_C * ( x ) );          \
            inst_cnt[_LOOP] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _LOOP_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define INDIRECT( x )                                    \
    {                                                    \
        {                                                \
            ops_cnt += ( _INDIRECT_C * ( x ) );          \
            inst_cnt[_INDIRECT] += ( x );                \
            {                                            \
                static int pcnt;                         \
                if ( !pcnt )                             \
                {                                        \
                    pcnt = 1;                            \
                    prom_cnt += ( _INDIRECT_P * ( x ) ); \
                }                                        \
            }                                            \
        }                                                \
    }
#define PTR_INIT( x )                                    \
    {                                                    \
        {                                                \
            ops_cnt += ( _PTR_INIT_C * ( x ) );          \
            inst_cnt[_PTR_INIT] += ( x );                \
            {                                            \
                static int pcnt;                         \
                if ( !pcnt )                             \
                {                                        \
                    pcnt = 1;                            \
                    prom_cnt += ( _PTR_INIT_P * ( x ) ); \
                }                                        \
            }                                            \
        }                                                \
    }
#define TEST( x )                                    \
    {                                                \
        {                                            \
            ops_cnt += ( _TEST_C * ( x ) );          \
            inst_cnt[_TEST] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _TEST_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define POWER( x )                                    \
    {                                                 \
        {                                             \
            ops_cnt += ( _POWER_C * ( x ) );          \
            inst_cnt[_POWER] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _POWER_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define LOG( x )                                    \
    {                                               \
        {                                           \
            ops_cnt += ( _LOG_C * ( x ) );          \
            inst_cnt[_LOG] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _LOG_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define MISC( x )                                    \
    {                                                \
        {                                            \
            ops_cnt += ( _MISC_C * ( x ) );          \
            inst_cnt[_MISC] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _MISC_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }

#define FUNC( x )                                              \
    {                                                          \
        {                                                      \
            ops_cnt += ( _FUNC_C + _MOVE_C * ( x ) );          \
            inst_cnt[_FUNC]++;                                 \
            inst_cnt[_MOVE] += ( x );                          \
            {                                                  \
                static int pcnt;                               \
                if ( !pcnt )                                   \
                {                                              \
                    pcnt = 1;                                  \
                    prom_cnt += ( _FUNC_P + _MOVE_P * ( x ) ); \
                }                                              \
            }                                                  \
        }                                                      \
    }

#define DADD( x )                                   \
    {                                               \
        {                                           \
            ops_cnt += ( 2 * _ADD_C * ( x ) );      \
            inst_cnt[_ADD] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _ADD_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define DMULT( x )                                   \
    {                                                \
        {                                            \
            ops_cnt += ( 2 * _MULT_C * ( x ) );      \
            inst_cnt[_MULT] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _MULT_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define DMAC( x )                                   \
    {                                               \
        {                                           \
            ops_cnt += ( 2 * _MAC_C * ( x ) );      \
            inst_cnt[_MAC] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _MAC_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define DMOVE( x )                                   \
    {                                                \
        {                                            \
            ops_cnt += ( 2 * _MOVE_C * ( x ) );      \
            inst_cnt[_MOVE] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _MOVE_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define DSTORE( x )                                   \
    {                                                 \
        {                                             \
            ops_cnt += ( 2 * _STORE_C * ( x ) );      \
            inst_cnt[_STORE] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _STORE_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define DLOGIC( x )                                   \
    {                                                 \
        {                                             \
            ops_cnt += ( 2 * _LOGIC_C * ( x ) );      \
            inst_cnt[_LOGIC] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _LOGIC_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define DSHIFT( x )                                   \
    {                                                 \
        {                                             \
            ops_cnt += ( 2 * _SHIFT_C * ( x ) );      \
            inst_cnt[_SHIFT] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _SHIFT_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }
#define DDIV( x )                                   \
    {                                               \
        {                                           \
            ops_cnt += ( 2 * _DIV_C * ( x ) );      \
            inst_cnt[_DIV] += ( x );                \
            {                                       \
                static int pcnt;                    \
                if ( !pcnt )                        \
                {                                   \
                    pcnt = 1;                       \
                    prom_cnt += ( _DIV_P * ( x ) ); \
                }                                   \
            }                                       \
        }                                           \
    }
#define DSQRT( x )                                   \
    {                                                \
        {                                            \
            ops_cnt += ( 2 * _SQRT_C * ( x ) );      \
            inst_cnt[_SQRT] += ( x );                \
            {                                        \
                static int pcnt;                     \
                if ( !pcnt )                         \
                {                                    \
                    pcnt = 1;                        \
                    prom_cnt += ( _SQRT_P * ( x ) ); \
                }                                    \
            }                                        \
        }                                            \
    }
#define DTRANS( x )                                   \
    {                                                 \
        {                                             \
            ops_cnt += ( 2 * _TRANS_C * ( x ) );      \
            inst_cnt[_TRANS] += ( x );                \
            {                                         \
                static int pcnt;                      \
                if ( !pcnt )                          \
                {                                     \
                    pcnt = 1;                         \
                    prom_cnt += ( _TRANS_P * ( x ) ); \
                }                                     \
            }                                         \
        }                                             \
    }

extern double ops_cnt;
extern double prom_cnt;
extern double inst_cnt[NUM_INST];
extern int ops_cnt_activ;

void reset_wmops( void );
void push_wmops( const char *label );
void pop_wmops( void );
void update_wmops( void );
void update_mem( void );
void print_wmops( void );

#else /* WMOPS counting disabled */

#define reset_wmops()
extern int cntr_push_pop;
#define push_wmops( x ) ( cntr_push_pop++ )
#define pop_wmops()     ( cntr_push_pop-- )
#define update_wmops()  ( assert( cntr_push_pop == 0 ) )
#define update_mem()
#define print_wmops()

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

/* mac & msu (Non Instrumented Versions) */
#ifndef mac
#define mac( a, b, c ) ( ( a ) + ( b ) * ( c ) )
#endif
#ifndef mac
#define msu( a, b, c ) ( ( a ) - ( b ) * ( c ) )
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

/* '*ops_cnt_ptr' is Used to Avoid: "warning: operation on 'ops_cnt' may be undefined" with Cygwin gcc Compiler */
static double *ops_cnt_ptr = &ops_cnt;
#define OP_COUNT_( op, x )            ( *ops_cnt_ptr += ( op##_C * ( x ) ), inst_cnt[op] += ( x ) )

/******************************************************************/
/* NOTES:                                                         */
/*   The 'wmc_flag_' flag is global to avoid declaration in every */
/*   function and 'static' to avoid clashing with other modules   */
/*   that include this header file.                               */
/*                                                                */
/*   The declarations of 'wmc_flag_' and 'wops_' in this header   */
/*   file prevent the addition of a 'C' file to the Project.      */
/******************************************************************/

/* General Purpose Global Flag */
static int wmc_flag_ = 0;

/* Operation Counter Wrappers */
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
#define ABS_( x )      OP_COUNT_( _ABS, ( x ) / WMOPS_BOOST_FAC )
#define ADD_( x )      OP_COUNT_( _ADD, ( x ) / WMOPS_BOOST_FAC )
#define MULT_( x )     OP_COUNT_( _MULT, ( x ) / WMOPS_BOOST_FAC )
#define MAC_( x )      OP_COUNT_( _MAC, ( x ) / WMOPS_BOOST_FAC )
#define MOVE_( x )     OP_COUNT_( _MOVE, ( x ) / WMOPS_BOOST_FAC )
#define STORE_( x )    OP_COUNT_( _STORE, ( x ) / WMOPS_BOOST_FAC )
#define LOGIC_( x )    OP_COUNT_( _LOGIC, ( x ) / WMOPS_BOOST_FAC )
#define SHIFT_( x )    OP_COUNT_( _SHIFT, ( x ) / WMOPS_BOOST_FAC )
#define BRANCH_( x )   OP_COUNT_( _BRANCH, ( x ) / WMOPS_BOOST_FAC )
#define DIV_( x )      OP_COUNT_( _DIV, ( x ) / WMOPS_BOOST_FAC )
#define SQRT_( x )     OP_COUNT_( _SQRT, ( x ) / WMOPS_BOOST_FAC )
#define TRANS_( x )    OP_COUNT_( _TRANS, ( x ) / WMOPS_BOOST_FAC )
#define POWER_( x )    TRANS_( x )
#define LOG_( x )      TRANS_( x )
#define LOOP_( x )     OP_COUNT_( _LOOP, ( x ) / WMOPS_BOOST_FAC )
#define INDIRECT_( x ) OP_COUNT_( _INDIRECT, ( x ) / WMOPS_BOOST_FAC )
#define PTR_INIT_( x ) OP_COUNT_( _PTR_INIT, ( x ) / WMOPS_BOOST_FAC )
#define FUNC_( x )     ( OP_COUNT_( _MOVE, ( x ) / WMOPS_BOOST_FAC ), OP_COUNT_( _FUNC, 1 ) )
#define MISC_( x )     ABS_( x )

/* Math Operations */
#define abs_   OP_COUNT_WRAPPER1_( ABS_( 1 ), abs )
#define fabs_  OP_COUNT_WRAPPER1_( ABS_( 1 ), fabs )
#define labs_  OP_COUNT_WRAPPER1_( ABS_( 1 ), labs )
#define floor_ OP_COUNT_WRAPPER1_( MISC_( 1 ), floor )
#define sqrt_  OP_COUNT_WRAPPER1_( SQRT_( 1 ), sqrt )
#define pow_   OP_COUNT_WRAPPER1_( POWER_( 1 ), pow )
#define exp_   OP_COUNT_WRAPPER1_( POWER_( 1 ), exp )
#define log_   OP_COUNT_WRAPPER1_( LOG_( 1 ), log )
#define log10_ OP_COUNT_WRAPPER1_( LOG_( 1 ), log10 )
#define cos_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), cos )
#define sin_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), sin )
#define tan_   OP_COUNT_WRAPPER1_( TRANS_( 1 ), tan )
#define acos_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), acos )
#define asin_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), asin )
#define atan_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), atan )
#define atan2_ OP_COUNT_WRAPPER1_( TRANS_( 1 ), atan2 )
#define cosh_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), cosh )
#define sinh_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), sinh )
#define tanh_  OP_COUNT_WRAPPER1_( TRANS_( 1 ), tanh )
#define fmod_  OP_COUNT_WRAPPER1_( DIV_( 1 ), fmod )
/* these macros use any local macros already defined */
/* min/max and their Variants */
#define min_( a, b ) OP_COUNT_WRAPPER1_( MISC_( 1 ), min( ( a ), ( b ) ) )
#define max_( a, b ) OP_COUNT_WRAPPER1_( MISC_( 1 ), max( ( a ), ( b ) ) )
#define MIN_( a, b ) OP_COUNT_WRAPPER1_( MISC_( 1 ), MIN( ( a ), ( b ) ) )
#define MAX_( a, b ) OP_COUNT_WRAPPER1_( MISC_( 1 ), MAX( ( a ), ( b ) ) )
#define Min_( a, b ) OP_COUNT_WRAPPER1_( MISC_( 1 ), Min( ( a ), ( b ) ) )
#define Max_( a, b ) OP_COUNT_WRAPPER1_( MISC_( 1 ), Max( ( a ), ( b ) ) )
/* Square and its Variants */
#define sqr_( x )    OP_COUNT_WRAPPER1_( MULT_( 1 ), sqr( ( x ) ) )
#define Sqr_( x )    OP_COUNT_WRAPPER1_( MULT_( 1 ), Sqr( ( x ) ) )
#define SQR_( x )    OP_COUNT_WRAPPER1_( MULT_( 1 ), SQR( ( x ) ) )
#define square_( x ) OP_COUNT_WRAPPER1_( MULT_( 1 ), square( ( x ) ) )
#define Square_( x ) OP_COUNT_WRAPPER1_( MULT_( 1 ), Square( ( x ) ) )
#define SQUARE_( x ) OP_COUNT_WRAPPER1_( MULT_( 1 ), SQUARE( ( x ) ) )
/* Sign and its Variants */
#define sign_( x ) OP_COUNT_WRAPPER1_( MOVE_( 1 ), sign( ( x ) ) )
#define Sign_( x ) OP_COUNT_WRAPPER1_( MOVE_( 1 ), Sign( ( x ) ) )
#define SIGN_( x ) OP_COUNT_WRAPPER1_( MOVE_( 1 ), SIGN( ( x ) ) )
/* Square Root and its Variants */
#define sqrtf_( x ) OP_COUNT_WRAPPER1_( SQRT_( 1 ), sqrtf( ( x ) ) )
/* Invert Square Root and its Variants */
#define inv_sqrt_( x ) OP_COUNT_WRAPPER1_( SQRT_( 1 ), inv_sqrt( ( x ) ) )
/* Others */
#define log_base_2_( x ) OP_COUNT_WRAPPER1_( ( LOG_( 1 ), MULT_( 1 ) ), log_base_2( ( x ) ) )
#define log2_f_( x )     OP_COUNT_WRAPPER1_( ( LOG_( 1 ), MULT_( 1 ) ), log2_f( ( x ) ) )
/* The 'wmc_flag_=wmc_flag_' is Used to Avoid: "warning: left-hand operand of comma expression has no effect"
   with Cygwin gcc Compiler */
#define _round_( x )  OP_COUNT_WRAPPER1_( wmc_flag_ = wmc_flag_, _round( ( x ) ) )
#define round_f_( x ) OP_COUNT_WRAPPER1_( wmc_flag_ = wmc_flag_, round_f( ( x ) ) )
#define _squant_( x ) OP_COUNT_WRAPPER1_( wmc_flag_ = wmc_flag_, _squant( ( x ) ) )
/* Set Min/Max */
#define set_min_( a, b ) OP_COUNT_WRAPPER3_( ( ADD_( 1 ), BRANCH_( 1 ), MOVE_( 1 ) ), set_min( ( a ), ( b ) ) )
#define set_max_( a, b ) OP_COUNT_WRAPPER3_( ( ADD_( 1 ), BRANCH_( 1 ), MOVE_( 1 ) ), set_max( ( a ), ( b ) ) )
/* mac & msu (Instrumented Versions) */
#define mac_( a, b, c ) OP_COUNT_WRAPPER1_( MAC_( 1 ), mac( a, b, c ) )
#define msu_( a, b, c ) OP_COUNT_WRAPPER1_( MAC_( 1 ), msu( a, b, c ) )

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
                ops_cnt_ptr = ops_cnt_ptr;
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
 *   {"../lib_com/*.c", 0, NULL},
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

#if ( defined( _WIN32 ) && ( _MSC_VER <= 1800 ) && ( _MSC_VER >= 1300 ) )
#define __func__ __FUNCTION__
#elif defined( __STDC_VERSION__ ) && __STDC_VERSION__ < 199901L
#if ( __GNUC__ >= 2 )
#define __func__ __FUNCTION__
#else
#define __func__ "<unknown>"
#endif
#elif defined( __GNUC__ )
#define __func__ __extension__ __FUNCTION__
#endif


#ifdef WMOPS

void *mem_alloc( const char *func_name, int func_lineno, size_t size, char *alloc_str );
void mem_free( const char *func_name, int func_lineno, void *ptr );

#define malloc_( size )    mem_alloc( __func__, __LINE__, size, "m:" #size )
#define calloc_( n, size ) mem_alloc( __func__, __LINE__, ( n ) * ( size ), "c:" #n ", " #size )
#define free_( ptr )       mem_free( __func__, __LINE__, ptr )

void reset_mem( Counting_Size cnt_size );
void print_mem( ROM_Size_Lookup_Table Const_Data_PROM_Table[] );
#ifdef MEM_COUNT_DETAILS
void export_mem( const char *csv_filename );
#endif

int push_stack( const char *filename, const char *fctname );
int pop_stack( const char *filename, const char *fctname );

#ifdef WMOPS_DETAIL
#define STACK_DEPTH_FCT_CALL   ( push_wmops( __FUNCTION__ ), push_stack( __FILE__, __FUNCTION__ ) ) /* add push_wmops() in all function calls */
#define STACK_DEPTH_FCT_RETURN ( pop_wmops(), pop_stack( __FILE__, __FUNCTION__ ) )                 /* add pop_wmops() in all function returns */
#else
#define STACK_DEPTH_FCT_CALL   push_stack( __FILE__, __FUNCTION__ )
#define STACK_DEPTH_FCT_RETURN pop_stack( __FILE__, __FUNCTION__ )
#endif

void reset_stack( void );
#define func_start_ int stack_tree_level_ = STACK_DEPTH_FCT_CALL;

#else
#define malloc_( n1 )     malloc( n1 )
#define calloc_( n1, n2 ) calloc( n1, n2 )
#define free_( ptr )      free( ptr )
#define reset_mem( cnt_size )
#define print_mem( Const_Data_PROM_Table )
#define export_mem( csv_filename )

#define push_stack( file, fct )
#define pop_stack( file, fct )
#define reset_stack()
#define func_start_

#endif

#endif /* WMOPS_H */

