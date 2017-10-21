#include <iostream>
#include <tuple>

template <int N>
struct placeholder
{};

constexpr placeholder<1> _1;
constexpr placeholder<2> _2;
constexpr placeholder<3> _3;

template <typename A>
struct G
{
    G(A a)
        : a(a)
    {}

    template <typename ... Bs>
    A operator()(Bs ...) const
    {
        return a;
    }

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
struct G<placeholder<N> >
{
    G(placeholder<N>)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator()(B, Bs ... bs) const
    {
        G<placeholder<N - 1>> next((placeholder<N - 1>()));
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
        return f(std::get<0>(gs)(bs...),
                 std::get<1>(gs)(bs...),
                 std::get<2>(gs)(bs...));
    }

    F f;
    std::tuple<G<As>...> gs;
};

template <typename F, typename ... As>
bind_t<F, As...> bind(F f, As ... as)
{
    return bind_t<F, As...>(f, as...);
}


void f(int a, int b, int c)
{
    std::cout << a << b << c << std::endl;
}

int main()
{
    bind(&f, 1, 2, 3)();
    bind(&f, _1, _3, _2)(4, 6, 5);
    bind(&f, _1, _1, _2)(7, 8);
    return 0;
}
