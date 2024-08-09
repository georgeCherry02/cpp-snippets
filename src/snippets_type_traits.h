#ifndef INCLUDED_SNIPPETS_TYPE_TRAITS_H
#define INCLUDED_SNIPPETS_TYPE_TRAITS_H

#include <type_traits>
#include <variant>

namespace snippets {

template <typename, typename> struct is_same_template : std::false_type {};

template <template <typename...> class T, typename... A, typename... B>
struct is_same_template<T<A...>, T<B...>> : std::true_type {};

template <typename T> struct is_variant : std::false_type {};

template <typename... V>
struct is_variant<std::variant<V...>> : std::true_type {};

template <typename T> constexpr bool is_variant_v = is_variant<T>::value;

} // namespace snippets

#endif
