#pragma once

#include <concepts>
#include <string_view>
#include <optional>
#include <string>

#include <learner/table.hpp>

namespace learner {

template <typename L>
concept Language =
requires(L l, std::string_view s, const Table& t) {
    { l.Contains(s) } -> std::same_as<bool>;
    { l.Equivalent(t) } -> std::same_as<std::optional<std::string>>;
};

}  // namespace learner
