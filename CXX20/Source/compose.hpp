#pragma once

namespace compose
{

    template<typename F, typename G>
    auto compose(F&& f, G&& g)
    {
        return [=](auto x){return f(g(x));};
    }
    
    template<typename F, typename... Gs>
    auto compose(F&& f, Gs&&... gs)
    {
        return [=](auto x) {return f(compose(gs...)(x));};
    }

    template<typename F, typename G>
    auto operator*(F&& f, G&& g)
    {
        return compose(std::forward<F>(f), std::forward<G>(g));
    }

    template<typename F, typename... Gs>
    auto operator*(F&& f, Gs&&... gs)
    {
        return operator*(std::forward<F>(f), gs...);
    }
}