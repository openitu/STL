/*
 * wmc_auto.h
 *
 * Macros for estimating computational complexity and memory consumption.
 *
 * (C) 2022 copyright VoiceAge Corporation. All Rights Reserved.
 *
 * This software is protected by copyright law and by international treaties.
 * VoiceAge Corporation retain full ownership rights in their respective contributions in the software.
 * No license of any kind, including but not limited to patent license, of any foregoing parties is 
 * hereby granted by implication, estoppel or otherwise.
 *
 * This software is provided AS IS, without any express or implied warranties. The software is in the
 * development stage. It is intended exclusively for experts who have experience with such software and
 * solely for the purpose of inspection. All implied warranties of non-infringement, merchantability
 * and/or fitness for a particular purpose are hereby disclaimed and excluded.
 *
 * Written by Guy Richard, September 2009 - January 2016.
 *
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

#include "options.h"
#include "wmc_auto.h"

  
/*-------------------------------------------------------------------*
 * WMOPS counting tool
 *--------------------------------------------------------------------*/

#ifdef WMOPS

#define MAX_RECORDS     1024
#define MAX_CHAR        64
#define MAX_STACK       64
#define DOUBLE_MAX      0x80000000

struct wmops_record
{
  char   label[MAX_CHAR];
  long   call_number;
  long   update_cnt;                 
  int    call_tree[MAX_RECORDS];
  double start_selfcnt;
  double current_selfcnt;
  double max_selfcnt;
  double min_selfcnt;
  double tot_selfcnt;
  double start_cnt;     /* The following take into account the decendants */
  double current_cnt;
  double max_cnt;
  double min_cnt;
  double tot_cnt;
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


void reset_wmops (void)
{
  int i, j;

  for (i = 0; i < MAX_RECORDS; i++)
  {
    strcpy(&wmops[i].label[0], "\0");
    wmops[i].call_number = 0;
    wmops[i].update_cnt = 0;           
    for (j = 0; j < MAX_RECORDS; j++)
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
  }

  for (i = 0; i < MAX_STACK; i++)
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


void push_wmops (const char *label)
{
  int new_flag;
  int i, j;

  /* Check if new function record label */
  new_flag = 1;
  for (i = 0; i < num_records; i++)
  {
      if (strcmp(wmops[i].label, label) == 0)
      {
          new_flag = 0;
          break;
      }
  }

  /* Configure new record */
  if (new_flag)
  {
      if (num_records >= MAX_RECORDS)
      {
          fprintf(stdout, "push_wmops(): exceeded MAX_RECORDS count.\n\n");
          exit(-1);
      }
      strcpy(wmops[i].label, label);
      num_records++;
  }

  /* Push current context onto stack */
  if (current_record >= 0)
  {
      if (sptr >= MAX_STACK)
      {
          fprintf(stdout, "\r push_wmops(): stack exceeded, try inreasing MAX_STACK\n");
          exit(-1);
      }
      stack[sptr++] = current_record;

      /* accumulate op counts */
      wmops[current_record].current_selfcnt += ops_cnt - wmops[current_record].start_selfcnt;

      /* update call tree */
      for (j = 0; j < MAX_RECORDS; j++)
      {
          if (wmops[i].call_tree[j] == current_record)
          {
              break;
          }
          else if (wmops[i].call_tree[j] == -1)
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

  return;
}


void pop_wmops (void)
{

  /* Check for underflow */
  if (current_record < 0)
  {
     fprintf (stdout, "\r pop_wmops(): stack underflow, too many calls to pop_wmops()\n");
     exit (-1);
  }

  /* update count of current record */
  wmops[current_record].current_selfcnt += ops_cnt - wmops[current_record].start_selfcnt;
  wmops[current_record].current_cnt += ops_cnt - wmops[current_record].start_cnt;

  /* Get back previous context from stack */
  if (sptr > 0)
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


void update_wmops (void)
{
  int i;
  double current_cnt;

  if ( sptr != 0 )
  {
      fprintf (stdout, "update_wmops(): Stack must be empty!\n");
      exit (-1);
  }

  for (i = 0; i < num_records; i++)
  {
      wmops[i].tot_selfcnt += wmops[i].current_selfcnt;
      wmops[i].tot_cnt += wmops[i].current_cnt;

      if (wmops[i].current_selfcnt > 0)
      {
          if (wmops[i].current_selfcnt > wmops[i].max_selfcnt)
          {
              wmops[i].max_selfcnt = wmops[i].current_selfcnt;
          }

          if (wmops[i].current_selfcnt < wmops[i].min_selfcnt)
          {
              wmops[i].min_selfcnt = wmops[i].current_selfcnt;
          }
      }

      wmops[i].current_selfcnt = 0;

      if (wmops[i].current_cnt > 0)
      {
          if (wmops[i].current_cnt > wmops[i].max_cnt)
          {
              wmops[i].max_cnt = wmops[i].current_cnt;
          }

          if (wmops[i].current_cnt < wmops[i].min_cnt)
          {
              wmops[i].min_cnt = wmops[i].current_cnt;
          }

          wmops[i].update_cnt++;
      }

      wmops[i].current_cnt = 0;
  }

  current_cnt = ops_cnt - start_cnt;
  if (current_cnt > max_cnt)
  {
      max_cnt = current_cnt;

      for (i = 0; i < NUM_INST; i++)
      {
          inst_cnt_wc[i] = inst_cnt[i];
      }

      fnum_cnt_wc = update_cnt + 1;
  }

  if (current_cnt < min_cnt)
  {
      min_cnt = current_cnt;
  }

  for (i = 0; i < NUM_INST; i++)
  {
      inst_cnt[i] = 0.0;
  }

  start_cnt = ops_cnt;
  update_cnt++;

  return;
}


void print_wmops (void)
{
  int i;

  char *sfmts = "%20s %8s %8s %7s %7s\n";
  char *dfmts = "%20s %8.2f %8.3f %7.3f %7.3f\n";
  char *sfmt =  "%20s %8s %8s %7s %7s  %7s %7s %7s\n";
  char *dfmt =  "%20s %8.2f %8.3f %7.3f %7.3f  %7.3f %7.3f %7.3f\n";

#if ENABLE_TREE

  int j;
  char *sfmtt =  "%20s %4s %15s\n";
  char *dfmtt =  "%20s %4d  ";

#endif

  fprintf(stdout, "\n\n --- Weighted MOPS Analysis ---  \n\n");
  fprintf(stdout, "WMOPS boost factor: %4.2f\n\n", WMOPS_BOOST_FAC);

  fprintf(stdout, "%54s  %23s\n", "|------  SELF  ------|"
                                  ,"|---  CUMULATIVE  ---|");
  fprintf(stdout, sfmt, "        routine", " calls", "  min ", "  max ", "  avg ", "  min ", "  max ", "  avg ");
  fprintf(stdout, sfmt, "---------------", "------", "------", "------", "------", "------", "------", "------");
  for (i = 0; i < num_records; i++)
  {
      fprintf(stdout, dfmt,
          wmops[i].label,
          (float)wmops[i].call_number / update_cnt,
          FAC * wmops[i].min_selfcnt,
          FAC * wmops[i].max_selfcnt,
          /* FAC*wmops[i].tot_selfcnt/update_cnt, */
          FAC * wmops[i].tot_selfcnt / wmops[i].update_cnt,             /* added -- JPA */
          FAC * wmops[i].min_cnt,
          FAC * wmops[i].max_cnt,
          /* FAC*wmops[i].tot_cnt/update_cnt); */
          FAC * wmops[i].tot_cnt / wmops[i].update_cnt);                /* added -- JPA */
  }
  fprintf (stdout, sfmts, "---------------", "------", "------", "------", "------");
  fprintf (stdout, dfmts, 
           "total",
           (float)update_cnt,
           FAC*min_cnt,
           FAC*max_cnt,
           FAC*ops_cnt/update_cnt);
  fprintf (stdout, "\n"); 

#if ENABLE_TREE
  fprintf (stdout, "\nCall Tree:\n\n");
  fprintf (stdout, sfmtt, "       function", "num", "called by:    ");
  fprintf (stdout, sfmtt, "---------------", "---", "--------------");

  for (i = 0; i < num_records; i++)
    {
      fprintf (stdout, dfmtt, wmops[i].label, i);
      for (j = 0; wmops[i].call_tree[j] != -1; j++)
        {
          if (j != 0)
            {
              fprintf (stdout, ", ");
            }
          fprintf (stdout, "%d", wmops[i].call_tree[j]);
        }
      fprintf (stdout, "\n");

    }

  fprintf (stdout, sfmtt, "---------------", "---", "--------------");
  fprintf (stdout, "\n\n");

  fprintf(stdout, "\nInstruction Type Analysis (for worst case frame #%ld):\n\n", fnum_cnt_wc);   /* added -- JPA */
  for (i = 0; i < NUM_INST; i++)
  {
      switch ((enum instructions)i)
      {
      case _ADD:       fprintf(stdout, "\tAdds:          %12.1f\n", inst_cnt_wc[i]);  break;
      case _ABS:       fprintf(stdout, "\tAbsolutes:     %12.1f\n", inst_cnt_wc[i]);  break;
      case _MULT:      fprintf(stdout, "\tMultiplies:    %12.1f\n", inst_cnt_wc[i]);  break;
      case _MAC:       fprintf(stdout, "\tMACs:          %12.1f\n", inst_cnt_wc[i]);  break;
      case _MOVE:      fprintf(stdout, "\tMoves:         %12.1f\n", inst_cnt_wc[i]);  break;
      case _STORE:     fprintf(stdout, "\tStores:        %12.1f\n", inst_cnt_wc[i]);  break;
      case _LOGIC:     fprintf(stdout, "\tLogicals:      %12.1f\n", inst_cnt_wc[i]);  break;
      case _SHIFT:     fprintf(stdout, "\tShifts:        %12.1f\n", inst_cnt_wc[i]);  break;
      case _BRANCH:    fprintf(stdout, "\tBranches:      %12.1f\n", inst_cnt_wc[i]);  break;
      case _DIV:       fprintf(stdout, "\tDivisions:     %12.1f\n", inst_cnt_wc[i]);  break;
      case _SQRT:      fprintf(stdout, "\tSquare Root:   %12.1f\n", inst_cnt_wc[i]);  break;
      case _TRANS:     fprintf(stdout, "\tTrans:         %12.1f\n", inst_cnt_wc[i]);  break;
      case _FUNC:      fprintf(stdout, "\tFunc Call:     %12.1f\n", inst_cnt_wc[i]);  break;
      case _LOOP:      fprintf(stdout, "\tLoop Init:     %12.1f\n", inst_cnt_wc[i]);  break;
      case _INDIRECT:  fprintf(stdout, "\tIndirect Addr: %12.1f\n", inst_cnt_wc[i]);  break;
      case _PTR_INIT:  fprintf(stdout, "\tPointer Init:  %12.1f\n", inst_cnt_wc[i]);  break;
      case _TEST:      fprintf(stdout, "\tExtra condit.: %12.1f\n", inst_cnt_wc[i]);  break;
      case _POWER:     fprintf(stdout, "\tExponential:   %12.1f\n", inst_cnt_wc[i]);  break;
      case _LOG:       fprintf(stdout, "\tLogarithm:     %12.1f\n", inst_cnt_wc[i]);  break;
      case _MISC:      fprintf(stdout, "\tAll other op.: %12.1f\n", inst_cnt_wc[i]);  break;
      default:         fprintf(stdout, "\tERROR: Invalid instruction type: %d\n\n", i);
      }
  }
#endif

  return;
}


/*-------------------------------------------------------------------*
 * Stack counting tool
 *--------------------------------------------------------------------*/

int16_t* ptr_base_stack = 0; /* Pointer to the bottom of stack (base pointer). Stack grows up. */
int16_t* ptr_max_stack = 0;  /* Pointer to the maximum stack pointer (the farest point from the bottom of stack) */
int32_t wc_frame = 0;        /* Frame corresponding to the worst-case stack usage */
char location_max_stack[256] = "undefined";
#define MAX_FCT_NAME_LENGTH 30

typedef struct
{
    char function_name[MAX_FCT_NAME_LENGTH + 1];
    int16_t* stack_ptr;
} caller_info;
int current_calls = 0;
#define MAX_RECORDABLE_CALLS 40
caller_info stack_callers[2][MAX_RECORDABLE_CALLS];


/*-------------------------------------------------------------------*
 * reset_stack()
 *
 * Initialize/reset the base stack counter..
 *--------------------------------------------------------------------*/

void reset_stack()
{
    int16_t something;

    ptr_base_stack = &something;
    ptr_max_stack = ptr_base_stack;
}

/*-------------------------------------------------------------------*
 * push_stack()
 *
 * Check the current stack pointer and update the maximum stack pointer, if new maximum found.
 *--------------------------------------------------------------------*/

int push_stack(const char* filename, const char* fctname)
{
    int16_t something;

    (void)*filename; /* to avoid compilation warning */

    /* Is there room to save the caller's information? */
    if (current_calls >= MAX_RECORDABLE_CALLS)
    { /* No */
        fprintf(stderr, "No more room to store call stack info. Please increase MAX_RECORDABLE_CALLS");
        exit(-1);
    }

    /* Valid Function Name? */
    if (fctname[0] == 0)
    { /* No */
        fprintf(stderr, "Invalid function name for call stack info.");
        exit(-1);
    }

    /* Save the Name of the Calling Function in the Table */
    strncpy(stack_callers[0][current_calls].function_name, fctname, MAX_FCT_NAME_LENGTH);
    stack_callers[0][current_calls].function_name[MAX_FCT_NAME_LENGTH] = 0; /* Nul Terminate */

    /* Save the Stack Pointer */
    stack_callers[0][current_calls].stack_ptr = &something;

    /* Increase Stack Calling Tree Level */
    current_calls++;

    /* Is this the First Time or the Worst Case? */
    if (&something < ptr_max_stack || ptr_max_stack == NULL)
    { /* Yes */
        /* Save Info about it */
        ptr_max_stack = &something;

        //wc_frame = frame;
        wc_frame = update_cnt;
        strncpy(location_max_stack, fctname, sizeof(location_max_stack) - 1);
        location_max_stack[sizeof(location_max_stack) - 1] = '\0';

        /* Save Call Tree */
        memmove(stack_callers[1], stack_callers[0], sizeof(caller_info) * current_calls);

        /* Terminate the List (Unless Full) */
        if (current_calls < MAX_RECORDABLE_CALLS)
        {
            stack_callers[1][current_calls].function_name[0] = 0;
        }
    }

    return 0 /* for Now */;
}

int pop_stack(const char* filename, const char* fctname)
{
    caller_info* caller_info_ptr;

    (void)*filename; /* to avoid compilation warning */

    /* Decrease Stack Calling */
    current_calls--;

    /* Get Pointer to Caller Information */
    caller_info_ptr = &stack_callers[0][current_calls];

    /* Check if Names Match */
    if (strncmp(caller_info_ptr->function_name, fctname, MAX_FCT_NAME_LENGTH) != 0)
    {
        fprintf(stderr, "Invalid usage of pop_stack()");
        exit(-1);
    }

    /* Erase Entry */
    caller_info_ptr->function_name[0] = 0;

    return 0 /* for Now */;
}

void print_stack_call_tree(void)
{
    caller_info* caller_info_ptr;
    int call_level;

    fprintf(stdout, "Stack Call Tree (frame #%5d)            Stack Usage in words\n", wc_frame);
    caller_info_ptr = &stack_callers[1][0];
    for (call_level = 0; call_level < MAX_RECORDABLE_CALLS; call_level++)
    {
        /* Done? */
        if (caller_info_ptr->function_name[0] == 0)
            break;

        /* Print Name */
        fprintf(stdout, "%-42s", caller_info_ptr->function_name);

        /* Print Stack Usage (Based on Difference) */
        if (call_level != 0)
        {
            fprintf(stdout, "%ld\n", ((caller_info_ptr - 1)->stack_ptr - caller_info_ptr->stack_ptr) * sizeof(int16_t) / sizeof(float));
        }
        else
        {
            fprintf(stdout, "\n");
        }

        /* Advance */
        caller_info_ptr++;
    }

    fprintf(stdout, "\n");

    return;
}



/*-------------------------------------------------------------------*
 * RAM Counting Tool
 *-------------------------------------------------------------------*/

/* How to use the tool notes
   =========================

   The tool measures dynamic memory allocated by malloc(), calloc() and deallocated by free().

   In order to run the tool, one needs to:
     1) Call mem_count_init() at the beginning of the encoding/decoding process
        - The first param allows to set a limit on the RAM that can be allocated.
          While the codec is running, if memory is requested such that the maximum
          is passed (Mem Alloc > Limit), the codec will stop and the summary will
          be printed.
        - The second param allows to decide the units (bytes, shorts or longs).
          Please note that the unit specified is combined with the limit param to
          produce the total size that can be allocated. Ex ..init(1000, USE_16BITS);
          will set the limit to 1000 x 16 Bits Word (2000 bytes in total).
     2) Call mem_count_summary() at the end of the encoding/decoding process to print-out the results.
        - The first parameter allows to overwrite the initial units configuration.
          The Total size allocated (always in bytes internally) will be converted
          to the unit selected before being reported.

   The macro 'WMOPS' needs to be activated to enable memory counting.
   To avoid the isntrumentation of any malloc()/calloc()/free() calls, wrap it with
   #define WMC_TOOL_MAN ... #undef WMC_TTOL_MAN macro pair.
   Optionally, it is possible to activate the macro 'MEM_COUNT_DETAILS' to print detailed sub-structure information.
*/

/* Local Constants */

/* This is the maximum number of allocations for which to keep information.
   It can be increased if required. */
#define MAX_INFO_RECORDS 3000

/* This is the length after which the function name
   will be truncated when the summary is printed. */
#define MAX_FUNCTION_NAME_LENGTH 18

/* This is the length after which the parameter to
   the allocating function will be truncated when
   the summary is printed. */
#define MAX_PARAMS_LENGTH 36

/* This is the value (in bytes) towards which the block size
   is rounded. For example, a block of 123 bytes, when using
   a 32 bits system, will end up taking 124 bytes since
   the last unused byte cannot be used for another block. */
#ifdef MEM_ALIGN_64BITS
#define BLOCK_ROUNDING 8
#else /* Align on 32 Bits Instead */
#define BLOCK_ROUNDING 4
#endif

#define N_32BITS_BLOCKS ( BLOCK_ROUNDING / sizeof( int32_t ) )

 /* Special Value to See if Memory was ever written */
#define MAGIC_VALUE_OOB  0x12A534F0           /* Put Before & After Buffer */
#define MAGIC_VALUE_USED ( ~MAGIC_VALUE_OOB ) /* To Detect if Memory was Written */
/* OOB Flags */
#define OOB_START 0x1
#define OOB_END   0x2

/* Local Macros */
#define ROUND_BLOCK_SIZE( n ) ( ( ( n ) + BLOCK_ROUNDING - 1 ) & ~( BLOCK_ROUNDING - 1 ) )
#define IS_CALLOC( str )      ( ( size_str[0] ) == 'c' )

/* Local Types */
typedef struct
{
    void* block_ptr;
    size_t block_size;
    int lineno;
    char name[MAX_FUNCTION_NAME_LENGTH + 1]; /* +1 for NUL */
    char params[1 + MAX_PARAMS_LENGTH + 1];  /* +1 for 'm'/'c' alloc & +1 for NUL */
    int used;
    int OOB_Flag;
    int seq_no; /* To Help Sorting by Order of Creation when all else is identical */
    int noccurances;
} allocator_record;

typedef allocator_record allocation_list[MAX_INFO_RECORDS];

/* Local Data */
static allocation_list Current_Allocations, Peak_Allocations, Freed_Allocations;
static size_t RAM_Limit = NO_RAM_LIMIT;      /* Always in Bytes */
static size_t Current_RAM = 0, Peak_RAM = 0; /* In Bytes */
static unsigned int Num_Records_Cur_RAM = 0, Num_Records_Peak_RAM = 0, Num_Records_Freed_RAM = 0;
static int Seq_No = 0;

static size_t Stat_Cnt_Size = USE_BYTES;

#ifndef DISABLE_NULL_PTR_FREE_WARNING
static allocation_list Missing_Allocations;
static unsigned int Num_Records_Missing_Alloc_Warnings = 0;
#endif

/* Suffix after numeral value printed in the summary */
/* One char for each size */
static const char Count_suffix[] = "BsL"; /* Using 's' because 'S' looks like '5'. */
static const char* Count_Name[] = { "Bytes", "Shorts", "Longs" };

/* Global Functions */

/* It is not necessary to call dyn_mem_stats_init() since all static
   variables are pre-initialised. It can be used to run stats again. */
void mem_count_init(size_t limit, Counting_Size cnt_size)
{
    if (cnt_size != USE_DEFAULT)
        Stat_Cnt_Size = cnt_size;
    Current_RAM = Peak_RAM = 0;
    RAM_Limit = limit << Stat_Cnt_Size;
    Num_Records_Cur_RAM = Num_Records_Peak_RAM = Num_Records_Freed_RAM = 0;
#ifndef DISABLE_NULL_PTR_FREE_WARNING
    Num_Records_Missing_Alloc_Warnings = 0;
#endif
    Seq_No = 0;
}

/* This Function basically uses 'malloc' to allocate memory but also
   records information about which functions allocated memory, the peak, ...
   When, dyn_mem_print_stats() is called, all this info is printed as
   well as the memory that has not been de-allocated; it is useful to
   find memory leaks. */
void* mem_alloc(
    const char* func_name,
    int func_lineno,
    size_t size,
    char* size_str /* the first char indicates m-alloc or c-alloc */)
{
    size_t rounded_size;
    void* block_ptr;
    char* tmp_ptr;
    allocator_record* record_ptr;
    size_t n, f;
    int32_t fill_value;
    int32_t* ptr32;
    int32_t mask, temp;
    unsigned int check;

    /* Do not Support Empty Requests */
    if (size == 0)
    {
        printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Size of Zero not Supported");
        exit(-1);
    }

    if (Num_Records_Cur_RAM == MAX_INFO_RECORDS)
    {
        printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Too Many Allocs. Increase 'MAX_INFO_RECORDS'");
        exit(-1);
    }

    /* Round Up Block Size */
    rounded_size = ROUND_BLOCK_SIZE(size);

    /* Allocate using the standard mem allocator.
       Allocate a bit More to Have Room for Signature Values */
    block_ptr = malloc(rounded_size + BLOCK_ROUNDING * 2);

    /* the split line is to prevent a 'search/replace' adding a '_' to the name */
    /* We request more memory to have room to put signatures at the start
       and end of the allocated buffer to check for OOBounds accesses. */
       /* Stop if it Failed */
    if (block_ptr == NULL)
    {
        printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Out of System RAM");
        exit(-1);
    }

    /* Cannot use #if sizeof(int32_t) to catch this at compile time
       because the preprocessor doesn't know anything about types
       or sizes. */
    check = sizeof(int32_t);
    if (check != 4)
    {
        printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Expecting 'int32_t' to be a 32 Bits Integer");
        exit(-1);
    }

    /* Set Signatures and Fill (or Clear) Memory */
    ptr32 = (int32_t*)block_ptr;
    /* Signature at Start of Block */
    n = N_32BITS_BLOCKS;
    do
    {
        *ptr32++ = MAGIC_VALUE_OOB;
    } while (--n);
    /* Fill with Pattern or Clear Memory */
    fill_value = MAGIC_VALUE_USED;
    if (IS_CALLOC(size_str))
    {
        fill_value = 0x00000000;
    }
    n = size / sizeof(int32_t);
    while (n--)
    {
        *ptr32++ = fill_value;
    }
    n = rounded_size - size;
    /* When Initializing with '0' */
    /* Need to Adapt the Magic Value */
    f = n % sizeof(int32_t);
    if (f != 0)
    {
        /* For f=, shift by [1->24, 2->16, 3->8] */
        mask = 0xFFFFFFFF << ((sizeof(int32_t) - f) * 8); /* (1) */
        temp = MAGIC_VALUE_OOB & mask;
        if (fill_value != 0x0)
        { /* Using M-Alloc */
            /* Merge Fill Value */
            temp += (~mask) & MAGIC_VALUE_USED;
        } /* for C-Alloc, the code in (1) hereabove already introduces zeros */
        *ptr32++ = temp;
    }
    n /= sizeof(int32_t);
    n += N_32BITS_BLOCKS;
    /* Signature at End of Block */
    do
    {
        *ptr32++ = MAGIC_VALUE_OOB;
    } while (--n);

    /* Adjust Pointer (Magic Value Before and After the Memory Region Requested) */
    tmp_ptr = (char*)block_ptr;
    tmp_ptr += BLOCK_ROUNDING;
    block_ptr = (void*)tmp_ptr;

    /* Save Information about Function Requesting the RAM */
    record_ptr = &Current_Allocations[Num_Records_Cur_RAM];

    /* Save Name (and NUL Terminate it) */
    strncpy(record_ptr->name, func_name, MAX_FUNCTION_NAME_LENGTH);
    record_ptr->name[MAX_FUNCTION_NAME_LENGTH] = '\0';
    /* Save Params (and NUL Terminate it)
       There string starts with a Marker (either 'm' or 'c')
       that indicates the type of allocation requested. */
    strncpy(record_ptr->params, size_str, MAX_PARAMS_LENGTH);
    record_ptr->params[MAX_PARAMS_LENGTH] = '\0';
    /* Save Other Info */
    record_ptr->lineno = func_lineno;
    record_ptr->block_ptr = block_ptr;
    record_ptr->block_size = size;
    record_ptr->used = -1; /* By default do not check mem usage */
    record_ptr->seq_no = ++Seq_No;
    record_ptr->noccurances = 1;

    /* Increase # of Records */
    Num_Records_Cur_RAM++;
    /* Update Amount of RAM Allocated */
    Current_RAM += rounded_size;

    /* Is this the Worst Case */
    if (Peak_RAM < Current_RAM)
    { /* Yes */
        /* Update the Peak RAM */
        Peak_RAM = Current_RAM;
        /* Keep the Information */
        memmove(Peak_Allocations, Current_Allocations, sizeof(allocator_record) * Num_Records_Cur_RAM);
        Num_Records_Peak_RAM = Num_Records_Cur_RAM;
    }

    /* Limit Busted? */
    if (RAM_Limit != NO_RAM_LIMIT)
    {
        if (Current_RAM > RAM_Limit)
        {
            char info_msg[100];
            sprintf(info_msg, "Alloc Limit of %lu %s was Passed", (unsigned long)RAM_Limit >> Stat_Cnt_Size, Count_Name[Stat_Cnt_Size]);
            printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, info_msg);

#ifdef MEM_COUNT_DETAILS
            mem_count_summary(USE_DEFAULT);
#endif
            exit(-1);
        }
    }

    return block_ptr;
}

/* Calculate Memory Usage of Block (Look for Signature) */
static void mem_set_usage(allocator_record* record_ptr)
{
    int total_bytes_used;

    size_t n;
    int32_t* ptr32;
    char* ptr8;
    size_t total_bytes;
    int32_t fill_value;

    fill_value = MAGIC_VALUE_USED;
    if ((record_ptr->params[0]) == 'c')
    {
        fill_value = 0x00000000;
    }

    total_bytes = record_ptr->block_size;

    /* Check 4 bytes at a time */
    ptr32 = (int32_t*)record_ptr->block_ptr;
    total_bytes_used = 0;
    for (n = total_bytes / sizeof(int32_t); n > 0; n--)
    {
        if (*ptr32++ != fill_value)
            total_bytes_used += sizeof(int32_t);
    }

    /* Check Remaining Bytes (If Applicable) */
    ptr8 = (char*)ptr32;
    for (n = total_bytes % sizeof(int32_t); n > 0; n--)
    {
        if (*ptr8++ != (char)fill_value)
            total_bytes_used++;
        /* Update Value */
        fill_value >>= 8;
    }

    /* Save Space Used */
    record_ptr->used = total_bytes_used;
}

static void mem_check_OOB(allocator_record* record_ptr)
{
    int32_t* ptr32;
    unsigned int OOB_Flag = 0x0;
    int32_t mask;
    size_t i;
    int f;

    ptr32 = (int32_t*)record_ptr->block_ptr - N_32BITS_BLOCKS;
    /* Check at Beginning of Block */
    i = N_32BITS_BLOCKS;
    do
    {
        if (*ptr32++ ^ MAGIC_VALUE_OOB)
            OOB_Flag |= OOB_START;
    } while (--i);

    /* Advance to End (Snap to lowest 32 Bits) */
    ptr32 += record_ptr->block_size / sizeof(int32_t);

    /* Calculate Unused Space That has been
       added to get to the rounded Block Size */
    i = ROUND_BLOCK_SIZE(record_ptr->block_size) - record_ptr->block_size;

    /* Partial Check (For Block Size that have been rounded) */
    f = i % sizeof(int32_t);
    if (f != 0)
    {
        mask = 0xFFFFFFFF << ((sizeof(int32_t) - f) * 8);
        if ((*ptr32++ ^ MAGIC_VALUE_OOB) & mask)
            OOB_Flag |= OOB_END;
    }

    /* Full Check (all 32 Bits) for Remaining */
    i /= sizeof(int32_t);
    i += N_32BITS_BLOCKS;
    do
    {
        if (*ptr32++ ^ MAGIC_VALUE_OOB)
            OOB_Flag |= OOB_END;
    } while (--i);

    record_ptr->OOB_Flag = OOB_Flag;
}

/* Just to make the code cleaner */
static int is_same_record(const allocator_record* record_ptr1, allocator_record* record_ptr2)
{
    return record_ptr2->block_size == record_ptr1->block_size &&
        record_ptr2->lineno == record_ptr1->lineno &&
        strcmp(record_ptr2->name, record_ptr1->name) == 0 &&
        strcmp(record_ptr2->params, record_ptr1->params) == 0 &&
        record_ptr2->OOB_Flag == record_ptr1->OOB_Flag ? 1 : 0;
}

/* This Function basically uses 'free' and removes the
   Information about the memory block from the list of
   currently allocated Memory */
void mem_free(const char* func_name, int func_lineno, void* ptr)
{
    unsigned int i, j, k;
    char* tmp_ptr;
    allocator_record* record_ptr, * record_ptr2;

    char info_msg[100];

    /* Search for the Block Pointer in the List */
    record_ptr = &Current_Allocations[0];
    for (i = 0; i < Num_Records_Cur_RAM; i++)
    {
        /* Same Pointer? */
        if (ptr == record_ptr->block_ptr)
        { /* Yes, Found it */
            if (Num_Records_Freed_RAM == MAX_INFO_RECORDS)
            {
                printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Too Many Allocs. Increase 'MAX_INFO_RECORDS'");
                exit(-1);
            }

            for (j = 0; j < Num_Records_Peak_RAM; j++)
            {
                /* Is this Block Part of the Peak RAM? */
                if (memcmp(record_ptr, &Peak_Allocations[j], sizeof(*record_ptr)) == 0)
                {
                    break; /* Stop the 'j' loop */
                }
            }

            mem_set_usage(record_ptr);
            mem_check_OOB(record_ptr);

            /* De-Allocated Block was Part of Peak RAM? */
            if (j == Num_Records_Peak_RAM)
            { /* No */
                /* Here, in order to avoid filling this list with repetitive blocks */
                /* that are allocated and deallocated repeatedly, we look for a block */
                /* that has the same module, line #, size & same OOB characteristics. */
                /* We then just increase the # of occurances of this block definition. */
                /* The % used will be merged */
                record_ptr2 = &Freed_Allocations[0];
                for (k = 0; k < Num_Records_Freed_RAM; k++)
                {
                    /* Same Block but allocated many times */
                    if (is_same_record(record_ptr2, record_ptr))
                    {
                        record_ptr2->noccurances++;
                        record_ptr2->used += record_ptr->used;
                        break;
                    }
                    record_ptr2++;
                }
                /* Found it */
            }
            else
            {
                /* Force Add to List */
                k = Num_Records_Freed_RAM;
            }

            if (k == Num_Records_Freed_RAM)
            { /* No */
                /* Add to List */
                memmove(&Freed_Allocations[Num_Records_Freed_RAM], record_ptr, sizeof(allocator_record));
                /* Increase # of Records for Deallocated Block List */
                Num_Records_Freed_RAM++;
            }

            /* De-Allocated Block was Part of Peak RAM? */
            if (j != Num_Records_Peak_RAM)
            { /* Yes */
                /* Update Block Info There too (The Info here is Bytes Used and OOB Flags) */
                memmove(&Peak_Allocations[j], record_ptr, sizeof(*record_ptr));
            }

            /* First Adjust Pointer to Get to the Start of the Block */
            tmp_ptr = (char*)ptr;
            tmp_ptr -= BLOCK_ROUNDING;
            ptr = (void*)tmp_ptr;
            /* De-allocate using the standard memory facilities */
            free(ptr); /* the split line is to prevent a 'search/replace' adding a '_' to the name */

                            /* Decrease # of Records */
            Num_Records_Cur_RAM--;
            /* Update (decrease) Amount of RAM Allocated */
            Current_RAM -= ROUND_BLOCK_SIZE(record_ptr->block_size);

            /* Erase the entry (Move the Last One over it) */
            memmove(record_ptr, &Current_Allocations[Num_Records_Cur_RAM], sizeof(allocator_record));

            return;
        }
        record_ptr++;
    }

    /* Not Found, Problem! */
    if (ptr != NULL)
    {
        /* Stop */
        sprintf(info_msg, "Invalid Pointer: '%p'", ptr);
        printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, info_msg);
        exit(-1);
    }
#ifndef DISABLE_NULL_PTR_FREE_WARNING
    else
    { /* Warn about Freeing of NULL Pointers */
        /* Search to Warn Only Once. */
        record_ptr = &Missing_Allocations[0];
        for (i = 0; i < Num_Records_Missing_Alloc_Warnings; i++)
        {
            /* Same Name? */
            if (strncmp(record_ptr->name, func_name, MAX_FUNCTION_NAME_LENGTH) == 0)
            { /* Yes */
                /* Same Line Number? */
                if (record_ptr->lineno == func_lineno)
                { /* Yes */
                    /* No Need to Warn Again */
                    return;
                }
            }
            record_ptr++;
        }
        /* Save Name */
        strncpy(record_ptr->name, func_name, MAX_FUNCTION_NAME_LENGTH);
        record_ptr->name[MAX_FUNCTION_NAME_LENGTH] = '\0';

        /* Save Line No */
        record_ptr->lineno = func_lineno;
        /* Save Pointer */
        record_ptr->block_ptr = ptr;
        /* Save Size */
        record_ptr->block_size = 0; /* It is an Unknown Block, so there is no size, we put a '0' */

        Num_Records_Missing_Alloc_Warnings++;

        printf("Fct=%s, Ln=%i: %s!\n", func_name, func_lineno, "Warning: Trying to Free 'NULL' Pointer");
    }
#endif

    return;
}

#ifdef MEM_COUNT_DETAILS
/* Used to Sort Block Information Records */
static int compare_mem_records(const void* ptr1, const void* ptr2)
{
    const allocator_record* record_ptr1 = (const allocator_record*)ptr1,
        * record_ptr2 = (const allocator_record*)ptr2;
    int result;

    /* Sort First by 'Name', then by 'Line No' and finaly by 'Block Size' */

    /* Compare Function Name */
    result = strcmp(record_ptr1->name, record_ptr2->name);
    /* Same Function Name? */
    if (result == 0)
    { /* Yes */
        /* Compare Line Number */
        result = record_ptr1->lineno - record_ptr2->lineno;
        /* Same Line Number */
        if (result == 0)
        { /* Yes */
            /* Compare Block Size */
            result = record_ptr1->block_size - record_ptr2->block_size;
            /* Same Size? */
            if (result == 0)
            {
                result = record_ptr1->seq_no - record_ptr2->seq_no;
            }
        }
    }

    return result;
}

static void mem_print_records(allocator_record* record_ptr, int n_records, Counting_Size cnt_size, int print_size)
{
    int i, sum, total;
    char* last_name = NULL;
    int single_flag;
    char chr = Count_suffix[cnt_size];
    size_t nwords, rounded_block_size;
    char nblocks_str[22];
    char quantity_str[22];
    char format_str[50];
    char* tmp_ptr;
    int nblocks;

    char* OOB_str[] = { "- ", "beg", "end", "b+e" };

    strcpy(format_str, "\n    %-*s %5i %-*s %3s %3.0f%% %8s");
    if (print_size != 0)
        strcat(format_str, " %6u");

    /* Sort it */
    qsort(record_ptr, n_records, sizeof(allocator_record), compare_mem_records);

    single_flag = 0;

    nblocks = 0;

    if (n_records > 0)
    {
        /* Print Header */
        printf("    %-*s Ln No %-*s OOB Used Quantity%s", MAX_FUNCTION_NAME_LENGTH, "Function Name", MAX_PARAMS_LENGTH, "Parameters ('m' or 'c' alloc)", print_size != 0 ? "   Size" : "");

        total = sum = 0;
        for (i = n_records - 1; i >= 0; i--)
        {
            if (sum == 0)
            {
                last_name = record_ptr->name;
            }

            rounded_block_size = ROUND_BLOCK_SIZE(record_ptr->block_size) >> cnt_size;
            nwords = rounded_block_size * record_ptr->noccurances;

            /* Calc Usage (It has already been done for a De-Allocated Block
               That was part of the Peak Mem. But not for a Block that is
               still allocated (never freed). */
            if (record_ptr->used < 0)
            {
                mem_set_usage(record_ptr);
                /* Check Out of Bounds Too */
                mem_check_OOB(record_ptr);
            }

            sprintf(quantity_str, "%i", record_ptr->noccurances);

            /* Print Quantity x Size */
            tmp_ptr = strchr(quantity_str, '\0');
            *tmp_ptr++ = 'x';
            sprintf(tmp_ptr, "%i", (int)rounded_block_size);

            printf(format_str, MAX_FUNCTION_NAME_LENGTH, record_ptr->name, record_ptr->lineno, MAX_PARAMS_LENGTH, record_ptr->params, OOB_str[record_ptr->OOB_Flag], (record_ptr->used * 100.0f / record_ptr->block_size / record_ptr->noccurances), quantity_str, (uint32_t)nwords);
            /* Count $ of Blocks */
            nblocks += record_ptr->noccurances;
            /* Add Size */
            sum += nwords;
            /* Advance */
            record_ptr++;

            if (print_size != 0)
            {
                /* End Reached or New Function? */
                if (i == 0 || strcmp(last_name, record_ptr->name) != 0)
                {
                    /* Cumulate Total */
                    total += sum;

                    if (i == 0 && total == sum)
                        single_flag = 1;
#ifdef MEM_COUNT_TOTAL_PER_FUNC
                    /* Print 'Total' on the Line if there is only one Function
                       that allocated all the RAM */
                    printf(" %7i%c", sum, chr);
#endif
                    /* Reset Sum */
                    sum = 0;
                }
            }
        }
        /* Print the Total */
        if (!single_flag && print_size != 0)
        {
#ifdef MEM_COUNT_TOTAL_PER_FUNC
#define SPACES 9
#else
#define SPACES 0
#endif

            sprintf(nblocks_str, "%i Blocks", nblocks);

            printf("\n    %*s %*s                      %*s  --------\n", MAX_FUNCTION_NAME_LENGTH, "", MAX_PARAMS_LENGTH, "", SPACES, "");
            printf("    %-*s       %-*s                  %*s%7i%c\n", MAX_FUNCTION_NAME_LENGTH, "Total", MAX_PARAMS_LENGTH, nblocks_str, SPACES, "", total, chr);
        }
        else
            printf("\n");
    }
    else
    {
        printf("    <None>\n");
    }
}
#endif

#ifdef MEM_COUNT_DETAILS
/* Detailed SRAM Summary */
size_t mem_count_summary(Counting_Size cnt_size)
{
    printf("\n\n====== STATIC RAM COUNTING TOOL: MEMORY SUMMARY ======\n\n");

    printf("  NOTES\n"
        "  1) %c = %s, %c = %s (16 Bits Words) & %c = %s (32 Bits Words).\n"
        "  2) The '%%' of 'Used' memory is not very reliable for c-alloc.\n"
        "  3) The Out Of Bounds (OOB) Checking is primitive as it checks\n"
        "     only for writing before and past the buffer when it is freed\n"
        "     or when the summary is printed.\n"
        "  4) Quantity Filed format is N x Block Size.\n"
        "     When it is 1: Size = Block Size\n"
        "     Otherwise, Size = The Total Size Allocated for the 'N' Blocks.\n"
        "\n",
        Count_suffix[USE_BYTES], Count_Name[USE_BYTES], Count_suffix[USE_16BITS], Count_Name[USE_16BITS], Count_suffix[USE_32BITS], Count_Name[USE_32BITS]);

    /* Create a Temporary List of Block to Print */
    /* from all the Block Allocated in the Peak */
    /* Scenario. We want to Merge Entries allocated */
    /* from the exact same code location and having */
    /* the same block size. In order to simplifiy the */
    /* printout. */
    num = 0;
    for (i = 0; i < Num_Records_Peak_RAM; i++)
    {
        /* Check if an Identical Block Before Adding to List */
        for (j = 0; j < num; j++)
        {
            if (is_same_record(&Peak_Allocations[i], &Allocations[j]))
            {
                Allocations[j].noccurances++;
                Allocations[j].used += Peak_Allocations[i].used;
                break;
            }
        }

        /* Unique Block? */
        if (j == num)
        { /* Yes */
            /* Add to List */
            memmove(&Allocations[num++], &Peak_Allocations[i], sizeof(allocator_record));
        }
    }
    /* Print the Peak Allocated */
    printf("  Peak Memory Allocated\n");
    mem_print_records(&Allocations[0], num, cnt_size, 1);
    printf("\n");

    /* Create a Temporary List of Block to Print */
    /* from all the Block Allocated BUT excluding */
    /* blocks that are part of the peak (and that */
    /* are already printed). */
    /* This is done every time the summary is printed */
    /* so that the code can continue to be executed */
    /* and the summary can be printed again with blocks */
    /* that are part of the peak having changed and thus */
    /* those that end up in this 'catch all' category. */
    num = 0;
    for (i = 0; i < Num_Records_Freed_RAM; i++)
    {
        /* Check if Record is in the Peak */
        for (j = 0; j < Num_Records_Peak_RAM; j++)
        {
            /* Is this Block Part of the Peak RAM OR Still Allocated ? */
            if (is_same_record(&Freed_Allocations[i], &Peak_Allocations[j]))
            {          /* Yes */
                break; /* Stop the 'j' loop */
            }
        }
        /* If part of the Peak, skip it */
        if (j == Num_Records_Peak_RAM)
        { /* Not Part of Peak */
            /* Check if an Identical Block Before Adding to List */
            for (j = 0; j < num; j++)
            {
#ifndef MEM_COUNT_SEPARATE_OTHER_BLOCKS
                /* Just Check for Same Size, Params and OOB Result */
                if (Freed_Allocations[i].block_size == Allocations[j].block_size &&
                    strcmp(Freed_Allocations[i].params, Allocations[j].params) == 0 &&
                    Freed_Allocations[i].OOB_Flag == Allocations[j].OOB_Flag)
                {
                    Allocations[j].noccurances += Freed_Allocations[i].noccurances;
                    Allocations[j].used += Freed_Allocations[i].used;
                    /* Indicate that Locations are Multiple */
                    strncpy(Allocations[j].name, "MULTIPLE LOCATIONS", MAX_FUNCTION_NAME_LENGTH);
                    /* Wipe the Line # */
                    Allocations[j].lineno = 0;
                    break;
                }
#else
                if (is_same_record(&Freed_Allocations[i], &Allocations[j]))
                {
                    Allocations[j].noccurances++;
                    Allocations[j].used += Freed_Allocations[i].used;
                    break;
                }
#endif
            }

            /* Unique Block? */
            if (j == num)
            { /* Yes */
                /* Add to List */
                memmove(&Allocations[num++], &Freed_Allocations[i], sizeof(allocator_record));
            }
        }
    }

    if (num != 0)
    {
        /* Print all Other Block (Those that have been Freed but are not Part
           of the Peak Memory). */
        printf("  Other Memory Allocated\n");
        mem_print_records(&Allocations[0], num, cnt_size, 0 /*Do not print the size column*/);
        printf("\n");
    }

    /* If we have busted the RAM Limit, we will end up in the
       summary function (here) and abort. Hence, no point printing
       the memory still allocated, it will be the same as the Peak! */
    if (RAM_Limit == 0 || Current_RAM <= RAM_Limit)
    {
        /* Print the Not Deallocated */
        printf("  Memory Still Allocated\n");
        mem_print_records(&Current_Allocations[0], Num_Records_Cur_RAM, cnt_size, 1);
        printf("\n");
    }

    return;
}
#endif

void reset_mem(size_t limit, Counting_Size cnt_size)
{
    reset_stack();
    mem_count_init(limit, cnt_size);
}


void print_mem( ROM_Size_Lookup_Table Const_Data_PROM_Table[] )
{
    int i, nElem;

    fprintf(stdout, "\n\n --- PROM usage ---  \n\n");

    nElem = 0;
    while (strcmp(Const_Data_PROM_Table[nElem].file_spec, "") != 0) nElem++;

    for (i = 0; i < nElem; i++)
    {
        fprintf(stdout, "PROM size (%s): %d words (or instructions)\n", Const_Data_PROM_Table[i].file_spec, Const_Data_PROM_Table[i].PROM_size);
    }

    fprintf(stdout, "\n\n --- Table ROM (const data) usage ---  \n\n");

    for (i = 0; i < nElem; i++)
    {
        if (Const_Data_PROM_Table[i].Get_Const_Data_Size_Func == NULL)
        {
            fprintf(stdout, "Error: Cannot retrieve or calculate Table ROM size of (%s)!\n", Const_Data_PROM_Table[i].file_spec);
        }

        fprintf(stdout, "Table ROM size (%s): %d words\n", Const_Data_PROM_Table[i].file_spec, Const_Data_PROM_Table[i].Get_Const_Data_Size_Func() );
    }

    fprintf(stdout, "\n\n --- Stack usage ---  \n\n");

    fprintf(stdout, "Stack size (encoder):                     %ld words in %s() in frame #%d\n", ((ptr_base_stack - ptr_max_stack) * sizeof(int16_t)) / sizeof(float),
            location_max_stack, wc_frame);
    print_stack_call_tree();

    fprintf(stdout, "Note: this is an optimistic estimate of the memory consumption assuming\n");
    fprintf(stdout, "      that each variable (short, long or float) in the codec requires\n");
    fprintf(stdout, "      32 bits of memory and may therefore be represented by 1 word.\n");
    fprintf(stdout, "      The following formula is used: sizeof('memory array')/sizeof(float)\n\n");

    /* Static RAM usage */
    fprintf(stdout, "\n\n --- Static RAM usage ---  \n\n");
    printf("Peak SRAM Allocated: %i%c\n", (int)(Peak_RAM >> Stat_Cnt_Size), Count_suffix[Stat_Cnt_Size]);

#ifdef MEM_COUNT_DETAILS
    /* Print detailed SRAM info */
    mem_count_summary(USE_DEFAULT);
#else
    if (Num_Records_Cur_RAM > 0)
    {
        printf("\nWarning: Part of the SRAM is still allocated! Activate MEM_COUNT_DETAILS for more details.\n");
    }
#endif

    return;
}


#endif /* WMOPS */

#ifndef WMOPS
int cntr_push_pop = 0;      /* global counter for checking balanced push_wmops()/pop_wmops() pairs when WMOPS is not activated */
#endif






