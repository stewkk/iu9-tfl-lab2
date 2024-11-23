#include <gtest/gtest.h>

#include <string_view>
#include <vector>
#include <string>

#include <learner/direction.hpp>
#include <learner/mat_advanced12iq.hpp>
#include <learner/mat_language.hpp>
#include <learner/min_prefix.hpp>
#include <learner/exits.hpp>

namespace learner {

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

class Labirinth {
public:
  using Position = std::pair<std::int32_t, std::int32_t>;
  using Direction = char;

  Labirinth(std::int32_t height, std::int32_t width);

  auto AddWall(Position from, Direction to) -> void;
  auto IsWall(Position from, Direction to) -> std::optional<bool>;
};

Labirinth::Labirinth(std::int32_t height, std::int32_t width) {}

auto Labirinth::AddWall(Position from, Direction to) -> void {}

auto Labirinth::IsWall(Position from, Direction to) -> std::optional<bool> {
  return true;
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

    // NOTE: example path from MAT: "NNSEWNESSNN"sv
}

TEST(LabirinthTest, Create) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);

  l.AddWall(learner::Labirinth::Position{0, 1}, 'S');
  auto got = l.IsWall(learner::Labirinth::Position{0, 1}, 'S');

  ASSERT_EQ(got, true);
}
