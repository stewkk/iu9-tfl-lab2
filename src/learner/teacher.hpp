#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <fstream>

namespace learner {

namespace {

auto QueryContains(const std::string& s) -> bool {
    std::cerr << "QueryContains: " << s << std::endl;
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

void WriteStats(const std::string& path, std::int32_t count_all, std::int32_t count_actual, std::int32_t equivalence) {
    std::ofstream f(path);
    f << count_all << ',' << count_actual << ',' << equivalence << std::endl;
}

}  // namespace

template <typename Table>
class TeacherLanguage {
public:
    auto Contains(const std::string& s) -> bool;
    // NOTE: последовательность префиксов, последовательность суффиксов, ф-ция для получения элемента таблицы
    auto Equivalent(const Table& table) -> std::optional<std::string>;

    std::int32_t count_contains_all_ = 0;
    std::int32_t count_contains_actual_ = 0;
    std::int32_t count_equivalent_ = 0;
private:
    std::unordered_map<std::string, bool> cache_;
};

template <typename Table>
auto TeacherLanguage<Table>::Contains(const std::string& s) -> bool {
    ++count_contains_all_;
    if (auto it = cache_.find(s); it != cache_.end()) {
        return it->second;
    }
    ++count_contains_actual_;
    auto res = QueryContains(s);
    cache_[s] = res;
    return res;
}

template <typename Table>
auto TeacherLanguage<Table>::Equivalent(const Table& table) -> std::optional<std::string> {
    ++count_equivalent_;
    std::cerr << "requesting equivalence" << std::endl;
    std::cout << "table" << std::endl;
    std::cerr << "table" << std::endl;
    for (const auto& el : table.GetSuffixes()) {
        if (el == "") {
            std::cout << "e ";
            std::cerr << "e ";
            continue;
        }
        std::cout << el << ' ';
        std::cerr << el << ' ';
    }
    std::cout << std::endl;
    std::cerr << std::endl;

    for (const auto& prefix : table.GetPrefixes()) {
        if (prefix == "") {
            std::cout << "e ";
            std::cerr << "e ";
        } else {
            std::cout << prefix << ' ';
            std::cerr << prefix << ' ';
        }
        for (const auto &suffix : table.GetSuffixes()) {
            std::cout << Contains(prefix+suffix) << ' ';
            std::cerr << Contains(prefix+suffix) << ' ';
        }
        std::cout << std::endl;
        std::cerr << std::endl;
    }

    for (const auto& prefix : table.GetExtended()) {
        if (prefix == "") {
            std::cout << "e ";
            std::cerr << "e ";
        } else {
            std::cout << prefix << ' ';
            std::cerr << prefix << ' ';
        }
        for (const auto &suffix : table.GetSuffixes()) {
            std::cout << Contains(prefix+suffix) << ' ';
            std::cerr << Contains(prefix+suffix) << ' ';
        }
        std::cout << std::endl;
        std::cerr << std::endl;
    }

    std::cout << "end" << std::endl;
    std::cerr << "end" << std::endl;

    std::string ans;
    std::cin >> ans;
    if (ans == "TRUE") {
        WriteStats("benchmarks/queries", count_contains_all_, count_contains_actual_, count_equivalent_);
        return std::nullopt;
    }
    return ans;
}

}  // learner
