#ifndef WSUDO_ENV_HPP
#define WSUDO_ENV_HPP
#include <bela/base.hpp>
#include <bela/phmap.hpp>
#include <bela/ascii.hpp>
#include <bela/env.hpp>
#include <unordered_map>
#include <functional>

namespace wsudo {

class EnvDerivator {
public:
  using conatiner_t = phmap::flat_hash_map<std::wstring, std::wstring>;
  using apply_t = std::function<void(std::wstring_view, std::wstring_view)>;
  EnvDerivator() = default;
  EnvDerivator(const EnvDerivator &) = delete;
  EnvDerivator &operator=(const EnvDerivator &) = delete;
  bool Append(std::wstring_view s) {
    auto pos = s.find(L'=');
    if (pos == std::wstring_view::npos) {
      em.insert_or_assign(bela::AsciiStrToUpper(s), std::wstring());
      return true;
    }
    auto k = s.substr(0, pos);
    if (k.empty()) {
      return false;
    }
    auto v = s.substr(pos + 1);
    em.insert_or_assign(bela::AsciiStrToUpper(k), bela::ExpandEnv(v));
    return true;
  }
  // Normal
  bool Appendable(std::wstring_view ws) {
    auto pos = ws.find(L'=');
    if (pos == std::wstring_view::npos || pos == 0) {
      return false;
    }
    auto k = ws.substr(0, pos);
    auto v = ws.substr(pos + 1);
    em.insert_or_assign(bela::AsciiStrToUpper(k), bela::ExpandEnv(v));
    return true;
  }
  bool Apply(apply_t fn) const {
    if (em.empty()) {
      return true;
    }
    for (const auto &e : em) {
      fn(e.first, e.second); /// callback
      SetEnvironmentVariableW(e.first.data(),
                              e.second.empty() ? nullptr : e.second.data());
    }
    return true;
  }
  const conatiner_t &Items() const { return em; }

private:
  conatiner_t em;
};
} // namespace wsudo

#endif
