add_library(
    cpp_snippets
    STATIC
    snippets_multi_invoke_visitor.cpp
    snippets_pointer_to_member.cpp
    snippets_type_traits.cpp
)

target_include_directories(
    cpp_snippets
    PUBLIC
    .
)

target_compile_options(
    cpp_snippets
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)
