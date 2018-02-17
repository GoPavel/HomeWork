template <typename T, T... value>
struct integer_sequence {};

template <typename T, typename Seq, T N>
struct append;

template <typename T, T... vs,T N>
struct append <T, integer_sequence<vs...>, N - 1> {
    typedef integer_sequence<vs..., N> type;
};

template <typename T, T size, typename a = void >
struct make_integer_sequence {
    typedef append<T, make_integer_sequence <T, size -1>::type, size>::type type;
};

template <typename T, T N>
struct make_integer_sequence<T, N, enable_if<N == 0>::type> {
    typedef integer_sequence<T> type;
};
