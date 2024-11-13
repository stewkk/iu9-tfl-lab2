#include <gtest/gtest.h>

#include <boost/process.hpp>

#include <iostream>
#include <format>
#include <string_view>
#include <cstdint>
#include <stdexcept>

namespace learner {

template <typename L>
concept Language =
requires(L l, std::string_view s) {
    {l.Contains(s)} -> std::same_as<bool>;
};

auto GetMinPrefixInLang(Language auto& lang, const std::string_view path_in_lang) -> std::string {
    for (auto it = path_in_lang.cbegin(); it != path_in_lang.cend(); ++it) {
        std::string_view path_prefix(path_in_lang.cbegin(), it+1);
        if (lang.Contains(path_prefix)) {
            return std::string(path_prefix);
        }
    }
    return std::string(path_in_lang);
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

boost::process::environment GetCustomEnv(std::int32_t seed, std::int32_t height, std::int32_t width) {
    boost::process::environment env = boost::this_process::environment();
    env["RANDOM_SEED"] = std::to_string(seed);
    env["HEIGHT"] = std::to_string(height);
    env["WIDTH"] = std::to_string(width);
    return env;
}

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
  return QueryContains(mat_out_, mat_in_, s);
}

} // namespace learner

using std::literals::operator""s;
using std::literals::operator""sv;

TEST(LearnerTest, GetMinPrefixInLang) {
    auto seed = 10;
    auto width = 2;
    auto height = 2;
    // NOTE:
    //      _
    //     |   |
    //     | | |
    //      ‾ ‾
    auto mat = learner::MATadvanced12iq(seed, width, height);

    auto got = learner::GetMinPrefixInLang(mat, "EENWS"sv);

    ASSERT_EQ(got, "EEN"s);
}
