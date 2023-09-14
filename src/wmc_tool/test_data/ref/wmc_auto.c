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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#ifndef _MSC_VER
#include <dirent.h>
#include <errno.h>
#else
#include <windows.h>
#endif

#include "wmc_auto.h"


#define WMC_TOOL_SKIP /* Skip the instrumentation of this file, if invoked by accident */

#ifdef WMOPS

/*-------------------------------------------------------------------*
 * Complexity counting tool
 *--------------------------------------------------------------------*/

#define MAX_RECORDS 1024
#define MAX_CHAR    64
#define MAX_STACK   64
#define DOUBLE_MAX  0x80000000

struct wmops_record
{
    char label[MAX_CHAR];
    long call_number;
    long update_cnt;
    int call_tree[MAX_RECORDS];
    double start_selfcnt;
    double current_selfcnt;
    double max_selfcnt;
    double min_selfcnt;
    double tot_selfcnt;
    double start_cnt; /* The following take into account the decendants */
    double current_cnt;
    double max_cnt;
    double min_cnt;
    double tot_cnt;
#ifdef WMOPS_WC_FRAME_ANALYSIS
    int32_t current_call_number;
    double wc_cnt;
    double wc_selfcnt;
    int32_t wc_call_number;
#endif
};

double ops_cnt;
double prom_cnt;
double inst_cnt[NUM_INST];

static struct wmops_record wmops[MAX_RECORDS];
static int stack[MAX_STACK];
static int sptr;
static int num_records;
static int current_record;
static long update_cnt;
static double start_cnt;
static double max_cnt;
static double min_cnt;
static double inst_cnt_wc[NUM_INST];
static long fnum_cnt_wc;

static int *heap_allocation_call_tree = NULL, heap_allocation_call_tree_size = 0, heap_allocation_call_tree_max_size = 0;


void reset_wmops( void )
{
    int i, j;

    for ( i = 0; i < MAX_RECORDS; i++ )
    {
        strcpy( &wmops[i].label[0], "\0" );
        wmops[i].call_number = 0;
        wmops[i].update_cnt = 0;
        for ( j = 0; j < MAX_RECORDS; j++ )
        {
            wmops[i].call_tree[j] = -1;
        }
        wmops[i].start_selfcnt = 0.0;
        wmops[i].current_selfcnt = 0.0;
        wmops[i].max_selfcnt = 0.0;
        wmops[i].min_selfcnt = DOUBLE_MAX;
        wmops[i].tot_selfcnt = 0.0;
        wmops[i].start_cnt = 0.0;
        wmops[i].current_cnt = 0.0;
        wmops[i].max_cnt = 0.0;
        wmops[i].min_cnt = DOUBLE_MAX;
        wmops[i].tot_cnt = 0.0;
#ifdef WMOPS_WC_FRAME_ANALYSIS
        wmops[i].wc_cnt = 0.0;
        wmops[i].wc_selfcnt = 0.0;
        wmops[i].current_call_number = 0;
#endif
    }

    for ( i = 0; i < MAX_STACK; i++ )
    {
        stack[i] = -1;
    }
    sptr = 0;
    num_records = 0;
    current_record = -1;
    update_cnt = 0;

    max_cnt = 0.0;
    min_cnt = DOUBLE_MAX;
    start_cnt = 0.0;
    ops_cnt = 0.0;
}


void push_wmops( const char *label )
{
    int new_flag;
    int i, j;

    /* Check if new function record label */
    new_flag = 1;
    for ( i = 0; i < num_records; i++ )
    {
        if ( strcmp( wmops[i].label, label ) == 0 )
        {
            new_flag = 0;
            break;
        }
    }

    /* Configure new record */
    if ( new_flag )
    {
        if ( num_records >= MAX_RECORDS )
        {
            fprintf( stdout, "push_wmops(): exceeded MAX_RECORDS count.\n\n" );
            exit( -1 );
        }
        strcpy( wmops[i].label, label );
        num_records++;
    }

    /* Push current context onto stack */
    if ( current_record >= 0 )
    {
        if ( sptr >= MAX_STACK )
        {
            fprintf( stdout, "\r push_wmops(): stack exceeded, try inreasing MAX_STACK\n" );
            exit( -1 );
        }
        stack[sptr++] = current_record;

        /* accumulate op counts */
        wmops[current_record].current_selfcnt += ops_cnt - wmops[current_record].start_selfcnt;

        /* update call tree */
        for ( j = 0; j < MAX_RECORDS; j++ )
        {
            if ( wmops[i].call_tree[j] == current_record )
            {
                break;
            }
            else if ( wmops[i].call_tree[j] == -1 )
            {
                wmops[i].call_tree[j] = current_record;
                break;
            }
        }
    }

    /* init current record */
    current_record = i;
    wmops[current_record].start_selfcnt = ops_cnt;
    wmops[current_record].start_cnt = ops_cnt;
    wmops[current_record].call_number++;
#ifdef WMOPS_WC_FRAME_ANALYSIS
    wmops[current_record].current_call_number++;
#endif

    return;
}


void pop_wmops( void )
{

    /* Check for underflow */
    if ( current_record < 0 )
    {
        fprintf( stdout, "\r pop_wmops(): stack underflow, too many calls to pop_wmops()\n" );
        exit( -1 );
    }

    /* update count of current record */
    wmops[current_record].current_selfcnt += ops_cnt - wmops[current_record].start_selfcnt;
    wmops[current_record].current_cnt += ops_cnt - wmops[current_record].start_cnt;

    /* Get back previous context from stack */
    if ( sptr > 0 )
    {
        current_record = stack[--sptr];
        wmops[current_record].start_selfcnt = ops_cnt;
    }
    else
    {
        current_record = -1;
    }

    return;
}


void update_wmops( void )
{
    int i;
    double current_cnt;
#ifdef WMOPS_PER_FRAME
    static FILE *fid = NULL;
    const char filename[] = "wmops_analysis";
    float tmpF;
#endif

    if ( sptr != 0 )
    {
        fprintf( stdout, "update_wmops(): Stack must be empty!\n" );
        exit( -1 );
    }

#ifdef WMOPS_PER_FRAME
    /* Check, if the output file has already been opened */
    if ( fid == NULL )
    {
        fid = fopen( filename, "wb" );

        if ( fid == NULL )
        {
            fprintf( stderr, "\nCannot open %s!\n\n", filename );
            exit( -1 );
        }
    }

    /* Write current complexity to the external file */
    tmpF = (float) ( FAC * wmops[0].current_cnt );
    fwrite( &tmpF, sizeof( float ), 1, fid );
#endif

#ifdef WMOPS_WC_FRAME_ANALYSIS
    if ( ops_cnt - start_cnt > max_cnt )
    {
        for ( i = 0; i < num_records; i++ )
        {
            wmops[i].wc_cnt = wmops[i].current_cnt;
            wmops[i].wc_selfcnt = wmops[i].current_selfcnt;
            wmops[i].wc_call_number = wmops[i].current_call_number;
        }
    }
#endif

    for ( i = 0; i < num_records; i++ )
    {
        wmops[i].tot_selfcnt += wmops[i].current_selfcnt;
        wmops[i].tot_cnt += wmops[i].current_cnt;

        if ( wmops[i].current_selfcnt > 0 )
        {
            if ( wmops[i].current_selfcnt > wmops[i].max_selfcnt )
            {
                wmops[i].max_selfcnt = wmops[i].current_selfcnt;
            }

            if ( wmops[i].current_selfcnt < wmops[i].min_selfcnt )
            {
                wmops[i].min_selfcnt = wmops[i].current_selfcnt;
            }
        }

        wmops[i].current_selfcnt = 0;

        if ( wmops[i].current_cnt > 0 )
        {
            if ( wmops[i].current_cnt > wmops[i].max_cnt )
            {
                wmops[i].max_cnt = wmops[i].current_cnt;
            }

            if ( wmops[i].current_cnt < wmops[i].min_cnt )
            {
                wmops[i].min_cnt = wmops[i].current_cnt;
            }

            wmops[i].update_cnt++;
        }

        wmops[i].current_cnt = 0;
#ifdef WMOPS_WC_FRAME_ANALYSIS
        wmops[i].current_call_number = 0;
#endif
    }

    current_cnt = ops_cnt - start_cnt;
    if ( current_cnt > max_cnt )
    {
        max_cnt = current_cnt;

        for ( i = 0; i < NUM_INST; i++ )
        {
            inst_cnt_wc[i] = inst_cnt[i];
        }

        fnum_cnt_wc = update_cnt + 1;
    }

    if ( current_cnt < min_cnt )
    {
        min_cnt = current_cnt;
    }

    for ( i = 0; i < NUM_INST; i++ )
    {
        inst_cnt[i] = 0.0;
    }

    start_cnt = ops_cnt;

    /* increment frame counter */
    update_cnt++;

    return;
}


