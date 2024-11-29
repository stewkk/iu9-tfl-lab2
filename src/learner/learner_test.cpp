#include <gtest/gtest.h>

#include <string_view>
#include <vector>
#include <string>

#include <learner/direction.hpp>
#include <learner/mat_advanced12iq.hpp>
#include <learner/mat_language.hpp>
#include <learner/min_prefix.hpp>
#include <learner/exits.hpp>
#include <learner/labirinth.hpp>
#include <learner/labirinth_builder.hpp>
#include <learner/table.hpp>
#include <learner/table_builder.hpp>

using std::literals::operator""s;
using std::literals::operator""sv;

using Direction = learner::Direction;
using Position = learner::Position;

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

    auto [suffixes, _] = learner::GetOtherExitsSuffixes(mat, "EEN"sv, height, width);

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

    auto [got, _] = learner::GetOtherExitsSuffixes(mat, "WSSW"sv, height, width);

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

    auto [got, _] = learner::GetOtherExitsSuffixes(mat, "ESE"sv, height, width);

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

    auto [got, _] = learner::GetOtherExitsSuffixes(mat, "EN"sv, height, width);

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

TEST(LabirinthTest, IsWall) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);

  l.AddWall(learner::Position{0, 1}, 'S');

  ASSERT_EQ(l.IsWall(learner::Position{0, 1}, 'S'), true);
  ASSERT_EQ(l.IsWall(learner::Position{0, 2}, 'S'), std::nullopt);
}

TEST(LabirinthTest, GetExitPosition) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);

  auto got = l.GetExit(Direction{'N'}, 2);

  auto expected = Position{0, 2};
  ASSERT_EQ(got.pos, expected);
}

TEST(LabirinthTest, GetExitPositionFromSouth) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);

  auto got = l.GetExit(Direction{'S'}, 2);

  auto expected = Position{3, 1};
  ASSERT_EQ(got.pos, expected);
}

TEST(LabirinthTest, GetExitPositionFromWest) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);

  auto got = l.GetExit(Direction{'W'}, 1);

  auto expected = Position{2, 0};
  ASSERT_EQ(got.pos, expected);
}

TEST(LabirinthTest, GetExitPositionFromEast) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);

  auto got = l.GetExit(Direction{'E'}, 2);

  auto expected = Position{2, 3};
  ASSERT_EQ(got.pos, expected);
}

TEST(LabirinthTest, GetOtherExitPosition) {
  learner::Exit exit{.pos = {0, 2}, .direction = 'N'};
  std::string suffix = "WWSSE";

  auto got = learner::GetOtherExit(exit, suffix);

  auto expected = learner::Exit{.pos = {2, 0}, .direction='W'};
  ASSERT_EQ(got, expected);
}

TEST(LabitinthTest, SetExits) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);
  learner::Exit exit{.pos = {0, 2}, .direction = 'N'};
  std::vector<std::string> other_exits_suffixes{"WWSSE", "ESW"};

  SetExits(l, exit, other_exits_suffixes);

  ASSERT_EQ(l.IsWall({0, 2}, 'S'), false);
  ASSERT_EQ(l.IsWall({1, 3}, 'W'), false);
  ASSERT_EQ(l.IsWall({2, 0}, 'E'), false);
}

TEST(LabirinthTest, FillBorders) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);
  learner::Exit exit{.pos = {0, 2}, .direction = 'N'};
  std::vector<std::string> other_exits_suffixes{"WWSSE", "ESW"};
  SetExits(l, exit, other_exits_suffixes);
  FillBorders(l);

  auto got = l.GetWalls();

  std::vector<std::pair<Position, Direction>> expected = {
  {{0, 1}, 'S'},
  {{0, 3}, 'E'},
  {{1, 0}, 'E'},
  {{1, 3}, 'E'},
  {{2, 1}, 'S'},
  {{2, 2}, 'E'},
  {{2, 2}, 'S'},
  {{2, 3}, 'E'},
  {{3, 0}, 'S'},
  {{3, 1}, 'S'},
  {{3, 2}, 'S'},
  {{3, 3}, 'E'},
  {{3, 3}, 'S'},
  };
  ASSERT_EQ(got, expected);
}

TEST(LabirinthTest, ExploreLabirinthWithSingleExit) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);
  learner::Exit exit{.pos = {0, 2}, .direction = 'N'};
  std::vector<std::string> other_exits_suffixes;
  SetExits(l, exit, other_exits_suffixes);
  FillBorders(l);

  auto seed = 10;
  // NOTE:
  //      _
  //     |   |
  //     | | |
  //      ‾ ‾
  auto mat = learner::MATadvanced12iq(seed, height, width);

  ExploreLabirinth(l, mat, exit, "EN", other_exits_suffixes);
  auto got = l.GetWalls();

  std::vector<std::pair<Position, Direction>> expected = {
  {{0, 1}, 'S'},
  {{1, 0}, 'E'},
  {{1, 2}, 'E'},
  {{2, 0}, 'E'},
  {{2, 1}, 'E'},
  {{2, 1}, 'S'},
  {{2, 2}, 'E'},
  {{2, 2}, 'S'},
  };
  for (auto wall : expected) {
    ASSERT_NE(std::find(got.begin(), got.end(), wall), got.end());
  }
}


