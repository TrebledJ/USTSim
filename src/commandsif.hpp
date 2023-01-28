#ifndef COMMANDSIF_HPP
#define COMMANDSIF_HPP

#include "commands.hpp"

#include <memory>
#include <string>


namespace cmd
{
    std::unique_ptr<Command> parse(const std::string_view& input);
} // namespace cmd


#endif