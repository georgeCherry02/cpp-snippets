#include <snippets_abstract_base_class.h>

#include <catch2/catch_test_macros.hpp>

#include <string>

namespace snippets::test {

struct InMemoryConnection {
  std::string store;

  void write(const std::string &data) { store = data; }
  std::string read() { return store; }
};

constexpr auto MOCK_DATA = "MOCK";
struct MockConnection {
  void write(const std::string &data){};
  std::string read() { return MOCK_DATA; }
};

SCENARIO("Basic usage of the connection ABC") {
  GIVEN("A mock in memory connection") {
    std::string init_data{"INIT"};
    Connection conn{InMemoryConnection{.store = init_data}};
    WHEN("The initialised memory is read from") {
      std::string read = conn.read();
      THEN("The default construction is read") { CHECK(read == init_data); }
    }
    WHEN("The in memory connection is written to and there's a sequential "
         "read") {
      std::string new_data{"NEW"};
      conn.write(new_data);
      std::string read = conn.read();
      THEN("The connection holds the expected data") {
        CHECK(read == new_data);
      }
    }
  }
  GIVEN("A fully mocked connection") {
    Connection conn{MockConnection{}};
    WHEN("The mock memory is read from") {
      std::string read = conn.read();
      THEN("The read holds the mock data") { CHECK(read == MOCK_DATA); }
    }
  }
}

} // namespace snippets::test
