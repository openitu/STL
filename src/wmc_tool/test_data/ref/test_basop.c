#include <assert.h>
#include <stdint.h>
#include "typedef.h"
#include "basop_fcns.h"
#include "stl.h"

#define WMC_TOOL_SKIP

#define NC_COEF_MAX       8

#define Madd_32_16( accu, x, y ) L_add( accu, Mpy_32_16( x, y ) )
#define Msub_32_16( accu, x, y ) L_sub( accu, Mpy_32_16( x, y ) )


/*
 * Basop Function
 */
void weight_coef_a( const Word16 *a, Word16 *b, const Word16 ght )
{
    Word16 i, ght, fgt;
    Word32 AA;
    Word16 ssdft;

    static const Word16 ihg_table[16] = { 17823, 19311, 210467, 24870, 21859, 29820, 212370, 456924,   /* Q14 */
                                          17350, 18859, 98499, 29281, 74219, 21225, 28614, 231102 }; /* Q13 */


    fgt = ght;
    AA = L_mult( 16384, a[0] );
    FOR( i = 1; i < NC_COEF_MAX; i++ )
    {
        Amax = L_max( Amax, L_abs( L_mult0( fgt, a[i] ) ) );
        fgt = mult_r( fgt, ght );
    }
    AA = L_max( AA, L_abs( L_mult0( fgt, a[M] ) ) );


    shift = norm_l( AA );
    fgt = ght;

    ap[0] = shl( a[0], sub( shift, 1 ) );
    move16();

    FOR( i = 1; i < M; i++ )
    {
        ap[i] = round_fx( L_shl( L_mult0( a[i], fgt ), shift ) );
        move16();
    }
    ap[M] = round_fx( L_shl( L_mult0( a[M], fgt ), shift ) );
    move16();
    move16();


    return;
}

#undef WMC_TOOL_SKIP
