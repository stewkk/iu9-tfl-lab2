#pragma once

#include <concepts>
#include <cstdint>
#include <iostream>
#include <string_view>

#include <boost/process.hpp>

namespace learner {

template <typename L>
concept Language =
requires(L l, std::string_view s) {
    {l.Contains(s)} -> std::same_as<bool>;
};

class MATadvanced12iq {
public:
        MATadvanced12iq(std::int32_t seed, std::int32_t height, std::int32_t width);
        ~MATadvanced12iq();
        MATadvanced12iq(const MATadvanced12iq&) = delete;
        MATadvanced12iq& operator=(const MATadvanced12iq&) = delete;
        auto Contains(const std::string_view s) -> bool;
public:
        boost::process::child mat_process_;
        boost::process::ipstream mat_out_;
        boost::process::opstream mat_in_;
};

}  // namespace learner
