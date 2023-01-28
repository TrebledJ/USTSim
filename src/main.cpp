#include "defines.hpp"
#include "game.hpp"
#include "utils.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <memory>

using namespace std;


ostream& operator<<(ostream& os, const vector<string_view>& v)
{
    for (const auto& e : v)
        os << " " << e;
    return os;
}

#if DEBUG
// void compress(const char* src)
// {
//     uint8_t buffer[256];
//     P::comp::store(buffer, src);

//     for (int i = 0; buffer[i]; i++)
//         printf("\\x%02x", buffer[i]);
//     std::cout << "\n";
// }

void print_map(const char* map)
{
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            std::cout << map[i * WIDTH + j];
        }
        std::cout << "\n";
    }
}
#endif

int main()
{
#if USE_ENCRYPTED_M
    M::l();
#endif
#if USE_ENCRYPTED_P
    P::l();
#endif
#if DEBUG
    // compress(P::F[i]);
#endif
#if LOGGING_TYPED
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    // print_map(M::F[2]);

    Game game;
    while (game) {
        game.loop();
    }
}
