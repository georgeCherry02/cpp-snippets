/* MultiInvokeVisitor
 *
 * This is a kinda dumb visitor that's going to call any signature you give it
 * if it can. The fact it will implicitly cast and call multiple functions it's
 * "visiting" if it's possible is core to it's purpose - typically a normal
 * visitor's going to be sufficient, so it's redundant in most scenarios.
 *
 * Note the currying approach was used to get around the fact that a normal
 * std::visit requires all arguments to be variants - a more hacky approach to
 * this but one that leads to simpler code is to just wrap anything that isn't
 * a variant into a variant of one variety. This can be done quite easily with
 * the is_variant type trait below.
 */
#ifndef INCLUDED_SNIPPETS_MULTI_INVOKE_VISITOR_H
#define INCLUDED_SNIPPETS_MULTI_INVOKE_VISITOR_H

#include <snippets_type_traits.h>

#include <tuple>
#include <utility>
#include <variant>

namespace snippets
{

template <typename... HANDLERS>
class MultiInvokeVisitor
{
  private:
    std::tuple<HANDLERS...> d_handlers;

    // Base case
    template <typename CURRIED_FUNC>
    constexpr void curry(CURRIED_FUNC&& func) const
    {
        std::forward<CURRIED_FUNC>(func)();
    }

    template <typename FUNC, typename FIRST, typename... REST>
    constexpr void curry(FUNC&& func, FIRST&& first, REST&&... rest) const
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
                    func(std::forward<FIRST>(first), std::forward<decltype(rest)>(rest)...);
                }
            },
            std::forward<REST>(rest)...);
    }

    template <typename FUNC, typename VARIANT, typename... REST>
    constexpr void expand_variant(FUNC&& func, VARIANT&& variant, REST&&... rest) const
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

    template <typename IntType, IntType... Idxs, typename... ARGS>
    constexpr void apply_helper(std::integer_sequence<IntType, Idxs...>, ARGS&&... args) const
    {
        // The fold expression allows us to call our currying logic with all the
        // handlers this dispatcher has been instantiated with
        // We don't forward the ARGS here as it could lead to UB with moves
        // within a fold expression (that of the handlers)
        (curry(std::get<Idxs>(d_handlers), args...), ...);
    }

  public:
    explicit MultiInvokeVisitor(HANDLERS... handlers)
        : d_handlers{std::move(handlers)...} {};

    template <typename... ARGS>
    constexpr void operator()(ARGS&&... args) const
    {
        apply_helper(std::make_index_sequence<sizeof...(HANDLERS)>(), std::forward<ARGS>(args)...);
    }
};

template <typename... HANDLERS>
constexpr decltype(auto) make_dispatcher(HANDLERS&&... handlers)
{
    return MultiInvokeVisitor<std::decay_t<HANDLERS>...>{std::forward<HANDLERS>(handlers)...};
}

} // namespace snippets

#endif
