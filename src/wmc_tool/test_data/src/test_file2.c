/*====================================================================================
    COMMENTS
  ====================================================================================*/

#include <math.h>
#include "wmops.h"


/*-------------------------------------------------------------------*
 * Arbitrary function
 *
 *-------------------------------------------------------------------*/

#define MAX_SHIFT 0.534f

float calc_shift_value(       /* o  : asdfasdf     */
    const float totalStep     /* i  : fsdfg */
)
{
    float sdf, reset_value;

    reset_value = 0.23f;
    if( totalStep > 23.3456f )  /* make it double 0.256*/
    {
        /* try out some other stuff */
        reset_value = 56.345e-4f * (float)exp( 34.456 * totalStep ) - 2.65f;
    }
    
    if( reset_value > MAX_SHIFT )
    {
        reset_value = MAX_SHIFT;
    }
    return reset_value;
}
