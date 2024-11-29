#include <learner/table.hpp>

namespace learner {

auto Table::AddPrefix(std::string prefix) -> void {
    prefixes_.push_back(std::move(prefix));
}

auto Table::GetPrefixes() const -> const std::vector<std::string>& {
    return prefixes_;
}

}  // namespace learner
