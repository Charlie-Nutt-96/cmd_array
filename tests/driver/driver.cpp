#include <stdio.h>

#include "Array.h"

int main ()
{
    printf("hello world!\n");

    arr::Array2D<float> my_array(2,2,2.6);
    my_array.print();

    my_array.reshape(1,4);
    my_array.print();
}