    /*====================================================================================
        COMMENTS
      ====================================================================================*/

    #include <math.h>
    #include "wmops.h"
/*AddedByWMC_Tool*/#include "wmc_auto.h"


    /*-------------------------------------------------------------------*
     * Arbitrary function
     *
     *-------------------------------------------------------------------*/

    #define MAX_SHIFT 0.534f

    float calc_shift_value(       /* o  : asdfasdf     */
        const float totalStep     /* i  : fsdfg */
    )
    { func_start_
        float sdf, reset_value;

$("M")  reset_value = 0.23f;
$("Gt") if_( totalStep > 23.3456f )  /* make it double 0.256*/
        {
            /* try out some other stuff */
$("M**-")   reset_value = 56.345e-4f * (float)exp_( 34.456 * totalStep ) - 2.65f;
        }

$("Gt") if_( reset_value > MAX_SHIFT )
        {
$("M")      reset_value = MAX_SHIFT;
        }
        return_ reset_value;
    }
/*AddedByWMC_Tool*/ int PROM_Size_Func( test_file2 ) { return 12; }
