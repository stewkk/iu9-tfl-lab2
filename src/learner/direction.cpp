#include <learner/direction.hpp>

#include <stdexcept>

namespace learner {

auto LeftOf(char direction) -> char {
  switch (direction) {
  case 'N':
    return 'W';
  case 'S':
    return 'E';
  case 'W':
    return 'S';
  case 'E':
    return 'N';
  }

  throw std::invalid_argument("invalid direction");
}

auto RightOf(char direction) -> char {
  switch (direction) {
  case 'N':
    return 'E';
  case 'S':
    return 'W';
  case 'W':
    return 'N';
  case 'E':
    return 'S';
  }

  throw std::invalid_argument("invalid direction");
}

auto SideSizeByExitDirection(char exit_direction, std::int32_t height, std::int32_t width) -> std::int32_t {
  switch (exit_direction) {
  case 'N':
    return width;
  case 'S':
    return width;
  case 'W':
    return height;
  case 'E':
    return height;
  }

  throw std::invalid_argument("invalid direction");
}

auto MakeMove(Position pos,
              Direction move) -> Position {
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

auto MakeMoves(Position pos, const std::span<const Direction> moves) -> Position {
    for (auto move : moves) {
      pos = MakeMove(pos, move);
    }
    return pos;
  }

}  // namespace learner
