#ifndef _EASY_BIND_HEADER_
#define _EASY_BIND_HEADER_

template <int N>
struct placeholder { };

constexpr placeholder<1> _1;
constexpr placeholder<2> _2;
constexpr placeholder<3> _3;

//TODO G_i()
template <typename A>
struct G {
    G(A a): a(a);
};

template <>
struct G<placeholder<1>> {

    G(placeholder<1>) {}

    template<typename B1, typename B2, typename B3>
    B1 operator()(B1 b1, B2 b2, B3 b3) const { // choose first arg
        return b1;
    }

    template<typename B1, typename B2>
    B1 operator()(B1 b1, B2 b2) const { // choose first arg
        return b1;
    }

    template<typename B1>
    B1 operator()(B1 b1) const { // choose first arg
        return b1;
    }
};

template <>
struct G<placeholder<2>> {

    G(placeholder<2>) {}

    template<typename B1, typename B2, typename B3>
    B2 operator()(B1 b1, B2 b2, B3 b3) const { // choose second arg
        return b2;
    }

    template<typename B1, typename B2>
    B2 operator()(B1 b1, B2 b2) const { // choose second arg
        return b2;
    }
};
template <>
struct G<placeholder<3>> {
    G(placeholder<3>) {}

    template<typename B1, typename B2, typename B3>
    B3 operator()(B1 b1, B2 b2, B3 b3) const { // choose third arg
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

#endif
