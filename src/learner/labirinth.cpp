#include <learner/labirinth.hpp>

#include <cstdint>
#include <stdexcept>
#include <utility>

#include <learner/direction.hpp>

namespace learner {

namespace {

auto MakeMove(std::pair<std::int32_t, std::int32_t> pos,
              char move) -> std::pair<std::int32_t, std::int32_t> {
  switch (move) {
  case 'N':
    return {pos.first - 1, pos.second};
  case 'S':
    return {pos.first + 1, pos.second};
  case 'W':
    return {pos.first, pos.second - 1};
  case 'E':
    return {pos.first, pos.second + 1};
  }
  throw std::logic_error("unreachable");
}

}  // namespace

Labirinth::Labirinth(std::int32_t height, std::int32_t width)
    : walls_(height+2, std::vector<CellWalls>(width+2)) {}

auto Labirinth::AddWall(Position from, Direction to) -> void {
  if (to == 'N' || to == 'W') {
    from = MakeMove(from, to);
    to = LeftOf(LeftOf(to));
  }

  auto [x, y] = from;
  if (to == 'E') {
    walls_[x][y][0] = true;
    return;
  }
  walls_[x][y][1] = true;
}

auto Labirinth::IsWall(Position from, Direction to) -> std::optional<bool> {
  return true;
}

auto Labirinth::GetWalls() -> std::vector<std::pair<Position, Direction>> {
  std::vector<std::pair<Position, Direction>> res;
  for (std::int32_t i = 0; i < walls_.size(); i++) {
    for (std::int32_t j = 0; j < walls_.front().size(); j++) {
      if (walls_[i][j][0].value_or(false)) {
        res.emplace_back(Position{i, j}, 'E');
      }
      if (walls_[i][j][1].value_or(false)) {
        res.emplace_back(Position{i, j}, 'S');
      }
    }
  }
  return res;
}

}  // namespace learner
