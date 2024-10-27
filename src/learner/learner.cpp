#include <learner/learner.hpp>

#include <iostream>

namespace learner {

auto BuildAutomata(std::int32_t n, std::int32_t m) -> void {
    std::cout << "BuildAutomata called with: " << n << ' ' << m << std::endl;
}

}  // namespace learner
