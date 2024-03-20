/*------------------------------------------------------------------------------------------*
 * Testing file
 *------------------------------------------------------------------------------------------*/
 
#include <stdio.h>
#include <string.h>
#ifdef WMOPS
#include "wmc_auto.h"
#endif
#include <stdlib.h>
#ifdef DEBUGGING
#include "debug.h"
#endif
#include "render_config_reader.h"

#define WMC_TOOL_SKIP

/*------------------------------------------------------------------------------------------*
 * Local constants, enums, structures
 *------------------------------------------------------------------------------------------*/

#if !defined( DEBUGGING ) && !defined( WMOPS )
static
#endif
    int32_t frame = 0; /* Counter of frames */

#define MIN_NUM_BITS               5698
#define NUM_BITS_SILENCE_FRAME     88
#define NUM_BITS_ACTIVE_FR_TYPE    156


/*------------------------------------------------------------------------------------------*
 * Local functions prototypes
 *------------------------------------------------------------------------------------------*/

static bool parseCmdlCodec( int16_t argc, char **argv, DecArguments *arg );
static void usage_codec( void );
#ifdef DEBUGGING
static int16_t app_own_random( int16_t *seed );
#endif


/*------------------------------------------------------------------------------------------*
 * main()
 *
 * Main function for command-line interface
 *------------------------------------------------------------------------------------------*/

int main(
    int argc,
    char *argv[] )
{ 

#ifdef WMOPS
    print_wmops();
    print_mem( NULL );
#endif
    return 0;
}

#undef WMC_TOOL_SKIP