#pragma once

#include <concepts>
#include <string_view>

namespace learner {

template <typename L>
concept Language =
requires(L l, std::string_view s) {
    {l.Contains(s)} -> std::same_as<bool>;
};

}  // namespace learner
