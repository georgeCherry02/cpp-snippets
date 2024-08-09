#include <snippets_abstract_base_class.h>

namespace snippets {

void Connection::write(const std::string &data) { impl->write(data); }
std::string Connection::read() { return impl->read(); }

} // namespace snippets
