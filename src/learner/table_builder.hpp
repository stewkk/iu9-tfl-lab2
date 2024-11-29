#pragma once

#include <learner/table.hpp>
#include <learner/labirinth.hpp>

namespace learner {

auto BuildPrefixes(Table& table, const Labirinth& labirinth) -> void;

}  // namespace learner