void print_wmops( void )
{
    int i, label_len, max_label_len;

    char *sfmts = "%*s %8s %8s %7s %7s\n";
    char *dfmts = "%*s %8.2f %8.3f %7.3f %7.3f\n";
    char *sfmt  = "%*s %8s %8s %7s %7s  %7s %7s %7s\n";
    char *dfmt  = "%*s %8.2f %8.3f %7.3f %7.3f  %7.3f %7.3f %7.3f\n";

#ifdef WMOPS_WC_FRAME_ANALYSIS
    int j;
    char *sfmtt = "%20s %4s %15s\n";
    char *dfmtt = "%20s %4d  ";
#endif

    /* calculate maximum label length for compact prinout */
    max_label_len = 0;
    for ( i = 0; i < num_records; i++ )
    {
        label_len = strlen( wmops[i].label );
        if ( label_len > max_label_len )
        {
            max_label_len = label_len;
        }
    }
    max_label_len += 4;

    fprintf( stdout, "\n\n --- Complexity analysis [WMOPS] ---  \n\n" );
    
    fprintf( stdout, "%*s %33s  %23s\n", max_label_len, "", "|------  SELF  ------|", "|---  CUMULATIVE  ---|" );
    fprintf( stdout, sfmt, max_label_len, "        routine", " calls", "  min ", "  max ", "  avg ", "  min ", "  max ", "  avg " );
    fprintf( stdout, sfmt, max_label_len, "---------------", "------", "------", "------", "------", "------", "------", "------" );

    for ( i = 0; i < num_records; i++ )
    {
        fprintf( stdout, dfmt, max_label_len, wmops[i].label, update_cnt == 0 ? 0 : (float) wmops[i].call_number / update_cnt,
                 wmops[i].min_selfcnt == DOUBLE_MAX ? 0 : FAC * wmops[i].min_selfcnt,
                 FAC * wmops[i].max_selfcnt,
                 wmops[i].update_cnt == 0 ? 0 : FAC * wmops[i].tot_selfcnt / wmops[i].update_cnt,
                 wmops[i].min_cnt == DOUBLE_MAX ? 0 : FAC * wmops[i].min_cnt,
                 FAC * wmops[i].max_cnt,
                 wmops[i].update_cnt == 0 ? 0 : FAC * wmops[i].tot_cnt / wmops[i].update_cnt );
    }

    fprintf( stdout, sfmts, max_label_len, "---------------", "------", "------", "------", "------" );
    fprintf( stdout, dfmts, max_label_len, "total", (float) update_cnt, update_cnt == 0 ? 0 : FAC * min_cnt, FAC * max_cnt, update_cnt == 0 ? 0 : FAC * ops_cnt / update_cnt );
    fprintf( stdout, "\n" );

#ifdef WMOPS_WC_FRAME_ANALYSIS
    fprintf( stdout, "\nComplexity analysis for the worst-case frame %ld:\n", fnum_cnt_wc );
    fprintf( stdout, "%*s %8s %10s %12s\n", max_label_len,   "        routine", " calls", " SELF", "  CUMULATIVE" );
    fprintf( stdout, "%*s %8s %10s   %10s\n", max_label_len, "---------------", "------", "------", "----------" );

    for ( i = 0; i < num_records; i++ )
    {
        fprintf( stdout, "%*s %8d %10.3f %12.3f\n", max_label_len, wmops[i].label, wmops[i].wc_call_number, FAC * wmops[i].wc_selfcnt, FAC * wmops[i].wc_cnt );
    }

    fprintf( stdout, "\nCall Tree:\n\n" );
    fprintf( stdout, sfmtt, "       function", "num", "called by:    " );
    fprintf( stdout, sfmtt, "---------------", "---", "--------------" );

    for ( i = 0; i < num_records; i++ )
    {
        fprintf( stdout, dfmtt, wmops[i].label, i );
        for ( j = 0; wmops[i].call_tree[j] != -1; j++ )
        {
            if ( j != 0 )
            {
                fprintf( stdout, ", " );
            }
            fprintf( stdout, "%d", wmops[i].call_tree[j] );
        }
        fprintf( stdout, "\n" );
    }

    fprintf( stdout, sfmtt, "---------------", "---", "--------------" );
    fprintf( stdout, "\n\n" );

    fprintf( stdout, "\nInstruction type analysis for the worst-case frame %ld:\n\n", fnum_cnt_wc ); /* added -- JPA */
    for ( i = 0; i < NUM_INST; i++ )
    {
        switch ( (enum instructions) i )
        {
            case _ADD:
                fprintf( stdout, "\tAdds:          %12.1f\n", inst_cnt_wc[i] );
                break;
            case _ABS:
                fprintf( stdout, "\tAbsolutes:     %12.1f\n", inst_cnt_wc[i] );
                break;
            case _MULT:
                fprintf( stdout, "\tMultiplies:    %12.1f\n", inst_cnt_wc[i] );
                break;
            case _MAC:
                fprintf( stdout, "\tMACs:          %12.1f\n", inst_cnt_wc[i] );
                break;
            case _MOVE:
                fprintf( stdout, "\tMoves:         %12.1f\n", inst_cnt_wc[i] );
                break;
            case _STORE:
                fprintf( stdout, "\tStores:        %12.1f\n", inst_cnt_wc[i] );
                break;
            case _LOGIC:
                fprintf( stdout, "\tLogicals:      %12.1f\n", inst_cnt_wc[i] );
                break;
            case _SHIFT:
                fprintf( stdout, "\tShifts:        %12.1f\n", inst_cnt_wc[i] );
                break;
            case _BRANCH:
                fprintf( stdout, "\tBranches:      %12.1f\n", inst_cnt_wc[i] );
                break;
            case _DIV:
                fprintf( stdout, "\tDivisions:     %12.1f\n", inst_cnt_wc[i] );
                break;
            case _SQRT:
                fprintf( stdout, "\tSquare Root:   %12.1f\n", inst_cnt_wc[i] );
                break;
            case _TRANS:
                fprintf( stdout, "\tTrans:         %12.1f\n", inst_cnt_wc[i] );
                break;
            case _FUNC:
                fprintf( stdout, "\tFunc Call:     %12.1f\n", inst_cnt_wc[i] );
                break;
            case _LOOP:
                fprintf( stdout, "\tLoop Init:     %12.1f\n", inst_cnt_wc[i] );
                break;
            case _INDIRECT:
                fprintf( stdout, "\tIndirect Addr: %12.1f\n", inst_cnt_wc[i] );
                break;
            case _PTR_INIT:
                fprintf( stdout, "\tPointer Init:  %12.1f\n", inst_cnt_wc[i] );
                break;
            case _TEST:
                fprintf( stdout, "\tExtra condit.: %12.1f\n", inst_cnt_wc[i] );
                break;
            case _POWER:
                fprintf( stdout, "\tExponential:   %12.1f\n", inst_cnt_wc[i] );
                break;
            case _LOG:
                fprintf( stdout, "\tLogarithm:     %12.1f\n", inst_cnt_wc[i] );
                break;
            case _MISC:
                fprintf( stdout, "\tAll other op.: %12.1f\n", inst_cnt_wc[i] );
                break;
            default:
                fprintf( stdout, "\tERROR: Invalid instruction type: %d\n\n", i );
        }
    }
#endif

    return;
}


/*-------------------------------------------------------------------*
 * Memory counting tool measuring RAM usage (stack and heap)
 *
 * Maximum RAM is measured by monitoring the total allocated memory (stack and heap) in each frame.
 *
 * Maximum stack is measured by monitoring the difference between the 'top' and 'bottom' of the stack. The 'bottom' of the stack is updated in each function
 * with a macro 'func_start_' which is inserted automatically to all functions during the instrumentation process.
 *
 * Maximum heap is measured by summing the sizes of all memory blocks allocated by malloc() or calloc() and deallocated by free(). The maximum heap size is
 * updated each time when the macros malloc_() or calloc_() is invoked. The macros 'malloc_ and calloc_' are inserted automatically during the instrumentation process.
 * As part of heap measurements, intra-frame heap and inter-frame heap are measured separately. Intra-frame heap refers to heap memory which is allocated and deallocated
 * within a single frame. Inter-frame heap, on the contrary, refers to heap memory which is reserved for more than one frame.
 *
 * In order to run the memory counting tool the function reset_mem(cnt_size) must be called at the beginning of the encoding/decoding process.
 * The unit in which memory consumption is reported is set via the parameter 'cnt_size'. It can be set to 0 (bytes), 1 (32b words) or 2 (64b words).
 * At the end of the encoding/decoding process, 'print_mem()' function may be called to print basic information about memory consumption. If the macro 'MEM_COUNT_DETAILS'
 * is activated, detailed information is printed
 *
 * The macro 'WMOPS' needs to be activated to enable memory counting. To avoid the instrumentation of malloc()/calloc()/free() calls, use
 * #define WMC_TOOL_SKIP ... #undef WMC_TOOL_SKIP macro pair around the malloc(), calloc() and free().
 *--------------------------------------------------------------------*/

