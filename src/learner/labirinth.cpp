#include <learner/labirinth.hpp>

#include <cstdint>
#include <stdexcept>
#include <utility>

#include <learner/direction.hpp>

namespace learner {

Labirinth::Labirinth(std::int32_t height, std::int32_t width)
  : walls_(height+2, std::vector<CellWalls>(width+2)) {}


auto Labirinth::GetHeight() const -> std::int32_t {
  return walls_.size();
}

auto Labirinth::GetWidth() const -> std::int32_t {
  return walls_.front().size();
}

auto Labirinth::get_wall(Position from, Direction to) -> std::optional<bool>& {
  if (to == 'N' || to == 'W') {
    from = MakeMove(from, to);
    to = LeftOf(LeftOf(to));
  }

  auto [x, y] = from;
  if (to == 'E') {
    return walls_[x][y][0];
  }
  return walls_[x][y][1];
}

auto Labirinth::get_wall(Position from, Direction to) const -> std::optional<bool> {
  if (to == 'N' || to == 'W') {
    from = MakeMove(from, to);
    to = LeftOf(LeftOf(to));
  }

  auto [x, y] = from;
  if (to == 'E') {
    return walls_[x][y][0];
  }
  return walls_[x][y][1];
}

auto Labirinth::AddWall(Position from, Direction to) -> void {
  get_wall(from, to) = true;
}

auto Labirinth::IsWall(Position from, Direction to) const -> std::optional<bool> {
  return get_wall(from, to);
}

auto Labirinth::AddPath(Position from, Direction to) -> void {
  get_wall(from, to) = false;
}

auto Labirinth::GetWalls() const -> std::vector<std::pair<Position, Direction>> {
  std::vector<std::pair<Position, Direction>> res;
  for (std::int32_t i = 0; i < GetHeight(); i++) {
    for (std::int32_t j = 0; j < GetWidth(); j++) {
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

auto Labirinth::GetExit(Direction exit_direction, std::int32_t steps_to_lhs_border) const -> Exit {
  Position pos;
  switch (exit_direction) {
    case 'N':
      pos = Position{0, steps_to_lhs_border};
      break;
    case 'S':
      pos = Position{GetHeight()-1, GetWidth()-steps_to_lhs_border-1};
      break;
    case 'W':
      pos = Position{GetHeight()-1-steps_to_lhs_border, 0};
      break;
    case 'E':
      pos = Position{steps_to_lhs_border, GetWidth()-1};
      break;
    default:
     throw std::logic_error{"unreachable"};
  }

  return Exit{.pos = pos, .direction = exit_direction};
}

}  // namespace learner
