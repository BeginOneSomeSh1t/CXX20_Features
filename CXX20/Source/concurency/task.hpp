#pragma once
#include <coroutine>

namespace details
{
    struct promise_base
    {
        auto initial_suspend() noexcept
        {
            return std::suspend_always{};
        }

        auto final_suspend() noexcept
        {
            return std::suspend_always{};
        }

        [[noreturn]] void unhandled_exception()
        {
            std::terminate();
        }
    };

    template<typename _Ty>
    struct promise final : public promise_base
    {
        auto get_return_object()
        {
            return std::coroutine_handle<promise>::from_promise(*this);
        }

        template<typename _Vy,
                 typename = std::enable_if_t<
                        std::is_convertible_v<_Vy&&, _Ty>
                     >>
        void return_value(_Vy&& _Val) noexcept( std::is_nothrow_constructible_v<_Ty, _Vy&&> )
        {
            _Value = std::forward<_Vy>(_Val);
        }

        _Ty get_value() const noexcept { return _Value; }
    private:
        _Ty _Value;
    };

    template<>
    struct promise<void> final : promise_base
    {
        auto get_return_object()
        {
            return std::coroutine_handle<promise>::from_promise(*this);
        }

        void return_void() noexcept {}
    };

    template<typename _Ty>
    struct promise<_Ty&> final : public promise_base
    {
        auto get_return_object()
        {
            return std::coroutine_handle<promise>::from_promise(*this);
        }

        void return_value(_Ty& _Val) noexcept
        {
            _Value_ptr = std::addressof(_Val);
        }

        _Ty& get_value() const noexcept { return *_Value_ptr; }
        
    private:
        _Ty* _Value_ptr = nullptr;
    };
    
}

template<typename _Ty = void>
struct task
{
    using promise_type = details::promise<_Ty>;

    struct task_awaiter
    {
        task_awaiter(std::coroutine_handle<promise_type> _Coroutine)
            :
        _Handle(_Coroutine)
        {}

        bool await_ready() const noexcept
        {
            return !_Handle || _Handle.done();
        }

        void await_suspend(std::coroutine_handle<> _Coroutine) noexcept
        {
            _Handle.resume();
        }

        decltype(auto) await_resume()
        {
            if(!_Handle)
                throw std::runtime_error{ "bad promise" };

            return _Handle.promise().get_value();
        }
        
    friend struct task<_Ty>;
    private:
        std::coroutine_handle<promise_type> _Handle;
    };

    task(std::coroutine_handle<promise_type> _Coroutine_handle)
        :
    _Handle(_Coroutine_handle)
    {}

    task(task&& _Other) noexcept
        :
    _Handle(nullptr)
    {
        *this = std::move(_Other);
    }
    
    task& operator=(task&& _Other)  noexcept
    {
        if(std::addressof(_Other) != this)
        {
            if(_Handle) _Handle.destroy();
            _Handle = _Other._Handle;
            _Other._Handle = nullptr;
        }

        return *this;
    }

    task(task const&) = delete;
    task& operator=(task const&) = delete;

    bool is_ready() noexcept
    {
        return !_Handle || _Handle.done();
    }

    void resume()
    {
        _Handle.resume();
    }

    _Ty value() const noexcept { return _Handle.promise().get_value(); }

    auto operator co_await() const& noexcept
    {
        return task_awaiter{ _Handle };
    }
    

private:
    std::coroutine_handle<promise_type> _Handle;
};