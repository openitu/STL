#include <stdio.h>


//Prototype
int test_precision();
int create_default_tests_prec();
int test_sanity();

int  main( int argc, char* argv[] )
{
    //
    int RetVal = 0;

if(strcmp(argv[1],"Test_type=0") == 0)
{
    printf("Starting Sanity Tests...\n");
    RetVal = test_sanity();
    printf("Sanity tests Completed\n");
}
else if(strcmp(argv[1],"Test_type=1") == 0) /* Test_type=1 */
{
    printf("Starting Precision Analysis...\n");

    printf("Creating default test cases for precision testing...\n");
    create_default_tests_prec();
    printf("Completed\n");

    printf("Executing various precision tests...\n");
    RetVal = test_precision();

    if (!RetVal)
    {
        printf(" Succesfully completed :: Test result = %d \n", RetVal);
    }
    else
    {
        printf(" Tests Failure :: Test result = %d \n", RetVal);
    }

    printf("Tests complete \n");
}  /* regression test */
else
	{
   	printf("No proper runtime argument provided. Please specify Test_type=0 or Test_type=1\n");
	RetVal = -1;
	}
    return RetVal;
}
/* End of file */


