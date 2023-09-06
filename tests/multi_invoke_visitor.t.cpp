#include <catch2/catch_test_macros.hpp>

#include <multi_invoke_visitor.h>

SCENARIO("Example Scenario")
{
    GIVEN("Example Given")
    {
        WHEN("Example When")
        {
            THEN("Example Then")
            {
                snippets::MultiInvokeVisitor v{};
                CHECK(v.stub() == 5);
            }
        }
    }
}