#define MAX_RECORDABLE_CALLS         100
#define MAX_FUNCTION_NAME_LENGTH     35  /* Maximum length that the function string will be truncated to */
#define MAX_PARAMS_LENGTH            50  /* Maximum length that the parameter string will be truncated to */
#define MAX_NUM_RECORDS              300 /* Initial maximum number of memory records -> mightb be increased during runtime, if needed */
#define MAX_NUM_RECORDS_REALLOC_STEP 50  /* When re-allocating the list of memory records, increase the number of records by this number */

/* This is the value (in bytes) towards which the block size is rounded. For example, a block of 123 bytes, when using
   a 32 bits system, will end up taking 124 bytes since the last unused byte cannot be used for another block. */
#ifdef MEM_ALIGN_64BITS
#define BLOCK_ROUNDING 8 /* Align on 64 Bits */
#else
#define BLOCK_ROUNDING 4 /* Align on 32 Bits */
#endif

#define N_32BITS_BLOCKS ( BLOCK_ROUNDING / sizeof( int32_t ) )

#define MAGIC_VALUE_OOB  0x12A534F0           /* Signature value which is inserted before and after each allocated memory block, used to detect out-of-bound access */
#define MAGIC_VALUE_USED ( ~MAGIC_VALUE_OOB ) /* Value used to pre-fill allocated memory blocks, used to calculate actual memory usage */
#define OOB_START        0x1                  /* Flag indicating out-of-bounds access before memory block */
#define OOB_END          0x2                  /* Flag indicating out-of-bounds access after memory block */

#define ROUND_BLOCK_SIZE( n ) ( ( ( n ) + BLOCK_ROUNDING - 1 ) & ~( BLOCK_ROUNDING - 1 ) )
#define IS_CALLOC( str )      ( str[0] == 'c' )

#ifdef MEM_COUNT_DETAILS
const char *csv_filename = "mem_analysis.csv";
static FILE *fid_csv_filename = NULL;
#endif

typedef struct
{
    char function_name[MAX_FUNCTION_NAME_LENGTH + 1];
    int16_t *stack_ptr;
} caller_info;

caller_info stack_callers[2][MAX_RECORDABLE_CALLS];

typedef struct
{
    char name[MAX_FUNCTION_NAME_LENGTH + 1]; /* +1 for NUL */
    char params[1 + MAX_PARAMS_LENGTH + 1];  /* +1 for 'm'/'c' alloc & +1 for NUL */
    unsigned long hash;
    int lineno;
    void *block_ptr;
    int block_size;
    unsigned long total_block_size; /* Cumulative sum of the allocated size in the session */
    unsigned long total_used_size;  /* Cumulative sum of the used size in the session */
    int wc_heap_size_intra_frame;   /* Worst-Case Intra-Frame Heap Size */
    int wc_heap_size_inter_frame;   /* Worst-Case Inter-Frame Heap Size */
    int frame_allocated;            /* Frame number in which the Memory Block has been allocated (-1 if not allocated at the moment) */
    int OOB_Flag;
    int noccurances; /* Number of times that the memory block has been allocated in a frame */
} allocator_record;

allocator_record *allocation_list = NULL;

static int16_t *ptr_base_stack = 0;    /* Pointer to the bottom of stack (base pointer). Stack grows up. */
static int16_t *ptr_current_stack = 0; /* Pointer to the current stack pointer */
static int16_t *ptr_max_stack = 0;     /* Pointer to the maximum stack pointer (the farest point from the bottom of stack) */
static int32_t wc_stack_frame = 0;     /* Frame corresponding to the worst-case stack usage */
static int32_t wc_ram_size, wc_ram_frame;
static int32_t current_heap_size;
static int current_calls = 0;
static char location_max_stack[256] = "undefined";
static int Num_Records, Max_Num_Records;
static size_t Stat_Cnt_Size = USE_BYTES;
static const char *Count_Unit[] = { "bytes", "words", "words" };

static int *list_wc_intra_frame_heap, n_items_wc_intra_frame_heap, max_items_wc_intra_frame_heap, size_wc_intra_frame_heap, location_wc_intra_frame_heap;
static int *list_current_inter_frame_heap, n_items_current_inter_frame_heap, max_items_current_inter_frame_heap, size_current_inter_frame_heap;
static int *list_wc_inter_frame_heap, n_items_wc_inter_frame_heap, max_items_wc_inter_frame_heap, size_wc_inter_frame_heap, location_wc_inter_frame_heap;

/* Local Functions */
static unsigned long malloc_hash( const char *func_name, int func_lineno, char *size_str );
allocator_record *get_mem_record( unsigned long *hash, const char *func_name, int func_lineno, char *size_str, int *index_record );
static void *mem_alloc_block( size_t size, const char *size_str );

/*-------------------------------------------------------------------*
 * reset_mem()
 *
 * Initialize/reset memory counting tool (stack and heap)
 *--------------------------------------------------------------------*/

void reset_mem( Counting_Size cnt_size )
{
    int16_t something;
    size_t tmp_size;

    /* initialize stack pointers */
    ptr_base_stack = &something;
    ptr_max_stack = ptr_base_stack;
    ptr_current_stack = ptr_base_stack;

    Stat_Cnt_Size = cnt_size;

    /* Check, if sizeof(int32_t) is 4 bytes */
    tmp_size = sizeof( int32_t );
    if ( tmp_size != 4 )
    {
        fprintf( stderr, "Error: Expecting 'int32_t' to be a 32 Bits Integer!" );
        exit( -1 );
    }

    /* create allocation list for malloc() memory blocks */
    if ( allocation_list == NULL )
    {
        allocation_list = malloc( MAX_NUM_RECORDS * sizeof( allocator_record ) );
    }

    if ( allocation_list == NULL )
    {
        fprintf( stderr, "Error: Unable to Create List of Memory Blocks!" );
        exit( -1 );
    }

    Num_Records = 0;
    Max_Num_Records = MAX_NUM_RECORDS;

    wc_ram_size = 0;
    wc_ram_frame = -1;
    current_heap_size = 0;

    /* heap allocation tree */
    heap_allocation_call_tree_max_size = MAX_NUM_RECORDS;
    if ( heap_allocation_call_tree == NULL )
    {
        heap_allocation_call_tree = (int *) malloc( heap_allocation_call_tree_max_size * sizeof( int ) );
        memset( heap_allocation_call_tree, -1, heap_allocation_call_tree_max_size * sizeof( int ) );
    }
    heap_allocation_call_tree_size = 0;

    /* wc intra-frame heap */
    max_items_wc_intra_frame_heap = MAX_NUM_RECORDS;
    if ( list_wc_intra_frame_heap == NULL )
    {
        list_wc_intra_frame_heap = (int *) malloc( max_items_wc_intra_frame_heap * sizeof( int ) );
        memset( list_wc_intra_frame_heap, -1, max_items_wc_intra_frame_heap * sizeof( int ) );
    }
    n_items_wc_intra_frame_heap = 0;
    size_wc_intra_frame_heap = 0;
    location_wc_intra_frame_heap = -1;

    /* current inter-frame heap */
    max_items_current_inter_frame_heap = MAX_NUM_RECORDS;
    if ( list_current_inter_frame_heap == NULL )
    {
        list_current_inter_frame_heap = (int *) malloc( max_items_current_inter_frame_heap * sizeof( int ) );
        memset( list_current_inter_frame_heap, -1, max_items_current_inter_frame_heap * sizeof( int ) );
    }
    n_items_current_inter_frame_heap = 0;
    size_current_inter_frame_heap = 0;

    /* wc inter-frame heap */
    max_items_wc_inter_frame_heap = MAX_NUM_RECORDS;
    if ( list_wc_inter_frame_heap == NULL )
    {
        list_wc_inter_frame_heap = (int *) malloc( max_items_wc_inter_frame_heap * sizeof( int ) );
        memset( list_wc_inter_frame_heap, -1, max_items_wc_inter_frame_heap * sizeof( int ) );
    }
    n_items_wc_inter_frame_heap = 0;
    size_wc_inter_frame_heap = 0;
    location_wc_inter_frame_heap = -1;

#ifdef MEM_COUNT_DETAILS
    /* Check, if the .csv file has already been opened */
    if ( fid_csv_filename == NULL )
    {
        fid_csv_filename = fopen( csv_filename, "wb" );

        if ( fid_csv_filename == NULL )
        {
            fprintf( stderr, "\nCannot open %s!\n\n", csv_filename );
            exit( -1 );
        }
    }
    else
    {
        /* reset file */
        rewind( fid_csv_filename );
    }
#endif

    return;
}

