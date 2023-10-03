/********************************************************************
 *
 * File :        discard.c
 * Description : Discard enhancement bits.
 * Created :     24/Jul/96 12:54
 * Written by :  Maxim A. Lagerev
 *
 *******************************************************************/

#include <stdlib.h>
#include <stdio.h>
/*AddedByWMC_Tool*/#include "wmc_auto.h"
/*AddedByWMC_Tool*/#ifdef WMOPS
/*AddedByWMC_Tool*/static int Get_Const_Data_Size_out_test_c(void)
/*AddedByWMC_Tool*/{
/*AddedByWMC_Tool*/    int total_size = 0, sz;
/*AddedByWMC_Tool*/    Get_Const_Data_Size(test_file1, &sz); total_size += sz;
/*AddedByWMC_Tool*/    Get_Const_Data_Size(test_rom, &sz); total_size += sz;
/*AddedByWMC_Tool*/    return total_size;
/*AddedByWMC_Tool*/}
/*AddedByWMC_Tool*/ROM_Size_Lookup_Table Const_Data_PROM_Table[] =
/*AddedByWMC_Tool*/{
/*AddedByWMC_Tool*/    { "out/test*.c", 119, Get_Const_Data_Size_out_test_c },
/*AddedByWMC_Tool*/    { "", -1, NULL }
/*AddedByWMC_Tool*/};
/*AddedByWMC_Tool*/#endif


FILE *in, *out;

int getWord (int wordLen);
void putWord (unsigned char word, int wordLen);


static void error (const char *errMsg) {
  fputs (errMsg, stderr);
  exit (EXIT_FAILURE);
}


int main (int argc, char **argv) {
  int n;
  int ne;
  int word;

  if (argc < 5)
    error ("Usage: discard N Ne <infile> <outfile>\n");

#ifdef WMOPS
    reset_wmops();
    reset_mem(0, USE_DEFAULT );
#endif

  n = atoi (argv[1]);
  ne = atoi (argv[2]);

  if (!(2 <= n && n <= 8 && 1 <= ne && ne < n))
    error ("Incorrect parameters\n");

  if ((in = fopen (argv[3], "rb")) == NULL)
    error ("Can't open input file\n");

  if ((out = fopen (argv[4], "wb")) == NULL)
    error ("Can't open output file\n");

  while ((word = getWord (n)) != EOF)
    putWord ((unsigned char) (word >> ne), n - ne);

  fclose (in);
  fclose (out);

#ifdef WMOPS
  print_mem( Const_Data_PROM_Table );
#endif

  return EXIT_SUCCESS;
}


int getWord (int wordLen) {
  int fieldLen = 8;
  static int firstFl = 1;
  static unsigned char currFieldi = 0;
  static int currPosi = 0;
  int oldPos = currPosi;
  unsigned char retw;
  int ch;

  if (firstFl) {
    int ch = fgetc (in);

    if (ch == EOF)
      return EOF;

    currFieldi = ch;
    firstFl = 0;
  }

  currPosi += wordLen;

  if (currPosi < fieldLen)
    return (unsigned char) ((currFieldi >> oldPos) & ~(0xffff << wordLen));

  retw = currFieldi >> oldPos;
  currPosi -= fieldLen;
  ch = fgetc (in);

  if (ch == EOF) {
    firstFl = 1;
    return (currPosi == 0) ? retw : EOF;
  }

  currFieldi = ch;

  if (currPosi == 0)
    return retw;

  return (unsigned char) (((currFieldi << (fieldLen - oldPos)) | retw) & ~(0xffff << wordLen));
}


void putWord (unsigned char word, int wordLen) {
  int fieldLen = 8;
  static unsigned char currFieldo = 0;
  static int currPoso = 0;
  int oldPos = currPoso;

  if (wordLen == 0) {
    if (currPoso != 0)
      fputc (currFieldo, out);

    fflush (out);
    return;
  }

  currPoso += wordLen;
  currFieldo |= word << oldPos;

  if (currPoso < fieldLen)
    return;

  currPoso -= fieldLen;
  fputc (currFieldo, out);
  currFieldo = 0;

  if (currPoso == 0)
    return;

  currFieldo = word >> (fieldLen - oldPos);
}
