#include <stdio.h>


//Prototype
int test_precision();
int create_default_tests_prec();
int test_sanity();

int  main()
{
    //
    int RetVal = 0;

#ifdef CHECK_SANITY
    printf("Starting Sanity Tests...\n");
    test_sanity();
    printf("Sanity tests Completed\n");

#else // CHECK_SANITY
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
#endif // CHECK_SANITY

 	return 0 ;
}
/* End of file */


