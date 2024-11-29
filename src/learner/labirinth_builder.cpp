#include <learner/labirinth_builder.hpp>

#include <span>
#include <queue>
#include <utility>
#include <cassert>

namespace learner {

auto GetOtherExit(Exit from, std::string_view path_to_other) -> Exit {
    auto other_pos = learner::MakeMoves(from.pos, std::span(path_to_other.cbegin(), std::prev(path_to_other.cend())));
    return Exit{
        .pos = other_pos,
        .direction = LeftOf(LeftOf(path_to_other.back())),
    };
}

auto SetExits(Labirinth& l, Exit exit, const std::vector<std::string>& other_exits_suffixes) -> void {
    l.AddPath(exit.pos, LeftOf(LeftOf(exit.direction)));

    for (const auto& suffix : other_exits_suffixes) {
        auto pos = MakeMoves(exit.pos, std::span(suffix.cbegin(), std::prev(suffix.cend())));
        auto direction = suffix.back();
        l.AddPath(pos, direction);
    }
}

auto FillBorders(Labirinth& l) -> void {
    for (std::int32_t i = 1; i < l.GetWidth()-1; i++) {
        if (!l.IsWall({0, i}, 'S').has_value()) {
            l.AddWall({0, i}, 'S');
        }
        if (!l.IsWall({l.GetHeight()-1, i}, 'N').has_value()) {
            l.AddWall({l.GetHeight()-1, i}, 'N');
        }
    }

    for (std::int32_t i = 1; i < l.GetHeight()-1; i++) {
        if (!l.IsWall({i, 0}, 'E').has_value()) {
            l.AddWall({i, 0}, 'E');
        }
        if (!l.IsWall({i, l.GetHeight()-1}, 'W').has_value()) {
            l.AddWall({i, l.GetHeight()-1}, 'W');
        }
    }
}

auto GetExitsSuffixes(Position from, const Labirinth& labirinth) -> std::vector<std::string> {
    std::vector<std::string> res;
    std::queue<std::pair<Position, std::string>> q;
    std::vector<std::vector<char>> used(labirinth.GetHeight(), std::vector<char>(labirinth.GetWidth(), false));
    q.push({from, ""});
    used[from.first][from.second] = true;
    while (!q.empty()) {
        auto [pos, path] = q.front();
        q.pop();

        for (auto direction : {'N', 'S', 'W', 'E'}) {
            auto is_wall = labirinth.IsWall(pos, direction);
            assert(is_wall.has_value());
            if (is_wall.value()) {
                continue;
            }

            auto next = MakeMove(pos, direction);
            if (used[next.first][next.second]) {
                continue;
            }
            used[next.first][next.second] = true;

            if (labirinth.IsIn(next)) {
                res.push_back(path+direction);
                continue;
            }

            q.push({next, path+direction});
        }
    }
    return res;
}

}  // namespace learner
