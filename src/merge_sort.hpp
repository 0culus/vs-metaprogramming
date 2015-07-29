#include <ostream>
#include <type_traits>
#include <utility>

// display it
template <typename T, T... Ts>
std::ostream& operator<<(std::ostream& os, std::integer_sequence<T, Ts...>) {
  using swallow = int[];
  (void)swallow { (void(os<<Ts<<' '), 0)... };
  return os;
}

// helper types
template <typename T>
using empty_list = std::integer_sequence<T>;


// partitioning of int lists
template <std::size_t Count, typename T, typename E, E... Result>
struct lower_half; // obtains the lower half of a list rounded down

template <std::size_t Count, typename T, T First, T... Ts, T... Result>
struct lower_half<Count, std::integer_sequence<T, First, Ts...>, T, Result...> : lower_half<Count-1, std::integer_sequence<T, Ts...>, T, Result..., First> { };

template <typename T, T First, T... Ts, T... Result>
struct lower_half<0, std::integer_sequence<T, First, Ts...>, T, Result...> {
  using type = std::integer_sequence<T, Result...>;
};

template <std::size_t Count, typename T, T... Result>
struct lower_half<Count, empty_list<T>, T, Result...> {
  using type = empty_list<T>;
};


template <std::size_t Count, typename T>
struct upper_half; // obtains the upper half of a list rounded up

template <std::size_t Count, typename T, T First, T... Ts>
struct upper_half<Count, std::integer_sequence<T, First, Ts...>> : upper_half<Count-1, std::integer_sequence<T, Ts...>> { };

template <typename T, T First, T... Ts>
struct upper_half<0, std::integer_sequence<T, First, Ts...>> {
  using type = std::integer_sequence<T, First, Ts...>;
};

template <std::size_t Count, typename T>
struct upper_half<Count, empty_list<T>> {
  using type = empty_list<T>;
};


// helper types + functions
template <typename T, T... Ts>
using make_upper_half = typename upper_half<sizeof...(Ts)/2, std::integer_sequence<T, Ts...>>::type;

template <typename T, T... Ts>
using make_lower_half = typename lower_half<sizeof...(Ts)/2, std::integer_sequence<T, Ts...>, T>::type;

template <typename T, T... Ts>
make_upper_half<T, Ts...> make_upper_half_from_list(std::integer_sequence<T, Ts...>) {
  return {};
}

template <typename T, T... Ts>
make_lower_half<T, Ts...> make_lower_half_from_list(std::integer_sequence<T, Ts...>) {
  return {};
}


// sorting
template <typename T, T First, T Second>
using int_pair = std::integer_sequence<T, First, Second>;

template <typename T>
struct less_than;

template <typename T, T Lhs, T Rhs>
struct less_than<int_pair<T, Lhs, Rhs>> {
    static const T result = Lhs < Rhs? Lhs: Rhs;
    static const bool compare_result = Lhs < Rhs;
};

template <typename T>
struct greater_than;

template <typename T, T Lhs, T Rhs>
struct greater_than<int_pair<T, Lhs, Rhs>> {
    static const T result = Lhs > Rhs? Lhs: Rhs;
    static const bool compare_result = Lhs > Rhs;
};

template <typename L1, typename L2, typename T, template <typename> class Comp, T... Result>
struct merge_lists; // merges two lists and emits a resulting list

template <typename T, T First, T... L1, T Second, T... L2, template <typename> class Comp, T... Result>
struct merge_lists<std::integer_sequence<T, First, L1...>, std::integer_sequence<T, Second, L2...>, T, Comp, Result...> : merge_lists<
  typename std::conditional<Comp<int_pair<T, First, Second>>::compare_result, std::integer_sequence<T, L1...>, std::integer_sequence<T, First, L1...>>::type,
  typename std::conditional<!Comp<int_pair<T, First, Second>>::compare_result, std::integer_sequence<T, L2...>, std::integer_sequence<T, Second, L2...>>::type,
  T,
  Comp,
  Result..., Comp<int_pair<T, First, Second>>::result> { };

template <typename T, T... L1, template <typename> class Comp, T... Result>
struct merge_lists<std::integer_sequence<T, L1...>, empty_list<T>, T, Comp, Result...> : merge_lists<empty_list<T>, std::integer_sequence<T, L1...>, T, Comp, Result...> { };
 
template <typename T, T... L2, template <typename> class Comp, T... Result>
struct merge_lists<empty_list<T>, std::integer_sequence<T, L2...>, T, Comp, Result...> {
  using type = std::integer_sequence<T, Result..., L2...>;
};


template <typename List, template <typename> class Comp>
struct merge_sort; // merge sort algorithm

template <typename T, T... Ts, template <typename> class Comp>
struct merge_sort<std::integer_sequence<T, Ts...>, Comp> {
  using lower = make_lower_half<T, Ts...>;
  using upper = make_upper_half<T, Ts...>;
  using type  = typename merge_lists<typename merge_sort<lower, Comp>::type, typename merge_sort<upper, Comp>::type, T, Comp>::type;
};

template <typename T, T Elm, template <typename> class Comp>
struct merge_sort<std::integer_sequence<T, Elm>, Comp> {
  using type = std::integer_sequence<T, Elm>;
};

// struct tags for sorting
struct ascending { };
struct descending { };

namespace detail {

template <typename T, T... Ts>
typename merge_sort<std::integer_sequence<T, Ts...>, less_than>::type sort_impl(std::integer_sequence<T, Ts...>, ascending);

template <typename T, T... Ts>
typename merge_sort<std::integer_sequence<T, Ts...>, greater_than>::type sort_impl(std::integer_sequence<T, Ts...>, descending);

}

// dispatch function
template <typename T, T... Ts, typename Comp = ascending>
decltype(detail::sort_impl(std::integer_sequence<T, Ts...>{}, Comp{})) compile_sort(std::integer_sequence<T, Ts...>, Comp = {}) {
    return {};
}
