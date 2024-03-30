#pragma once
#include <iterator>
#include <stdexcept>
#include <cassert>
#include <functional>

template<typename T, typename size_t const Size>
class dummy_array_iterator_type;

template<typename T, size_t const Size>
class dummy_array
{
public:
    // aliases
    typedef dummy_array_iterator_type<T, Size> iterator;
    typedef dummy_array_iterator_type<T const, Size> constant_iterator;
    
public:
    // methods
    iterator begin()
    {
        return iterator(data, 0);
    }

    iterator end()
    {
        return iterator(data, Size);
    }

    constant_iterator being() const
    {
        return constant_iterator(data, 0);
    }

    constant_iterator end() const
    {
        return constant_iterator(data, Size);
    }
private:
    T data[Size];
};


template<typename T, size_t const Size>
class dummy_array_iterator_type
{
public:
    typedef dummy_array_iterator_type       self_type;
    typedef T                               value_type;
    typedef T*                              pointer;
    typedef T&                              reference;
    typedef std::random_access_iterator_tag iterator_category;
    typedef ptrdiff_t                       difference_type;
    
public:
    // constructors
    dummy_array_iterator_type() = default;
    explicit dummy_array_iterator_type(pointer ptr, size_t const index)
        : ptr(ptr), index(index)
    {}
    dummy_array_iterator_type(dummy_array_iterator_type const&) = default;
    dummy_array_iterator_type& operator=(dummy_array_iterator_type const&) = default;
    ~dummy_array_iterator_type() = default;

    // operators
    self_type& operator++()
    {
        if(index >= Size)
            throw std::out_of_range("Iterator can't be incremented past the end range!");

        ++index;
        return *this;
    }

    self_type operator++(int)
    {
        self_type tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(self_type const& other) const
    {
        assert(compatible(other));
        return index == other.index;
    }

    bool operator!=(self_type const& other) const
    {
        return !(*this == other);
    }

    reference operator*() const
    {
        if(ptr == nullptr)
            throw std::bad_function_call();

        return *(ptr + index);
    }

    reference operator->() const
    {
        if(ptr == nullptr)
            throw std::bad_function_call();

        return *(ptr + index);
    }


    self_type& operator--()
    {
        if(index < 0)
            throw std::out_of_range("Iterator can't be decremented past the start range!");

        --index;
        return *this;
    }

    self_type operator--(int)
    {
        self_type tmp = *this;
        --(*this);
        return tmp;
    }

    self_type operator+(difference_type offset) const
    {
        self_type tmp = *this;
        return tmp += offset;
    }
    self_type operator-(difference_type offset) const
    {
        self_type tmp = *this;
        return tmp -= offset;
    }

    difference_type operator-(self_type const& other) const
    {
        assert(compatible(other));
        return (index - other.index);
    }

    bool operator<(self_type const& other) const
    {
        assert(compatible(other));
        return index < other.index;
    }

    bool operator>(self_type const& other) const
    {
        return other < *this;
    }

    bool operator<=(self_type const& other) const
    {
        return !(other < *this);
    }

    bool operator>=(self_type const& other) const
    {
        return !(*this < other);
    }

    self_type& operator+=(difference_type const offset)
    {
        if(index + offset < 0 || index + offset > Size)
            throw std::out_of_range("Iterator can't be incremented past the end range!");

        index += offset;
        return *this;
    }

    self_type& operator-=(difference_type const offset)
    {
        return *this += -offset;
    }

    value_type& operator[](difference_type const offset)
    {
        return (*(*this + offset));
    }

    value_type const& operator[](difference_type const offset) const
    {
        return (*(*this + offset));
    }
private:
    bool compatible(self_type const& other) const
    {
        return ptr == other.ptr;
    }

private:
    pointer ptr = nullptr;
    size_t index = 0;
};


