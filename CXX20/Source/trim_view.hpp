#pragma once

#include <ranges>

namespace rg = std::ranges;
namespace rv = std::ranges::views;

template<rg::input_range _Rng, typename _Pr> requires rg::view<_Rng>
class trim_view : public rg::view_interface<trim_view<_Rng, _Pr>>
{
    _Rng _Base;
    _Pr _Pred;

    mutable rg::iterator_t<_Rng> _Begin{ std::begin(_Base) };
    mutable rg::iterator_t<_Rng> _End{ std::end(_Base) };
    mutable bool _Evaluated{ false };

    void _Evaluate_base() const
    {
        if(!_Evaluated)
        {
            while(_Begin != std::end(_Base) && _Pred(*_Begin)) _Begin = std::next(_Begin);
            while(_End != _Begin && _Pred(*std::prev(_End))) _End = std::prev(_End);
            _Evaluated = true;
        }
    }

public:
    trim_view() = default;

    trim_view(_Rng _Base, _Pr _Pred)
        :
    _Base(std::move(_Base)),
    _Pred(std::move(_Pred)),
    _Begin(std::begin(_Base)),
    _End(std::end(_Base))
    {}

    constexpr _Rng base() const & { return _Base; }
    constexpr _Rng base() && { return std::move(_Base); }

    constexpr rg::iterator_t<_Rng> begin() const { _Evaluate_base(); return _Begin; }
    constexpr rg::iterator_t<_Rng> end() const { _Evaluate_base(); return _End; }

    constexpr auto size() requires rg::sized_range<_Rng>{ return std::distance(_Begin, _End); }
    constexpr auto size() const requires rg::sized_range<_Rng>{ return std::distance(_Begin, _End); }
};

template<typename _Rng, typename _Pr>
trim_view(_Rng&& _Base, _Pr _Pred) -> trim_view<rg::views::all_t<_Rng>, _Pr>;

namespace details
{
    template<typename _Pr>
    struct trim_view_adaptor_closure
    {
        _Pr _Pred;

        constexpr trim_view_adaptor_closure(_Pr _Pred)
            :
        _Pred(_Pred)
        {}

        template<rg::viewable_range _VRng>
        constexpr auto operator()(_VRng&& _Range) const
        {
            return trim_view(std::forward<_VRng>(_Range), _Pred);
        }
    };

    struct trim_view_adaptor
    {
        template<rg::viewable_range _VRng, typename _Pr>
        constexpr auto operator()(_VRng&& _Range, _Pr _Pred)
        {
            return trim_view(std::forward<_VRng>(_Range), _Pred);
        }

        template<typename _Pr>
        constexpr auto operator()(_Pr _Pred)
        {
            return trim_view_adaptor_closure(_Pred);
        }
    };

    template<rg::viewable_range _VRng, typename _Pr>
    constexpr auto operator | (
        _VRng&& _Range,
        trim_view_adaptor_closure<_Pr> const& _Adaptor
    )
    {
        return _Adaptor(std::forward<_VRng>(_Range));
    }
}

namespace views
{
    inline static details::trim_view_adaptor trim;
}
