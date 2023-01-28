#include "commandsif.hpp"

#include "defines.hpp"
#include "utils.hpp"

#include <algorithm>
#include <memory>
#include <vector>


using namespace std;

namespace tok
{
    inline constexpr string_view number_wildcard = "%d";
    inline constexpr string_view string_wildcard = "%s";
    inline constexpr string_view word_wildcard = "%w";

    struct matcher
    {
        vector<string_view> matches;

        matcher(vector<string_view>&& eqliterals) : matches{eqliterals} {}

        bool accepts(const string_view& input) const;

        bool is_arg() const
        {
            return matches[0][0] == '%';
        }
    };

    template <typename... Ts>
    inline matcher token(const Ts&... ss)
    {
        return matcher{{ss...}};
    }

    inline matcher go = matcher{{"go", "g", "walk", "amble", "shuffle", "run", "visit", "step"}};
    inline matcher above = matcher{{"above", "a"}};
    inline matcher below = matcher{{"below", "b"}};
#if INVERTED_MAP
    inline matcher up = matcher{{"south", "s", "up"}};
    inline matcher down = matcher{{"north", "n", "down"}};
    inline matcher left = matcher{{"east", "e", "left"}};
    inline matcher right = matcher{{"west", "w", "right"}};
#else
    inline matcher up = matcher{{"north", "n", "up"}};
    inline matcher down = matcher{{"south", "s", "down"}};
    inline matcher left = matcher{{"west", "w", "left"}};
    inline matcher right = matcher{{"east", "e", "right"}};
#endif
    inline matcher floor = matcher{{"floor", "f", "level"}};
    inline matcher interact = matcher{{"interact", "i"}};
    inline matcher help = matcher{{"help", "h"}};
    inline matcher restart = matcher{{"restart"}};
    inline matcher number = token(number_wildcard);
    inline matcher string = token(string_wildcard);
    inline matcher word = token(word_wildcard);

    inline vector<string_view> stop_words = {"to", "by"};

    inline optional<vector<string_view>> match(const vector<matcher>& ms, const vector<string_view>& xs)
    {
        if (ms.size() != xs.size())
            return nullopt;

        vector<string_view> args;

        // Check if input matches cmd, and collect args.
        for (size_t i = 0; i < xs.size(); i++) {
            if (!ms[i].accepts(xs[i]))
                return nullopt;

            if (ms[i].is_arg())
                args.push_back(xs[i]);
        }

        return {args};
    }
} // namespace tok


bool tok::matcher::accepts(const std::string_view& input) const
{
    if (matches[0] == tok::number_wildcard)
        return std::all_of(input.begin(), input.end(), [](char c) { return isdigit(c); });

    if (matches[0] == tok::string_wildcard)
        return true;

    if (matches[0] == tok::word_wildcard)
        return std::all_of(input.begin(), input.end(), [](char c) { return isalpha(c); });

    return utils::contains(matches, input);
}

using cmdgen = function<unique_ptr<Command>(vector<string_view>&&)>;


static vector<pair<vector<tok::matcher>, cmdgen>> cmdmap = {
    {{tok::help}, [](auto&&) { return unique_ptr<Command>(new HelpCommand()); }},
    {{tok::restart}, [](auto&&) { return unique_ptr<Command>(new RestartCommand()); }},
    {{tok::go, tok::up}, [](auto&&) { return unique_ptr<Command>(new DirCommand(UP)); }},
    {{tok::go, tok::down}, [](auto&&) { return unique_ptr<Command>(new DirCommand(DOWN)); }},
    {{tok::go, tok::left}, [](auto&&) { return unique_ptr<Command>(new DirCommand(LEFT)); }},
    {{tok::go, tok::right}, [](auto&&) { return unique_ptr<Command>(new DirCommand(RIGHT)); }},
    {{tok::go, tok::above}, [](auto&&) { return unique_ptr<Command>(new UpCommand()); }},
    {{tok::go, tok::below}, [](auto&&) { return unique_ptr<Command>(new DownCommand()); }},
    {{tok::go, tok::floor, tok::number},
     [](auto&& args) { return unique_ptr<Command>(new RandomAccessFloorCommand(*utils::to_int(args[0]))); }},
    {{tok::interact}, [](auto&&) { return unique_ptr<Command>(new InteractCommand()); }},
    {{tok::token("end", "finish", "quit")}, [](auto&&) { return unique_ptr<Command>(new EndCommand()); }},
    {{tok::token("fart")}, [](auto&&) { return unique_ptr<Command>(new FartCommand()); }},
    {{tok::token("sing")}, [](auto&&) { return unique_ptr<Command>(new SingCommand()); }},
#if DEBUG
    {{tok::token("teleport", "tp"), tok::number, tok::number, tok::number},
     [](auto&& args) {
         return unique_ptr<Command>(
             new TeleportCommand(*utils::to_int(args[0]), *utils::to_int(args[1]), *utils::to_int(args[2])));
     }},
#endif
};


namespace cmd
{
    unique_ptr<Command> parse_impl(const std::vector<std::string_view>& input)
    {
        if (input.empty())
            return nullptr;

        for (const auto& [cmd, gen] : cmdmap)
            if (auto margs = tok::match(cmd, input); margs)
                return gen(std::move(*margs));

        return nullptr;
    }

    unique_ptr<Command> parse(const std::string_view& input)
    {
        return parse_impl(utils::exclude(utils::split(input), tok::stop_words));
    }
} // namespace cmd
