#include "game.hpp"

#include "commands.hpp"
#include "commandsif.hpp"
#include "defines.hpp"
#include "loot.hpp"
#include "utils.hpp"


using namespace std;


void Game::start()
{
    log.info("Welcome to UST!");
    gmap.set_pos({WIDTH - 1, HEIGHT - 1, 0});
    trail = 0;
    loot.clear();
    push_loot();
}

void Game::restart()
{
    log.info("Are you sure you wanna restart?");
    log.info("That was a rhetorical question.");
    log.info("Let's restart!");
    std::this_thread::sleep_for(3s);
    log.info();
    reset_loot();
    start();
}

void Game::loop()
{
    auto [pre, name] = monosodiumglutamate::tile::get_name(gmap.get(gmap.pos()));
    log.info("You're now ", pre, ' ', name, ".");

#if DEBUG
    log.info("Coords: ", gmap.pos(), ", ", gmap.get(gmap.pos()));
    log.info("Path: ", pmap.get(gmap.pos()));
    log.info("Loot: ", loot);
#endif

    string input;
    cout << "> ";
    getline(cin, input);

    if (input.empty()) {
        log.info("Type 'help' for help.");
        return;
    }

    auto cmd = cmd::parse(input);

    // Is it a valid command?
    if (!cmd) {
        log.fail("You consulted your memory banks, but you don't know how to '", input, "'.");
        return;
    }

    // Check if the command can be executed at the current tile.
    auto curr_tile = gmap.get(gmap.pos());
    if (!cmd->sources().empty() && !utils::contains(cmd->sources(), curr_tile)) {
        log.fail("You can't do that in your current position.");
        return;
    }

    // Execute the command.
    cmd->trigger(*this);
}

void Game::reset_loot()
{
    loot::reset();
}

void Game::push_loot()
{
    // Importante.
    auto pos = gmap.pos();
    if (pmap.get(pos) == 'X') {
        trail++;
    }
    loot += loot::get(gmap.get(pos));
}

void Game::finish()
{
    // Importante.
    auto [pre, name] = monosodiumglutamate::tile::get_name(gmap.get(gmap.pos()));
    log.info("You ended up ", pre, " ", name, " ", gmap.pos(), ".");
    if (utils::djb2(loot + to_string(loot.size())) == FLAG_HASH && loot.size() == trail && trail == FLAG_LEN) {
        log.success("Seems like you found the flag!");
        log.success("Congratulations! Enjoy the rest of the CTF!");
        log.success("Here's your loot: ", loot, ".");
    } else {
        log.warn("Seems like you didn't find the flag. :(");
        log.warn("Better luck next time!");
    }
    running = false;
}
