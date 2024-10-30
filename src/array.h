

#ifndef CMD_ARRAY_H_
#define CMD_ARRAY_H_

#include <array>
#include <vector>
#include <iostream>

/**
 * @brief contiguous multidimensional array (cmda) namespace
 * 
 */
namespace cmda
{
    /**
     * @brief contiguous multidimensional array object
     * 
     * @tparam T data type the array is of
     * @tparam ND the number of dimensions
     */
    template <typename T, size_t ND>
    class Base_array
    {
        protected:
        T* data_ = nullptr; ///< contiguous block of data for the array
        size_t size_ = 0; ///< the length of the contiguous block of data
        std::array<size_t,ND> dims_ = {0}; ///< the length of each dimension of the array
        std::array<size_t,ND-1> strides_; ///< the stride length of each dimension
        bool owns_data_ = true; ///< flag to show if the class owns the data it points to

        /**
         * @brief Initilise the array, if null ptr allocates the data.
         * 
         * @param extant_data pointer to extant data to use, if nullptr then allocate its own memory
         * @param dims the length of each dimension
         */
        void init (T* extant_data, std::array<size_t,ND> dims)
        {
            clear();
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
        }

        /**
         * @brief copy function
         * 
         * @param c source for copy
         */
        void copy(const Base_array& c)
        {
            size_ = c.size_;
            dims_ = c.dims_;
            strides_ = c.strides_;
            owns_data_ = true;
           
            data_ = new T[size_];
            std::copy(c.begin(), c.end(), begin());
        }

        public: 

        /**
         * @brief Resets the Base_array object
         * 
         */
        void clear()
        {
            if (owns_data_ && data_!=nullptr)
                delete[] data_;

            size_ = 0;
            dims_ = {0};
            strides_.fill(0);
            data_ = nullptr;
        }

        /**
         * @brief Destroy the Base_array object
         * 
         */
        virtual ~Base_array()
        {
            clear();
        }

        //  constructors
        /**
         * @brief Construct a new Base_array object
         * 
         */
        Base_array() {}

        /**
         * @brief Construct a new Base_array object as a copy
         * 
         * @param c source to copy from
         */
        Base_array(const Base_array& c)
        {
            copy(c);
        }

        /**
         * @brief copy assignment
         * 
         * @param c source to copy from
         * @return Base_array& 
         */
        Base_array& operator=(const Base_array& c)
        {
            copy(c);
            return *this;
        }

        /**
         * @brief Construct a new Base_array object (move)
         * 
         * @param s source to move from
         */
        Base_array(Base_array&& s)
        {
            size_ = s.size_;
            dims_ = s.dims_;
            strides_ = s.strides_;
            owns_data_ = s.owns_data_;

            data_ = s.data_;

            s.owns_data_ = false;
            s.clear();
        }

        /**
         * @brief move assignment operator
         * 
         * @param c source of the move
         * @return Base_array& 
         */
        Base_array& operator=(Base_array&& s)
        {
            if (this != &s)
            {
                clear();

                size_ = s.size_;
                dims_ = s.dims_;
                strides_ = s.strides_;
                owns_data_ = s.owns_data_;

                data_ = s.data_;

                s.owns_data_ = false;
                s.clear();
            }
            return *this;
        }
        
        /**
         * @brief Construct a new Base_array object and allocate the size
         * 
         * @param dims the size of each dimension
         */
        Base_array(const std::array<size_t,ND> dims) { init(nullptr,dims); }

        /**
         * @brief Construct a new Base_array object, allocate it and fill with the given value
         * 
         * @param dims size of each dimension
         * @param fill_value value to fill the array with
         */
        Base_array(const std::array<size_t,ND> dims, T fill_value)
        {
            init(nullptr,dims);
            fill(fill_value);
        }

        /**
         * @brief Copy Construct a new Base_array object from a std::vector
         * 
         * @param dims size of each dimension
         * @param fill_values vector to construct from
         */
        Base_array(const std::array<size_t,ND> dims, const std::vector<T>& fill_values)
        {
            init(nullptr,dims);
            if (size_ != fill_values.size())
                throw std::runtime_error("cannot initialise array from vector of different size");
            std::copy(fill_values.begin(), fill_values.end(),begin());
        }

