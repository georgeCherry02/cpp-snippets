find_package(Catch2 3 REQUIRED)

add_executable(
    cpp_snippet_tests
    abstract_base_class.t.cpp
    multi_invoke_visitor.t.cpp
    pointer_to_member.t.cpp
    scope_guard.t.cpp
    scott_meyers_singleton.t.cpp
)

target_link_libraries(
    cpp_snippet_tests
    PRIVATE
    Catch2::Catch2WithMain
    cpp_snippets
)

add_test(
    NAME cpp_snippet_tests
    COMMAND $<TARGET_FILE:cpp_snippet_tests> -r console::out=-::colour-mode=ansi -r JUnit::out=cpp_snippet.unit.tests.xunit.xml
)
