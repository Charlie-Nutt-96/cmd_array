#include <stdio.h>

#include "base_array.h"

int main ()
{
    printf("hello world!\n");

    std::array<float,6> arr = {1.1,2.2,3.3,4.4,5.5,6.6};
    cmda::Array<float,2> my1darray(3,2,arr);

    my1darray.print();

    printf("%f\n",my1darray(1,1));
}