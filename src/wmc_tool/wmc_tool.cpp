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
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>

#ifndef _MSC_VER
#include <dirent.h>
#include <errno.h>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#else
#include <windows.h>
#endif

#include "wmc_tool.h"
#include "constants.h"
#include "parsing_defs.h"
#include "c_parser.h"
#include "output.h"
#include "text_utils.h"

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef VERSION_STL
#define VERSION_STL "STL2023"
#endif

/*-------------------------------------------------------------------*
 * Local Constants
 *-------------------------------------------------------------------*/

#define FILE_MEM_INCREMENT 4 * 4096 /* in bytes */
#define FILE_MEM_EXTRA     4 * 8    /* in bytes */
#define MAX_RECORDS        500      /* maximum number of file specification on the command line */

/* Operating Modes */
#define DESINSTRUMENT_ONLY   0x001
#define REMOVE_MANUAL_INSTR  0x002
#define VERBOSE              0x004
#define NO_BACKUP            0x020
#define INSTR_INFO_ONLY      0x040
#define OUTPUT_WMOPS_FILES   0x080
#define INSTRUMENT_ROM       0x100

/* Other Constants */
#define BACKUP_SUFFIX        ".bak"


 /*-------------------------------------------------------------------*
  * Parse_Command_Line
  *-------------------------------------------------------------------*/

static void usage()
{
    Print("Usage: wmc_tool [options] filename1 filename2 ...\n\n"
          "WMC tool v%s - %s\n\n"
          "Mandatory arguments:\n"
          "     space-separated list of filenames or directories with file mask, e.g. ./lib_enc/array*.c\n"
          "     note: if file mask is not specified *.c is assumed by default\n\n"
          "Options:\n"
          "     -h [--help]: print help\n"
          "     -v [--verbose]: print warnings and other information messages\n"
          "     -i [--info-only]: only print instrumentation information\n"
          "     -d [--desinstrument]: desintrument only\n"
          "     -m filename [--rom filename]: add statistics about ROM and RAM consumption\n"
          "        note: filename shall point to a .c file containing the print_mem() function\n"
          "     -b [--no-backup]: no backup of original files\n"
          "     -c dirname [--generate-wmc-files dirname]: copy wmc_auto.h and wmc_auto.c to a user-specified directory\n\n",
          WMC_TOOL_VERSION_NO, VERSION_STL);

    return;
}

static TOOL_ERROR Parse_Command_Line(
    int nargs,
    char* args[],
    int *i_last_cmd__line_option,
    int* Operation,
    unsigned int* Tool_Warning_Mask,
    char* Const_Data_PROM_File,
    char* wmops_output_dir
)
{
    int i;
    char *arg_name;
    char PathName[MAX_PATH + 1];    /* +1 for NUL Char*/
    struct stat s;

    strcpy(Const_Data_PROM_File, "");
    strcpy(wmops_output_dir, "");

    /* Check # of Args */
    if (nargs < 1)
    {
        usage();
        fprintf(stderr, "Missing Arguments!\n\n");
        return ERR_MISSING_ARGS;
    }

    /* Check if the user specified -h [--help] */
    for (i = 0; i < nargs; i++)
    {
        if (_stricmp(args[i], "-h") == 0 || _stricmp(args[i], "--help") == 0)
        {
            usage();
            return ERR_HELP;
        }
    }

    /* Parse all options */
    *i_last_cmd__line_option = 0;
    i = 0;
    while (i < nargs && *args[i] == '-')
    {
        /* get argument name (string after the '-' dash) */
        arg_name = args[i] + 1;
        if (*arg_name == '-')
        {
            /* increment pointer, if long argument name has been specified */
            arg_name++;
        }

        /* parse the options */
        if (_stricmp(arg_name, "v") == 0 || _stricmp(arg_name, "verbose") == 0)
        {
            *Tool_Warning_Mask = WARN_ALL;   /* set warning mask for all warnings */
            *Operation |= VERBOSE;
        }
        else if (_stricmp(arg_name, "d") == 0 || _stricmp(arg_name, "desinstrument") == 0)
        {
            *Operation |= DESINSTRUMENT_ONLY;
        }
        else if (_stricmp(arg_name, "m") == 0 || _stricmp(arg_name, "rom") == 0)
        {
            /* get the next argument - must be an existing .c file */
            i++;

            /* get the filename */
            strcpy(PathName, args[i]);

            /* strip off the surrounding quotes "" */
            strip(PathName, '"');

            /* Replace '\' Windows Directory Delimiter by UNIX '/' */
            strcsub(PathName, '\\', '/');

            /* check if file/dir exists */
            if (stat(PathName, &s) == 0 && s.st_mode & S_IFREG)
            {
                if (strlen(PathName) > MAX_PATH)
                {
                    fprintf(stderr, "The filename %s exceeds the maximum size of %d characters!\n\n", PathName, MAX_PATH);
                    return ERR_CMD_LINE;
                }
            }
            else
            {
                fprintf(stderr, "The file %s does not exist!\n\n", PathName);
                return ERR_FILE_FIND_FAIL;
            }

            /* copy pathname to the output variable */
            strcpy(Const_Data_PROM_File, PathName);
        }
        else if (_stricmp(arg_name, "b") == 0 || _stricmp(arg_name, "no-backup") == 0)
        {
            *Operation |= NO_BACKUP;
        }
        else if (_stricmp(arg_name, "i") == 0 || _stricmp(arg_name, "info-only") == 0)
        {
            *Operation |= INSTR_INFO_ONLY;
        }
        else if (_stricmp(arg_name, "c") == 0 || _stricmp(arg_name, "generate-wmc-files") == 0)
        {
            /* get the next argument - must be an existing direectory */
            i++;

            /* get the directory name */
            strcpy(PathName, args[i]);

            /* strip off the surrounding quotes "" */
            strip(PathName, '"');

            /* Replace '\' Windows Directory Delimiter by UNIX '/' */
            strcsub(PathName, '\\', '/');

            /* check if file/dir exists */
            if (stat(PathName, &s) == 0 && s.st_mode & S_IFDIR)
            {
                if (strlen(PathName) > MAX_PATH)
                {
                    fprintf(stderr, "The directory name %s exceeds the maximum size of %d characters", PathName, MAX_PATH);
                    return ERR_CMD_LINE;
                }
            }
            else
            {
                fprintf(stderr, "The directory %s does not exist!", PathName);
                return ERR_FILE_FIND_FAIL;
            }

            /* Copy to output variable */
            strcpy(wmops_output_dir, PathName);

            *Operation |= OUTPUT_WMOPS_FILES;
        }

        /* Move to the next argument */
        i++;
    }

    /* return the position of the first non-option argument */
    *i_last_cmd__line_option = i;

    return NO_ERR;
}


