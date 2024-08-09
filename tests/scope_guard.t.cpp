#include <snippets_scope_guard.h>

#include <catch2/catch_test_macros.hpp>

#include <string>

namespace snippets::tests {

using StringGuard = ScopeGuard<std::string>;

StringGuard sg(std::string{"Base"});

StringGuard::AccessType no_override_access() { return StringGuard::get(); }
StringGuard::AccessType overriden_access() {
  StringGuard sg_lower{"Override"};
  return no_override_access();
}
void override_at_lower_scope_and_unwind() { StringGuard sg_lowest{"Lowest"}; }
StringGuard::AccessType override_lower() {
  override_at_lower_scope_and_unwind();
  return StringGuard::get();
}
void double_unwind() {
    StringGuard{"Temp"}; // NOLINT
    override_at_lower_scope_and_unwind();
}

SCENARIO("Expected usage of scope guard") {
  GIVEN("A populated guard in main") {
    WHEN("This guard is accessed") {
      auto read = no_override_access();
      THEN("This read holds what was populated on main") {
        REQUIRE(read);
        CHECK(*read == "Base");
      }
    }
    WHEN("This guard is overridden at a lower scope, and is accessed") {
      auto read = overriden_access();
      THEN("This read holds the override") {
        REQUIRE(read);
        CHECK(*read == "Override");
      }
    }
    WHEN("This guard is overridden at a multiple scopes, and is accessed") {
      StringGuard sg_middle{"Middle"};
      auto read = overriden_access();
      THEN("This read holds the lowest level override") {
        REQUIRE(read);
        CHECK(*read == "Override");
      }
    }
    WHEN("This guard is overriden at a lower scope and unwound, and is "
         "accesed") {
      StringGuard sg_middle{"Middle"};
      auto read = override_lower();
      THEN("This read holds what was just populate main") {
        REQUIRE(read);
        CHECK(*read == "Middle");
      }
    }
    THEN("This guard is override twice and unwound, is not null") {
      StringGuard sg_middle{"Middle"};
      double_unwind();
      auto read = StringGuard::get();
      THEN("This read holds what was just populated") {
        REQUIRE(read);
        CHECK(*read == "Middle");
      }
    }
  }
}

} // namespace snippets::tests
