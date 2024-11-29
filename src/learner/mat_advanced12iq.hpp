#pragma once

#include <cstdint>
#include <string_view>
#include <optional>
#include <unordered_map>
#include <string>

#include <boost/process.hpp>

#include <learner/table.hpp>

namespace learner {

class MATadvanced12iq {
public:
        MATadvanced12iq(std::int64_t seed, std::int32_t height, std::int32_t width);
        ~MATadvanced12iq();
        MATadvanced12iq(const MATadvanced12iq&) = delete;
        MATadvanced12iq& operator=(const MATadvanced12iq&) = delete;

        auto Contains(const std::string_view s) -> bool;
        auto Equivalent(const Table& table) -> std::optional<std::string>;
private:
        boost::process::child mat_process_;
        boost::process::ipstream mat_out_;
        boost::process::opstream mat_in_;

        std::unordered_map<std::string, bool> cache_;
};

}  // namespace learner
