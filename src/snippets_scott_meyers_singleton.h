#ifndef SNIPPETS_SCOTT_MEYES_SINGLETON_H
#define SNIPPETS_SCOTT_MEYES_SINGLETON_H

namespace snippets {

struct Singleton {
public:
  static Singleton &get();

private:
  explicit Singleton();
  ~Singleton() = default;
};

} // namespace snippets

#endif
