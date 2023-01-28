#include "commands.hpp"

#include "game.hpp"


void DirCommand::trigger(Game& game)
{
    auto [log, map] = game.interface();

    auto res = map.move(dir);
    if (!res) {
        log.fail("Why are you trying to walk off the map? There's a flag to be found!");
        return;
    }
    auto [prev, curr] = *res;
    log.success(monosodiumglutamate::move::get_msg(prev, curr));
    game.push_loot();
}

void UpCommand::trigger(Game& game)
{
    auto [log, map] = game.interface();
    if (!map.up()) {
        log.fail("You can't go up further.");
        return;
    }
    log.success("You went up.");
    game.push_loot();
}

void DownCommand::trigger(Game& game)
{
    auto [log, map] = game.interface();
    if (!map.down()) {
        log.fail("You can't go down further.");
        return;
    }
    log.success("You went down.");
    game.push_loot();
}

void RandomAccessFloorCommand::trigger(Game& game)
{
    auto [log, map] = game.interface();
    if (map.pos().z == floor) {
        log.warn(
            "You're already at this floor. We should've installed some sign posts in this simulation, but didn't "
            "bother.");
        return;
    }
    if (!map.go_to_floor(floor)) {
        log.fail("You can't go to that floor from this elevator.");
        return;
    }
    log.success("Ding. You rode the elevator to floor ", floor, ".");
    game.push_loot();
}

void InteractCommand::trigger(Game& game)
{
    auto [log, map] = game.interface();
    log.success(monosodiumglutamate::interact::get_msg(map.get(map.pos())));
}

void TeleportCommand::trigger(Game& game)
{
    auto [log, map] = game.interface();
    if (!map.set_pos(pos)) {
        log.info("Unable to teleport to ", pos, ".");
        return;
    }
    log.info("Teleported to ", pos, ".");
}

void EndCommand::trigger(Game& game)
{
    auto [log, _] = game.interface();
    log.info("Ending the game...");
    game.finish();
}

void HelpCommand::trigger(Game& game)
{
    auto [log, _] = game.interface();
    log.info(help_text);
}
// Handled in the game class.
void RestartCommand::trigger(Game& game)
{
    game.restart();
}

void FartCommand::trigger(Game& game)
{
    auto [log, _] = game.interface();
    log.success(
        "pppffffffft. pfft. pffftt pfft. You felt embarassed but relieved as stress and tension left your body.");
}

void SingCommand::trigger(Game& game)
{
    auto [log, _] = game.interface();
    log.success(you_know_the_rules);
}