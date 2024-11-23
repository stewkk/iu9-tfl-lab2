#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace learner {

class Labirinth {
public:
  using Position = std::pair<std::int32_t, std::int32_t>;
  using Direction = char;

  Labirinth(std::int32_t height, std::int32_t width);

  auto AddWall(Position from, Direction to) -> void;
  auto IsWall(Position from, Direction to) -> std::optional<bool>;
  auto GetWalls() -> std::vector<std::pair<Position, Direction>>;

private:
  using CellWalls = std::array<std::optional<bool>, 2>;
  std::vector<std::vector<CellWalls>> walls_;
};

}  // namespace learner
