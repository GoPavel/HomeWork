#ifndef _EASY_BIND_HEADER_
#define _EASY_BIND_HEADER_
#include <iostream>

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

template <typename F, typename A1, typename A2, typename A3>
struct bind_t
{
    bind_t(F f, A1 a1, A2 a2, A3 a3)
        : f(f)
        , g1(a1)
        , g2(a2)
        , g3(a3)
    {}

    void operator()() const
    {
        return f(g1(), g2(), g3());
    }

    template <typename B1>
    void operator()(B1 b1) const
    {
        return f(g1(b1), g2(b1), g3(b1));
    }

    template <typename B1, typename B2>
    void operator()(B1 b1, B2 b2) const
    {
        return f(g1(b1, b2), g2(b1, b2), g3(b1, b2));
    }

    template <typename B1, typename B2, typename B3>
    void operator()(B1 b1, B2 b2, B3 b3) const
    {
        return f(g1(b1, b2, b3), g2(b1, b2, b3), g3(b1, b2, b3));
    }

    F f;
    G<A1> g1;
    G<A2> g2;
    G<A3> g3;
};

template <typename F, typename... MANY_A>
bind_t<F, MANY_A...> bind(F f, MANY_A... many_a)
{
    return bind_t<F, MANY_A...>(f, many_a);
}

#endif
