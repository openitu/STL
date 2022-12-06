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
/*AddedByWMC_Tool*/#ifdef WMOPS
/*AddedByWMC_Tool*/extern int Const_Data_Size_test_rom(void);
/*AddedByWMC_Tool*/ROM_Size_Lookup_Table Const_Data_PROM_Table[] =
/*AddedByWMC_Tool*/{
/*AddedByWMC_Tool*/    { "out/test_rom.c", 0, Const_Data_Size_test_rom },
/*AddedByWMC_Tool*/    { "", -1, NULL }
/*AddedByWMC_Tool*/};
/*AddedByWMC_Tool*/#endif

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
    print_mem( Const_Data_PROM_Table );
#endif
    return 0;
}
