#include <learner/learner.hpp>

#include <learner/learner_impl.hpp>

namespace learner {

auto BuildAutomata(std::int32_t n, std::int32_t m) -> void {
    Learner l(n, m, TeacherLanguage<Table>());
    l.Build();
}

}  // namespace learner