/*-------------------------------------------------------------------*
 * reset_stack()
 *
 * Reset stack pointer
 *--------------------------------------------------------------------*/

void reset_stack( void )
{
    int16_t something;

    /* initialize/reset stack pointers */
    ptr_base_stack = &something;
    ptr_max_stack = ptr_base_stack;
    ptr_current_stack = ptr_base_stack;

    return;
}

/*-------------------------------------------------------------------*
 * push_stack()
 *
 * Check the current stack pointer and update the maximum stack pointer, if new maximum found.
 *--------------------------------------------------------------------*/

int push_stack( const char *filename, const char *fctname )
{
    int16_t something;
    int32_t current_stack_size;

    ptr_current_stack = &something;

    (void) *filename; /* to avoid compilation warning */

    /* Is there room to save the caller's information? */
    if ( current_calls >= MAX_RECORDABLE_CALLS )
    { /* No */
        fprintf( stderr, "No more room to store call stack info. Please increase MAX_RECORDABLE_CALLS" );
        exit( -1 );
    }

    /* Valid Function Name? */
    if ( fctname[0] == 0 )
    { /* No */
        fprintf( stderr, "Invalid function name for call stack info." );
        exit( -1 );
    }

    /* Save the Name of the Calling Function in the Table */
    strncpy( stack_callers[0][current_calls].function_name, fctname, MAX_FUNCTION_NAME_LENGTH );
    stack_callers[0][current_calls].function_name[MAX_FUNCTION_NAME_LENGTH] = 0; /* Nul Terminate */

    /* Save the Stack Pointer */
    stack_callers[0][current_calls].stack_ptr = ptr_current_stack;

    /* Increase Stack Calling Tree Level */
    current_calls++;

    /* Is this the First Time or the Worst Case? */
    if ( ptr_current_stack < ptr_max_stack || ptr_max_stack == NULL )
    { /* Yes */
        /* Save Info about it */
        ptr_max_stack = ptr_current_stack;

        wc_stack_frame = update_cnt; /* current frame number is stored in the variable update_cnt and updated in the function update_wmops() */
        strncpy( location_max_stack, fctname, sizeof( location_max_stack ) - 1 );
        location_max_stack[sizeof( location_max_stack ) - 1] = '\0';

        /* Save Call Tree */
        memmove( stack_callers[1], stack_callers[0], sizeof( caller_info ) * current_calls );

        /* Terminate the List (Unless Full) */
        if ( current_calls < MAX_RECORDABLE_CALLS )
        {
            stack_callers[1][current_calls].function_name[0] = 0;
        }
    }

    /* Check, if This is the New Worst-Case RAM (stack + heap) */
    current_stack_size = (int32_t) ( ( ( ptr_base_stack - ptr_current_stack ) * sizeof( int16_t ) ) );

    if ( current_stack_size < 0 )
    {
        /* prevent negative stack size */
        current_stack_size = 0;
    }

    if ( current_stack_size + current_heap_size > wc_ram_size )
    {
        wc_ram_size = current_stack_size + current_heap_size;
        wc_ram_frame = update_cnt;
    }

    return 0 /* for Now */;
}

/*-------------------------------------------------------------------*
 * pop_stack()
 *
 * Remove stack caller entry from the list
 *--------------------------------------------------------------------*/

int pop_stack( const char *filename, const char *fctname )
{
    caller_info *caller_info_ptr;

    (void) *filename; /* to avoid compilation warning */

    /* Decrease Stack Calling */
    current_calls--;

    /* Get Pointer to Caller Information */
    caller_info_ptr = &stack_callers[0][current_calls];

    /* Check, if Names Match */
    if ( strncmp( caller_info_ptr->function_name, fctname, MAX_FUNCTION_NAME_LENGTH ) != 0 )
    {
        fprintf( stderr, "Invalid usage of pop_stack()" );
        exit( -1 );
    }

    /* Erase Entry */
    caller_info_ptr->function_name[0] = 0;

    /* Retrieve previous stack pointer */
    if ( current_calls == 0 )
    {
        ptr_current_stack = ptr_base_stack;
    }
    else
    {
        ptr_current_stack = stack_callers[0][current_calls - 1].stack_ptr;
    }

    return 0 /* for Now */;
}

#ifdef MEM_COUNT_DETAILS
/*-------------------------------------------------------------------*
 * print_stack_call_tree()
 *
 * Print detailed information about worst-case stack usage
 *--------------------------------------------------------------------*/

static void print_stack_call_tree( void )
{
    caller_info *caller_info_ptr;
    int call_level;
    char fctname[MAX_FUNCTION_NAME_LENGTH + 1];

    fprintf( stdout, "\nList of functions when maximum stack size is reached:\n\n" );

    caller_info_ptr = &stack_callers[1][0];
    for ( call_level = 0; call_level < MAX_RECORDABLE_CALLS; call_level++ )
    {
        /* Done? */
        if ( caller_info_ptr->function_name[0] == 0 )
        {
            break;
        }

        /* Print Name */
        strncpy( fctname, caller_info_ptr->function_name, MAX_FUNCTION_NAME_LENGTH );
        strcat( fctname, "()" );
        fprintf( stdout, "%-42s", fctname );

        /* Print Stack Usage (Based on Difference) */
        if ( call_level != 0 )
        {
            fprintf( stdout, "%lu %s\n", ( ( ( caller_info_ptr - 1 )->stack_ptr - caller_info_ptr->stack_ptr ) * sizeof( int16_t ) ) >> Stat_Cnt_Size, Count_Unit[Stat_Cnt_Size] );
        }
        else
        {
            fprintf( stdout, "%lu %s\n", ( ( ptr_base_stack - caller_info_ptr->stack_ptr ) * sizeof( int16_t ) ) >> Stat_Cnt_Size, Count_Unit[Stat_Cnt_Size] );
        }

        /* Advance */
        caller_info_ptr++;
    }

    fprintf( stdout, "\n" );

    return;
}
#endif


/*-------------------------------------------------------------------*
 * mem_alloc()
 *
 * Creates new record, stores auxiliary information about which function allocated the memory, line number, parameters, etc.
 * Finally, it allocates physical memory using malloc()
 * The function also updates worst-case heap size and worst-case RAM size
 *--------------------------------------------------------------------*/

