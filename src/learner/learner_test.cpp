#include <gtest/gtest.h>

#include <iostream>
#include <format>
#include <string_view>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>
#include <utility>

#include <learner/mat_language.hpp>

namespace learner {

auto GetMinPrefixInLang(Language auto& lang, const std::string_view path_in_lang) -> std::string {
    for (auto it = path_in_lang.cbegin(); it != path_in_lang.cend(); ++it) {
        std::string_view path_prefix(path_in_lang.cbegin(), it+1);
        if (lang.Contains(path_prefix)) {
            return std::string(path_prefix);
        }
    }
    return std::string(path_in_lang);
}

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

auto GetOtherExitsSuffixes(Language auto& lang,
                           const std::string_view path_to_exit, std::int32_t height,
                           std::int32_t width) -> std::vector<std::string> {
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

  return exit_suffixes;
}

auto IsSameExit(Language auto &lang, const std::string &prefix,
                const std::string &suffix,
                const std::vector<std::string> &other_exits_suffixes) -> bool {
  std::ostringstream ss;
  ss << prefix << suffix;
  if (!lang.Contains(ss.str())) {
    return false;
  }

  for (const auto &other_exit_suffix : other_exits_suffixes) {
    std::ostringstream tmp;
    tmp << ss.str() << other_exit_suffix;
    if (!lang.Contains(tmp.str())) {
      return false;
    }
  }

  if (other_exits_suffixes.empty()) {
    return !lang.Contains(ss.str().substr(0, ss.str().size() - 1));
  }

  return true;
}

auto MakeMove(std::pair<std::int32_t, std::int32_t> pos,
              char move) -> std::pair<std::int32_t, std::int32_t> {
  switch (move) {
  case 'N':
    return {pos.first, pos.second - 1};
  case 'S':
    return {pos.first, pos.second + 1};
  case 'W':
    return {pos.first - 1, pos.second};
  case 'E':
    return {pos.first + 1, pos.second};
  }
  throw std::logic_error("unreachable");
}

} // namespace learner

using std::literals::operator""s;
using std::literals::operator""sv;

TEST(GetMinPrefixInLangTest, ReturnsMinPrefix) {
    auto seed = 10;
    auto width = 2;
    auto height = 2;
    // NOTE:
    //      _
    //     |   |
    //     | | |
    //      ‾ ‾
    auto mat = learner::MATadvanced12iq(seed, height, width);

    auto got = learner::GetMinPrefixInLang(mat, "EENWS"sv);

    ASSERT_EQ(got, "EEN"s);
}

TEST(GetOtherExitsSuffixesTest, NoOtherExits) {
    auto seed = 10;
    auto width = 2;
    auto height = 2;
    // NOTE:
    //      _
    //     |   |
    //     | | |
    //      ‾ ‾
    auto mat = learner::MATadvanced12iq(seed, height, width);

    auto suffixes = learner::GetOtherExitsSuffixes(mat, "EEN"sv, height, width);

    ASSERT_EQ(suffixes, std::vector<std::string>{});
}

TEST(GetOtherExitsSuffixesTest, TwoExits) {
    auto seed = 6259;
    auto width = 2;
    auto height = 2;
    // NOTE:
    //        _
    //     |   |
    //       | |
    //      ‾ ‾
    auto mat = learner::MATadvanced12iq(seed, height, width);

    auto got = learner::GetOtherExitsSuffixes(mat, "WSSW"sv, height, width);

    std::vector<std::string> expected{"NNES"};
    ASSERT_EQ(got, expected);
}

TEST(GetOtherExitsSuffixesTest, TwoExitsAtCorner) {
    auto seed = 991;
    auto width = 2;
    auto height = 2;
    // NOTE:
    //      ___
    //     |   |
    //       |
    //      ‾
    auto mat = learner::MATadvanced12iq(seed, height, width);

    auto got = learner::GetOtherExitsSuffixes(mat, "ESE"sv, height, width);

    std::vector<std::string> expected{"SWN", "SWWWNE"};
    ASSERT_EQ(got, expected);
}

TEST(GetOtherExitsSuffixesTest, ManyExits) {
    auto seed = 12345;
    auto width = 2;
    auto height = 2;
    // NOTE:
    //
    //      _
    //        |
    //
    auto mat = learner::MATadvanced12iq(seed, height, width);

    auto got = learner::GetOtherExitsSuffixes(mat, "EN"sv, height, width);

    std::vector<std::string> expected{"WS", "ESW", "ESSSWN", "ESSSWWN", "WWSE", "WWSSE"};
    ASSERT_EQ(got, expected);
}

TEST(TODO, TODO) {
    auto seed = 10;
    auto width = 2;
    auto height = 2;
    // NOTE:
    //      _
    //     |   |
    //     | | |
    //      ‾ ‾
    auto mat = learner::MATadvanced12iq(seed, height, width);
    std::vector<std::string> other_exits_suffixes;


    // auto got = learner::MinimizePath(mat, "NNSEWNESSNN"sv, other_exits_suffixes);

    // ASSERT_EQ(got, "SNESNN"s);
}
