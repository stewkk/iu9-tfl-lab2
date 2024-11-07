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
    Table(std::int32_t n, std::int32_t m, TeacherLanguage<Table> teacher) :
        n_(n),
        m_(m),
        teacher_language_(std::move(teacher)) {}

    auto AddPrefix(const std::string& s) -> void {
        prefixes_.insert(s);
        for (const auto& suffix : suffixes_) {
            Contains(s+suffix);
        }
    }

    auto AddSuffix(const std::string& s) -> void {
        suffixes_.insert(s);
        for (const auto& prefix : prefixes_) {
            Contains(prefix+s);
        }
        for (const auto& prefix : extended_) {
            Contains(prefix+s);
        }
    }

    auto AddExtendedPrefix(const std::string& s) -> void {
        extended_.insert(s);
        for (const auto& suffix : suffixes_) {
            Contains(s+suffix);
        }
    }

    auto ContainsExtendedPrefix(const std::string& s) const -> bool {
        return extended_.contains(s);
    }

    auto Contains(const std::string& s) -> bool {
        return teacher_language_.Contains(s);
    }

    auto Extend() -> void {
        for (const auto& prefix : prefixes_) {
            for (auto l : std::array{'N', 'S', 'W', 'E'}) {
                auto extent = prefix+l;
                if (!prefixes_.contains(extent)) {
                    AddExtendedPrefix(extent);
                }
            }
        }
    }

    auto GetCounterexample() -> std::optional<std::string> {
        return teacher_language_.Equivalent(*this);
    }

    auto AddExample(const std::string& example) -> void {
        for (auto it = example.cbegin(); it != example.cend(); ++it) {
            std::string suffix(it, example.cend());
            if (!suffixes_.contains(suffix)) {
                AddSuffix(suffix);
            }
        }
    }

    auto GetRow(const std::string& prefix) -> std::string {
      std::string row;
      for (const auto &suffix : suffixes_) {
          if (Contains(prefix + suffix)) {
              row.push_back('1');
          } else {
              row.push_back('0');
          }
      }
      return row;
    }


    auto MakeClosed() -> bool {
        // NOTE: тут, если в extended части таблицы появились строки, которых
        // нет в верхней части, надо добавить в верхнюю часть префиксы, которые
        // соответствуют этой строке в расширенной части (их может быть несколько)

        std::cerr << "MakeClosed" << std::endl;

        bool madeChanges = false;

        std::unordered_set<std::string> main;
        for (const auto& prefix : prefixes_) {
            auto row = GetRow(prefix);
            std::cerr << "prefix: " << prefix << ' ' << row << std::endl;
            main.insert(row);
        }

        for (auto it = extended_.begin(); it != extended_.end();) {
            const auto& prefix = *it;
            auto row = GetRow(prefix);
            std::cerr << "extended: " << prefix << ' ' << row << std::endl;

            if (!main.contains(row)) {
                AddPrefix(prefix);
                it = extended_.erase(it);
                madeChanges = true;
            } else {
                ++it;
            }
        }

        return madeChanges;
    }

    auto MakeConsistent() -> bool {
        std::cerr << "MakeConsistent" << std::endl;

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
                                AddSuffix(y+suffix);
                                std::cerr << "Found distinguishing suffix: " << y+suffix << std::endl;
                                if ((y+suffix).size() > n_+ n_/2 || (y+suffix).size() > m_ + m_/2) {
                                    return false;
                                }
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

    auto GetPrefixes() const -> const std::unordered_set<std::string>& {
        return prefixes_;
    }

    auto GetExtended() const -> const std::unordered_set<std::string>& {
        return extended_;
    }

    auto GetSuffixes() const -> const std::unordered_set<std::string>& {
        return suffixes_;
    }

    auto Get() const -> const std::unordered_set<std::string>& {
        return suffixes_;
    }

private:
    std::unordered_set<std::string> prefixes_;
    std::unordered_set<std::string> suffixes_;
    std::unordered_set<std::string> extended_;
    TeacherLanguage<Table> teacher_language_;
    std::int32_t n_;
    std::int32_t m_;
};

class Learner {
public:
    Learner(std::int32_t n, std::int32_t m, TeacherLanguage<Table> teacher)
        : n_(n), m_(m), table_(n, m, std::move(teacher)) {}

    auto Build() -> void {
        // NOTE: init with epsilon
        table_.AddPrefix("");
        table_.AddSuffix("");

        // TODO: тут надо сгенерировать и добавить строки, которые ходят внутри
        // квадрата (это комба), и просто добавить их с помощью AddPrefix,
        // AddSuffix

        for (int i = 0; i < 10000; i++) {
            table_.Extend();

            if (table_.MakeClosed()) {
                std::cerr << "Close" << std::endl;
                continue;
            }

            if (table_.MakeConsistent()) {
                std::cerr << "Consistent" << std::endl;
                continue;
            }

            std::cerr << "Counterexample" << std::endl;

            auto counterexample = table_.GetCounterexample();
            if (!counterexample.has_value()) {
                std::cerr << "Success!" << std::endl;
                return;
            }

            std::cerr << "Got counterexample: " << counterexample.value() << std::endl;

            table_.AddExample(counterexample.value());
        }
    }


private:
    std::int32_t n_;
    std::int32_t m_;
    Table table_;
};

}  // learner

