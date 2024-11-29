#pragma once

#include <learner/table.hpp>
#include <learner/labirinth.hpp>

namespace learner {

auto BuildPrefixes(Table& table, const Labirinth& labirinth) -> void;

auto BuildSuffixes(Table& table, const Labirinth& labirinth, const std::vector<std::pair<Exit, const std::vector<std::string>&>> exits) -> void;

}  // namespace learner
