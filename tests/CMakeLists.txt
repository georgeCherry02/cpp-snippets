find_package(Catch2 3 REQUIRED)

add_executable(
    cpp_snippet_tests
    multi_invoke_visitor.t.cpp
)

target_link_libraries(
    cpp_snippet_tests
    PRIVATE
    Catch2::Catch2WithMain
    cpp_snippets
)

add_test(
    NAME cpp_snippet_tests
    COMMAND $<TARGET_FILE:cpp_snippet_tests> -r -console>
)
