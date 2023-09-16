#include <catch2/catch_test_macros.hpp>

#include <multi_invoke_visitor.h>

struct TestStruct
{
    int value;
};

SCENARIO("Basic usage of the visitor")
{
    GIVEN("A dispatcher with 3 unique signatures")
    {
        bool one_called, two_called, three_called;
        auto dispatch = snippets::make_dispatcher(
            [&one_called](std::string_view s) {
                one_called = true;
                CHECK(s == "Test string view");
            },
            [&two_called](std::string_view s, int i) {
                two_called = true;
                CHECK(s == "Test string view");
                CHECK(i == 5);
            },
            [&three_called](std::string_view s, int i, TestStruct ts) {
                three_called = true;
                CHECK(s == "Test string view");
                CHECK(i == 5);
                CHECK(ts.value == 5);
            });
        WHEN("The dispatcher is called on three occasions with these different signatures")
        {
            dispatch("Test string view");
            dispatch("Test string view", 5);
            dispatch("Test string view", 5, TestStruct{5});
            THEN("Then the outside state flags have been modified")
            {
                CHECK(one_called);
                CHECK(two_called);
                CHECK(three_called);
            }
        }
    }
}
