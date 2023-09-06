add_library(
    cpp_snippets
    STATIC
    multi_invoke_visitor.cpp
)

target_include_directories(
    cpp_snippets
    PUBLIC
    .
)

