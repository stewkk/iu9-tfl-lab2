#pragma once

#include <cstdint>

namespace learner {

auto LeftOf(char direction) -> char;

auto RightOf(char direction) -> char;

auto SideSizeByExitDirection(char exit_direction, std::int32_t height, std::int32_t width) -> std::int32_t;

}  // namespace learner