void *mem_alloc(
    const char *func_name,
    int func_lineno,
    size_t size,
    char *size_str /* the first char indicates m-alloc or c-alloc */ )
{
    int index_record;
    int32_t current_stack_size;
    unsigned long hash;
    allocator_record *ptr_record;

    if ( size == 0 )
    {
        fprintf( stderr, "Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Size of Zero not Supported" );
        exit( -1 );
    }

    /* Search for an existing record (that has been de-allocated before) */
    index_record = 0;
    while ( ( ptr_record = get_mem_record( &hash, func_name, func_lineno, size_str, &index_record ) ) != NULL )
    {
        if ( ptr_record->frame_allocated == -1 )
        {
            break;
        }
        else
        {
            index_record++;
        }
    }

    /* Create new record */
    if ( ptr_record == NULL )
    {
        if ( Num_Records >= Max_Num_Records )
        {
            /* There is no room for a new record -> reallocate memory */
            Max_Num_Records += MAX_NUM_RECORDS_REALLOC_STEP;
            allocation_list = realloc( allocation_list, Max_Num_Records * sizeof( allocator_record ) );
        }

        ptr_record = &( allocation_list[Num_Records] );

        /* Initialize new record */
        ptr_record->hash = hash;
        ptr_record->noccurances = 0;
        ptr_record->total_block_size = 0;
        ptr_record->total_used_size = 0;
        ptr_record->frame_allocated = -1;
        ptr_record->OOB_Flag = 0;
        ptr_record->wc_heap_size_intra_frame = -1;
        ptr_record->wc_heap_size_inter_frame = -1;

        index_record = Num_Records;
        Num_Records++;
    }

    /* Allocate memory block for the new record, add signature before the beginning and after the memory block and fill it with magic value */
    ptr_record->block_ptr = mem_alloc_block( size, size_str );

    if ( ptr_record->block_ptr == NULL )
    {
        fprintf( stderr, "Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Error: Cannot Allocate Memory!" );
        exit( -1 );
    }

    /* Save all auxiliary information about the memory block */
    strncpy( ptr_record->name, func_name, MAX_FUNCTION_NAME_LENGTH );
    ptr_record->name[MAX_FUNCTION_NAME_LENGTH] = '\0';
    strncpy( ptr_record->params, size_str, MAX_PARAMS_LENGTH ); /* Note: The size string starts with either 'm' or 'c' to indicate 'm'alloc or 'c'alloc */
    ptr_record->params[MAX_PARAMS_LENGTH] = '\0';
    ptr_record->lineno = func_lineno;
    ptr_record->block_size = size;
    ptr_record->total_block_size += size;

#ifdef MEM_COUNT_DETAILS
    /* Export heap memory allocation record to the .csv file */
    fprintf( fid_csv_filename, "A,%d,%s,%d,%d\n", update_cnt, ptr_record->name, ptr_record->lineno, ptr_record->block_size );
#endif

    if ( ptr_record->frame_allocated != -1 )
    {
        fprintf( stderr, "Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Error: Attempt to Allocate the Same Memory Block with Freeing it First!" );
        exit( -1 );
    }

    ptr_record->frame_allocated = update_cnt; /* Store the current frame number -> later it will be used to determine the total duration */

    /* Update Heap Size in the current frame */
    current_heap_size += ptr_record->block_size;

    /* Check, if this is the new Worst-Case RAM (stack + heap) */
    current_stack_size = (int32_t) ( ( ( ptr_base_stack - ptr_current_stack ) * sizeof( int16_t ) ) );
    if ( current_stack_size + current_heap_size > wc_ram_size )
    {
        wc_ram_size = current_stack_size + current_heap_size;
        wc_ram_frame = update_cnt;
    }

    /* Add new entry to the heap allocation call tree */
    if ( heap_allocation_call_tree == NULL )
    {
        fprintf( stderr, "Error: Heap allocation call tree not created!" );
        exit( -1 );
    }

    /* check, if the maximum size of the call tree has been reached -> resize if so */
    if ( heap_allocation_call_tree_size >= heap_allocation_call_tree_max_size )
    {
        heap_allocation_call_tree_max_size += MAX_NUM_RECORDS_REALLOC_STEP;
        heap_allocation_call_tree = (int *) realloc( heap_allocation_call_tree, heap_allocation_call_tree_max_size * sizeof( int ) );
    }

    /* push new entry (positive number means push op, neagtive number means pop op; zero index must be converted to 0.01 :-) */
    heap_allocation_call_tree[heap_allocation_call_tree_size++] = index_record;

    return ptr_record->block_ptr;
}

/*-------------------------------------------------------------------*
 * mem_alloc_block()
 *
 * Physical allocation of memory using malloc(). Appends 'signature' before and after the block,
 * pre-fills memory block with magic value
 *--------------------------------------------------------------------*/

static void *mem_alloc_block( size_t size, const char *size_str )
{
    size_t rounded_size;
    void *block_ptr;
    char *tmp_ptr;
    size_t n, f;
    int32_t fill_value;
    int32_t *ptr32;
    int32_t mask, temp;

    /* Round Up Block Size */
    rounded_size = ROUND_BLOCK_SIZE( size );

    /* Allocate memory using the standard malloc() by adding room for Signature Values */
    block_ptr = malloc( rounded_size + BLOCK_ROUNDING * 2 );

    if ( block_ptr == NULL )
    {
        return NULL;
    }

    /* Add Signature Before the Start of the Block */
    ptr32 = (int32_t *) block_ptr;
    n = N_32BITS_BLOCKS;
    do
    {
        *ptr32++ = MAGIC_VALUE_OOB;
    } while ( --n );

    /* Fill Memory Block with Magic Value or 0 */
    fill_value = MAGIC_VALUE_USED;
    if ( IS_CALLOC( size_str ) )
    {
        fill_value = 0x00000000;
    }
    n = size / sizeof( int32_t );
    while ( n-- )
    {
        *ptr32++ = fill_value;
    }

    /* Fill the Reminder of the Memory Block - After Rounding */
    n = rounded_size - size;
    f = n % sizeof( int32_t );
    if ( f != 0 )
    {
        /* when filling with '0' need to adapt the magic value */
        /* shift by [1->24, 2->16, 3->8] */
        mask = 0xFFFFFFFF << ( ( sizeof( int32_t ) - f ) * 8 ); /* (1) */
        temp = MAGIC_VALUE_OOB & mask;
        if ( fill_value != 0x0 )
        { /* for malloc merge fill value */
            temp += ( ~mask ) & MAGIC_VALUE_USED;
        } /* for calloc the code in (1) above already introduces zeros */
        *ptr32++ = temp;
    }
    n /= sizeof( int32_t );
    n += N_32BITS_BLOCKS;

    /* Add Signature After the End of Block */
    do
    {
        *ptr32++ = MAGIC_VALUE_OOB;
    } while ( --n );

    /* Adjust the Memory Block Pointer (Magic Value Before and After the Memory Block Requested) */
    tmp_ptr = (char *) block_ptr;
    tmp_ptr += BLOCK_ROUNDING;
    block_ptr = (void *) tmp_ptr;

    return block_ptr;
}

/*-------------------------------------------------------------------*
 * mem_set_usage()
 *
 * Calculates actual usage of memory block by checking the magic value that was used to pre-fill
 * each memory block during its allocation
 *--------------------------------------------------------------------*/

static int mem_set_usage( allocator_record *record_ptr )
{
    int total_bytes_used;

    size_t n;
    int32_t *ptr32;
    char *ptr8;
    size_t total_bytes;
    int32_t fill_value;

    fill_value = MAGIC_VALUE_USED;
    if ( ( record_ptr->params[0] ) == 'c' )
    {
        fill_value = 0x00000000;
    }

    total_bytes = record_ptr->block_size;

    /* Check 4 bytes at a time */
    ptr32 = (int32_t *) record_ptr->block_ptr;
    total_bytes_used = 0;
    for ( n = total_bytes / sizeof( int32_t ); n > 0; n-- )
    {
        if ( *ptr32++ != fill_value )
        {
            total_bytes_used += sizeof( int32_t );
        }
    }

    /* Check remaining bytes (If Applicable) 1 byte at a time */
    ptr8 = (char *) ptr32;
    for ( n = total_bytes % sizeof( int32_t ); n > 0; n-- )
    {
        if ( *ptr8++ != (char) fill_value )
        {
            total_bytes_used++;
        }

        /* Update Value */
        fill_value >>= 8;
    }

    return total_bytes_used;
}

/*-------------------------------------------------------------------*
 * mem_check_OOB()
 *
 * Checks, if out-of-bounds access has occured. This is done by inspecting the 'signature' value
 * taht has been added before and after the memory block during its allocation
 *--------------------------------------------------------------------*/

static unsigned int mem_check_OOB( allocator_record *record_ptr )
{
    int32_t *ptr32;
    unsigned int OOB_Flag = 0x0;
    int32_t mask;
    size_t i;
    int f;

    ptr32 = (int32_t *) record_ptr->block_ptr - N_32BITS_BLOCKS;

    /* Check the Signature at the Beginning of Memory Block */
    i = N_32BITS_BLOCKS;
    do
    {
        if ( *ptr32++ ^ MAGIC_VALUE_OOB )
        {
            OOB_Flag |= OOB_START;
        }
    } while ( --i );

    /* Advance to End (Snap to lowest 32 Bits) */
    ptr32 += record_ptr->block_size / sizeof( int32_t );

    /* Calculate Unused Space That has been added to get to the rounded Block Size */
    i = ROUND_BLOCK_SIZE( record_ptr->block_size ) - record_ptr->block_size;

    /* Partial Check of Signature at the End of Memory Block (for block size that has been rounded) */
    f = i % sizeof( int32_t );
    if ( f != 0 )
    {
        mask = 0xFFFFFFFF << ( ( sizeof( int32_t ) - f ) * 8 );
        if ( ( *ptr32++ ^ MAGIC_VALUE_OOB ) & mask )
        {
            OOB_Flag |= OOB_END;
        }
    }

    /* Full Check of Signature at the End of Memory Block, i.e. all 32 Bits (for the remainder after rounding) */
    i /= sizeof( int32_t );
    i += N_32BITS_BLOCKS;
    do
    {
        if ( *ptr32++ ^ MAGIC_VALUE_OOB )
        {
            OOB_Flag |= OOB_END;
        }
    } while ( --i );

    return OOB_Flag;
}

