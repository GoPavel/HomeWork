#ifndef _EASY_BIND_HEADER_
#define _EASY_BIND_HEADER_
#include <iostream>
#include <tuple>
#includ "integer_sequence.h"

template <int N>
struct placeholder { };

constexpr placeholder<1> _1;
constexpr placeholder<2> _2;
constexpr placeholder<3> _3;

//TODO G_i()
template <typename A>
struct G {
    G(A a): a(a);

    template <typename ... Bs>
    A operator()(Bs ...) const
    {
        return a;
    }

private:
    A a;
};

template <>
struct G<placeholder<1> >
{
    G(placeholder<1>)
    {}

    template <typename B1, typename ... Bs>
    B1 operator()(B1 b1, Bs ...) const
    {
        return b1;
    }
};


template <int N>
struct G<placeholder<N>> {
    G (placeholder<N>) {};

    template <typename B, typename _Bs>
    decltype(auto) operator() (B b, _Bs... bs) const{
        G<placeholder<N - 1>> next((placeholder<N - 1>()))
        return next(bs...);
    }
};

template <typename F, typename ... As>
struct bind_t
{
    bind_t(F f, As ... as)
        : f(f)
        , gs(as...)
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs ... bs) const
    {
        return call(make_integer_sequence<int, sizeof... (As), bs...>)
    }

private:

    template <int... ks, typename... Bs>
    decltype(auto) call(integer_sequence<int, ks...>, Bs... bs) const {
        return f(std::get<ks>(gs)(bs...)...);
    }


    F f;
    std::tuple<G<As>...> gs;
};

template <typename F, typename... MANY_A>
bind_t<F, MANY_A...> bind(F f, MANY_A... many_a)
{
    return bind_t<F, MANY_A...>(f, many_a);
}

#endif
