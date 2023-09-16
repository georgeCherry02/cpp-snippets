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

class MultiInvokeVisitor
{
  public:
    int stub();
};

} // namespace snippets

#endif