/*-------------------------------------------------------------------*
 * strmatch()
 *
 * Function that matches input string to a given wildcard pattern
 *-------------------------------------------------------------------*/

static int strmatch(const char str[], const char pattern[])
{
    int i, j, n, m;
    int **lookup, result;       /* lookup table for storing results of subproblems */

    n = (int)strlen(str);
    m = (int)strlen(pattern);

    /* empty pattern can only match with empty string */
    if (m <= 0 || n <= 0)
    {
        return 0;
    }

    /* initialize lookup table to false */
    lookup = (int**)calloc(n+1, sizeof(int*));
    for (i = 0; i <= n; i++)
    {
        lookup[i] = (int*)calloc(m+1, sizeof(int));
    }

    /* empty pattern can match with empty string */
    lookup[0][0] = 1;

    /* Only '*' can match with empty string */
    for (j = 1; j <= m; j++)
    {
        if (pattern[j - 1] == '*')
        {
            lookup[0][j] = lookup[0][j - 1];
        }
    }

    /* fill the table in bottom-up fashion */
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= m; j++)
        {
            /* Two cases if we see a '*'
               a) We ignore '*' character and move
                  to next  character in the pattern,
                   i.e., '*' indicates an empty sequence.
               b) '*' character matches with ith
                   character in input */
            if (pattern[j - 1] == '*')
            {
                lookup[i][j] = lookup[i][j - 1] || lookup[i - 1][j];
            }
            /* Current characters are considered as
               matching in two cases
               (a) current character of pattern is '?'
               (b) characters actually match */
            else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1])
            {
                lookup[i][j] = lookup[i - 1][j - 1];
            }
            /* If characters don't match */
            else
            {
                lookup[i][j] = 0;
            }
        }
    }

    result = lookup[n][m];

    /* free allocated memory */
    for (i = 0; i <= n; i++)
    {
        free(lookup[i]);
    }
    free(lookup);

    return result;
}

/*-------------------------------------------------------------------*
 * strip()
 *
 * Function that strips character from the left and right of a given string
 *-------------------------------------------------------------------*/

char *strip(char *str, char c)
{
    int n;

    n = (int)strlen(str);

    /* checkl if string is empty */
    if (n == 0)
    {
        return str;
    }

    /* remove leading character */
    if (str[0] == c)
    {
        str += 1;
        n--;
    }

    /* remove trailing character */
    if (str[n - 1] == c)
    {
        str[n] = '\0';
    }

    return str;
}

/*-------------------------------------------------------------------*
 * add_file_to_list()
 *
 * Function appends new filename (string) to the existing list (string array)
 * - it also updates the total number of files in the list
 *-------------------------------------------------------------------*/

char **add_file_to_list(const char* filename, char** file_list, int* nfiles)
{
    char** ptr_temp;

    /* check if file_list exists */
    if (file_list == NULL)
    {
        /* create new empty list */
        ptr_temp = (char**)malloc(1 * sizeof(char*));
        *nfiles = 0;
    }
    else
    {
        /* increase the size of the file_list array by one */
        ptr_temp = (char**)realloc((void*)file_list, (size_t)(*nfiles + 1) * sizeof(char*));
    }

    if (ptr_temp != NULL)
    {
        file_list = ptr_temp;
    }
    else
    {
        fprintf( stderr, "Critical Error : Unable to create/update file_list!\n");
        exit(-1);
    }

    /* create new entry */
    file_list[*nfiles] = (char*)malloc(strlen(filename) * sizeof(char) + 1);

    /* copy pathanme to the new entry */
    strcpy(file_list[*nfiles], filename);
    (*nfiles)++;

    return file_list;
}

/*-------------------------------------------------------------------*
 * free_list_of_strings()
 *
 * Free memory for an array of strings
 *-------------------------------------------------------------------*/
char **free_list_of_strings(char** file_list, int nfiles)
{
    int i;

    if (file_list == NULL)
    {
        return NULL;
    }

    for (i = 0; i < nfiles; i++)
    {
        if (file_list[i] != NULL)
        {
            free(file_list[i]);
        }
    }

    free(file_list);

    return NULL;
}


/*-------------------------------------------------------------------*
 * add_files_in_dir_to_list()
 *
 * Function that opens specified directory (string), lists all files matching the specified pattern (string)
 * and appends them to the file list (array of strings)
 * - the function also updates the total number of files in the list
 *-------------------------------------------------------------------*/

char **add_files_in_dir_to_list(const char* dir, const char* pattern, char** file_list, int *nfiles)
{
#ifndef _MSC_VER
    DIR* dirstr;
    struct dirent* ent;
    char pathname[MAX_PATH];

    /* open the directory -> create DIR structure */
    dirstr = opendir(dir);

    /* loop over all files within the directory */
    while ((ent = readdir(dirstr)) != NULL)
    {
        /* check if filename matches the pattern */
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0) && strmatch(ent->d_name, pattern))
        {
            /* combine directory name with filename */
            strcpy(pathname, dir);
            if (pathname[strlen(pathname) - 1] != '/')
            {
                strcat(pathname, "/");
            }
            strcat(pathname, ent->d_name);

            /* append to the list */
            file_list = add_file_to_list(ent->d_name, file_list, nfiles);
        }
    }

    closedir(dirstr);
