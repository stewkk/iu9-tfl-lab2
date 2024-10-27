#pragma once

#include <optional>
#include <string>
#include <map>

namespace learner {

class TeacherLanguage {
public:
    using Table = std::map<std::pair<std::string, std::string>, bool>;

    auto Contains(const std::string& s) -> bool const;
    auto Equivalent(const Table& table) -> std::optional<std::string> const;
};

}  // learner
