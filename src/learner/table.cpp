#include <learner/table.hpp>

namespace learner {

auto Table::AddPrefix(std::string prefix) -> void {
    prefixes_.push_back(std::move(prefix));
}

auto Table::GetPrefixes() const -> const std::vector<std::string>& {
    return prefixes_;
}

auto Table::AddSuffix(std::string prefix) -> void {
    suffixes_.push_back(std::move(prefix));
}

auto Table::GetSuffixes() const -> const std::vector<std::string>& {
    return suffixes_;
}

}  // namespace learner
