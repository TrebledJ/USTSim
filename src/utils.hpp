#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <charconv>
#include <optional>
#include <string>
#include <vector>


namespace utils
{
    inline bool is_delimiter(char c)
    {
        return c == ' ';
    }

    inline std::vector<std::string_view> split(const std::string_view& s)
    {
        std::vector<std::string_view> v;
        size_t i = 0, j = 0;
        while (j < s.size()) {
            if (is_delimiter(s[j])) {
                if (j > i) {
                    v.push_back(s.substr(i, j - i));
                    i = j + 1;
                } else if (i == j) {
                    i++;
                }
            }
            j++;
        }
        if (j > i)
            v.push_back(s.substr(i, j - i));
        return v;
    }

    template <typename T, typename U>
    inline bool contains(const T& container, const U& elem)
    {
        return std::find(container.begin(), container.end(), elem) != container.end();
    }

    inline bool contains(const std::string& container, char elem)
    {
        return container.find(elem) != std::string::npos;
    }

    template <typename T>
    inline std::vector<T> exclude(const std::vector<T>& v, const std::vector<T>& blacklist)
    {
        std::vector<T> out;
        for (const auto& s : v)
            if (!contains(blacklist, s))
                out.push_back(s);
        return out;
    }

    inline std::optional<int> to_int(const std::string& input)
    {
        return {std::stoi(input)};
    }

    inline std::optional<int> to_int(const std::string_view& input)
    {
        int out;
        const std::from_chars_result result = std::from_chars(input.data(), input.data() + input.size(), out);
        return result.ec == std::errc{} ? std::optional{out} : std::nullopt;
    }

    inline size_t djb2(const std::string_view& s)
    {
        size_t hash = 5381;
        for (char c : s)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash;
    }

} // namespace utils

#endif