#include <pointer_to_member.h>

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>

namespace snippets::test {

constexpr static char NAME[] = "name";
constexpr static char AGE[] = "age";

struct Person {
  std::string name;
  int age;

  constexpr static auto schema = make_schema(define_member<NAME>(&Person::name),
                                             define_member<AGE>(&Person::age));
};

SCENARIO("Basic usage of the streamable") {
  GIVEN("A simple streamable object and an output stream") {
    Person person{.name = std::string{"Keir Starmer"}, .age = 61};
    std::stringstream oss;
    WHEN("The streamable object is streamed") {
      stream(oss, person);
      THEN("The output is expected") {
        CHECK(oss.str() == "{ name: Keir Starmer, age: 61 }");
      }
    }
  }
}

} // namespace snippets::test