/*-------------------------------------------------------------------*
 * malloc_hash()
 *
 * Calculate hash from function name, line number and malloc size
 *--------------------------------------------------------------------*/

static unsigned long malloc_hash( const char *func_name, int func_lineno, char *size_str )
{
    unsigned long hash = 5381;
    const char *ptr_str;

    ptr_str = func_name;
    while ( ptr_str != NULL && *ptr_str != '\0' )
    {
        hash = ( ( hash << 5 ) + hash ) + *ptr_str++; /* hash * 33 + char */
    }

    hash = ( ( hash << 5 ) + hash ) + func_lineno; /* hash * 33 + func_lineno */

    ptr_str = size_str;
    while ( ptr_str != NULL && *ptr_str != '\0' )
    {
        hash = ( ( hash << 5 ) + hash ) + *ptr_str++; /* hash * 33 + char */
    }

    return hash;
}

/*-------------------------------------------------------------------*
 * get_mem_record()
 *
 * Search for memory record in the internal list, return NULL if not found
 * Start from index_record
 *--------------------------------------------------------------------*/

allocator_record *get_mem_record( unsigned long *hash, const char *func_name, int func_lineno, char *size_str, int *index_record )
{
    int i;

    if ( *index_record < 0 || *index_record > Num_Records )
    {
        return NULL;
    }

    /* calculate hash */
    *hash = malloc_hash( func_name, func_lineno, size_str );

    for ( i = *index_record; i < Num_Records; i++ )
    {
        /* check, if memory block is not allocated at the moment and the hash matches */
        if ( allocation_list[i].block_ptr == NULL && *hash == allocation_list[i].hash )
        {
            *index_record = i;
            return &( allocation_list[i] );
        }
    }

    /* not found */
    *index_record = -1;
    return NULL;
}


/*-------------------------------------------------------------------*
 * mem_free()
 *
 * This function de-allocatesd the memory block and frees the mphysical memory with free().
 * It also updates actual and average usage of the memory block.
 *
 * Note: The record is not removed from the list and may be reused later on in mem_alloc()!
 *--------------------------------------------------------------------*/

void mem_free( const char *func_name, int func_lineno, void *ptr )
{
    int i, index_record;
    char *tmp_ptr;
    allocator_record *ptr_record;

    /* Search for the Block Pointer in the List */
    ptr_record = NULL;
    index_record = -1;
    for ( i = 0; i < Num_Records; i++ )
    {
        if ( ptr == allocation_list[i].block_ptr )
        { /* Yes, Found it */
            ptr_record = &( allocation_list[i] );
            index_record = i;
            break;
        }
    }

    if ( ptr_record == NULL )
    {
        fprintf( stderr, "Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Error: Unable to Find Record Corresponding to the Allocated Memory Block!" );
        exit( -1 );
    }

    /* Update the Heap Size */
    current_heap_size -= ptr_record->block_size;

    /* Calculate the Actual Usage of the Memory Block (Look for Signature) */
    ptr_record->total_used_size += mem_set_usage( ptr_record );

    /* Check, if Out-Of-Bounds Access has been Detected */
    ptr_record->OOB_Flag = mem_check_OOB( ptr_record );

#ifdef MEM_COUNT_DETAILS
    /* Export heap memory de-allocation record to the .csv file */
    fprintf( fid_csv_filename, "D,%d,%s,%d,%d\n", update_cnt, ptr_record->name, ptr_record->lineno, ptr_record->block_size );
#endif

    /* De-Allocate Memory Block */
    tmp_ptr = (char *) ptr;
    tmp_ptr -= BLOCK_ROUNDING;
    ptr = (void *) tmp_ptr;
    free( ptr );

    /* Add new entry to the heap allocation call tree */
    if ( heap_allocation_call_tree == NULL )
    {
        fprintf( stderr, "Error: Heap allocation call tree not created!" );
        exit( -1 );
    }

    /* check, if the maximum size of the call tree has been reached -> resize if so */
    if ( heap_allocation_call_tree_size >= heap_allocation_call_tree_max_size )
    {
        heap_allocation_call_tree_max_size += MAX_NUM_RECORDS_REALLOC_STEP;
        heap_allocation_call_tree = (int *) realloc( heap_allocation_call_tree, heap_allocation_call_tree_max_size * sizeof( int ) );
    }

    heap_allocation_call_tree[heap_allocation_call_tree_size++] = -index_record;

    /* Reset memory block pointer (this is checked when updating wc intra-frame and inter-frame memory) */
    ptr_record->block_ptr = NULL;

    return;
}


/*-------------------------------------------------------------------*
 * update_mem()
 *
 * This function updates the worst-case intra-frame memory and the worst-case inter-frame memory.
 *--------------------------------------------------------------------*/

