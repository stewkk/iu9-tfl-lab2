#include <gmock/gmock.h>

#include <optional>
#include <string>

#include <learner/teacher.hpp>

namespace learner {

class MockTeacher {
public:
    MOCK_METHOD(bool, Contains, (const std::string&), (const));
    MOCK_METHOD(std::optional<std::string>, Equivalent, (const TeacherLanguage::Table&), (const));
};

TEST(LearnerTest, GuessesEmptyLabyrinth) {
    MockTeacher teacher;
}

}  // namespace learner