#else
    HANDLE hFind = NULL;
    WIN32_FIND_DATA FileFindData;
    char pathname[MAX_PATH];

    /* concatenate directory and pattern */
    strcpy(pathname, dir);
    strcat(pathname, "/");
    strcat(pathname, pattern);

    /* open the directory -> find first file */
    if ((hFind = FindFirstFile(pathname, &FileFindData)) == INVALID_HANDLE_VALUE)
    {
        /* directory is empty */
        return file_list;
    }

    do
    {
        /* check if filename matches the pattern */
        if (strcmp(FileFindData.cFileName, ".") != 0 && strcmp(FileFindData.cFileName, "..") != 0)
        {
            /* combine directory name with filename */
            strcpy(pathname, dir);
            if (pathname[strlen(pathname)-1] != '/') strcat(pathname, "/");
            strcat(pathname, FileFindData.cFileName);

            file_list = add_file_to_list(FileFindData.cFileName, file_list, nfiles);
        }
    }
    while (FindNextFile(hFind, &FileFindData));

    FindClose(hFind);
#endif

    return file_list;
}



/*-------------------------------------------------------------------*
 * Free_File_Memory
 *-------------------------------------------------------------------*/
static void Free_File_Memory(
    File_def *File_ptr )
{
    if ( File_ptr->MaxSize != 0 )
    {
        free( File_ptr->Data - FILE_MEM_EXTRA );
        /* Mark as Free */
        File_ptr->MaxSize = 0;
    }
}

/*-------------------------------------------------------------------*
 * Allocate_File_Memory
 *   1) Allocates Memory on First Use
 *   2) Increases Memory Allocation if Needed
 *
 * NOTE: This Functions Allocates Extra Memory at
 *       Both Ends of the Data Memory Requested.
 *-------------------------------------------------------------------*/
