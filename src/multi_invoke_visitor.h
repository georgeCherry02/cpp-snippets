#ifndef INCLUDED_MULTI_INVOKE_VISITOR_H
#define INCLUDED_MULTI_INVOKE_VISITOR_H

#include <tuple>
#include <utility>
#include <variant>

namespace snippets
{

template <typename T>
struct is_variant : std::false_type
{
};

template <typename... V>
struct is_variant<std::variant<V...>> : std::true_type
{
};

template <typename T>
constexpr bool is_variant_v = is_variant<T>::value;

template <typename... HANDLERS>
class MultiInvokeVisitor
{
  private:
    std::tuple<HANDLERS...> d_handlers;

    // Base case
    template <typename CURRIED_FUNC>
    constexpr void curry(CURRIED_FUNC&& func)
    {
        std::forward<CURRIED_FUNC>(func)();
    };

    template <typename FUNC, typename FIRST, typename... REST>
    constexpr void curry(FUNC&& func, FIRST&& first, REST&&... rest)
    {
        if constexpr (is_variant_v<std::decay_t<FIRST>>)
        {
            // Visit the variant to unfold
            return expand_variant(std::forward<FUNC>(func), std::forward<FIRST>(first), std::forward<REST>(rest)...);
        }

        // Curry the func with the first argument and then use tail recursion
        // to move on to the next argument
        curry(
            [&first, &func](auto&&... rest) {
                // Note the type of `rest` here is different to the `REST` in the encapsulating function
                // Hence the constexpr invoke check needs to be here
                if constexpr (std::is_invocable_v<FUNC, FIRST, decltype(rest)...>)
                {
                    func(std::forward<FIRST>(first), std::forward<REST>(rest)...);
                }
            },
            std::forward<REST>(rest)...);
    }

    template <typename FUNC, typename VARIANT, typename... REST>
    constexpr void expand_variant(FUNC&& func, VARIANT&& variant, REST&&... rest)
    {
        // We can use a lambda accepting auto&& as regardless of the alternative
        // contained within the variant we want to call the curring function with
        // it
        std::visit(
            [&func, rest_tup = std::forward_as_tuple(std::forward<REST>(rest)...), this](auto&& alt) {
                // The `std::apply` is necessary because you can't capture parameter packs until C++ 23 therefore you
                // have to forward as tuple and expand...
                std::apply(
                    [&func, &alt, this](auto&&... r) {
                        curry(
                            std::forward<FUNC>(func),
                            std::forward<decltype(alt)>(alt),
                            std::forward<decltype(rest)>(r)...);
                    },
                    rest_tup);
            },
            std::forward<VARIANT>(variant));
    }

  public:
    explicit MultiInvokeVisitor(HANDLERS... handlers)
        : d_handlers{std::move(handlers)...} {};

    int stub();
};

template <typename... HANDLERS>
constexpr decltype(auto) make_dispatcher(HANDLERS&&... handlers)
{
    return MultiInvokeVisitor<std::decay_t<HANDLERS>...>{std::forward<HANDLERS>(handlers)...};
}

} // namespace snippets

#endif
