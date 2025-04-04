#pragma once

#include <cstdint>

namespace learner {

auto BuildDfaTable(std::int32_t height, std::int32_t width, std::int64_t seed) -> bool;

}  // namespace learner

