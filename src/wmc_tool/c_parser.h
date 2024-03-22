/*
 * (C) 2023 copyright VoiceAge Corporation. All Rights Reserved.
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

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h> 
#include "wmc_tool.h"    
#include "parsing_defs.h" 

/*-------------------------------------------------------------------*
 * Prototypes
 *-------------------------------------------------------------------*/

void Free_Parse_Table_Memory( Parse_Tbl_def *ParseTable_ptr );
void Free_Insert_Table_Memory( Insert_Tbl_def *InsertTable_ptr );
void Free_FctCall_Table_Memory( FctCall_Tbl_def *FctCallTable_ptr );
void Free_Pointer_Table_Memory( Pointer_Tbl_def *PointerTable_ptr );

TOOL_ERROR Setup_Regions( Parse_Context_def *ParseContext_ptr, bool verbose );
TOOL_ERROR DesInstrument( Parse_Context_def *ParseContext_ptr, bool keep_manual );
TOOL_ERROR DesInstrument_ROM(Parse_Context_def* ParseCtx_ptr);
TOOL_ERROR Instrument(Parse_Context_def* ParseContext_ptr, bool instr_const);
TOOL_ERROR Instrument_Const_Data_PROM_Table( Parse_Context_def *ParseContext_ptr, T_FILE_BOOK file_book[], int nRecords );
TOOL_ERROR Finalize( Parse_Context_def *ParseContext_ptr );
TOOL_ERROR Include_Header(Parse_Context_def* ParseContext_ptr, char** ptr_end_preproc_block );

void Print_Information( void );

int Print_Warnings( Parse_Context_def *ParseContext_ptr,
                    unsigned int tool_warning_mask,
                    bool warn_once,
                    bool di_only );

#endif /* #ifndef __PARSER_H__ */
