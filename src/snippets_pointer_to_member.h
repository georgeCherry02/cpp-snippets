#ifndef INCLUDED_SNIPPETS_POINTER_TO_MEMBER_H
#define INCLUDED_SNIPPETS_POINTER_TO_MEMBER_H

#include <snippets_type_traits.h>

#include <iostream>
#include <tuple>

namespace snippets {

template <const char *NAME, typename BOUND_OBJECT, typename MEMBER_TYPE>
struct StreamableMember {
  const char *name = NAME;
  MEMBER_TYPE BOUND_OBJECT::*member;
  constexpr StreamableMember(MEMBER_TYPE BOUND_OBJECT::*member)
      : member{member} {}
};

template <const char *NAME, typename BOUND_OBJECT, typename MEMBER_TYPE>
constexpr StreamableMember<NAME, BOUND_OBJECT, MEMBER_TYPE>
define_member(MEMBER_TYPE BOUND_OBJECT::*member) {
  return StreamableMember<NAME, BOUND_OBJECT, MEMBER_TYPE>{member};
}

template <typename... STREAMABLES> struct StreamSchema {
  std::tuple<STREAMABLES...> streamables;
  constexpr static auto STREAM_SIZE = sizeof...(STREAMABLES);

  explicit constexpr StreamSchema(STREAMABLES &&...streamables)
      : streamables{std::forward<STREAMABLES>(streamables)...} {}
};

template <typename... STREAMABLES>
constexpr auto make_schema(STREAMABLES &&...streamables) {
  return StreamSchema{std::forward<STREAMABLES>(streamables)...};
}

template <typename STREAMABLE>
concept IsStreamable =
    is_same_template<std::decay_t<decltype(STREAMABLE::schema)>,
                     StreamSchema<>>::value;

template <IsStreamable STREAMABLE, typename MEMBER>
void stream_member_impl(std::ostream &os, const STREAMABLE &streamable,
                        const MEMBER &member_def, bool is_not_last) {
  os << member_def.name << ": " << streamable.*(member_def.member);
  if (is_not_last) {
    os << ",";
  }
  os << " ";
};

template <IsStreamable STREAMABLE, typename IntType, IntType... Idx>
void stream_impl(std::ostream &os, const STREAMABLE &streamable,
                 std::integer_sequence<IntType, Idx...>) {
  (..., stream_member_impl(os, streamable,
                           std::get<Idx>(streamable.schema.streamables),
                           (Idx < streamable.schema.STREAM_SIZE - 1)));
}

template <IsStreamable STREAMABLE>
std::ostream &operator<<(std::ostream &os, const STREAMABLE &streamable) {
  os << "{ ";
  stream_impl(os, streamable,
              std::make_index_sequence<streamable.schema.STREAM_SIZE>());
  os << "}";
  return os;
}

} // namespace snippets

#endif
