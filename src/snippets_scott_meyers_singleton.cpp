#include <snippets_scott_meyers_singleton.h>

#include <iostream>

namespace snippets {

Singleton::Singleton() { std::cout << "Constructor called" << std::endl; };

Singleton &Singleton::get() {
  std::cout << "Get called" << std::endl;
  static Singleton s = Singleton(); // MVP
  return s;
};

} // namespace snippets