TEST(LabirinthTest, ExploreLabirinthWithMultipleExits) {
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);
  learner::Exit exit{.pos = {3, 2}, .direction = 'S'};
  std::vector<std::string> other_exits_suffixes{"ENW", "WWNE"};
  SetExits(l, exit, other_exits_suffixes);
  FillBorders(l);

  auto seed = 991;
  // NOTE:
  //      ___
  //     |   |
  //       |
  //      ‾
  auto mat = learner::MATadvanced12iq(seed, height, width);

  ExploreLabirinth(l, mat, exit, "ESS", other_exits_suffixes);
  auto got = l.GetWalls();

  std::vector<std::pair<Position, Direction>> expected = {
  {{0, 1}, 'S'},
  {{0, 2}, 'S'},
  {{1, 0}, 'E'},
  {{1, 2}, 'E'},
  {{2, 1}, 'E'},
  {{2, 1}, 'S'},
  };
  for (auto wall : expected) {
    ASSERT_NE(std::find(got.begin(), got.end(), wall), got.end());
  }
}

TEST(TableTest, AddPrefix) {
  learner::Table t;

  t.AddPrefix("oaoa");
  auto got = t.GetPrefixes();

  std::vector<std::string> expected{"oaoa"};
  ASSERT_EQ(got, expected);
}

TEST(TableTest, BuildPrefixes) {
  learner::Table t;
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);
  learner::Exit exit{.pos = {0, 2}, .direction = 'N'};
  std::vector<std::string> other_exits_suffixes;
  SetExits(l, exit, other_exits_suffixes);
  FillBorders(l);
  auto seed = 10;
  // NOTE:
  //      _
  //     |   |
  //     | | |
  //      ‾ ‾
  auto mat = learner::MATadvanced12iq(seed, height, width);
  ExploreLabirinth(l, mat, exit, "EN", other_exits_suffixes);

  BuildPrefixes(t, l);
  auto got = t.GetPrefixes();

  std::vector<std::string> expected{
      "",     "N",     "W",     "S",     "SS",     "SW",     "SE",    "E",
      "EE",   "EN",    "ENN",   "ES",    "ESS",    "ESW",    "ESE",   "ENW",
      "ENWN", "ENWS",  "ENE",   "ENEN",  "ENEE",   "ENWW",   "ENWWN", "ENWWW",
      "ENES", "ENESW", "ENESE", "ENWWS", "ENWWSW", "ENWWSE", "ENESS", "ENESSW"};
  for (auto path : expected) {
    ASSERT_NE(std::find(got.begin(), got.end(), path), got.end());
  }
}

TEST(TableTest, BuildSuffixes) {
  learner::Table t;
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);
  learner::Exit exit{.pos = {0, 2}, .direction = 'N'};
  std::vector<std::string> other_exits_suffixes;
  SetExits(l, exit, other_exits_suffixes);
  FillBorders(l);
  auto seed = 10;
  // NOTE:
  //      _
  //     |   |
  //     | | |
  //      ‾ ‾
  auto mat = learner::MATadvanced12iq(seed, height, width);
  ExploreLabirinth(l, mat, exit, "EN", other_exits_suffixes);
  std::vector<std::pair<learner::Exit, const std::vector<std::string>&>> exits{{exit, other_exits_suffixes}};

  BuildSuffixes(t, l, exits);
  auto got = t.GetSuffixes();

  std::vector<std::string> expected{ "N", "NN", "EN", "NEN" };
  ASSERT_EQ(got, expected);
}

TEST(TableTest, BuildOuterSuffixes) {
  learner::Table t;
  std::int32_t height = 2;
  std::int32_t width = 2;
  learner::Labirinth l(height, width);
  learner::Exit exit{.pos = {0, 2}, .direction = 'N'};
  std::vector<std::string> other_exits_suffixes;
  SetExits(l, exit, other_exits_suffixes);
  FillBorders(l);
  auto seed = 10;
  // NOTE:
  //      _
  //     |   |
  //     | | |
  //      ‾ ‾
  auto mat = learner::MATadvanced12iq(seed, height, width);
  ExploreLabirinth(l, mat, exit, "EN", other_exits_suffixes);
  std::vector<std::pair<learner::Exit, const std::vector<std::string>&>> exits{{exit, other_exits_suffixes}};

  learner::BuildOuterSuffixes(t, l);
  auto got = t.GetSuffixes();

  std::vector<std::string> expected{ "WS", "EES", "NWS", "NEES", "NNWS", "NNEES", "NNNWS", "NNNEES", "WNNNEES", "ES", "ENNNWS", "S" };
  ASSERT_EQ(got, expected);
}
