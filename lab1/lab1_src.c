/* 
* lab1_src.c -- Code to print out the number of bytes and bits in each of the 
* basic data types in C.  We opted to include the number of bits per data-type as well 
* because in C, a byte is defined as the number of bits in the char data-type. 
* so it is possible for there to be systems that define a non 8-bit byte. 
* Also printed is the values in several of the limit constants and two of the fast types
* defined in inttypes.h
* Written by Joseph Albert and Alyssa Wardwell for ECE477 Hardware Applications of C
*/

#include <stdio.h> 
#include <limits.h>
#include <inttypes.h>
/*
* Create a macro to take a variable type as an argument to a function
* We learned how to do this from this stack exchange thread from comment 
* by user FloatingRock (unrelated application): 
* https://stackoverflow.com/questions/6658593/passing-variable-type-as-function-parameter
*/

#define print_size(type) ({ \
    printf("%s is stored in %lu bytes, which is %lu bits\n\r", #type, \
    sizeof(type), CHAR_BIT * sizeof(type));})

int main()
{
    printf("Data type sizes in bits and bytes:\n\r");
    print_size(char);
    print_size(short);
    print_size(int_fast16_t);
    print_size(int);
    print_size(int_fast32_t);
    print_size(long);
    print_size(float);
    print_size(double);
    

    printf("\n\rDatatype limit constants:\n\r");
    printf("The value of INT_MAX is %d in decimal and %X in hexadecimal\n\r", 
        INT_MAX, INT_MAX);
    printf("The value of UINT_MAX is %u in decimal and %X in hexadecimal\n\r",
        UINT_MAX, UINT_MAX);
    printf("The value of LONG_MAX is %ld in decimal and %lX in hexadecimal\n\r", 
        LONG_MAX, LONG_MAX);
    printf("The value of ULONG_MAX is %lu in decimal and %lX in hexadecimal\n\r", 
        ULONG_MAX, ULONG_MAX);
    
    
    return 0;
}

