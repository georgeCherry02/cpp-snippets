#ifndef INCLUDED_SNIPPETS_ABSTRACT_BASE_CLASS_H
#define INCLUDED_SNIPPETS_ABSTRACT_BASE_CLASS_H

#include <memory>
#include <string>
#include <utility>

namespace snippets {

struct AbstractBase {
  virtual void write(const std::string &data) = 0;
  virtual std::string read() = 0;
  virtual ~AbstractBase() = default;
};

template <typename IMPL> struct ContractEnforcer : public AbstractBase {
private:
  IMPL dispatch;

public:
  explicit ContractEnforcer(IMPL &&dispatch) : dispatch{std::move(dispatch)} {};
  ~ContractEnforcer() override = default;

  void write(const std::string &data) override { dispatch.write(data); }
  std::string read() override { return dispatch.read(); }
};

// Masks the pimpl to a . interface
struct Connection {
private:
  std::shared_ptr<AbstractBase> impl;

public:
  Connection(const Connection &) = delete;
  Connection &operator=(const Connection &) = delete;

  Connection(Connection &&) = default;
  Connection &operator=(Connection &&) = default;
  ~Connection() = default;

  template <typename IMPL>
  explicit Connection(IMPL &&impl)
      : impl{std::make_shared<ContractEnforcer<std::decay_t<IMPL>>>(
            std::forward<IMPL>(impl))} {}

  void write(const std::string &data);
  std::string read();
};

} // namespace snippets

#endif