void update_mem( void )
{
    int i, j, flag_alloc = -1, i_record;
    int size_current_intra_frame_heap;
    int *list_current_intra_frame_heap = NULL, n_items_current_intra_frame_heap;
    allocator_record *ptr_record;

    /* process the heap allocation call tree and prepare lists of intra-frame and inter-frame heap memory blocks for this frame */
    n_items_current_intra_frame_heap = 0;
    size_current_intra_frame_heap = 0;
    for ( i = 0; i < heap_allocation_call_tree_size; i++ )
    {
        /* get the record */
        i_record = heap_allocation_call_tree[i];

        if ( i_record > 0 )
        {
            flag_alloc = 1;
        }
        else if ( i_record < 0 )
        {
            flag_alloc = 0;
            i_record = -i_record;
        }
        ptr_record = &( allocation_list[i_record] );

        if ( ptr_record->frame_allocated == update_cnt && ptr_record->block_ptr == NULL )
        {
            /* intra-frame heap memory */
            if ( list_current_intra_frame_heap == NULL )
            {
                list_current_intra_frame_heap = (int *) malloc( heap_allocation_call_tree_size * sizeof( int ) );
                memset( list_current_intra_frame_heap, -1, heap_allocation_call_tree_size * sizeof( int ) );
            }

            /* zero index doesn't have sign to determine whether it's allocated or de-allocated -> we need to search the list */
            if ( i_record == 0 )
            {
                flag_alloc = 1;
                for ( j = 0; j < n_items_current_intra_frame_heap; j++ )
                {
                    if ( list_current_intra_frame_heap[j] == i_record )
                    {
                        flag_alloc = 0;
                        break;
                    }
                }
            }

            if ( flag_alloc )
            {
                /* add to list */
                list_current_intra_frame_heap[n_items_current_intra_frame_heap++] = i_record;
                size_current_intra_frame_heap += ptr_record->block_size;

                /* no need to re-size the list -> the initially allocated size should be large enough */
            }
            else
            {
                /* remove from list */
                for ( j = 0; j < n_items_current_intra_frame_heap; j++ )
                {
                    if ( list_current_intra_frame_heap[j] == i_record )
                    {
                        break;
                    }
                }
                memmove( &list_current_intra_frame_heap[j], &list_current_intra_frame_heap[j + 1], ( n_items_current_intra_frame_heap - j ) * sizeof( int ) );
                n_items_current_intra_frame_heap--;
                size_current_intra_frame_heap -= ptr_record->block_size;

                /* reset block size */
                ptr_record->frame_allocated = -1;
                ptr_record->block_size = 0;
            }
        }
        else
        {
            /* inter-frame heap memory */

            /* zero index doesn't have sign to determine whether it's  allocated or de-allocated -> we need to search the list */
            if ( i_record == 0 )
            {
                flag_alloc = 1;
                for ( j = 0; j < n_items_current_inter_frame_heap; j++ )
                {
                    if ( list_current_inter_frame_heap[j] == i_record )
                    {
                        flag_alloc = 0;
                        break;
                    }
                }
            }

            if ( flag_alloc )
            {
                /* add to list */
                if ( n_items_current_inter_frame_heap >= max_items_current_inter_frame_heap )
                {
                    /* resize list, if needed */
                    max_items_current_inter_frame_heap = n_items_current_inter_frame_heap + MAX_NUM_RECORDS_REALLOC_STEP;
                    list_current_inter_frame_heap = realloc( list_current_inter_frame_heap, max_items_current_inter_frame_heap * sizeof( int ) );
                }

                list_current_inter_frame_heap[n_items_current_inter_frame_heap++] = i_record;
                size_current_inter_frame_heap += ptr_record->block_size;
            }
            else
            {
                /* remove from list */
                for ( j = 0; j < n_items_current_inter_frame_heap; j++ )
                {
                    if ( list_current_inter_frame_heap[j] == i_record )
                    {
                        break;
                    }
                }
                memmove( &list_current_inter_frame_heap[j], &list_current_inter_frame_heap[j + 1], ( n_items_current_inter_frame_heap - j ) * sizeof( int ) );
                n_items_current_inter_frame_heap--;
                size_current_inter_frame_heap -= ptr_record->block_size;

                /* reset block size */
                ptr_record->frame_allocated = -1;
                ptr_record->block_size = 0;
            }
        }
    }

    /* check, if this is the new worst-case for intra-frame heap memory */
    if ( size_current_intra_frame_heap > size_wc_intra_frame_heap )
    {
        if ( n_items_current_intra_frame_heap >= max_items_wc_intra_frame_heap )
        {
            /* resize the list, if needed */
            max_items_wc_intra_frame_heap = n_items_current_intra_frame_heap + MAX_NUM_RECORDS_REALLOC_STEP;
            list_wc_intra_frame_heap = realloc( list_wc_intra_frame_heap, max_items_wc_intra_frame_heap * sizeof( int ) );
        }

        /* copy current-frame list to worst-case list */
        memmove( list_wc_intra_frame_heap, list_current_intra_frame_heap, n_items_current_intra_frame_heap * sizeof( int ) );
        n_items_wc_intra_frame_heap = n_items_current_intra_frame_heap;
        size_wc_intra_frame_heap = size_current_intra_frame_heap;
        location_wc_intra_frame_heap = update_cnt;

        /* update the wc numbers in all individual records */
        for ( i = 0; i < n_items_wc_intra_frame_heap; i++ )
        {
            i_record = list_wc_intra_frame_heap[i];
            ptr_record = &( allocation_list[i_record] );
            ptr_record->wc_heap_size_intra_frame = ptr_record->block_size;
        }
    }

    /* check, if this is the new worst-case for inter-frame heap memory */
    if ( size_current_inter_frame_heap > size_wc_inter_frame_heap )
    {
        if ( n_items_current_inter_frame_heap >= max_items_wc_inter_frame_heap )
        {
            /* resize list, if needed */
            max_items_wc_inter_frame_heap = n_items_current_inter_frame_heap + MAX_NUM_RECORDS_REALLOC_STEP;
            list_wc_inter_frame_heap = realloc( list_wc_inter_frame_heap, max_items_wc_inter_frame_heap * sizeof( int ) );
        }

        /* copy current-frame list to worst-case list */
        memmove( list_wc_inter_frame_heap, list_current_inter_frame_heap, n_items_current_inter_frame_heap * sizeof( int ) );
        n_items_wc_inter_frame_heap = n_items_current_inter_frame_heap;
        size_wc_inter_frame_heap = size_current_inter_frame_heap;
        location_wc_inter_frame_heap = update_cnt;

        /* update the wc numbers in all individual records */
        for ( i = 0; i < n_items_wc_inter_frame_heap; i++ )
        {
            i_record = list_wc_inter_frame_heap[i];
            ptr_record = &( allocation_list[i_record] );
            ptr_record->wc_heap_size_inter_frame = ptr_record->block_size;
        }
    }

    /* reset heap allocation call tree */
    heap_allocation_call_tree_size = 0;

    /* de-allocate list of intra-frame heap memory blocks in the current fraeme - it's needed only inside this function */
    if ( list_current_intra_frame_heap )
    {
        free( list_current_intra_frame_heap );
    }

    return;
}

#ifdef MEM_COUNT_DETAILS
/*-------------------------------------------------------------------*
 * subst()
 *
 * Substitute character in string
 *--------------------------------------------------------------------*/

static void subst( char *s, char from, char to )
{
    while ( *s == from )
    {
        *s++ = to;
    }

    return;
}


/*-------------------------------------------------------------------*
 * mem_count_summary()
 *
 * Print detailed (per-item) information about heap memory usage
 *--------------------------------------------------------------------*/

static void mem_count_summary( void )
{
    int i, j, index, index_record;
    size_t length;
    char buf[300], format_str[50], name_str[MAX_FUNCTION_NAME_LENGTH + 3], parms_str[MAX_PARAMS_LENGTH + 1], type_str[10], usage_str[20], size_str[20], line_str[10];
    allocator_record *ptr_record, *ptr;

    /* Prepare format string */
    sprintf( format_str, "%%-%ds %%5s %%6s %%-%ds %%20s %%6s ", MAX_FUNCTION_NAME_LENGTH, MAX_PARAMS_LENGTH );

    if ( n_items_wc_intra_frame_heap > 0 )
    {
        /* Intra-Frame Heap Size */
        fprintf( stdout, "\nList of memory blocks when maximum intra-frame heap size is reached:\n\n" );

        /* Find duplicate records (same hash and worst-case heap size) */
        for ( i = 0; i < n_items_wc_intra_frame_heap; i++ )
        {
            index_record = list_wc_intra_frame_heap[i];
            if ( index_record == -1 )
            {
                continue;
            }

            ptr_record = &( allocation_list[index_record] );
            for ( j = i + 1; j < n_items_wc_intra_frame_heap; j++ )
            {
                index = list_wc_intra_frame_heap[j];
                if ( index == -1 )
                {
                    continue;
                }
                ptr = &( allocation_list[index] );

                if ( ptr->hash == ptr_record->hash && ptr->wc_heap_size_intra_frame == ptr_record->wc_heap_size_intra_frame )
                {
                    ptr_record->noccurances++;
                    list_wc_intra_frame_heap[j] = -1;
                }
            }
        }

        /* Print Header */
        sprintf( buf, format_str, "Function Name", "Line", "Type", "Function Parameters", "Maximum Size", "Usage" );
        puts( buf );
        length = strlen( buf );
        sprintf( buf, "%0*d\n", (int) length - 1, 0 );
        subst( buf, '0', '-' );
        puts( buf );

        for ( i = 0; i < n_items_wc_intra_frame_heap; i++ )
        {
            index_record = list_wc_intra_frame_heap[i];

            if ( index_record != -1 )
            {
                /* get the record */
                ptr_record = &( allocation_list[index_record] );

                /* prepare information strings */
                strncpy( name_str, ptr_record->name, MAX_FUNCTION_NAME_LENGTH );
                strcat( name_str, "()" );
                name_str[MAX_FUNCTION_NAME_LENGTH] = '\0';
                strncpy( parms_str, &( ptr_record->params[2] ), MAX_PARAMS_LENGTH );
                parms_str[MAX_PARAMS_LENGTH] = '\0';

                if ( ptr_record->params[0] == 'm' )
                {
                    strcpy( type_str, "malloc" );
                }
                else
                {
                    strcpy( type_str, "calloc" );
                }

                sprintf( line_str, "%d", ptr_record->lineno );

                /* prepare average usage & memory size strings */
                sprintf( usage_str, "%d%%", (int) ( ( (float) ptr_record->total_used_size / ( ptr_record->total_block_size + 1 ) ) * 100.0f ) );

                if ( ptr_record->noccurances > 1 )
                {
                    sprintf( size_str, "%dx%d %s", ptr_record->noccurances, (int) ( ptr_record->wc_heap_size_intra_frame >> Stat_Cnt_Size ), Count_Unit[Stat_Cnt_Size] );
                }
                else
                {
                    sprintf( size_str, "%d %s", (int) ( ptr_record->wc_heap_size_intra_frame >> Stat_Cnt_Size ), Count_Unit[Stat_Cnt_Size] );
                }

                sprintf( buf, format_str, name_str, line_str, type_str, parms_str, size_str, usage_str );
                puts( buf );
            }
        }

        fprintf( stdout, "\n" );
    }

    if ( n_items_wc_inter_frame_heap > 0 )
    {
        /* Inter-Frame Heap Size */
        fprintf( stdout, "\nList of memory blocks when maximum inter-frame heap size is reached:\n\n" );

        /* Find duplicate records (same hash and worst-case heap size) */
        for ( i = 0; i < n_items_wc_inter_frame_heap; i++ )
        {
            index_record = list_wc_inter_frame_heap[i];
            if ( index_record == -1 )
            {
                continue;
            }
            ptr_record = &( allocation_list[index_record] );
            ptr_record->noccurances = 1; /* reset the counter as some blocks may have been both, intra-frame and inter-frame */
            for ( j = i + 1; j < n_items_wc_inter_frame_heap; j++ )
            {
                index = list_wc_inter_frame_heap[j];
                if ( index == -1 )
                {
                    continue;
                }
                ptr = &( allocation_list[index] );

                if ( ptr->hash == ptr_record->hash && ptr->wc_heap_size_inter_frame == ptr_record->wc_heap_size_inter_frame )
                {
                    ptr_record->noccurances++;
                    list_wc_inter_frame_heap[j] = -1;
                }
            }
        }

        /* Print Header */
        sprintf( buf, format_str, "Function Name", "Line", "Type", "Function Parameters", "Memory Size", "Usage" );
        puts( buf );
        length = strlen( buf );
        sprintf( buf, "%0*d\n", (int) length - 1, 0 );
        subst( buf, '0', '-' );
        puts( buf );

        for ( i = 0; i < n_items_wc_inter_frame_heap; i++ )
        {
            index_record = list_wc_inter_frame_heap[i];

            if ( index_record != -1 )
            {
                /* get the record */
                ptr_record = &( allocation_list[index_record] );

                /* prepare information strings */
                strncpy( name_str, ptr_record->name, MAX_FUNCTION_NAME_LENGTH );
                strcat( name_str, "()" );
                name_str[MAX_FUNCTION_NAME_LENGTH] = '\0';
                strncpy( parms_str, &( ptr_record->params[2] ), MAX_PARAMS_LENGTH );
                parms_str[MAX_PARAMS_LENGTH] = '\0';

                if ( ptr_record->params[0] == 'm' )
                {
                    strcpy( type_str, "malloc" );
                }
                else
                {
                    strcpy( type_str, "calloc" );
                }

                sprintf( line_str, "%d", ptr_record->lineno );

                /* prepare average usage & memory size strings */
                sprintf( usage_str, "%d%%", (int) ( ( (float) ptr_record->total_used_size / ( ptr_record->total_block_size + 0.1f ) ) * 100.0f + 0.5f ) );

                if ( ptr_record->noccurances > 1 )
                {
                    sprintf( size_str, "%dx%d %s", ptr_record->noccurances, (int) ( ptr_record->wc_heap_size_inter_frame >> Stat_Cnt_Size ), Count_Unit[Stat_Cnt_Size] );
                }
                else
                {
                    sprintf( size_str, "%d %s", (int) ( ptr_record->wc_heap_size_inter_frame >> Stat_Cnt_Size ), Count_Unit[Stat_Cnt_Size] );
                }

                sprintf( buf, format_str, name_str, line_str, type_str, parms_str, size_str, usage_str );
                puts( buf );
            }
        }

        fprintf( stdout, "\n" );
    }

    return;
}

