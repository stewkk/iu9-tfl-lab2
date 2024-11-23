#pragma once

#include <string>
#include <string_view>

#include <learner/mat_language.hpp>

namespace learner {

auto GetMinPrefixInLang(Language auto& lang, const std::string_view path_in_lang) -> std::string {
    for (auto it = path_in_lang.cbegin(); it != path_in_lang.cend(); ++it) {
        std::string_view path_prefix(path_in_lang.cbegin(), it+1);
        if (lang.Contains(path_prefix)) {
            return std::string(path_prefix);
        }
    }
    return std::string(path_in_lang);
}

}  // namespace learner
