#pragma once

#include <optional>
#include <string>

namespace learner {

// NOTE: надо уметь задавать сид для генерации лабиринта, чтобы были воспроизводимые тесты!

template <typename Table>
class TeacherLanguage {
public:
    auto Contains(const std::string& s) -> bool const;
        // последовательность префиксов, последовательность суффиксов, ф-ция для получения элемента таблицы
    auto Equivalent(const Table& table) -> std::optional<std::string> const;
};

template <typename Table>
auto TeacherLanguage<Table>::Contains(const std::string& s) -> bool const {
    return false;
}
template <typename Table>
auto TeacherLanguage<Table>::Equivalent(const Table& table) -> std::optional<std::string> const {
    return std::nullopt;
}

}  // learner
