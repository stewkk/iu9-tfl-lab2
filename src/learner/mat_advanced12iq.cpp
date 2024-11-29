#include <learner/mat_advanced12iq.hpp>

#include <stdexcept>
#include <format>
#include <iostream>
#include <sstream>

namespace learner {

namespace {

boost::process::environment GetCustomEnv(std::int32_t seed, std::int32_t height, std::int32_t width) {
    boost::process::environment env = boost::this_process::environment();
    env["RANDOM_SEED"] = std::to_string(seed);
    env["HEIGHT"] = std::to_string(height);
    env["WIDTH"] = std::to_string(width);
    return env;
}

auto QueryContains(std::istream& in, std::ostream& out, const std::string_view s) -> bool {
    if (s == "") {
        return false;
    }

    out << std::format("isin\n{}", s) << std::endl;

    std::string ans;
    in >> ans;

    std::cerr << std::format("QueryContains({}) == {}\n", s, ans);

    if (ans == "True") {
        return true;
    } else if (ans == "False") {
        return false;
    }

    throw std::logic_error("got unexpected message from MAT");
}

}  // namespace


MATadvanced12iq::MATadvanced12iq(std::int32_t seed, std::int32_t height,
                                 std::int32_t width)
    : mat_in_(), mat_out_(), mat_process_() {
  mat_process_ = boost::process::child(
      "python mat/advanced12iq/main.py", boost::process::std_out > mat_out_,
      boost::process::std_in < mat_in_, GetCustomEnv(seed, height, width));
}

MATadvanced12iq::~MATadvanced12iq() {
    mat_in_ << "end" << std::endl;
    mat_process_.wait();
}

auto MATadvanced12iq::Contains(const std::string_view s) -> bool {
    if (auto it = cache_.find(std::string(s)); it != cache_.end()) {
        return it->second;
    }

    bool res = QueryContains(mat_out_, mat_in_, s);
    cache_[std::string(s)] = res;
    return res;
}

auto MATadvanced12iq::Equivalent(const Table& table) -> std::optional<std::string> {
    std::ostringstream ss;
    ss << "table" << std::endl;
    for (const auto& el : table.GetSuffixes()) {
        if (el == "") {
            ss << "e ";
            continue;
        }
        ss << el << ' ';
    }
    ss << std::endl;

    for (const auto& prefix : table.GetPrefixes()) {
        if (prefix == "") {
            ss << "e ";
        } else {
            ss << prefix << ' ';
        }
        for (const auto &suffix : table.GetSuffixes()) {
            auto contains = Contains(prefix+suffix);
            ss << contains << ' ';
        }
        ss << std::endl;
    }

    ss << "end" << std::endl;

    std::cerr << ss.str() << std::flush;
    mat_in_ << ss.str() << std::flush;

    std::string ans;
    mat_out_ >> ans;
    if (ans == "TRUE") {
        return std::nullopt;
    }
    return ans;
}

}  // namespace learner

