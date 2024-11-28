#pragma once

#include <cstdint>
#include <utility>
#include <span>

namespace learner {

using Position = std::pair<std::int32_t, std::int32_t>;
using Direction = char;

auto LeftOf(char direction) -> char;

auto RightOf(char direction) -> char;

auto SideSizeByExitDirection(char exit_direction, std::int32_t height, std::int32_t width) -> std::int32_t;

auto MakeMove(Position pos,
              Direction move) -> Position;

auto MakeMoves(Position pos, const std::span<const Direction> moves) -> Position;

}  // namespace learner