static TOOL_ERROR Allocate_File_Memory(
    File_def *File_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char *ptr;
    int size;

    char temp[10 + 3 + 1]; /* 10 Digits for 32 Bits Int, 3 Commas, 1 for NUL*/

    /* Need more Room? (File Size is Never 0, Trapped Earlier) */
    if ( File_ptr->MaxSize < File_ptr->Size )
    {
        /* Free Currently Allocated File Memory */
        Free_File_Memory( File_ptr );
        /* Calculate Bigger Size */
        size = File_ptr->Size + FILE_MEM_EXTRA * 2;
        /* Round to Upper Multiple of FILE_MEM_INCREMENT */
        size += FILE_MEM_INCREMENT - 1;
        size /= FILE_MEM_INCREMENT;
        size *= FILE_MEM_INCREMENT;
        /* Allocate Memory */
        if ( ( ptr = (char *) malloc( size ) ) == NULL )
        {
            ErrCode = ERR_MEM_FILE;
            Error( ERROR_MSG_MEM_FILE, ErrCode, itos( temp, size ), File_ptr->Name );
            goto ret;
        }
        /* Set File Data Memory Pointer in File Context */
        File_ptr->Data = ptr + FILE_MEM_EXTRA;
        /* Set File Data Max Size in File Context */
        File_ptr->MaxSize = size - FILE_MEM_EXTRA * 2;
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Read_File
 *-------------------------------------------------------------------*/
static TOOL_ERROR Read_File(
    char *LongFileName,
    File_def *File_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    FILE *file;

    /* Try Opening the File (need the Long File Name) */
    if ( ( file = fopen( LongFileName, "rb" ) ) == NULL )
    {
        ErrCode = ERR_FILE_OPEN;
        Error( "Cannot Open " DQUOTE( "%s" ), ErrCode, File_ptr->Name );
        goto ret2;
    }
    /* Get its size */
    if ( fseek( file, 0, SEEK_END ) != 0 )
    {
        ErrCode = ERR_FILE_SEEK;
        Error( ERROR_MSG_SEEK, ErrCode, File_ptr->Name );
        goto ret;
    }
    /* Set File Size in Context */
    File_ptr->Size = ftell( file );
    /* Size is OK?  */
    if ( File_ptr->Size == 0 )
    { /* No */
        ErrCode = ERR_FILE_EMPTY;
        Error( ERROR_MSG_FILE_EMPTY, ErrCode, File_ptr->Name );
        goto ret;
    }
    /* Allocate File Memory */
    if ( ( ErrCode = Allocate_File_Memory( File_ptr ) ) != NO_ERR )
    {
        goto ret;
    }
    /* Rewind File */
    if ( fseek( file, 0, SEEK_SET ) != 0 )
    {
        ErrCode = ERR_FILE_SEEK;
        Error( ERROR_MSG_SEEK, ErrCode, File_ptr->Name );
        goto ret;
    }
    /* Read File */
    if ( (int)(fread( File_ptr->Data, sizeof( char ), File_ptr->Size, file )) != File_ptr->Size )
    {
        ErrCode = ERR_FILE_READ;
        Error( "Cannot Read from " DQUOTE( "%s" ), ErrCode, File_ptr->Name );
        goto ret;
    }
    /* Put File Termination */
    File_ptr->Data[File_ptr->Size] = EOF_CHAR;

ret:
    /* Close File */
    fclose( file );
ret2:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Remove_Tabs
 *-------------------------------------------------------------------*/
static TOOL_ERROR Remove_Tabs(
    File_def *File_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    File_def file;

    char *ptr, *src;
    int size;

    int nTabs;

    /* Set File Pointer */
    ptr = File_ptr->Data;
    /* Reset # of Tabs Counted */
    nTabs = 0;
    /* Count Them */
    while ( ( ptr = (char *) memchr( ptr, TAB_CHAR, File_ptr->Size - ( ptr - File_ptr->Data ) ) ) != NULL )
    {
        do
        {
            nTabs++;
        } while ( *++ptr == TAB_CHAR );
    }

    /* Any Found? */
    if ( nTabs > 0 )
    { /* Yes */
        /* Copy the Context */
        memmove( &file, File_ptr, sizeof( file ) );

        /* Calculate Bigger Size */
        size = File_ptr->Size + nTabs * ( TAB_SIZE - 1 );
        /* Set Increased File Size in Context */
        File_ptr->Size = size;

        /* Enough Room to Insert the Equivalent Spaces? */
        if ( File_ptr->MaxSize < size )
        { /* No */
            /* Mark as Free */
            File_ptr->MaxSize = 0;
            /* Allocate File Memory */
            if ( ( ErrCode = Allocate_File_Memory( File_ptr ) ) != NO_ERR )
            {
                goto ret;
            }
        }
        else
        { /* Yes */
            /* Mark as Free (So it won't be De-Allocated Later) */
            file.MaxSize = 0;
        }

        /* Set File Pointers */
        src = file.Data + file.Size;
        ptr = File_ptr->Data + size;

        /* Substitute Tabs for Spaces (Start from End) */
        do
        {
            if ( ( *--ptr = *--src ) == TAB_CHAR )
            {
                *ptr = SPACE_CHAR;
                nTabs = TAB_SIZE - 1;
                do
                {
                    *--ptr = SPACE_CHAR;
                } while ( --nTabs );
                /* Pointer Match? */
                if ( ptr == src )
                { /* Yes */
                    /* Stop Copy */
                    break;
                }
            }
        } while ( ptr != File_ptr->Data );

        /* Free Currently Allocated File Memory */
        Free_File_Memory( &file );
    }

ret:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Write_Open_Error
 *-------------------------------------------------------------------*/
static TOOL_ERROR Write_Open_Error( const char *filename, const char *suffix )
{
    TOOL_ERROR ErrCode = ERR_FILE_OPEN_WRITING;

    Error( "Cannot Open %s%s for Writing", ErrCode, filename, suffix );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Write_Error
 *-------------------------------------------------------------------*/
static TOOL_ERROR Write_Error( const char *filename,
                               const char *suffix = "" )
{
    TOOL_ERROR ErrCode = ERR_FILE_WRITE;

    Error( "Cannot Write to " DQUOTE( "%s%s" ), ErrCode, filename, suffix );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Backup_File
 *-------------------------------------------------------------------*/
static TOOL_ERROR Backup_File(
    char *LongFileName,
    File_def *File_ptr)
{
    TOOL_ERROR ErrCode = NO_ERR;

    FILE *file;
    size_t size;

    char LongBackupFileName[MAX_PATH + STRING_LENGTH( BACKUP_SUFFIX ) + 1]; /* +1 for NUL Char*/

    /* Create Backup FileNames */
    size = strlen( strcat( strcpy( LongBackupFileName, LongFileName ), BACKUP_SUFFIX ) );

    /* Open File for Writing */
    if ( ( file = fopen( LongBackupFileName, "wb" ) ) == NULL )
    {
        ErrCode = Write_Open_Error( File_ptr->Name, BACKUP_SUFFIX );
        goto ret2;
    }

    /* Write (size to write cannot be 0) */
    size = File_ptr->Size;
    if ( fwrite( File_ptr->Data, 1, size, file ) != size )
    {
        ErrCode = Write_Error( File_ptr->Name, BACKUP_SUFFIX );
        goto ret;
    }

ret:
    /* Close File */
    fclose( file );
ret2:
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Write_File
 *-------------------------------------------------------------------*/
static TOOL_ERROR Write_File(
    const char *LongFileName,
    Parse_Context_def *ParseCtx_ptr )
{
    TOOL_ERROR ErrCode = NO_ERR;

    FILE *file;

    size_t size;
    char *ptr, *ptr2;
    char *end;
    char *ptr3;
    char *ptr4;
    int n;

    File_def *file_ptr;
    Insert_Rec_def *InsertRec_ptr;

    /* Get File Address (for clarity) */
    file_ptr = &ParseCtx_ptr->File;

    /* Open File for Writing */
    if ( ( file = fopen( LongFileName, "wb" ) ) == NULL )
    {
        ErrCode = Write_Open_Error( file_ptr->Name, "");
        return ErrCode;
    }

    /* Finalize */
    if ( ( ErrCode = Finalize( ParseCtx_ptr ) ) != NO_ERR )
    {
        goto ret;
    }

    /* Set Range */
    ptr = file_ptr->Data;
    end = ptr + file_ptr->Size;

    /* Get Insert Entries Address (for clarity) */
    InsertRec_ptr = ParseCtx_ptr->InsertTbl.Data;
    /* Get 1st Insertion Location */
    ptr3 = InsertRec_ptr->Ptr;
    /* Calc # of Insertions */
    n = ParseCtx_ptr->InsertTbl.Size;

    /* Do Whole File (Ignore Insertions Past End of File) */
    while ( ptr < end )
    {
        /* Set Minimum (if Next Insertion inside run of Deleted Text) */
        if ( ptr > ptr3 )
            ptr = ptr3;

        /******************************************
        * In this Loop:
        * ptr : Next Data to Write Position
        * ptr2: Last Deletion Character Position
        * ptr3: Next Insertion Position
        ******************************************/

        /* Find Deleted Text */
        if ( ( ptr2 = strchr( ptr, DEL_CHAR ) ) == NULL )
            ptr2 = end;

        do
        {
            /* Get Deleted Text Location */
            ptr4 = ptr2;

            /* Set Minimum */
            if ( ptr4 > ptr3 )
                ptr4 = ptr3;

            /* Write (size to write can be 0) */
            size = ptr4 - ptr;
            if ( fwrite( ptr, 1, size, file ) != size )
            {
                ErrCode = Write_Error( file_ptr->Name );
                goto ret;
            }

            /* Insertion? */
            while ( ptr4 == ptr3 )
            { /* Yes */
                /* Insert */
                if ( fputs( InsertRec_ptr++->String, file ) < 0 )
                {
                    ErrCode = Write_Error( file_ptr->Name );
                    goto ret;
                }
                /* Get Next Insertion Location */
                ptr3 = InsertRec_ptr->Ptr;
                /* Stop when all Insertions have been Done */
                if ( --n == 0 )
                    break;
            }
            ptr = ptr4;
            /* Continue Until Deletion has been Reached */
        } while ( ptr4 != ptr2 );

        /* Skip Deleted Text */
        while ( *ptr == DEL_CHAR )
            ptr++;
    }

ret:
    /* Close File */
    fclose( file );

    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Process_File
 *-------------------------------------------------------------------*/
static TOOL_ERROR Process_File(
    char *LongFileName,
    int Operation,
    Parse_Context_def *ParseCtx_ptr,
    size_t MaxFnLength,
    bool Backup,
    float Progress )
{
    TOOL_ERROR ErrCode = NO_ERR;

    char temp[10 + 3 + 1]; /* 10 Digits for 32 Bits Int, 3 Commas, 1 for NUL*/
    char temp2[10 + 3 + 1 + STRING_LENGTH( ", %7s PROM Bytes" )];
    int i = 0;

    File_def *file_ptr;

    /* Get File Address (for clarity) */
    file_ptr = &ParseCtx_ptr->File;

    /* Print Stats */
    fprintf(stdout, "%*s", (int) MaxFnLength, file_ptr->Name );

    /* Set State Failure (by default) */
    ParseCtx_ptr->State = FILE_FAILED;

    /* Read File into Memory */
    if ( ( ErrCode = Read_File( LongFileName, file_ptr ) ) != NO_ERR )
    {
        return ErrCode;
    }

    /* Print Stats */
    fprintf(stdout, ": %10s Bytes", itos( temp, file_ptr->Size ) );

    /* Print Progress (Only to Error Console) */
    i = fprintf( stdout, ",%6.1f%% Completed", Progress );

    /* Rewind */
    do
    {
        fputc(BS_CHAR, stdout);
    } while ( --i );

    fflush(stdout);

    /* Backup File? */
    if ( Backup )
    { /* Yes */
        if ( ( ErrCode = Backup_File( LongFileName, file_ptr ) ) != NO_ERR )
        {
            fprintf(stdout, "\n");
            return ErrCode;
        }
    }

    /* Remove Tabs */
    if ( ( ErrCode = Remove_Tabs( file_ptr ) ) != NO_ERR )
    {
        fprintf(stdout, "\n");
        return ErrCode;
    }

    /* Set State Success */
    ParseCtx_ptr->State = FILE_OK;

    /* Clear Program Memory Size String */
    temp2[0] = NUL_CHAR;

    /* Setup */
    if ( ( ErrCode = Setup_Regions( ParseCtx_ptr, (Operation & VERBOSE) != 0) ) != NO_ERR )
    {
        fprintf(stdout, "\n");
        return ErrCode;
    }

    /* DesInstrument Counters */
    if ( ( ErrCode = DesInstrument( ParseCtx_ptr, ( Operation & REMOVE_MANUAL_INSTR ) != 0 ) ) != NO_ERR )
    {
        fprintf(stdout, "\n");
        return ErrCode;
    }

    /* Instrument */
    if ( !( Operation & DESINSTRUMENT_ONLY ) )
    { /* Yes */
        /* Instrument */
        if ((ErrCode = Instrument(ParseCtx_ptr, (Operation & INSTRUMENT_ROM) != 0)) != NO_ERR)
        {
            fprintf(stdout, "\n");
            return ErrCode;
        }

        /* Output Program Memory Size? */
        if ( Operation & INSTRUMENT_ROM)
        { /* Yes */
            sprintf( temp2, ", %7s PROM Words", itos( temp, ParseCtx_ptr->PROMSize ) );
        }
    }

    /* Print Parsing Stats */
    fprintf(stdout, ", %6s Entries%s", itos( temp, ParseCtx_ptr->ParseTbl.Size ), temp2 );

    /* PROM Size Printed? */
    if ( temp2[0] == NUL_CHAR )
    { /* No */
        /* Print two Spaces to Erase the last 2 Letters of "% Completed" */
        fputs( "  \b\b", stdout );
    }

    fprintf(stdout, "\n" );
    return ErrCode;
}

/*-------------------------------------------------------------------*
 * Output_Wmops_File
 *-------------------------------------------------------------------*/

static TOOL_ERROR Output_Wmops_File( char *PathName, bool Backup )
{
    TOOL_ERROR ErrCode = NO_ERR;
    FILE *file;
    int i;
    size_t file_size;
    char LongFileName[MAX_PATH + 1]; /* +1 for NUL Char*/

    static const char wmops_auto_file_h[] =
#include "wmc_auto_h.txt"
        ;
    static const char wmops_auto_file_c[] =
#include "wmc_auto_c.txt"
        ;
    for (i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            /* Create Output Filename */
            strcpy(LongFileName, PathName);
            strcat(LongFileName, "/wmc_auto.h");

            /* Try opening the target file wmc_auto.h */
            if ((file = fopen(LongFileName, "wb")) == NULL)
            {
                ErrCode = ERR_FILE_OPEN;
                Error("Cannot Open " DQUOTE("%s"), ErrCode, "wmc_auto.h");
                goto ret;
            }

            /* Write it */
            file_size = sizeof(wmops_auto_file_h) - 1;
            if (fwrite(wmops_auto_file_h, 1, file_size, file) != file_size)
            {
                ErrCode = Write_Error(LongFileName);
                goto ret;
            }

            /* Close the File */
            fclose(file);
        }
        else
        {
            /* Create Output Filename */
            strcpy(LongFileName, PathName);
            strcat(LongFileName, "/wmc_auto.c");

            /* Try opening the target file wmc_auto.c */
            if ((file = fopen(LongFileName, "wb")) == NULL)
            {
                ErrCode = ERR_FILE_OPEN;
                Error("Cannot Open " DQUOTE("%s"), ErrCode, "wmc_auto.c");
                goto ret;
            }

            /* Write it */
            file_size = sizeof(wmops_auto_file_c) - 1;
            if (fwrite(wmops_auto_file_c, 1, file_size, file) != file_size)
            {
                ErrCode = Write_Error(LongFileName);
                goto ret;
            }

            /* Close the File */
            fclose(file);
        }
    }

ret:
    return ErrCode;
}


/*-------------------------------------------------------------------*
 * Main program of the 'Weighted MOPS Counter Tool'.
 *-------------------------------------------------------------------*/

int main( int argc, char *argv[] )
{
    int i, j, i_cmd_line, size, nRecords = 0;
    int nBytesProcessed, FolderPROMSize, len, MaxFnLength;
    int Operation;
    unsigned int Tool_Warning_Mask;
    char* ptr;
    char temp[MAX_CHARS_PER_LINE];
    char LongFileName[MAX_PATH];
    char Const_Data_PROM_File[MAX_PATH] = "";
    char wmops_output_dir[MAX_PATH];
    T_FILE_BOOK file_book[MAX_RECORDS];
    struct stat s;
    Parse_Context_def ParseContext;
    File_def *file_ptr;
    TOOL_ERROR ErrCode = NO_ERR;

    /* Initialization */
    ParseContext.File.MaxSize = 0;
    ParseContext.ParseTbl.MaxSize = 0;
    ParseContext.InsertTbl.MaxSize = 0;
    ParseContext.FctCallTbl.MaxSize = 0;
    ParseContext.PointerTbl.MaxSize = 0;
    ParseContext.PROMOpsWeightsSet = 1;     /* Set PROM Ops Weighting Set in Context (0 = Std, 1 = STL 2009) */

    /* Skip the Executable's Name */
    argc--;
    argv++;

    /* Set Default Modes */
    Operation = REMOVE_MANUAL_INSTR | INSTRUMENT_ROM;
    Tool_Warning_Mask = NO_WARNING;

    /* Parse Command-Line Options  */
    if ( ( ErrCode = Parse_Command_Line( argc, argv, &i_cmd_line, &Operation,
                                         &Tool_Warning_Mask,
                                         Const_Data_PROM_File,
                                         wmops_output_dir) ) != NO_ERR )
    {
        if (ErrCode == ERR_HELP)
        {
            return NO_ERR;
        }
        else
        {
            return ErrCode;
        }
    }

    if (Operation & VERBOSE)
    {
        Print("\n"
            "WMC Tool (WMOPS Automatic Instrumentation Tool) v%s - %s\n"
            "\n"
            "(C) 2022 copyright VoiceAge Corporation. All Rights Reserved.\n"
            "\n"
            "This software is protected by copyright law and by international treaties. The source code, and all of its derivations,\n"
            "is provided by VoiceAge Corporation under the \"ITU-T Software Tools' General Public License\". Please, read the license file\n"
            "or refer to ITU-T Recommendation G.191 on \"SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS\".\n"
            "\n"
            "Any use of this software is permitted provided that this notice is not removed and that neither the authors nor\n"
            "VoiceAge Corporation are deemed to have made any representations as to the suitability of this software\n"
            "for any purpose nor are held responsible for any defects of this software. THERE IS NO WARRANTY FOR THIS SOFTWARE.\n"
            "\n"
            "Authors: Guy Richard, Vladimir Malenovsky (Vladimir.Malenovsky@USherbrooke.ca)\n\n",
            WMC_TOOL_VERSION_NO, VERSION_STL);
    }

    /* Print Information Only? */
    if ( Operation & INSTR_INFO_ONLY )
    {
        fprintf(stdout, "Printing Instrumentation Information Only\n" );
        Print_Information();
        return 0;
    }

    if (Operation & VERBOSE)
    {
        /* Print Operating Modes */
        fprintf(stdout, "The following operations will be performed by the WMC tool:\n");

        if (!(Operation & NO_BACKUP))
        {
            fprintf(stdout, "- backing up original files\n");
        }

        fprintf(stdout, "- desinstrumenting all files\n");

        if (!(Operation & REMOVE_MANUAL_INSTR))
        {
            fprintf(stdout, "- keeping manual instrumentation\n");
        }

        if (!(Operation & DESINSTRUMENT_ONLY))
        {
            fprintf(stdout, "- instrumenting all functions and table (const) data memory\n");
        }

        if (Operation & OUTPUT_WMOPS_FILES)
        {
            fprintf(stdout, "- exporting wmc_auto.h and wmc_auto.c to %s\n", wmops_output_dir);
        }

        if (strcmp( Const_Data_PROM_File, "") != 0)
        {
            fprintf(stdout, "- adding statistics about ROM and RAM consumption in the print_mem() function in %s\n", Const_Data_PROM_File);
        }

        fprintf(stdout, "\n");
    }

    /* Create file list from user-specified command-line arguments */
    i = 0;
    MaxFnLength = STRING_LENGTH("Peak Memory Used");        /* Set Minimum Filename Length */
    for (; i_cmd_line < argc; i_cmd_line++)
    {
        /* If this is not a Command Line Option (it must be Filename or Directory) */
        if (*argv[i_cmd_line] != NUL_CHAR)
        {
            if (i == MAX_RECORDS)
            {
                ErrCode = ERR_NO_FILE_SPEC;
                fprintf(stderr, "Maximum number of file specifications on the command line exceeded! The limit is %d entries!\n", MAX_RECORDS);
                goto ret;
            }

            /* allocate space for a new record and initialize it */
            size = (int)strlen(argv[i_cmd_line]) * sizeof(char) + 1;
            file_book[i].cmd_line_spec = (char*)malloc(size);
            file_book[i].pathname = (char*)malloc(size);
            file_book[i].filename_or_mask = (char*)malloc(size);
            file_book[i].file_list = (char**)malloc(0 * sizeof(char*));
            file_book[i].nFiles = 0;

            /* Copy Command Line Argument (In Case we Need to Append a Default Extension) */
            strcpy(file_book[i].cmd_line_spec, argv[i_cmd_line]);

            /* strip off surrounding quotes "" */
            strip(file_book[i].cmd_line_spec, '"');

            /* Replace '\' Windows Directory Delimiter by UNIX '/' */
            strcsub(file_book[i].cmd_line_spec, '\\', '/');

            if (stat(file_book[i].cmd_line_spec, &s) == 0)
            {
                if (s.st_mode & S_IFREG)
                {
                    /* it's an existing single file */
                    /* locate the pathname */
                    ptr = strrchr(file_book[i].cmd_line_spec, '/');
                    if (ptr != NULL)
                    {
                        size = (int)(ptr - file_book[i].cmd_line_spec);
                        strncpy(file_book[i].pathname, file_book[i].cmd_line_spec, size);
                        file_book[i].pathname[size] = '\0';
                        strcpy(file_book[i].filename_or_mask, ptr+1);
                    }
                    else
                    {
                        /* no pathname -> use './' as the default pathname */
                        strcpy(file_book[i].pathname, ".");
                        strcpy(file_book[i].filename_or_mask, file_book[i].cmd_line_spec);
                    }
                    file_book[i].file_list = add_file_to_list(file_book[i].filename_or_mask, file_book[i].file_list, &file_book[i].nFiles);
                }
                else if (s.st_mode & S_IFDIR)
                {
                    /* it's an existing directory without file mask -> use *.c as default */
                    strcpy(file_book[i].pathname, file_book[i].cmd_line_spec);
                    /* remove the trailing '/', if any */
                    size = (int)strlen(file_book[i].pathname);
                    while (file_book[i].pathname[size - 1] == '/')
                    {
                        file_book[i].pathname[size - 1] = '\0';
                        size--;
                    }
                    strcpy(file_book[i].filename_or_mask, "*.c");
                    file_book[i].file_list = add_files_in_dir_to_list(file_book[i].pathname, file_book[i].filename_or_mask, file_book[i].file_list, &file_book[i].nFiles);
                }
                else
                {
                    ErrCode = ERR_FILE_FIND;
                    fprintf(stderr, "Unable to open %s!\n", argv[i]);
                    goto ret;
                }
            }
            else
            {
                /* it must be directory with file mask or file mask only */

                /* strip off the file mask */
                ptr = strrchr(file_book[i].cmd_line_spec, '/');
                if (ptr == NULL)
                {
                    /* no directory specified on the command line -> use "./" as default */
                    strcpy(file_book[i].pathname, ".");
                    strcpy(file_book[i].filename_or_mask, file_book[i].cmd_line_spec);
                }
                else
                {
                    /* directory with file mask */
                    size = (int)(ptr - file_book[i].cmd_line_spec);
                    strncpy(file_book[i].pathname, file_book[i].cmd_line_spec, size);
                    file_book[i].pathname[size] = '\0';
                    strcpy(file_book[i].filename_or_mask, ptr + 1);
                }

                /* try opening the directory and reading the filenames matching mask */
                if (stat(file_book[i].pathname, &s) == 0 && (s.st_mode & S_IFDIR))
                {
                    file_book[i].file_list = add_files_in_dir_to_list(file_book[i].pathname, file_book[i].filename_or_mask, file_book[i].file_list, &file_book[i].nFiles);
                }
                else
                {
                    ErrCode = ERR_FILE_FIND;
                    fprintf(stderr, "Unable to open directory %s!\n", file_book[i].pathname);
                    goto ret;
                }
            }

            /* at least one file or directory must be specififed */
            if (file_book[i].nFiles == 0)
            { /* No */
                ErrCode = ERR_NO_FILE_SPEC;
                fprintf(stderr, "No existing File/Dir was specified!\n");
                goto ret;
            }

            /* update the maximum filename length - to align all printout messages */
            for (j = 0; j < file_book[i].nFiles; j++)
            {
                len = (int)strlen(file_book[i].file_list[j]);
                if (len > MaxFnLength)
                {
                    MaxFnLength = len;
                }
            }

            i++;
        }
    }

    /* Save the total number of records */
    nRecords = i;

    fprintf(stdout, "\n");

    /* Process all Directories/Files Entered on the Command Line */
    for (i = 0; i < nRecords; i++)
    {
        /* Print Directory Name */
        fprintf(stdout, "Dir: %s\n", file_book[i].pathname);

        /* Reset Folder Program Memory Size */
        FolderPROMSize = 0;
        /* Reset # of Bytes Processed */
        nBytesProcessed = 0;

        /* Initialize Context (No Memory Allocated by Default) */
        ParseContext.File.MaxSize = 0;
        ParseContext.ParseTbl.MaxSize = 0;
        ParseContext.InsertTbl.MaxSize = 0;
        ParseContext.FctCallTbl.MaxSize = 0;
        ParseContext.PointerTbl.MaxSize = 0;

        for (j = 0; j < file_book[i].nFiles; j++)
        {
            ParseContext.File.Name = file_book[i].file_list[j];

            /* Get the LongFileName (directory + filename) */
            strcpy(LongFileName, file_book[i].pathname);
            strcat(LongFileName, "/");
            strcat(LongFileName, file_book[i].file_list[j]);

            /* Process File */
            ErrCode = Process_File(LongFileName, Operation, &ParseContext, MaxFnLength, (Operation & NO_BACKUP) == 0, j * 100.0f / file_book[i].nFiles);

            /* Update # of Bytes Processed */
            nBytesProcessed += ParseContext.File.Size;

            /* Update Folder Program Memory Size */
            FolderPROMSize += ParseContext.PROMSize;

            /* Check Previous State */
            if (ParseContext.State != FILE_FAILED)
            {
                if (Operation & VERBOSE)
                {
                    /* Print Warnings, if there are any */
                    if (Print_Warnings(&ParseContext, Tool_Warning_Mask, 0, (Operation & DESINSTRUMENT_ONLY) != 0) != 0 && ErrCode == NO_ERR)
                    {
                        strcpy(temp, file_book[i].file_list[j]);
                        strcat(temp, ": Warnings Encountered. ");
                    }
                }
            }

            /* Stop on Error */
            if (ErrCode != NO_ERR)
            {
                /* The error message should be prepared in the string error_msg -> print it */
                Print_Error();
                goto ret;
            }

            /* Write Output file */
            if ((ErrCode = Write_File(LongFileName, &ParseContext)) != NO_ERR)
            {
                goto ret;
            }
        }

        /* Print final statistics for each processed record */
        fprintf(stdout, "%*i File%s:%11s Bytes\n", MaxFnLength - (file_book[i].nFiles > 1 ? 6 : 5), file_book[i].nFiles, file_book[i].nFiles > 1 ? "s" : "", itos(temp, nBytesProcessed));
#ifdef DEBUG_PRINT
        fprintf(stdout, "%*s: %10s Bytes\n", MaxFnLength, "Peak Memory Used", itos(temp, ParseContext.File.MaxSize + ParseContext.ParseTbl.MaxSize * PARSE_REC_SIZE +
                   ParseContext.InsertTbl.MaxSize * INSERT_REC_SIZE + ParseContext.FctCallTbl.MaxSize * FCTCALL_REC_SIZE));
#endif
    }

    /* Output wmc_auto.[h|c] file? */
    if (Operation & OUTPUT_WMOPS_FILES)
    { /* Yes */

        if ((ErrCode = Output_Wmops_File(wmops_output_dir, (Operation & NO_BACKUP) == 0)) != NO_ERR)
        {
            goto ret;
        }
    }

    /* Instrument Const_Data_PROM_Table[], if requested on the command-line */
    if ((Operation & INSTRUMENT_ROM) && (strlen(Const_Data_PROM_File) > 0))
    {
        fprintf(stdout, "\n");

        /* Initialize Parsing Structure */
        file_ptr = &(ParseContext.File);     /* Get File Pointer */
        file_ptr->MaxSize = 0;
        file_ptr->Name = Const_Data_PROM_File;
        ParseContext.InsertTbl.Size = 0;     /* Erase Insert Table */
        ParseContext.InsertTbl.MaxSize = 0;

        ptr = strrchr(Const_Data_PROM_File, '/');
        fprintf(stdout, "Instrumenting Const_Data_PROM_Table[] in %s\n", ptr == NULL ? Const_Data_PROM_File : ptr + 1);

        /* Read File into Memory */
        if ((ErrCode = Read_File(Const_Data_PROM_File, file_ptr)) != NO_ERR)
        {
            goto ret;
        }

        ParseContext.State = FILE_OK;

        /* Backup File? */
        if ( (Operation & NO_BACKUP) == 0 )
        { /* Yes */
            if ((ErrCode = Backup_File(Const_Data_PROM_File, file_ptr)) != NO_ERR)
            {
                fprintf(stdout, "\n");
                goto ret;
            }
        }

        /* Setup Regions*/
        if ((ErrCode = Setup_Regions(&ParseContext, (Operation & VERBOSE) != 0)) != NO_ERR)
        {
            goto ret;
        }

        /* DesInstrument Const_Data_Size_XXX Functions */
        if ((ErrCode = DesInstrument_ROM(&ParseContext)) != NO_ERR)
        {
            goto ret;
        }

        /* Check, if "wmc_auto.h" is included */
        if ((ErrCode = Include_Header(&ParseContext, NULL)) != NO_ERR)
        {
            goto ret;
        }

        /* Instrument Const_Data_PROM_Table[] */
        if ((ErrCode = Instrument_Const_Data_PROM_Table(&ParseContext, file_book, nRecords)) != NO_ERR)
        {
            goto ret;
        }

        /* Write Output file */
        if ((ErrCode = Write_File(Const_Data_PROM_File, &ParseContext)) != NO_ERR)
        {
            goto ret;
        }

        fprintf(stdout, "\n");
    }

ret:

    /* free resources */
    for (i = 0; i < nRecords; i++)
    {
        if (file_book[i].cmd_line_spec != NULL) free(file_book[i].cmd_line_spec);
        if (file_book[i].pathname != NULL) free(file_book[i].pathname);
        if (file_book[i].filename_or_mask != NULL) free(file_book[i].filename_or_mask);
        for (j = 0; j < file_book[i].nFiles; j++)
        {
            if (file_book[i].file_list[j] != NULL) free(file_book[i].file_list[j]);
        }

        free(file_book[i].file_list);
    }

    Free_File_Memory( &ParseContext.File );
    Free_Parse_Table_Memory( &ParseContext.ParseTbl );
    Free_Insert_Table_Memory( &ParseContext.InsertTbl );
    Free_FctCall_Table_Memory( &ParseContext.FctCallTbl );
    Free_Pointer_Table_Memory( &ParseContext.PointerTbl );

    if ( ErrCode == NO_ERR )
    {
        fprintf(stdout, "100%% Completed Successfully!\n" );
    }

    return ErrCode;
}

