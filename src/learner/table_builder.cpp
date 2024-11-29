#include <learner/table_builder.hpp>

#include <queue>
#include <utility>
#include <cassert>

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

            if (is_wall.value()) {
                table.AddPrefix(path+direction);
                continue;
            }

            auto next = MakeMove(pos, direction);

            if (used[next.first][next.second]) {
                continue;
            }

            q.push({next, path+direction});
            used[next.first][next.second] = true;
        }
    }
}

}  // namespace learner