#endif

/*-------------------------------------------------------------------*
 * print_mem()
 *
 * Print information about ROM and RAM memory usage
 *--------------------------------------------------------------------*/

void print_mem( ROM_Size_Lookup_Table Const_Data_PROM_Table[] )
{
    int i, nElem;

    fprintf( stdout, "\n\n --- Memory usage ---  \n\n" );

    if ( Const_Data_PROM_Table != NULL )
    {
        nElem = 0;
        while ( strcmp( Const_Data_PROM_Table[nElem].file_spec, "" ) != 0 )
            nElem++;

        for ( i = 0; i < nElem; i++ )
        {
            fprintf( stdout, "Program ROM size (%s): %d instruction words\n", Const_Data_PROM_Table[i].file_spec, Const_Data_PROM_Table[i].PROM_size );
        }

        for ( i = 0; i < nElem; i++ )
        {
            if ( Const_Data_PROM_Table[i].Get_Const_Data_Size_Func == NULL )
            {
                fprintf( stdout, "Error: Cannot retrieve or calculate Table ROM size of (%s)!\n", Const_Data_PROM_Table[i].file_spec );
            }

            fprintf( stdout, "Table ROM (const data) size (%s): %d %s\n", Const_Data_PROM_Table[i].file_spec, Const_Data_PROM_Table[i].Get_Const_Data_Size_Func() >> Stat_Cnt_Size, Count_Unit[Stat_Cnt_Size] );
        }
    }
    else
    {
        fprintf( stdout, "Program ROM size: not available\n" );
        fprintf( stdout, "Table ROM (const data) size: not available\n" );
    }

    if ( wc_ram_size > 0 )
    {
        fprintf( stdout, "Maximum RAM (stack + heap) size: %d %s in frame %d\n", wc_ram_size >> Stat_Cnt_Size, Count_Unit[Stat_Cnt_Size], wc_ram_frame );
    }
    else
    {
        fprintf( stdout, "Maximum RAM (stack + heap) size: not available\n" );
    }

    /* check, if the stack is empty */
    if ( ptr_current_stack != ptr_base_stack )
    {
        fprintf( stderr, "Warning: Stack is not empty.\n" );
    }

    if ( ptr_base_stack - ptr_max_stack > 0 )
    {
        fprintf( stdout, "Maximum stack size: %lu %s in frame %d\n", ( ( ptr_base_stack - ptr_max_stack ) * sizeof( int16_t ) ) >> Stat_Cnt_Size, Count_Unit[Stat_Cnt_Size],
                 wc_stack_frame );
    }
    else
    {
        fprintf( stdout, "Maximum stack size: not available\n" );
    }

    /* last update of intra-frame memory and inter-frame memory, if needed */
    if ( heap_allocation_call_tree_size > 0 )
    {
        update_mem();
    }

    /* check, if all memory blocks have been deallocated (freed) */
    for ( i = 0; i < Num_Records; i++ )
    {
        if ( allocation_list[i].block_ptr != NULL )
        {
            fprintf( stderr, "Fct=%s, Ln=%i: %s!\n", allocation_list[i].name, allocation_list[i].lineno, "Error: Memory Block has not been De-Allocated with free()!" );
            exit( -1 );
        }
    }

    if ( n_items_wc_intra_frame_heap > 0 )
    {
        fprintf( stdout, "Maximum intra-frame heap size: %d %s in frame %d\n", size_wc_intra_frame_heap >> Stat_Cnt_Size, Count_Unit[Stat_Cnt_Size], location_wc_intra_frame_heap );
    }
    else
    {
        fprintf( stdout, "Maximum intra-frame heap size: 0\n" );
    }

    if ( n_items_wc_inter_frame_heap > 0 )
    {
        fprintf( stdout, "Maximum inter-frame heap size: %d %s in frame %d\n", size_wc_inter_frame_heap >> Stat_Cnt_Size, Count_Unit[Stat_Cnt_Size], location_wc_inter_frame_heap );
    }
    else
    {
        fprintf( stdout, "Maximum inter-frame heap size: 0\n" );
    }

#ifdef MEM_COUNT_DETAILS
    /* Print detailed information about worst-case stack usage */
    if ( ptr_base_stack - ptr_max_stack > 0 )
    {
        print_stack_call_tree();
    }

    /* Print detailed information about worst-case heap usage */
    mem_count_summary();
#endif

    if ( Stat_Cnt_Size > 0 )
    {
        fprintf( stdout, "\nNote: 1 word = %d bits\n", 8 << Stat_Cnt_Size );
        fprintf( stdout, "This is an optimistic estimate of memory consumption assuming that each variable type is stored with sizeof(type) bits\n" );
    }

    if ( n_items_wc_intra_frame_heap > 0 )
    {
        fprintf( stdout, "Intra-frame heap memory is allocated and de-allocated in the same frame\n" );
    }

    /* De-allocate list of heap memory blocks */
    if ( allocation_list != NULL )
    {
        free( allocation_list );
    }

    /* De-allocate heap allocation call tree */
    if ( heap_allocation_call_tree != NULL )
    {
        free( heap_allocation_call_tree );
    }

    /* De-allocate intra-frame and inter-frame heap lists */
    if ( list_wc_intra_frame_heap != NULL )
    {
        free( list_wc_intra_frame_heap );
    }

    if ( list_current_inter_frame_heap != NULL )
    {
        free( list_current_inter_frame_heap );
    }

    if ( list_wc_inter_frame_heap != NULL )
    {
        free( list_wc_inter_frame_heap );
    }

#ifdef MEM_COUNT_DETAILS
    if ( fid_csv_filename != NULL )
    {
        fclose( fid_csv_filename );
    }
#endif

    return;
}

#endif /* WMOPS */

#ifndef WMOPS
int cntr_push_pop = 0; /* global counter for checking balanced push_wmops()/pop_wmops() pairs when WMOPS is not activated */
#endif

 