#pragma once

#include <array>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <learner/teacher.hpp>

namespace learner {

class Table {
public:
    Table(TeacherLanguage<Table> teacher) : teacher_language_(std::move(teacher)) {}

    auto AddPrefix(const std::string& s) -> void {
        prefixes_.insert(s);
    }

    auto AddSuffix(const std::string& s) -> void {
        suffixes_.insert(s);
    }

    auto AddExtendedPrefix(const std::string& s) -> void {
        extended_.insert(s);
    }

    auto ContainsExtendedPrefix(const std::string& s) -> bool {
        return extended_.contains(s);
    }

    auto Contains(const std::string& s) -> bool {
        if (auto it = cache_.find(s); it != cache_.end()) {
            return it->second;
        }

        auto res = teacher_language_.Contains(s);
        cache_[s] = res;
        return res;
    }

    auto Extend() -> void {
        for (const auto& prefix : prefixes_) {
            for (auto l : std::array{'N', 'S', 'W', 'E'}) {
                auto extent = prefix+l;
                if (!prefixes_.contains(extent))
                extended_.insert(extent);
            }
        }
    }

    auto GetCounterexample() -> std::optional<std::string> const {
        return teacher_language_.Equivalent(*this);
    }

    auto AddExample(const std::string& example) -> void {
        for (auto it = example.cbegin(); it != example.cend(); ++it) {
            std::string suffix(it, example.cend());
            if (!suffixes_.contains(suffix)) {
                suffixes_.insert(suffix);
            }
        }
    }

    auto GetRow(const std::string& prefix) -> std::string {
      std::string row;
      for (const auto &suffix : suffixes_) {
        row.push_back(Contains(prefix + suffix));
      }
      return row;
    }


    auto MakeClosed() -> bool {
        // NOTE: тут, если в extended части таблицы появились строки, которых
        // нет в верхней части, надо добавить в верхнюю часть префиксы, которые
        // соответствуют этой строке в расширенной части (их может быть несколько)

        bool madeChanges = false;

        std::unordered_set<std::string> main;
        for (const auto& prefix : prefixes_) {
            auto row = GetRow(prefix);
            main.insert(row);
        }
        for (const auto& prefix : extended_) {
            auto row = GetRow(prefix);

            if (!main.contains(row)) {
                prefixes_.insert(prefix);
                madeChanges = true;
            }
        }

        return madeChanges;
    }

    auto MakeConsistent() -> bool {
        bool madeChanges = false;
        std::unordered_map<std::string, std::vector<std::string>> main;
        for (const auto& prefix : prefixes_) {
            auto row = GetRow(prefix);
            if (auto it = main.find(row); it != main.end()) {
                for (const auto& other : it->second) {
                    for (auto y : std::array{'N', 'S', 'W', 'E'}) {
                        const auto lhs = prefix+y;
                        const auto rhs = other+y;
                        const auto cur_suffixes = suffixes_;
                        for (const auto& suffix : cur_suffixes) {
                            if (Contains(lhs+suffix) != Contains(rhs+suffix)) {
                                AddSuffix(suffix);
                                madeChanges = true;
                            }
                        }
                    }
                }
            }
            main[row].push_back(prefix);
        }

        return madeChanges;
    }

private:
    std::unordered_map<std::string, bool> cache_;
    std::unordered_set<std::string> prefixes_;
    std::unordered_set<std::string> suffixes_;
    std::unordered_set<std::string> extended_;
    TeacherLanguage<Table> teacher_language_;
};

class Learner {
public:
    Learner(std::int32_t n, std::int32_t m, TeacherLanguage<Table> teacher)
        : n_(n), m_(m), table_(std::move(teacher)) {}

    auto Build() -> void {
        // NOTE: init with epsilon
        table_.AddPrefix("");
        table_.AddSuffix("");

        // TODO: тут надо сгенерировать и добавить строки, которые ходят внутри
        // квадрата (это комба), и просто добавить их с помощью AddPrefix,
        // AddSuffix

        while (true) {
            table_.Extend();

            if (table_.MakeClosed()) {
                continue;
            }

            if (table_.MakeConsistent()) {
                continue;
            }

            auto counterexample = table_.GetCounterexample();
            if (!counterexample.has_value()) {
                return;
            }

            table_.AddExample(counterexample.value());
        }
    }


private:
    std::int32_t n_;
    std::int32_t m_;
    Table table_;
};

}  // learner

