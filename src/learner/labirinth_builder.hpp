#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <queue>
#include <sstream>

#include <learner/direction.hpp>
#include <learner/labirinth.hpp>
#include <learner/mat_language.hpp>

namespace learner {

auto GetOtherExit(Exit from, std::string_view path_to_other) -> Exit;

auto SetExits(Labirinth& l, Exit exit, const std::vector<std::string>& other_exits_suffixes) -> void;

auto FillBorders(Labirinth& l) -> void;

inline auto ReversedPath(const std::string_view path) -> std::string {
    std::string res;
    res.reserve(path.size());
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        auto direction = *it;
        res.push_back(LeftOf(LeftOf(direction)));
    }
    return res;
}

auto IsSameExit(Language auto& lang, const std::string& path,
                const std::vector<std::string>& other_exits_suffixes) -> bool {
  if (other_exits_suffixes.empty()) {
    return !lang.Contains(path.substr(0, path.size() - 1));
  }

  for (const auto &other_exit_suffix : other_exits_suffixes) {
    if (lang.Contains(path+other_exit_suffix)) {
      return false;
    }
  }

  return true;
}

auto IsWall(Language auto& lang, std::string path, Direction check_direction,
            std::string_view path_to_exit,
            const std::vector<std::string>& other_exits_suffixes) -> bool {
  std::ostringstream ss;
  ss << path << check_direction
     << ReversedPath(std::string_view(std::next(path.cbegin(), path_to_exit.size()), path.cend()));
  if (!lang.Contains(ss.str())) {
    return false;
  }

  return IsSameExit(lang, ss.str(), other_exits_suffixes);
}

auto ExploreLabirinth(Labirinth& labirinth, Language auto& lang, Exit exit,
                      std::string_view path_to_exit,
                      const std::vector<std::string>& other_exits_suffixes)
    -> void {
  auto start = exit.pos;
  start = MakeMove(start, LeftOf(LeftOf(exit.direction)));

  std::queue<std::pair<Position, std::string>> q;
  q.push({start, std::string(path_to_exit) + LeftOf(LeftOf(exit.direction))});
  while (!q.empty()) {
    auto [pos, path] = q.front();
    q.pop();
    for (auto direction : {'N', 'S', 'W', 'E'}) {
      if (labirinth.IsWall(pos, direction).has_value()) {
        continue;
      }

      bool is_wall =
          IsWall(lang, path, direction, path_to_exit, other_exits_suffixes);
      if (is_wall) {
        labirinth.AddWall(pos, direction);
      } else {
        labirinth.AddPath(pos, direction);
        q.push({MakeMove(pos, direction), path + direction});
      }
    }
  }
}

}  // namespace learner
