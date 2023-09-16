#ifndef INCLUDED_MULTI_INVOKE_VISITOR_H
#define INCLUDED_MULTI_INVOKE_VISITOR_H

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

template <typename... Handlers>
class MultiInvokeVisitor
{
  private:
    std::tuple<Handlers...> d_handlers;

  public:
    explicit MultiInvokeVisitor(Handlers... handlers)
        : d_handlers{std::move(handlers)...} {};

    int stub();
};

template <typename... Handlers>
constexpr decltype(auto) make_dispatcher(Handlers&&... handlers)
{
    return MultiInvokeVisitor<std::decay_t<Handlers>...>{std::forward<Handlers>(handlers)...};
}

} // namespace snippets

#endif
