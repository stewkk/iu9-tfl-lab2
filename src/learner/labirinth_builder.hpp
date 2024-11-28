#pragma once

#include <vector>
#include <string>
#include <string_view>

#include <learner/direction.hpp>
#include <learner/labirinth.hpp>

namespace learner {

auto GetOtherExit(Exit from, std::string_view path_to_other) -> Exit;

auto SetExits(Labirinth& l, Exit exit, const std::vector<std::string>& other_exits_suffixes) -> void;

auto FillBorders(Labirinth& l) -> void;

}  // namespace learner
