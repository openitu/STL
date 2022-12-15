        #include <stdint.h>
        #include "options.h"
        #ifdef DEBUGGING
        #include "debug.h"
        #endif
        #include <math.h>
        #include "cnst_file.h"
        #include "rom_states_ind.h"
        #include "prototyp.h"
/*AddedByWMC_Tool*/#include "wmc_auto.h"
/*AddedByWMC_Tool*/#define date_from_struct_ func_(date_from_struct,3)
/*AddedByWMC_Tool*/#define     My_function1_ func_(My_function1,2)
/*AddedByWMC_Tool*/#define     vad_function_ func_(vad_function,2)


        const int16_t try_yt = 0.74834f;        /* try constant */

        #define WMC_TOOL_SKIP
                const float fr_tr65[L_NON_BRT] =
                {
                    4.1, 4.5f, 5.2, 6, 7, 8.67,
                    4.3, 4.5f, 5.3, 6, 7, 8.89,
                    4.45, 4.5f, 5.45, 6, 7, 8.3,
                    4.7, 4.5f, 5.6, 6, 7, 8.8,
                };
        #undef WMC_TOOL_SKIP

        const float Ratios_WB_7[L_LEN_G] =
        {
            1, 1.5f, 2, 2.5f, 3, 3.5f, 4, 4.5f, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22.5f, 24, 25, 27, 28, 30, 35
        };

        const int16_t my_array_of_indices[SIZE_STATE][LEN_ELEM] = {
            5, 6, 8, 45, 9, 1, 7,
            5, 6, 8, 45, 9, 1, 7,
            5, 6, 8, 45, 9, 1, 7,
            5, 6, 8, 45, 9, 1, 7,
            5, 6, 8, 45, 9, 1, 7,
            5, 6, 8, 45, 9, 1, 7 };


        void Start_Noise_Envelope(
            State_frame *st,
            const int16_t L_frame,
            const SHAPE_FRAME_NET whitening_mode
        )
        { func_start_
            float noise_state_greed[L_TYPE_VECTOR];
            int16_t inc, start_idx, stop_idx;
            float *noiseLevelPtr, *ssn_bg, *ssn_shapr_tempo;
$("*")      Transitional_Tempo **transParams;
            HANDLE_DDT_FGR hDdtFgr;
$("[]")     float scc_gbn_tied[FDD_PPTS];

            push_wmops("Start_Noise_Envelope");

$("M")      inc = 1;
            inc++;

$("Ee")     if_ (inc == 1)
            {
                vad_function_(start_idx, *transParams);
            }

$("->M*")   hBabler_mem->ccs_dir = (float *) malloc_( sizeof( float ) * size );
        #define WMC_TOOL_SKIP
            p_dir_out_region = (float *) malloc( sizeof( h_Out_Region * m_outputs );
        #undef WMC_TOOL_SKIP

$("M/[]*")  enr1 = 1.0f / sqrt_( enr_norm[j] * enr1 ); /* 1/sqrt(energy)    */
$("M[]*")   enr1 = inv_sqrt_( enr_norm[j] * enr1 ); /* 1/sqrt(energy)    */

$("M")      start_idx = My_function1_(start_idx, &hDdtFgr);

            pop_wmops();

            return_;
        }


        float Another_function(
            AnalY_St *st,
            int bgf,
            const HGR_TEMP mod_ftr
        )
        { func_start_
            float ftrm, bgr;
            int16_t i, idx;
            float **my_ptr;


$("M")      ftrm = 0;
$("+=")     ftrm += (int16_t)bgf;

            return_ ftrm;
        }

        float Define_State_Place(
            int16_t *tied_state,
            int16_t bgf[],
            const float fgtr
        )
        { func_start_
            float ff, hjk;
            int16_t id1, id2, id3;
            float **ptr_comp;
            float define_place[23];


$("M")      ff = 0;
$("+=")     ff += date_from_struct_(fgtr, bgf, &ff);

            return_ hjk;
        }
/*AddedByWMC_Tool*/ int PROM_Size_Func( test_file1 ) { return 27; }
