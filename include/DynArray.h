/**
 * @file    DynArray.h
 * @author  Filipe Forte Tenreiro <filipe.tenreiro1@gmail.com>
 * @brief   Dynamic array header-only library.
 * @version 0.1
 * @date    march 2024
 */

#ifndef DYNARRAY_H
#define DYNARRAY_H

#include<iostream>
#include<stdexcept>

namespace vla{
    //------------------------------------------------------------------------------------------------------------------
    //!
    //! @brief Auxiliary aliases.
    //!
    using size_d = std::ptrdiff_t; //!<Type difference alias.
    using size_t = std::size_t;    //!<Type size       alias.
    //------------------------------------------------------------------------------------------------------------------
    //!
    //! @brief N-D (dynamic) array template.
    //!
    template<typename T, size_t N = 1U, template<typename U> typename A = std::allocator>
    class dynarray{
    private:
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Type aliases.
        //!
        using       type_v =       T;                    //!<         Type value      alias (contig. array).
        using const_type_v = const T;                    //!<Constant type value      alias (contig. array).
        using       type_p =       type_v*;              //!<         Type pointer    alias (contig. array).
        using const_type_p = const type_v*;              //!<Constant type pointer    alias (contig. array).
        using       type_a =     A<type_v>;              //!<         Type allocator  alias (contig. array).
        using       TYPE_V =       dynarray<T, N-1U, A>; //!<         Type value      alias (        array block(s)).
        using const_TYPE_V = const dynarray<T, N-1U, A>; //!<Constant type value      alias (        array block(s)).
        using       TYPE_P =       TYPE_V*;              //!<         Type pointer    alias (        array block(s)).
        using const_TYPE_P = const TYPE_V*;              //!<Constant type pointer    alias (        array block(s)).
        using       TYPE_R =       TYPE_V&;              //!<         Type reference  alias (        array block(s)).
        using const_TYPE_R = const TYPE_V&;              //!<Constant type reference  alias (        array block(s)).
        using       TYPE_A =     A<TYPE_V>;              //!<         Type allocator  alias (        array block(s)).
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Member variables.
        //!
        size_t size_;      //!<Type              size.           //!<n(0)*n(1)*n(2)*...
        type_a allocator_; //!<Type              allocator.
        type_p data_;      //!<Type (pointer to) data (content).
        type_p head_;      //!<Type (pointer to) head.
        type_p tail_;      //!<Type (pointer to) tail.
        size_t SIZE_;      //!<Type              size (N).       //!<n.
        TYPE_A ALLOCATOR_; //!<Type              allocator.
        TYPE_P DATA_;      //!<Type (pointer to) data (content).
        //--------------------------------------------------------------------------------------------------------------
    public:
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Implicitly-defined member functions.
        //!
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //! @note  Example: vla::dynarray<int, 2> a
        //!                 vla::dynarray<int, 3> a
        //!                 ...
        //!
        explicit dynarray() noexcept{
            // Initialise...
            size_ = 0U;
            data_ = nullptr;
            head_ = nullptr;
            tail_ = nullptr;
            SIZE_ = 0U;
            DATA_ = nullptr;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //!
        template<typename ... Args>
        explicit dynarray(size_t n, Args&& ... args){
            // Check the number of (input) arguments...
            if(1U+sizeof ... (args) < N){
                throw std::invalid_argument("dynarray<T, N, A>::dynarray(size_t, Args&& ...)");
            }
            // Get size of contig. array.
            size_ = n*TYPE_V::get(std::forward<Args>(args)...);
            SIZE_ = n;

            if(size_ == 0U){
                // Initialise...
                data_ = nullptr;
                head_ = nullptr;
                tail_ = nullptr;
                DATA_ = nullptr;
            }
            else{
                if(size_ >= max_size()){
                    throw std::invalid_argument("dynarray<T, N, A>::dynarray(size_t, Args&& ...)");
                }
                // Initialise...
                data_ = allocator_.allocate(size_); //!<Allocate n elements (contig. array).
                head_ = data_;
                tail_ = head_+size_-1U;
                DATA_ = ALLOCATOR_.allocate(SIZE_); //!<Allocate n blocks.
                // For each block...
                for(size_t i = 0U; i < SIZE_; ++i){
                    std::allocator_traits<TYPE_A>::construct(ALLOCATOR_, DATA_+i);
                    (DATA_+i)->allocator_ = allocator_;
                    (DATA_+i)->set(data_+i*size_/n, std::forward<Args>(args)...);
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Destructor.
        //!
        ~dynarray() noexcept{
            if(DATA_ != nullptr){
                for(size_t i = SIZE_; i > 0U; --i){
                    std::allocator_traits<TYPE_A>::destroy(ALLOCATOR_, DATA_+i-1U);
                }
                ALLOCATOR_.deallocate(DATA_, SIZE_);
            }
            if(data_ != nullptr){
                for(size_t i = size_; i > 0U; --i){
                    std::allocator_traits<type_a>::destroy(allocator_, data_+i-1U);
                }
                allocator_.deallocate(data_, size_);
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Template class.
        //!
        friend class dynarray<T, N+1U, A>;
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Element access: at.
        //!                        operator[].
        //!                        front.
        //!                        back.
        //!                        data.
        //!
        TYPE_R at(size_t i){
            if(i >= size()){
                throw std::out_of_range("dynarray<T, N, A>::at(size_t)");
            }
            return (*this)[i];
        }
        const_TYPE_R at(size_t i) const{
            if(i >= size()){
                throw std::out_of_range("dynarray<T, N, A>::at(size_t) const");
            }
            return (*this)[i];
        }
        TYPE_R operator[](size_t i) noexcept{
            return *(DATA_+i);
        }
        const_TYPE_R operator[](size_t i) const noexcept{
            return *(DATA_+i);
        }
        TYPE_R front() noexcept{
            return (*this)[0U];
        }
        const_TYPE_R front() const noexcept{
            return (*this)[0U];
        }
        TYPE_R back() noexcept{
            return (*this)[SIZE_-1U];
        }
        const_TYPE_R back() const noexcept{
            return (*this)[SIZE_-1U];
        }
        TYPE_P data() noexcept{
            return head_;
        }
        const_TYPE_P data() const noexcept{
            return head_;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Iterators: begin.
        //!                   end.
        //!
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Capacity: empty.
        //!                  size.
        //!                  max_size.
        //!
        bool empty() const{
            return size_ == 0U;
        }
        size_t size() const{

            std::cout << "X" << std::endl;

            return static_cast<size_t>(head_-tail_+1U);
        }
        size_t max_size() const{
            return static_cast<size_t>(std::numeric_limits<size_d>::max());
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Operations: fill.
        //!                    swap.
        //!
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Non-member functions: operator<< .
        //!                              operator== .
        //!                              operator!= .
        //!
        //--------------------------------------------------------------------------------------------------------------
    private:
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Auxiliary functions.
        //!
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief
        //!
        //--------------------------------------------------------------------------------------------------------------
        template<typename ... Args>
        static size_t get(size_t n, Args&& ... args){
            return TYPE_V::get(std::forward<Args>(args)...)*n;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief
        //!
        template<typename ... Args>
        void set(type_p p, size_t n, Args&& ... args){
            // Get size of array block.
            size_t bsize = TYPE_V::get(std::forward<Args>(args)...);

            // Initialise...
            data_ = nullptr;
            head_ = p;
            tail_ = head_+n*bsize-1U;
            DATA_ = ALLOCATOR_.allocate(n);
            // Construct...
            for(size_t i = 0U; i < n; ++i){
                std::allocator_traits<TYPE_A>::construct(ALLOCATOR_, p+i);
                (DATA_+i)->allocator_ = allocator_;
                (DATA_+i)->set(p+i*bsize, std::forward<Args>(args)...);
            }
        }
        //--------------------------------------------------------------------------------------------------------------
    };
    //------------------------------------------------------------------------------------------------------------------
    //!
    //! @brief 1-D (dynamic) array template.
    //!
    template<typename T, template<typename U> typename A>
    class dynarray<T, 1U, A>{
    private:
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Type aliases.
        //!
        using       type_v =       T;              //!<         Type value      alias.
        using const_type_v = const T;              //!<Constant type value      alias.
        using       type_p =       type_v*;        //!<         Type pointer    alias.
        using const_type_p = const type_v*;        //!<Constant type pointer    alias.
        using       type_r =       type_v&;        //!<         Type reference  alias.
        using const_type_r = const type_v&;        //!<Constant type reference  alias.
        using       type_a =     A<type_v>;        //!<         Type allocator  alias.
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Member variables.
        //!
        size_t size_;      //!<Type              size.
        type_a allocator_; //!<Type              allocator.
        type_p data_;      //!<Type (pointer to) data (content).
        type_p head_;      //!<Type (pointer to) head.
        type_p tail_;      //!<Type (pointer to) tail.
        //--------------------------------------------------------------------------------------------------------------
    public:
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Implicitly-defined member functions.
        //!
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //! @note  Example: vla::dynarray<int>     a
        //!                 vla::dynarray<int, 1U> a
        //!
        explicit dynarray() noexcept{
            // Initialise...
            size_ = 0U;
            data_ = nullptr;
            head_ = nullptr;
            tail_ = nullptr;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //! @note  Example: vla::dynarray<int>     a(1U)    : 5-element (integer) array filled with 0's (default value).
        //!                 vla::dynarray<int, 1U> a(1U)    : 5-element (integer) array filled with 0's (default value).
        //!                 vla::dynarray<int>     a(1U, 5) : 5-element (integer) array filled with 1's.
        //!                 vla::dynarray<int, 1U> a(1U, 5) : 5-element (integer) array filled with 1's.
        //! @see   https://stackoverflow.com/questions/37935393/pass-by-value-vs-pass-by-rvalue-reference
        //!
        template<typename ... Args>
        explicit dynarray(size_t n, Args&& ... args){
            if(n == 0U){
                // Initialise...
                size_ = 0U;
                data_ = nullptr;
                head_ = nullptr;
                tail_ = nullptr;
            }
            else{
                if(n >= max_size()){
                    throw std::invalid_argument("dynarray<T, 1, A>::dynarray(size_t, Args&& ...)");
                }
                // Initialise...
                size_ = n;
                data_ = allocator_.allocate(size_);
                head_ = data_;
                tail_ = head_+size_-1U;
                // Construct...
                for(size_t i = 0U; i < size_; ++i){
                    std::allocator_traits<type_a>::construct(allocator_, data_+i, std::forward<Args>(args)...);
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //! @note  Example: vla::dynarray<int>     a{1, 1, 1, 1, 1} : 5-element (integer) array filled with 1's.
        //!                 vla::dynarray<int, 1U> a{1, 1, 1, 1, 1} : 5-element (integer) array filled with 1's.
        //!
        explicit dynarray(std::initializer_list<T> il){
            if(il.size() == 0U){
                // Initialise...
                size_ = 0U;
                data_ = nullptr;
                head_ = nullptr;
                tail_ = nullptr;
            }
            else{
                if(il.size() >= max_size()){
                    throw std::invalid_argument("dynarray<T, 1, A>::dynarray(std::initializer_list<T>)");
                }
                // Initialise...
                size_ = il.size();
                data_ = allocator_.allocate(size_);
                head_ = data_;
                tail_ = head_+size_-1U;
                // Construct...
                for(size_t i = 0U; i < size_; ++i){
                    std::allocator_traits<type_a>::construct(allocator_, data_+i, *(il.begin()+i));
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Copy constructor (deep copy).
        //! @note  Example: vla::dynarray<int>     a(b) <=> vla::dynarray<int>     a = b.
        //!                 vla::dynarray<int, 1U> a(b) <=> vla::dynarray<int, 1U> a = b.
        //! @see   https://www.geeksforgeeks.org/shallow-copy-and-deep-copy-in-c/
        //!
        dynarray(const dynarray& other) noexcept{
            if(other.size() == 0U){
                // Initialise...
                size_ = 0U;
                data_ = nullptr;
                head_ = nullptr;
                tail_ = nullptr;
            }
            else{
                // Initialise...
                size_ = other.size();
                data_ = allocator_.allocate(size_);
                head_ = data_;
                tail_ = head_+size_-1U;
                // Construct...
                for(size_t i = 0U; i < size_; ++i){
                    std::allocator_traits<type_a>::construct(allocator_, data_+i, *(other.data_+i));
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Move constructor (initialisation via constructor: dynarray() noexcept).
        //! @note  Example: vla::dynarray<int>     a(std::move(b)) <=> vla::dynarray<int>     a = std::move(b).
        //!                 vla::dynarray<int, 1U> a(std::move(b)) <=> vla::dynarray<int, 1U> a = std::move(b).
        //!
        dynarray(dynarray&& other) noexcept : dynarray(){
                swap(*this, other);
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Copy/move assignment (calls copy/move constructor).
        //! @note  Example: a = b.
        //!                 a = std::move(b), where *this = a and other = b.
        //! @see   https://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
        //!        https://en.cppreference.com/w/cpp/language/copy_elision
        //!
        dynarray& operator=(dynarray other) noexcept{
            swap(*this, other);
            return *this;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Destructor.
        //! @note  Deleting a null pointer effectively amounts to a no-operation.
        //! @see   https://stackoverflow.com/questions/615355/is-there-any-reason-to-check-for-a-null-pointer-before-deleting
        //!        https://stackoverflow.com/questions/75968358/deleting-a-nullptr-may-or-may-not-call-a-deallocation-function-why-not-guarante
        //!
        ~dynarray() noexcept{
            if(data_ != nullptr){
                for(size_t i = size_; i > 0U; --i){
                    std::allocator_traits<type_a>::destroy(allocator_, data_+i-1U);
                }
                allocator_.deallocate(data_, size_);
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Template class.
        //!
        friend class dynarray<T, 2U, A>;
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Element access: at.
        //!                        operator[].
        //!                        front.
        //!                        back.
        //!                        data.
        //!
        type_r at(size_t i){
            if(i >= size()){
                throw std::out_of_range("dynarray<T, 1, A>::at(size_t)");
            }
            return (*this)[i];
        }
        const_type_r at(size_t i) const{
            if(i >= size()){
                throw std::out_of_range("dynarray<T, 1, A>::at(size_t) const");
            }
            return (*this)[i];
        }
        type_r operator[](size_t i) noexcept{
            return *(head_+i);
        }
        const_type_r operator[](size_t i) const noexcept{
            return *(head_+i);
        }
        type_r front() noexcept{
            return (*this)[0U];
        }
        const_type_r front() const noexcept{
            return (*this)[0U];
        }
        type_r back() noexcept{
            return (*this)[size()-1U];
        }
        const_type_r back() const noexcept{
            return (*this)[size()-1U];
        }
        type_p data() noexcept{
            return head_;
        }
        const_type_p data() const noexcept{
            return head_;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Iterators: begin.
        //!                   end.
        //!
        type_p begin() noexcept{
            return head_;
        }
        const_type_p begin() const noexcept{
            return head_;
        }
        type_p end() noexcept{
            return tail_+1U;
        }
        const_type_p end() const noexcept{
            return tail_+1U;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Capacity: empty.
        //!                  size.
        //!                  max_size.
        //!
        bool empty() const noexcept{
            return size() == 0U;
        }
        size_t size() const noexcept{
            return static_cast<size_t>(tail_-head_+1U);
        }
        size_t max_size() const noexcept{
            return static_cast<size_t>(std::numeric_limits<size_d>::max());
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Operations: fill.
        //!                    swap.
        //!
        void fill(const_type_v& value) noexcept{
            for(size_t i = 0U; i < size(); ++i){
                (*this)[i] = value;
            }
        }
        friend void swap(dynarray& lhs, dynarray& rhs) noexcept{
            std::swap(lhs.data_, rhs.data_);
            std::swap(lhs.head_, rhs.head_);
            std::swap(lhs.tail_, rhs.tail_);
            std::swap(lhs.size_, rhs.size_);
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Non-member functions: operator<< .
        //!                              operator== .
        //!                              operator!= .
        //!
        friend std::ostream& operator<<(std::ostream& o, const dynarray& other) noexcept{
            o << "[";
            for(size_t i = 0U; i < other.size(); ++i){
                if(i < other.size()-1U){
                    o << other[i] << " ";
                }
                else{
                    o << other[i];
                }
            }
            return o << "]";
        }
        friend bool operator==(const dynarray& lhs, const dynarray& rhs){
            return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }
        friend bool operator!=(const dynarray& lhs, const dynarray& rhs){
            return !(lhs == rhs);
        }
        //--------------------------------------------------------------------------------------------------------------
    private:
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Auxiliary functions.
        //! @see   https://stackoverflow.com/questions/10442404/invoke-a-c-class-method-without-a-class-instance
        //!
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief
        //!
        //--------------------------------------------------------------------------------------------------------------
        template<typename ... Args>
        static size_t get(size_t n, Args&& ...){
            return n;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief
        //!
        template<typename ... Args>
        void set(type_p p, size_t n, Args&& ... args){
            // Initialise...
            data_ = nullptr;
            head_ = p;
            tail_ = head_+n-1U;
            // Construct...
            for(size_t i = 0U; i < n; ++i){
                std::allocator_traits<type_a>::construct(allocator_, p+i, std::forward<Args>(args)...);
            }
        }
        //--------------------------------------------------------------------------------------------------------------
    };
    //------------------------------------------------------------------------------------------------------------------
}

#endif