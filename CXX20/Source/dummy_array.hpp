#pragma once

template<typename T, size_t const Size>
class dummy_array
{
    T data[Size];
public:
    T const& get_at(size_t const index) const
    {
        if(index < Size) return data[index];
        throw std::out_of_range("index is out of range!");
    }

    void set_at(size_t const index, T const& value)
    {
        if (index < Size) data[index] = value;
        else throw std::out_of_range("index is out of range!");
    }

    size_t get_size() const {return Size;}
};


template<typename T, typename C, size_t const Size>
class dummy_array_iterator_type
{
public:
    dummy_array_iterator_type(C& collection, size_t const index)
        :
        collection(collection),
        index(index)
    {}

    bool operator!=(dummy_array_iterator_type const& other) const
    {
        return index != other.index;
    }

    T const& operator*() const
    {
        return collection.get_at(index);
    }

    dummy_array_iterator_type& operator++()
    {
        ++index;
        return *this;
    }

    dummy_array_iterator_type operator++(int)
    {
        auto temp = *this;
        ++*temp;
        return temp;
    }

private:
    C& collection;
    size_t index;
};


template<typename T, size_t const Size>
using dummy_array_iterator = dummy_array_iterator_type<T, dummy_array<T, Size>, Size>;

template<typename T, size_t const Size>
using dummy_array_const_iterator = dummy_array_iterator_type<T, dummy_array<T, Size> const, Size>;

template<typename T, size_t const Size>
inline dummy_array_iterator<T, Size> begin(dummy_array<T, Size>& collection)
{
    return dummy_array_iterator<T, Size>(collection, 0);
}
template<typename T, size_t const Size>
inline dummy_array_iterator<T, Size> end(dummy_array<T, Size>& collection)
{
    return dummy_array_iterator<T, Size>(collection, collection.get_size());
}
template<typename T, size_t const Size>
inline dummy_array_const_iterator<T, Size> begin(dummy_array<T, Size> const& collection)
{
    return dummy_array_const_iterator<T, Size>(collection, 0);
}
template<typename T, size_t const Size>
inline dummy_array_const_iterator<T, Size> end(dummy_array<T, Size> const
    & collection)
{
    return dummy_array_const_iterator<T, Size>(collection, collection.get_size());
}