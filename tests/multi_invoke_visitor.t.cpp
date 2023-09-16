#include <catch2/catch_test_macros.hpp>

#include <multi_invoke_visitor.h>
#include <variant>

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
    GIVEN("A variant and a dispatcher to handle said variant")
    {
        using TestVariant = std::variant<int, std::string>;
        TestVariant test_int{int{0}};
        TestVariant test_string{std::string{"Hi"}};
        bool        int_called, string_called;
        auto        dispatcher = snippets::make_dispatcher(
            [&int_called](int) { int_called = true; }, [&string_called](std::string) { string_called = true; });
        WHEN("The dispatcher is called with each variant")
        {
            dispatcher(test_int);
            dispatcher(test_string);
            THEN("The variant is appropriately unpacked, and the flag is modified")
            {
                CHECK(int_called);
                CHECK(string_called);
            }
        }
    }
    GIVEN("A variant and a dispatcher with functions allowing implicit cast of variant members")
    {
        using TestVariant = std::variant<int, double>;
        TestVariant test_int{int{0}};
        CHECK(std::holds_alternative<int>(test_int));
        bool        int_called, double_called;
        auto        dispatcher = snippets::make_dispatcher(
            [&int_called](int) { int_called = true; }, [&double_called](double) { double_called = true; });
        WHEN("The dispatcher is called with the variant")
        {
            dispatcher(test_int);
            THEN("The variant is appropriately unpacked, and the multiple flags are modified")
            {
                CHECK(int_called);
                CHECK(double_called);
            }
        }
    }
}
