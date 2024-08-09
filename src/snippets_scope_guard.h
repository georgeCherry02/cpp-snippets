#ifndef INCLUDED_SNIPPETS_SCOPE_GAURD_H
#define INCLUDED_SNIPPETS_SCOPE_GAURD_H

#include <memory>
#include <type_traits>

namespace snippets {

template <typename T> struct ScopeGuard {
public:
  using AccessType = std::shared_ptr<std::remove_cvref_t<T>>;

private:
  static AccessType _impl;
  static AccessType _last;

public:
  ScopeGuard(const ScopeGuard &) = delete;
  ScopeGuard &operator=(const ScopeGuard &) = delete;
  ScopeGuard(ScopeGuard &&) = default;
  ScopeGuard &operator=(ScopeGuard &&) = default;

  ScopeGuard(T &&impl) {
    _last = ScopeGuard::get();
    _impl = std::make_shared<std::remove_cvref_t<T>>(impl);
  }
  ~ScopeGuard() { _impl = _last; }
  static AccessType get() { return _impl; }
};

template <typename T>
std::shared_ptr<std::remove_cvref_t<T>> ScopeGuard<T>::_impl = nullptr;
template <typename T>
std::shared_ptr<std::remove_cvref_t<T>> ScopeGuard<T>::_last = nullptr;

} // namespace snippets

#endif
