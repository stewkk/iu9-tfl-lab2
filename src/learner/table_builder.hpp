#pragma once

#include <learner/table.hpp>
#include <learner/labirinth.hpp>

namespace learner {

auto BuildPrefixes(Table& table, const Labirinth& labirinth) -> void;

auto BuildSuffixes(Table& table, const Labirinth& labirinth, const std::vector<std::pair<Exit, std::vector<std::string>>> exits) -> void;

auto BuildOuterSuffixes(Table& table, const Labirinth& labirinth) -> void;

}  // namespace learner
