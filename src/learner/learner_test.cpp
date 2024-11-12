#include <gtest/gtest.h>

#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/system_executor.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/v2/environment.hpp>
#include <boost/process/v2/popen.hpp>

#include <format>
#include <string_view>
#include <cstdint>
#include <unordered_map>

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

template <typename RWStream>
auto QueryContains(RWStream& stream, const std::string_view s) -> bool {
    if (s == "") {
        return false;
    }

    boost::asio::write(stream, boost::asio::buffer(std::format("isin\n{}\n", s)));

    std::string ans;
    auto n_read = boost::asio::read_until(stream, boost::asio::dynamic_buffer(ans), '\n');
    if (n_read > 0) {
        ans.erase(ans.end()-1);
    }

    std::cerr << std::format("QueryContains({}) == {}\n", s, ans);

    if (ans == "True") {
        return true;
    }
    return false;
}

boost::process::v2::process_environment GetCustomEnv(std::int32_t seed, std::int32_t height, std::int32_t width) {
  std::unordered_map<boost::process::v2::environment::key,
                     boost::process::v2::environment::value>
      custom_env = {
          {"RANDOM_SEED", "10"},
          {"WIDTH", "2"},
          {"HEIGHT", "2"},
      };
  for (auto &&var : boost::process::v2::environment::current()) {
    custom_env.emplace(var.key(), var.value());
  }
  return custom_env;
}

class MATadvanced12iq {
public:
        MATadvanced12iq(std::int32_t seed, std::int32_t height, std::int32_t width);
        // TODO: delete copy constructors
        ~MATadvanced12iq();
        auto Contains(const std::string_view s) -> bool;
private:
        boost::process::v2::popen mat_process_;
};

MATadvanced12iq::MATadvanced12iq(std::int32_t seed, std::int32_t height,
                                 std::int32_t width)
    : mat_process_(boost::asio::system_executor{},
                   boost::process::v2::environment::find_executable("python"),
                   {"mat/advanced12iq/main.py"},
                   GetCustomEnv(seed, height, width)) {}

MATadvanced12iq::~MATadvanced12iq() {
    boost::asio::write(mat_process_, boost::asio::buffer("end\n"));
    mat_process_.wait();
}

auto MATadvanced12iq::Contains(const std::string_view s) -> bool {
  return QueryContains(mat_process_, s);
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

