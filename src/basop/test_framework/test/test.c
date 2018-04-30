#include <stdio.h>

#ifndef GCC_BUILD
	#include <conio.h>
#endif // GCC_BUILD

//Prototype
int test_precision();
int create_default_tests_prec();

int  main()
{
    //
    int RetVal = 0;

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

#ifndef GCC_BUILD
    getchar();
#endif // #ifndef GCC_BUILD

 	return 0 ;
}
/* End of file */


