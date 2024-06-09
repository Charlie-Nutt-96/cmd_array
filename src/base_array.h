

#ifndef ARRAY_H_
#define ARRAY_H_

#include <array>
#include <vector>
#include <iostream>

namespace arr
{
    template <typename T, size_t ND>
    class Base_array
    {
        private:
        T* data_ = nullptr;
        size_t size_ = 0;
        std::array<size_t,ND> dims_ = {0};
        std::array<size_t,ND-1> strides_;
        bool owns_data_ = true;

        void init (T* extant_data, std::array<size_t,ND> dims)
        {
            size_ = 1;
            for (size_t dim_len : dims)
                size_ *= dim_len;

            if (extant_data == nullptr)
            {
                owns_data_ = true;
                data_ = new T[size_];
            }
            else
            {
                owns_data_ = false;
                data_ = extant_data;
            }

            dims_ = dims;

            size_t strd = 1;
            for (size_t idim = ND-1; idim>0; idim--)
            {
                strd *= dims_[idim];
                strides_[idim-1] = strd;
            }
            printf("Strides: ");
            for(size_t dim=0; dim<ND-1; dim++)
                printf("%li ",strides_[dim]);
            printf("\n");
        }

        public: 
        virtual ~Base_array()
        {
            if (owns_data_ && data_!=nullptr)
                delete[] data_;

            size_ = 0;
            dims_ = {0};
            strides_.fill(0);
            data_ = nullptr;
        }

        //  constructors
        Base_array() { }
        Base_array(const std::array<size_t,ND> dims) { init(nullptr,dims); }

        Base_array(const std::array<size_t,ND> dims, T fill_value)
        {
            init(nullptr,dims);
            fill(fill_value);
        }

        Base_array(const std::array<size_t,ND> dims, const std::vector<T>& fill_values)
        {
            init(nullptr,dims);
            if (size_ != fill_values.size())
                throw std::runtime_error("cannot initialise array from vector of different size");
            std::copy(fill_values.begin(), fill_values.end(),begin());
        }

        template<size_t S>
        Base_array(const std::array<size_t,ND> dims, const std::array<T,S>& fill_values)
        {
            init(nullptr,dims);
            if (size_ != fill_values.size())
                throw std::runtime_error("cannot initialise array from vector of different size");
            std::copy(fill_values.begin(), fill_values.end(),begin());
        }

        Base_array(T* extant_data, const std::array<size_t,ND> dims) { init(extant_data,dims); }



        // iterators
        T* begin() const {return data_;}
        T* end() const {return data_ + size_;}
        // const T* begin() const {return data_;}
        // const T* end() const {return data_ + size_;}
        const T* cbegin() const {return data_;}
        const T* cend() const {return data_ + size_;}

        // utility
        void allocate(std::array<size_t,ND> dims) {init(nullptr,dims);}

        void fill(T fill_value)
        {
            for (size_t ii=0; ii<size_; ii++)
                data_[ii] = fill_value;
        }

        size_t get_index(std::array<size_t,ND> dims) const
        {
            // TODO: bounds check on debug build

            size_t indx = 0;
            for(size_t ii=0;ii<ND-1;ii++)
            {
                indx += dims[ii] * (strides_[ii]);
            }
            return indx + dims[ND-1];
        }

        void print() const
        {
            printf("%li dimensional Array of shape: ",ND);
            for(size_t dim=0; dim<ND; dim++)
                printf("%li ",dims_[dim]);
            printf("\n");

            std::array<size_t,ND> indx = {0};
            print(ND,indx);
        }

        void print_row(std::array<size_t,ND> &indx) const
        {
            printf("Index ");
            for(size_t dd=0; dd<ND; dd++)
                printf("%li ",indx[dd]);
            size_t start_indx = get_index(indx);
            printf("= %li\n",start_indx);
            for (size_t ii=0; ii<dims_[ND-1]; ii++)
            {
                std::cout << data_[start_indx+ii] << " ";
            }
        }

        void print(size_t dim,std::array<size_t,ND> &indx) const
        {
            printf("dim: %li\n",dim);
            if (dim==1)
            {
                print_row(indx);
            }
            else
            {
                for(size_t ii=0; ii<dims_[ND-dim]; ii++)
                {
                    indx[ND-dim]=ii;
                    print(dim-1,indx);
                    std::cout << std::endl;
                }
            }
        }

        // accessors
        T& operator()(std::array<size_t,ND> dims) const
        {
            return data_[get_index(dims)];
        }

        T& operator[](size_t indx) const
        {
            return data_[indx];
        }

        T* data() const {return data_;}

        size_t size() const {return size_;}

        size_t n(size_t dim) const {return dims_[dim];}

    };


    template<typename T, size_t ND> 
    class Array : public Base_array<T,ND> {};

    template <typename T> class Array<T,1> : public Base_array<T,1>
    {
        private:
        using Base = Base_array<T,1>;
        public:
        using Base::Base;
        using Base::allocate;
        using Base::operator();

        Array(T* extant_data, size_t n1) : Base(extant_data,{n1}){}
        Array(size_t n1) : Base({n1}){}
        Array(size_t n1,T fill_value) : Base({n1}, fill_value){}
        template<size_t S>
        Array(size_t n1, std::array<T,S> &fill_values) : Base({n1},fill_values){}
        Array(size_t n1, std::vector<T> &fill_values) : Base({n1},fill_values){}

        void allocate(size_t n1) {allocate({n1});}

        T& operator()(size_t n1) {operator()({n1});}
    };

    template <typename T> class Array<T,2> : public Base_array<T,2>
    {
        private:
        using Base = Base_array<T,2>;
        public:
        using Base::Base;
        using Base::allocate;
        using Base::operator();

        Array(T* extant_data, size_t n1, size_t n2) : Base(extant_data,{n1,n2}){}
        Array(size_t n1, size_t n2) : Base({n1,n2}){}
        Array(size_t n1, size_t n2,T fill_value) : Base({n1,n2}, fill_value){}
        template<size_t S>
        Array(size_t n1, size_t n2, std::array<T,S> &fill_values) : Base({n1,n2},fill_values){}
        Array(size_t n1, size_t n2, std::vector<T> &fill_values) : Base({n1,n2},fill_values){}

        void allocate(size_t n1, size_t n2) {allocate({n1,n2});}

        T& operator()(size_t n1, size_t n2) {operator()({n1,n2});}
    };

    template <typename T> class Array<T,3> : public Base_array<T,3>
    {
        private:
        using Base = Base_array<T,3>;
        public:
        using Base::Base;
        using Base::allocate;
        using Base::operator();

        Array(T* extant_data, size_t n1, size_t n2, size_t n3) : Base(extant_data,{n1,n2,n3}){}
        Array(size_t n1, size_t n2, size_t n3) : Base({n1,n2,n3}){}
        Array(size_t n1, size_t n2, size_t n3,T fill_value) : Base({n1,n2,n3}, fill_value){}
        template<size_t S>
        Array(size_t n1, size_t n2, size_t n3, std::array<T,S> &fill_values) : Base({n1,n2,n3},fill_values){}
        Array(size_t n1, size_t n2, size_t n3, std::vector<T> &fill_values) : Base({n1,n2,n3},fill_values){}

        void allocate(size_t n1, size_t n2, size_t n3) {allocate({n1,n2,n3});}

        T& operator()(size_t n1, size_t n2, size_t n3) {operator()({n1,n2,n3});}
    };
}

#endif