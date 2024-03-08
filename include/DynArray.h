/**
 * @file   DynArray.h
 * @author Filipe Forte Tenreiro <filipe.tenreiro1@gmail.com>
 * @date   march 2024
 * @brief  Dynamic array header-only library.
 */

#ifndef DYNARRAY_H
#define DYNARRAY_H

#include<iostream>
#include<stdexcept>

namespace vla{
    //!
    //! @brief N-D (dynamic) array template.
    //!
    template<typename T, size_t N = 1U, template<typename U> typename A = std::allocator>
    class dynarray{
    };

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
        using const_type_v = const T;              //!<Constant Type value      alias.
        using       type_p =       type_v*;        //!<         Type pointer    alias.
        using const_type_p = const type_v*;        //!<Constant Type pointer    alias.
        using       type_r =       type_v&;        //!<         Type reference  alias.
        using const_type_r = const type_v&;        //!<Constant Type reference  alias.
        using       type_a =     A<type_v>;        //!<         Type allocator  alias.
        using       size_d =       std::ptrdiff_t; //!<         Type difference alias.
        using       size_t =       std::size_t;    //!<         Type size       alias.
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Member variables.
        //!
        type_a _allocator; //!<Container/self              allocator.
        type_p _data;      //!<Container/self (pointer to) data (content).
        type_p _head;      //!<Container/self (pointer to) head.
        type_p _tail;      //!<Container/self (pointer to) tail.
        size_t _size;      //!<Container/self              size.
        //--------------------------------------------------------------------------------------------------------------
    public:
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Implicitly-defined member functions.
        //!
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //! @note  Example: vla::dynarray<int>    a
        //!                 vla::dynarray<int, 1> a
        //!
        dynarray() noexcept{
            // Initialise...
            _size = 0U;
            _data = nullptr;
            _head = nullptr;
            _tail = nullptr;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //! @note  Example: vla::dynarray<int>    a(1)    : 5-element (integer) array filled with 0's (default value).
        //!                 vla::dynarray<int, 1> a(1)    : 5-element (integer) array filled with 0's (default value).
        //!                 vla::dynarray<int>    a(1, 5) : 5-element (integer) array filled with 1's.
        //!                 vla::dynarray<int, 1> a(1, 5) : 5-element (integer) array filled with 1's.
        //! @see   https://stackoverflow.com/questions/37935393/pass-by-value-vs-pass-by-rvalue-reference
        //!
        template<typename ... Args>
        explicit dynarray(size_t n, Args&& ... args){
            if(n == 0U){
                // Initialise...
                _size = 0U;
                _data = nullptr;
                _head = nullptr;
                _tail = nullptr;
            }
            else{
                if(n >= max_size()){
                    throw std::invalid_argument("dynarray<T, 1, A>::dynarray(size_t, Args&& ...)");
                }
                // Initialise...
                _size =  n;
                _data = _allocator.allocate(_size);
                _head = _data;
                _tail = _data+_size-1;
                // Assign...
                for(size_t i = 0U; i < _size; ++i){
                    std::allocator_traits<type_a>::construct(_allocator, _data+i, std::forward<Args>(args)...);
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Constructor.
        //! @note  Example: vla::dynarray<int>    a{1, 1, 1, 1, 1} : 5-element (integer) array filled with 1's.
        //!                 vla::dynarray<int, 1> a{1, 1, 1, 1, 1} : 5-element (integer) array filled with 1's.
        //!
        dynarray(std::initializer_list<T> il){
            if(il.size() == 0U){
                // Initialise...
                _size = 0U;
                _data = nullptr;
                _head = nullptr;
                _tail = nullptr;
            }
            else{
                if(il.size() >= max_size()){
                    throw std::invalid_argument("dynarray<T, 1, A>::dynarray(std::initializer_list<T>)");
                }
                // Initialise...
                _size =  il.size();
                _data = _allocator.allocate(_size);
                _head = _data;
                _tail = _data+_size-1;
                // Assign...
                for(size_t i = 0U; i < _size; ++i){
                    std::allocator_traits<type_a>::construct(_allocator, _data+i, *(il.begin()+i));
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Copy constructor (deep copy).
        //! @note  Example: vla::dynarray<int>    a(b) <=> vla::dynarray<int>    a = b.
        //!                 vla::dynarray<int, 1> a(b) <=> vla::dynarray<int, 1> a = b.
        //! @see   https://www.geeksforgeeks.org/shallow-copy-and-deep-copy-in-c/
        //!
        dynarray(const dynarray& other){
            if(other.size() == 0U){
                // Initialise...
                _size = 0U;
                _data = nullptr;
                _head = nullptr;
                _tail = nullptr;
            }
            else{
                // Initialise...
                _size =  other.size();
                _data = _allocator.allocate(_size);
                _head = _data;
                _tail = _data+_size-1;
                // Assign...
                for(size_t i = 0U; i < _size; ++i){
                    std::allocator_traits<type_a>::construct(_allocator, _data+i, *(other._data+i));
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Move constructor (initialisation via constructor: dynarray() noexcept).
        //! @note  Example: vla::dynarray<int>    a(std::move(b)) <=> vla::dynarray<int>    a = std::move(b).
        //!                 vla::dynarray<int, 1> a(std::move(b)) <=> vla::dynarray<int, 1> a = std::move(b).
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
            if(_data != nullptr){
                for(size_t i = _size; i > 0U; --i){
                    std::allocator_traits<type_a>::destroy(_allocator, _data+i-1U);
                }
                _allocator.deallocate(_data, size());
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Template class (N = 2).
        //!
        friend class dynarray<T, 2U, A>;
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Element access: at
        //!                        operator[]
        //!                        front
        //!                        back
        //!                        data
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
            return *(_head+i);
        }
        const_type_r operator[](size_t i) const noexcept{
            return *(_head+i);
        }
        type_r front() noexcept{
            return (*this)[0];
        }
        const_type_r front() const noexcept{
            return (*this)[0];
        }
        type_r back() noexcept{
            return (*this)[_size-1U];
        }
        const_type_r back() const noexcept{
            return (*this)[_size-1U];
        }
        type_p data() noexcept{
            return _head;
        }
        const_type_p data() const noexcept{
            return _head;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Iterators: begin.
        //!                   end.
        //!
        type_p begin() noexcept{
            return _head;
        }
        const_type_p begin() const noexcept{
            return _head;
        }
        type_p end() noexcept{
            return _tail+1;
        }
        const_type_p end() const noexcept{
            return _tail+1;
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Capacity: empty.
        //!                  size.
        //!                  max_size.
        //!
        bool empty() const{
            return _size == 0U;
        }
        size_t size() const{
            return _size;
        }
        size_t max_size() const{
            return static_cast<size_t>(std::numeric_limits<size_d>::max());
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Operations: fill.
        //!                    swap.
        //!
        void fill(const_type_v& value) noexcept{
            for(size_t i = 0U; i < _size; ++i){
                (*this)[i] = value;
            }
        }
        friend void swap(dynarray& lhs, dynarray& rhs) noexcept{
            std::swap(lhs._data, rhs._data);
            std::swap(lhs._head, rhs._head);
            std::swap(lhs._tail, rhs._tail);
            std::swap(lhs._size, rhs._size);
        }
        //--------------------------------------------------------------------------------------------------------------
        //!
        //! @brief Non-member functions: operator<< .
        //!                              operator== .
        //!                              operator!= .
        //!
        friend std::ostream& operator<<(std::ostream& o, const dynarray& other) noexcept{
            o << "[";
            for(size_t i = 0U; i < other._size; ++i){
                if(i < other._size-1){
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
    };
}
#endif