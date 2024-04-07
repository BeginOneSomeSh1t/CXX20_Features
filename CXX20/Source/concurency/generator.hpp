#pragma once
#include <coroutine>

template<typename _Ty>
struct generator
{
    struct promise_type
    {
        _Ty const* _Value_ptr;
        std::exception_ptr _Exc_ptr;

        auto get_return_object()
        {
            return generator{ *this };
        }

        auto initial_suspend() noexcept
        {
            return std::suspend_always{};
        }

        auto final_suspend() noexcept
        {
            return std::suspend_always{};
        }

        void unhandled_exception() noexcept
        {
            _Exc_ptr = std::current_exception();
        }

        void rethrow_if_exception()
        {
            if(_Exc_ptr)
            {
                std::rethrow_exception(_Exc_ptr);
            }
        }

        auto yield_value(_Ty const& _Val)
        {
            _Value_ptr = std::addressof(_Val);
            return std::suspend_always{};
        }

        void return_void() noexcept {}

        template<typename _Uy>
        _Uy&& await_transform(_Uy&& _UVal)
        {
            return std::forward<_Uy>(_UVal);
        }
    };

    struct iterator
    {
        using iterator_category = std::input_iterator_tag;
        using value_type = _Ty;
        using reference = _Ty const&;
        using pointer = _Ty const*;
        using difference_type = ptrdiff_t;


        iterator() = default;

        iterator(nullptr_t) : _Handle(nullptr) {}
        
        iterator(std::coroutine_handle<promise_type> _Coroutine)
            :
        _Handle(_Coroutine)
        {}

        iterator& operator++()
        {
            _Handle.resume();
            if(_Handle.done())
            {
                std::exchange(_Handle, {}).promise()
                                                        .rethrow_if_exception();
            }
            return *this;
        }

        void operator++(int)
        {
            ++*this;
        }

        bool operator==(const iterator& _Oth) const
        {
            return _Handle == _Oth._Handle;
        }

        bool operator!=(const iterator& _Oth) const
        {
            return !(*this == _Oth);
        }

        reference operator*() const
        {
            return *_Handle.promise()._Value_ptr;
        }

        pointer operator->()
        {
            return std::addressof(_Handle.promise()._Value_ptr);
        }
    private:
        
        std::coroutine_handle<promise_type> _Handle;
    };

    generator() = default;
    generator(promise_type& _Prom)
        :
    _Handle(std::coroutine_handle<promise_type>::from_promise(_Prom))
    {}

    generator(generator const&) = delete;
    generator& operator=(generator const&) = delete;

    generator(generator&& _Other) noexcept
        :
    _Handle(nullptr)
    {
        *this = std::move(_Other);
    }

    generator& operator=(generator&& _Other) noexcept
    {
        if(this != std::addressof(_Other))
        {
            if(_Handle) _Handle.destroy();
            _Handle = _Other._Handle;
            _Other._Handle = nullptr;
        }
        return *this;
    }

    ~generator()
    {
        if(_Handle)
            _Handle.destroy();
    }

    iterator begin()
    {
        if(_Handle)
        {
            _Handle.resume();
            if(_Handle.done())
            {
                _Handle.promise().rethrow_if_exception();
                return { nullptr };
            }
        }

        return { nullptr };
    }

    iterator end()
    {
        return { nullptr };
    }
private:
    std::coroutine_handle<promise_type> _Handle;
};