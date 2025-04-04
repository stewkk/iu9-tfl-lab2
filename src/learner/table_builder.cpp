#include <learner/table_builder.hpp>

#include <queue>
#include <utility>
#include <cassert>

#include <learner/labirinth_builder.hpp>

namespace learner {

auto BuildPrefixes(Table& table, const Labirinth& labirinth) -> void {
    Position start{1, 1};
    std::queue<std::pair<Position, std::string>> q;
    std::vector<std::vector<char>> used(labirinth.GetHeight(), std::vector<char>(labirinth.GetWidth(), false));
    q.push({start, ""});
    used[start.first][start.second] = true;
    while (!q.empty()) {
        auto [pos, path] = q.front();
        q.pop();

        table.AddPrefix(path);
        for (auto direction : {'N', 'S', 'W', 'E'}) {
            auto is_wall = labirinth.IsWall(pos, direction);
            assert(is_wall.has_value());

            auto next = MakeMove(pos, direction);

            // if (next.first < 0 || next.second < 0 || next.first >= labirinth.GetHeight() || next.second >= labirinth.GetWidth()) {
            //     continue;
            // }

            if (is_wall.value()) {
                table.AddPrefix(path+direction);
                continue;
            }

            if (used[next.first][next.second]) {
                table.AddPrefix(path+direction);
                continue;
            }

            q.push({next, path+direction});
            used[next.first][next.second] = true;
        }
    }
}

auto BuildSuffixes(Table& table, const Labirinth& labirinth, const std::vector<std::pair<Exit, std::vector<std::string>>> exits) -> void {
    table.AddSuffix("");
    struct BfsState {
        Position pos;
        std::string path;
        const std::vector<std::string>& other_exit_suffixes;
    };
    std::queue<BfsState> q;
    std::vector<std::vector<char>> used(labirinth.GetHeight(), std::vector<char>(labirinth.GetWidth(), false));
    for (const auto& exit : exits) {
        {
            auto [x, y] = exit.first.pos;
            used[x][y] = true;
        }
        auto [x, y] = MakeMove(exit.first.pos, LeftOf(LeftOf(exit.first.direction)));
        used[x][y] = true;
        q.push(BfsState{
                .pos = {x, y},
                .path = std::string{exit.first.direction},
                .other_exit_suffixes = exit.second,
            });
    }

    while (!q.empty()) {
        auto state = q.front();
        q.pop();

        std::string reversed_path(state.path.rbegin(), state.path.rend());
        for (const auto& other_exit_suffix : state.other_exit_suffixes) {
            table.AddSuffix(reversed_path+other_exit_suffix);
        }
        table.AddSuffix(std::move(reversed_path));

        for (auto direction : {'N', 'S', 'W', 'E'}) {
            auto is_wall = labirinth.IsWall(state.pos, direction);
            assert(is_wall.has_value());
            if (is_wall.value()) {
                continue;
            }

            auto next = MakeMove(state.pos, direction);
            if (used[next.first][next.second]) {
                continue;
            }

            q.push(BfsState{
                .pos = next,
                .path = state.path + LeftOf(LeftOf(direction)),
                .other_exit_suffixes = state.other_exit_suffixes,
            });
            used[next.first][next.second] = true;
        }
    }
}

auto BuildOuterSuffixes(Table& table, const Labirinth& labirinth) -> void {
    for (std::int32_t x = 0; x < labirinth.GetHeight(); x++) {
        auto suffixes = GetExitsSuffixes(Position{x, labirinth.GetWidth()-1}, labirinth);
        for (const auto& suffix : suffixes) {
            table.AddSuffix(suffix);
        }

        suffixes = GetExitsSuffixes(Position{x, 0}, labirinth);
        for (const auto& suffix : suffixes) {
            table.AddSuffix(suffix);
        }
    }
    for (std::int32_t y = 1; y < labirinth.GetWidth()-1; y++) {
        auto suffixes = GetExitsSuffixes(Position{labirinth.GetHeight()-1, y}, labirinth);
        for (const auto& suffix : suffixes) {
            table.AddSuffix(suffix);
        }

        suffixes = GetExitsSuffixes(Position{0, y}, labirinth);
        for (const auto& suffix : suffixes) {
            table.AddSuffix(suffix);
        }
    }
}

}  // namespace learner
