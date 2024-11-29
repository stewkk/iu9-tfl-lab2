#include <learner/learner.hpp>

#include <iostream>

#include <learner/mat_advanced12iq.hpp>
#include <learner/labirinth.hpp>
#include <learner/min_prefix.hpp>
#include <learner/exits.hpp>
#include <learner/labirinth_builder.hpp>
#include <learner/table_builder.hpp>

namespace learner {

auto BuildDfaTable(std::int32_t height, std::int32_t width, std::int32_t seed) -> bool {
    learner::MATadvanced12iq mat(seed, height, width);

    Table empty_table;
    empty_table.AddPrefix("");
    empty_table.AddSuffix("");
    auto counterexample = mat.Equivalent(empty_table);
    if (!counterexample.has_value()) {
        return true;
    }

    auto path_to_exit = counterexample.value();

    auto min_prefix = GetMinPrefixInLang(mat, path_to_exit);
    auto [start_exit_suffixes, steps_to_lhs] = GetOtherExitsSuffixes(mat, path_to_exit, height, width);

    Labirinth labirinth(height, width);

    auto exit = labirinth.GetExit(path_to_exit.back(), steps_to_lhs);
    SetExits(labirinth, exit, start_exit_suffixes);
    FillBorders(labirinth);

    std::vector<std::pair<Exit, std::vector<std::string>>> exits;
    ExploreLabirinth(labirinth, mat, exit, path_to_exit, start_exit_suffixes);
    exits.push_back({exit, start_exit_suffixes});

    for (const auto& suffix : start_exit_suffixes) {
        Exit other_exit = GetOtherExit(exit, suffix);

        auto other_exit_suffixes = GetExitsSuffixes(other_exit.pos, labirinth);
        std::swap(other_exit_suffixes.front(), other_exit_suffixes.back());
        other_exit_suffixes.pop_back();

        ExploreLabirinth(labirinth, mat, other_exit, path_to_exit+std::string(suffix.begin(), std::prev(suffix.end())), other_exit_suffixes);
        exits.push_back({other_exit, other_exit_suffixes});
    }

    Table table;
    BuildPrefixes(table, labirinth);
    BuildSuffixes(table, labirinth, exits);
    BuildOuterSuffixes(table, labirinth);

    auto res = mat.Equivalent(table);
    if (res.has_value()) {
        std::cerr << "got counterexample: " << res.value() << std::endl;
        return false;
    }

    return true;
}

}  // namespace learner
