#include <pointer_to_member.h>

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>

namespace snippets::test {

constexpr static char ID[] = "id";
constexpr static char NAME[] = "name";
constexpr static char AGE[] = "age";

struct Person {
  std::string name;
  int age;

  constexpr static auto schema = make_schema(define_member<NAME>(&Person::name),
                                             define_member<AGE>(&Person::age));
};

struct Employee : Person {
  size_t id;

  constexpr static auto schema = make_schema(
      define_member<ID>(&Employee::id), define_member<NAME>(&Employee::name));
};

SCENARIO("Basic usage of the streamable") {
  GIVEN("A simple streamable object and an output stream") {
    Person person{.name = std::string{"Keir Starmer"}, .age = 61};
    std::stringstream oss;
    WHEN("The streamable object is streamed") {
      oss << person;
      THEN("The output is expected") {
        CHECK(oss.str() == "{ name: Keir Starmer, age: 61 }");
      }
    }
  }
  GIVEN("An inheritance structure") {
    Employee employee{{.name = std::string{"Keir Starmer"}, .age = 61}, 1};
    std::stringstream oss;
    WHEN("The streamable object is streamed") {
      oss << employee;
      THEN("The output is expected") {
        CHECK(oss.str() == "{ id: 1, name: Keir Starmer }");
      }
    }
  }
}

} // namespace snippets::test