        /**
         * @brief Copy Construct a new Base_array object from a std::array
         * 
         * @tparam S size of the std::array
         * @param dims size of each dimension
         * @param fill_values std::array to copy construct from
         */
        template<size_t S>
        Base_array(const std::array<size_t,ND> dims, const std::array<T,S>& fill_values)
        {
            init(nullptr,dims);
            if (size_ != fill_values.size())
                throw std::runtime_error("cannot initialise array from vector of different size");
            std::copy(fill_values.begin(), fill_values.end(),begin());
        }

        /**
         * @brief Construct a new Base_array object from extant data, the class will not own the data
         * 
         * @param extant_data pointer to the contiguous data to construct from
         * @param dims size of each dimension
         */
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
            std::cout << ND <<"-dimensional Array of shape: ";
            for(size_t dim=0; dim<ND; dim++)
                std::cout << dims_[dim] << " ";
            std::cout << std::endl;

            std::array<size_t,ND> indx = {0};
            print(ND,indx);
        }

        void print_row(std::array<size_t,ND> &indx) const
        {
            
            size_t start_indx = get_index(indx);
            for (size_t ii=0; ii<dims_[ND-1]; ii++)
            {
                std::cout << data_[start_indx+ii] << " ";
            }
        }

        void print(size_t dim,std::array<size_t,ND> &indx) const
        {
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

        size_t dim() const {return ND;}

        bool is_empty() const {return data_ == nullptr;}

        bool is_equivalent_to(Base_array &c)
        {
            if(size_    != c.size_)    return false;
            if(dims_    != c.dims_)    return false;
            if(strides_ != c.strides_) return false;

            for (int i=0; i<size_; i++) 
                if (data_[i] != c.data_[i]) 
                    return false;

            return true;
        }

        bool has_equal_data(const std::vector<T> &c)
        {
            if(size_ != c.size()) 
                return false;

            for (int i=0; i<size_; i++) 
                if (data_[i] != c[i]) 
                    return false;

            return true;
        }

    };


    template<typename T, size_t ND> 
    class Array : public Base_array<T,ND> {};

    template <typename T> 
    class Array<T,1> : public Base_array<T,1>
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

        void allocate(size_t n1) { Base::allocate({n1}); }

        T& operator()(size_t n1) {return Base::operator()({n1});}
    };

    template <typename T> 
    class Array<T,2> : public Base_array<T,2>
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

        void allocate(size_t n1, size_t n2) {Base::allocate({n1,n2});}

        T& operator()(size_t n1, size_t n2) {return Base::operator()({n1,n2});}
    };

    template <typename T> 
    class Array<T,3> : public Base_array<T,3>
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

        void allocate(size_t n1, size_t n2, size_t n3) {Base::allocate({n1,n2,n3});}

        T& operator()(size_t n1, size_t n2, size_t n3) {return Base::operator()({n1,n2,n3});}
    };

    template <typename T> 
    class Array<T,4> : public Base_array<T,4>
    {
        private:
        using Base = Base_array<T,4>;
        public:
        using Base::Base;
        using Base::allocate;
        using Base::operator();

        Array(T* extant_data, size_t n1, size_t n2, size_t n3, size_t n4) : Base(extant_data,{n1,n2,n3,n4}){}
        Array(size_t n1, size_t n2, size_t n3, size_t n4) : Base({n1,n2,n3,n4}){}
        Array(size_t n1, size_t n2, size_t n3, size_t n4,T fill_value) : Base({n1,n2,n3,n4}, fill_value){}
        template<size_t S>
        Array(size_t n1, size_t n2, size_t n3, size_t n4, std::array<T,S> &fill_values) : Base({n1,n2,n3,n4},fill_values){}
        Array(size_t n1, size_t n2, size_t n3, size_t n4, std::vector<T> &fill_values) : Base({n1,n2,n3,n4},fill_values){}

        void allocate(size_t n1, size_t n2, size_t n3, size_t n4) {Base::allocate({n1,n2,n3,n4});}

        T& operator()(size_t n1, size_t n2, size_t n3, size_t n4) {return Base::operator()({n1,n2,n3,n4});}
    };
}

#endif