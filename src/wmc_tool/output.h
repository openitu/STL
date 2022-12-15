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

#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <stdlib.h> 

/*-------------------------------------------------------------------*
 * Global Constants
 *-------------------------------------------------------------------*/

#define MAX_CHARS_PER_LINE 160

/*-------------------------------------------------------------------*
 * Global Macros
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 * Prototypes
 *-------------------------------------------------------------------*/

void Print( const char *format, ... );
void Error( const char *format, ... );
void Print_Error( const char *msg = NULL );
void Print_Warning( const char *format, ... );
//void Print_Info( const char *format, ... );

#endif /* #ifndef __OUTPUT_H__ */
