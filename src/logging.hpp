#ifndef LOG_HPP
#define LOG_HPP

#include "defines.hpp"

#include <iostream>
#include <thread>


enum Code
{
    CODE_FG_RED = 31,
    CODE_FG_GREEN = 32,
    CODE_FG_YELLOW = 33,
    CODE_FG_BLUE = 34,
    CODE_FG_MAGENTA = 35,
    CODE_FG_CYAN = 36,
    CODE_FG_GREY = 37,
    CODE_FG_DEFAULT = 39,
    CODE_BG_RED = 41,
    CODE_BG_GREEN = 42,
    CODE_BG_YELLOW = 43,
    CODE_BG_BLUE = 44,
    CODE_BG_MAGENTA = 45,
    CODE_BG_CYAN = 46,
    CODE_BG_GREY = 47,
    CODE_BG_DEFAULT = 49
};

class Modifier
{
    Code code;

public:
    constexpr Modifier(Code pCode) : code(pCode) {}

    friend std::ostream& operator<<(std::ostream& os, const Modifier& mod)
    {
        return os << "\033[" << mod.code << "m";
    }
};

inline constexpr auto FG_RED = Modifier{CODE_FG_RED};
inline constexpr auto FG_YELLOW = Modifier{CODE_FG_YELLOW};
inline constexpr auto FG_GREEN = Modifier{CODE_FG_GREEN};
inline constexpr auto FG_BLUE = Modifier{CODE_FG_BLUE};
inline constexpr auto FG_MAGENTA = Modifier{CODE_FG_MAGENTA};
inline constexpr auto FG_CYAN = Modifier{CODE_FG_CYAN};
inline constexpr auto FG_GREY = Modifier{CODE_FG_GREY};
inline constexpr auto FG_DEFAULT = Modifier{CODE_FG_DEFAULT};
inline constexpr auto BG_RED = Modifier{CODE_BG_RED};
inline constexpr auto BG_YELLOW = Modifier{CODE_BG_YELLOW};
inline constexpr auto BG_GREEN = Modifier{CODE_BG_GREEN};
inline constexpr auto BG_BLUE = Modifier{CODE_BG_BLUE};
inline constexpr auto BG_MAGENTA = Modifier{CODE_BG_MAGENTA};
inline constexpr auto BG_CYAN = Modifier{CODE_BG_CYAN};
inline constexpr auto BG_GREY = Modifier{CODE_BG_GREY};
inline constexpr auto BG_DEFAULT = Modifier{CODE_BG_DEFAULT};


template <typename T, typename... Ts>
constexpr bool is_one_of_v = (std::is_same_v<std::decay_t<T>, Ts> || ...);


template <typename T>
constexpr bool is_string_like_v = is_one_of_v<T, std::string_view, std::string, char*, const char*>;


class Logger
{
    std::ostream& os;

public:
    Logger(std::ostream& os = std::cout) : os{os} {}

    template <typename... Ts>
    std::ostream& success(Ts&&... args)
    {
        return colored(FG_GREEN, std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    std::ostream& info(Ts&&... args)
    {
        return colored(FG_BLUE, std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    std::ostream& warn(Ts&&... args)
    {
        return colored(FG_YELLOW, std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    std::ostream& fail(Ts&&... args)
    {
        return colored(FG_RED, std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    std::ostream& colored(const Modifier& color, Ts&&... args)
    {
        os << color;
#if LOGGING_TYPED
        (print(os, std::forward<Ts>(args)), ...);
#else
        ((os << std::forward<Ts>(args)), ...);
#endif
        os << FG_DEFAULT;
        os << std::endl;
        return os;
    }

private:
#if LOGGING_TYPED
    template <typename T, std::enable_if_t<is_string_like_v<T>, int> = 0>
    void print(std::ostream& os, T&& arg)
    {
        using namespace std::literals::chrono_literals;

        for (char c : arg) {
            std::this_thread::sleep_for(std::chrono::milliseconds(LOGGING_DELAY));
            os << c;
        }
    }

    template <typename T, std::enable_if_t<!is_string_like_v<T>, int> = 0>
    void print(std::ostream& os, T&& arg)
    {
        os << std::forward<T>(arg);
    }
#endif
};

#endif