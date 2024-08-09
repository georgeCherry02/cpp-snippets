#ifndef INCLUDED_SNIPPETS_TYPE_TRAITS_H
#define INCLUDED_SNIPPETS_TYPE_TRAITS_H

#include <type_traits>

namespace snippets {

template <typename, typename> struct is_same_template : std::false_type {};

template <template <typename...> class T, typename... A, typename... B>
struct is_same_template<T<A...>, T<B...>> : std::true_type {};

}

#endif
