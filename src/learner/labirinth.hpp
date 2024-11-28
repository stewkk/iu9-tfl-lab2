#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include <learner/direction.hpp>

namespace learner {

struct Exit {
  Position pos;
  Direction direction;

  auto operator==(const Exit& other) const -> bool = default;
};

class Labirinth {
public:
  Labirinth(std::int32_t height, std::int32_t width);

  auto GetHeight() const -> std::int32_t;
  auto GetWidth() const -> std::int32_t;
  auto AddWall(Position from, Direction to) -> void;
  auto IsWall(Position from, Direction to) const -> std::optional<bool>;
  auto AddPath(Position from, Direction to) -> void;
  auto GetWalls() const -> std::vector<std::pair<Position, Direction>>;
  auto GetExit(Direction exit_direction, std::int32_t steps_to_lhs_border) const -> Exit;

private:
  auto get_wall(Position from, Direction to) -> std::optional<bool>&;
  auto get_wall(Position from, Direction to) const -> std::optional<bool>;

private:
  using CellWalls = std::array<std::optional<bool>, 2>;
  std::vector<std::vector<CellWalls>> walls_;
};

}  // namespace learner
