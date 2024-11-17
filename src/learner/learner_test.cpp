#include <gtest/gtest.h>

#include <boost/process.hpp>

#include <iostream>
#include <format>
#include <string_view>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>

namespace learner {

template <typename L>
concept Language =
requires(L l, std::string_view s) {
    {l.Contains(s)} -> std::same_as<bool>;
};

auto GetMinPrefixInLang(Language auto& lang, const std::string_view path_in_lang) -> std::string {
    for (auto it = path_in_lang.cbegin(); it != path_in_lang.cend(); ++it) {
        std::string_view path_prefix(path_in_lang.cbegin(), it+1);
        if (lang.Contains(path_prefix)) {
            return std::string(path_prefix);
        }
    }
    return std::string(path_in_lang);
}

auto QueryContains(std::istream& in, std::ostream& out, const std::string_view s) -> bool {
    if (s == "") {
        return false;
    }

    out << std::format("isin\n{}", s) << std::endl;

    std::string ans;
    in >> ans;

    std::cerr << std::format("QueryContains({}) == {}\n", s, ans);

    if (ans == "True") {
        return true;
    } else if (ans == "False") {
        return false;
    }

    throw std::logic_error("got unexpected message from MAT");
}

boost::process::environment GetCustomEnv(std::int32_t seed, std::int32_t height, std::int32_t width) {
    boost::process::environment env = boost::this_process::environment();
    env["RANDOM_SEED"] = std::to_string(seed);
    env["HEIGHT"] = std::to_string(height);
    env["WIDTH"] = std::to_string(width);
    return env;
}

class MATadvanced12iq {
public:
        MATadvanced12iq(std::int32_t seed, std::int32_t height, std::int32_t width);
        ~MATadvanced12iq();
        MATadvanced12iq(const MATadvanced12iq&) = delete;
        MATadvanced12iq& operator=(const MATadvanced12iq&) = delete;
        auto Contains(const std::string_view s) -> bool;
public:
        boost::process::child mat_process_;
        boost::process::ipstream mat_out_;
        boost::process::opstream mat_in_;
};

MATadvanced12iq::MATadvanced12iq(std::int32_t seed, std::int32_t height,
                                 std::int32_t width)
    : mat_in_(), mat_out_(), mat_process_() {
  mat_process_ = boost::process::child(
      "python mat/advanced12iq/main.py", boost::process::std_out > mat_out_,
      boost::process::std_in < mat_in_, GetCustomEnv(seed, height, width));
}

MATadvanced12iq::~MATadvanced12iq() {
    mat_in_ << "end" << std::endl;
    mat_process_.wait();
}

auto MATadvanced12iq::Contains(const std::string_view s) -> bool {
  return QueryContains(mat_out_, mat_in_, s);
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
    auto mat = learner::MATadvanced12iq(seed, width, height);

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
    auto mat = learner::MATadvanced12iq(seed, width, height);

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
    auto mat = learner::MATadvanced12iq(seed, width, height);

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
    auto mat = learner::MATadvanced12iq(seed, width, height);

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
    auto mat = learner::MATadvanced12iq(seed, width, height);

    auto got = learner::GetOtherExitsSuffixes(mat, "EN"sv, height, width);

    std::vector<std::string> expected{"WS", "ESW", "ESSSWN", "ESSSWWN", "WWSE", "WWSSE"};
    ASSERT_EQ(got, expected);
}
