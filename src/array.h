

#ifndef ARRAY_H_
#define ARRAY_H_

#include <iostream>
#include <stdexcept>

namespace arr
{
    template <typename T> class Array2D
    {
        private:
        T* data_;
        size_t dims_[2];
        size_t size_;

        public:

        void allocate(size_t n1,size_t n2)
        {
            size_ = n1*n2;
            data_ = new T[size_];
            dims_[0] = n1;
            dims_[1] = n2;
        }

        // constructors
        Array2D(size_t n1, size_t n2)
        {
            allocate(n1,n2);
        }

        Array2D(size_t n1, size_t n2, T init)
        {
            allocate(n1,n2);
            for (size_t ii=0; ii<size_; ii++)
                data_[ii] = init;
        }

        void reshape(size_t n1, size_t n2)
        {
            if (n1*n2 != size_)
            {
                throw std::runtime_error(
                    "cannot reshape arr of size" +
                    std::to_string(size_) + " to " +
                    std::to_string(n1) + "x" + std::to_string(n2)
                );
            }
            dims_[0] = n1;
            dims_[1] = n2;
        }

        void print()
        {
            size_t dd = 0;
            printf("arr::Array rank 2 is of size %li and shape (%lix%li)\n",size_,dims_[0],dims_[1]);
            for (size_t jj=0; jj<dims_[1]; jj++)
            {
                for (size_t ii=0; ii<dims_[0]; ii++)
                {
                    std::cout << data_[dd] << " ";
                    dd++;
                }
                std::cout << std::endl;
            }
        }

        


        
    };
}

#endif