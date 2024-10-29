#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

namespace learner {

namespace {

auto QueryContains(const std::string& s) -> bool {
    if (s == "") {
        return false;
    } else {
        std::cout << "isin\n" << s << std::endl;
    }
    std::string ans;
    std::cin >> ans;
    std::cerr << "Got ans: |" << ans << "| for string: |" << s << "|" << std::endl;
    if (ans == "True") {
        return true;
    }
    return false;
}

}  // namespace

template <typename Table>
class TeacherLanguage {
public:
    auto Contains(const std::string& s) -> bool;
    // NOTE: последовательность префиксов, последовательность суффиксов, ф-ция для получения элемента таблицы
    auto Equivalent(const Table& table) -> std::optional<std::string>;

private:
    std::unordered_map<std::string, bool> cache_;
};

template <typename Table>
auto TeacherLanguage<Table>::Contains(const std::string& s) -> bool {
    if (auto it = cache_.find(s); it != cache_.end()) {
        return it->second;
    }
    auto res = QueryContains(s);
    cache_[s] = res;
    return res;
}

template <typename Table>
auto TeacherLanguage<Table>::Equivalent(const Table& table) -> std::optional<std::string> {
    std::cout << "table" << std::endl;
    for (const auto& el : table.GetSuffixes()) {
        if (el == "") {
            std::cout << "e ";
            continue;
        }
        std::cout << el << ' ';
    }
    std::cout << std::endl;

    for (const auto& prefix : table.GetPrefixes()) {
        if (prefix == "") {
            std::cout << "e ";
        } else {
            std::cout << prefix << ' ';
        }
        for (const auto &suffix : table.GetSuffixes()) {
            std::cout << Contains(prefix+suffix) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "end" << std::endl;

    std::string ans;
    std::cin >> ans;
    if (ans == "TRUE") {
        return std::nullopt;
    }
    return ans;
}

}  // learner
