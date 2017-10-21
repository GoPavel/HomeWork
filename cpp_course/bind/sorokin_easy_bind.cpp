
Skip to content

    All gists
    GitHub

    New gist

0

    0

@sorokin sorokin/bind-1.cpp
Last active 4 minutes ago •
Code
Revisions 2
bind-1.cpp
#include <iostream>

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

    A operator()() const
    {
        return a;
    }

    template <typename B1>
    A operator()(B1 b1) const
    {
        return a;
    }

    template <typename B1, typename B2>
    A operator()(B1 b1, B2 b2) const
    {
        return a;
    }

    template <typename B1, typename B2, typename B3>
    A operator()(B1 b1, B2 b2, B3 b3) const
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

    template <typename B1>
    B1 operator()(B1 b1) const
    {
        return b1;
    }

    template <typename B1, typename B2>
    B1 operator()(B1 b1, B2 b2) const
    {
        return b1;
    }

    template <typename B1, typename B2, typename B3>
    B1 operator()(B1 b1, B2 b2, B3 b3) const
    {
        return b1;
    }
};

template <>
struct G<placeholder<2> >
{
    G(placeholder<2>)
    {}

    template <typename B1, typename B2>
    B2 operator()(B1 b1, B2 b2) const
    {
        return b2;
    }

    template <typename B1, typename B2, typename B3>
    B2 operator()(B1 b1, B2 b2, B3 b3) const
    {
        return b2;
    }
};

template <>
struct G<placeholder<3> >
{
    G(placeholder<3>)
    {}

    template <typename B1, typename B2, typename B3>
    B3 operator()(B1 b1, B2 b2, B3 b3) const
    {
        return b3;
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

template <typename F, typename A1, typename A2, typename A3>
bind_t<F, A1, A2, A3> bind(F f, A1 a1, A2 a2, A3 a3)
{
    return bind_t<F, A1, A2, A3>(f, a1, a2, a3);
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
