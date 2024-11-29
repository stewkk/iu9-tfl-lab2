#pragma once

// NOTE: можно переписать гораздо проще, но я скорее всего не буду этого делать

#include <cstdint>
#include <concepts>
#include <vector>
#include <string>
#include <stdexcept>

#include <learner/mat_language.hpp>
#include <learner/direction.hpp>

namespace learner {

namespace {

auto GetStepsToLhsBorder(
    Language auto& lang,
    const std::string_view path_to_exit, std::int32_t n, std::int32_t m,
    std::vector<std::string>& exit_suffixes) -> std::int32_t {
  char last_step = *(path_to_exit.end() - 1);

  auto left_step = LeftOf(last_step);
  std::string current_path(path_to_exit);
  for (std::int32_t steps_count = 1; steps_count <= std::max(n, m); steps_count++) {
      current_path.push_back(left_step);

      current_path.push_back(LeftOf(left_step));
      if (!lang.Contains(current_path)) {
          exit_suffixes.push_back(current_path.substr(path_to_exit.size()));
          current_path.pop_back();
          continue;
      }

      current_path.push_back(LeftOf(LeftOf(left_step)));
      if (!lang.Contains(current_path)) {
          return steps_count;
      }

      std::vector<char> reversed_path(steps_count-1, LeftOf(LeftOf(left_step)));
      current_path.insert(current_path.end(), reversed_path.begin(), reversed_path.end());

      current_path.push_back(LeftOf(LeftOf(last_step)));
      if (lang.Contains(current_path)) {
          return steps_count;
      }

      current_path.erase(current_path.end()-steps_count-2, current_path.end());
  }

  throw std::logic_error("unreachable");
}

template <typename T>
concept ToDirection = requires(T f, char direction) {
  { f(direction) } -> std::same_as<char>;
};

auto FindExits(Language auto &lang, std::int32_t steps_count,
               std::string prefix, std::size_t min_path_length, std::vector<std::string> &exit_suffixes,
               ToDirection auto &to_labirinth_direction) -> void {
  char last_step = *(prefix.end() - 1);
  for (std::int32_t done_steps = 0; done_steps < steps_count; done_steps++) {
    prefix.push_back(to_labirinth_direction(last_step));
    if (!lang.Contains(prefix)) {
      exit_suffixes.push_back(prefix.substr(min_path_length));
    }
    prefix.pop_back();
    prefix.push_back(last_step);
  }
}

}  // namespace

auto GetOtherExitsSuffixes(Language auto& lang,
                           const std::string_view path_to_exit, std::int32_t height,
                           std::int32_t width) -> std::pair<std::vector<std::string>, std::int32_t> {
  if (path_to_exit.empty()) {
    throw std::invalid_argument("path to exit cannot be empty");
  }

  std::vector<std::string> exit_suffixes;

  // NOTE: ищем выходы слева от текущего и в то же время находим границу лабиринта
  auto steps_to_lhs =
      GetStepsToLhsBorder(lang, path_to_exit, height, width, exit_suffixes);

  // NOTE: имея количество шагов до границы лабиринта слева, находим аналогичное
  // число для границы справа
  char last_step = *(path_to_exit.end() - 1);
  auto steps_to_rhs = SideSizeByExitDirection(last_step, height, width) - steps_to_lhs+1;

  // NOTE: ищем выходы справа от текущего
  std::string current_prefix(path_to_exit);
  current_prefix.push_back(RightOf(last_step));
  FindExits(lang, steps_to_rhs-1, current_prefix, path_to_exit.size(), exit_suffixes, RightOf);

  // NOTE: ищем выходы на смежной стороне справа от текущей
  std::vector<char> side_steps(steps_to_rhs - 1, RightOf(last_step));
  current_prefix.insert(current_prefix.end(), side_steps.begin(), side_steps.end());
  current_prefix.push_back(RightOf(RightOf(last_step)));
  FindExits(lang, SideSizeByExitDirection(RightOf(last_step), height, width), current_prefix, path_to_exit.size(), exit_suffixes, RightOf);

  // NOTE: ищем выходы на противоположной стороне лабиринта относительно текущей
  side_steps.assign(SideSizeByExitDirection(RightOf(last_step), height, width), RightOf(RightOf(last_step)));
  current_prefix.insert(current_prefix.end(), side_steps.begin(), side_steps.end());
  current_prefix.push_back(LeftOf(last_step));
  FindExits(lang, SideSizeByExitDirection(last_step, height, width), current_prefix, path_to_exit.size(), exit_suffixes, RightOf);

  // NOTE: ищем выходы на смежной стороне слева от текущей
  current_prefix = path_to_exit;
  side_steps.assign(steps_to_lhs, LeftOf(last_step));
  current_prefix.insert(current_prefix.end(), side_steps.begin(), side_steps.end());
  current_prefix.push_back(LeftOf(LeftOf(last_step)));
  FindExits(lang, SideSizeByExitDirection(LeftOf(last_step), height, width), current_prefix, path_to_exit.size(), exit_suffixes, LeftOf);

  return {exit_suffixes, steps_to_lhs};
}

}  // namespace learner

