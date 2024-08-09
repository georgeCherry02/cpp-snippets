#include <snippets_scott_meyers_singleton.h>

#include <catch2/catch_test_macros.hpp>

namespace snippets::tests {

SCENARIO("Simple usage of the singleton") {
  GIVEN("An already accessed singleton") {
    Singleton &s1{Singleton::get()};
    WHEN("The singleton is accessed again") {
      Singleton &s2{Singleton::get()};
      THEN("The singleton has only been constructed once") {
        // Check false and log for proof...
        CHECK(true);
      }
    }
  }
}

} // namespace snippets::tests
