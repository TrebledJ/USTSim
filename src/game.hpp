#ifndef GAME_HPP
#define GAME_HPP

#include "defines.hpp"
#include "gamemap.hpp"
#include "logging.hpp"
#include "map.hpp"

#include <string>


class Game
{
    bool running = true;

    Logger log;
    GameMap gmap{M::g(0), M::g(1), M::g(2)};
    GameMap pmap{P::g(0), P::g(1), P::g(2)};

    size_t trail;
    std::string loot;

    static constexpr size_t FLAG_HASH = 16573440731492339550ULL;
    static constexpr size_t FLAG_LEN = 60;

public:
    Game()
    {
        start();
    }

    std::pair<Logger&, GameMap&> interface()
    {
        return {log, gmap};
    }

    void start();
    void restart();

    void loop();

    void reset_loot();
    void push_loot();

    void finish();

    operator bool() const
    {
        return running;
    }
};


#endif