#pragma once

#include <string>
#include <vector>

namespace learner {

class Table {
public:
    auto AddPrefix(std::string prefix) -> void;
    auto GetPrefixes() const -> const std::vector<std::string>&;
private:
    std::vector<std::string> prefixes_;
};

}  // namespace learner
