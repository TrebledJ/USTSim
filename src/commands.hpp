#ifndef COMMANDS_H
#define COMMANDS_H

#include "gamemap.hpp"

#include <string>


class Game;

class Command
{
public:
    virtual ~Command() = default;

    virtual std::string sources() const
    {
        return ""; // All tiles by default.
    }
    virtual void trigger(Game& game) = 0;

    template <typename T>
    bool is() const
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }
};

struct DirCommand : public Command
{
    Direction dir;

    DirCommand(Direction dir) : dir{dir} {}

    void trigger(Game& game);
};

struct UpCommand : public Command
{
    std::string sources() const
    {
        return "ES";
    }

    void trigger(Game& game);
};

struct DownCommand : public Command
{
    std::string sources() const
    {
        return "es";
    }

    void trigger(Game& game);
};

struct RandomAccessFloorCommand : public Command
{
    int floor;

    RandomAccessFloorCommand(int floor) : floor{floor} {}

    std::string sources() const
    {
        return "|";
    }

    void trigger(Game& game);
};

struct InteractCommand : public Command
{
    void trigger(Game& game);
};

struct TeleportCommand : public Command
{
    Vec3 pos;
    TeleportCommand(int x, int y, int z) : pos{x, y, z} {}

    void trigger(Game& game);
};

struct EndCommand : public Command
{
    void trigger(Game& game);
};

struct HelpCommand : public Command
{
    static constexpr std::string_view help_text = R"(USTSim. The simulation you all need.

Commands:
    help            Prints this lovely, helpful text.
    go <dir>        Go in a certain direction on the same floor.
                        <dir>: north, south, east, west
    go above        Go to the floor above. Only available at certain locations.
    go below        Go to the floor below. Only available at certain locations.
    go floor <n>    Goes to floor <n>.
    interact        Interact with things in the current location.
    restart         Restart from zero, nothing, nada. Like when a car stalls, what do you do? You restart it!
    end             End this madness.
)";

    void trigger(Game&);
};

struct RestartCommand : public Command
{
    // Handled in the game class.
    void trigger(Game& game);
};


struct FartCommand : public Command
{
    void trigger(Game& game);
};

struct SingCommand : public Command
{
    static constexpr std::string_view you_know_the_rules = R"(Let's sing! https://www.youtube.com/watch?v=dQw4w9WgXcQ

Never gonna give you up!
Never gonna let you down!
Never gonna run around and desert you~
Never gonna make you cry!
Never gonna say goodbye!
Never gonna tell a lie and hurt you~)";

    void trigger(Game& game);
};


#endif