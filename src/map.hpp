#ifndef MAP_HPP
#define MAP_HPP

#include "defines.hpp"

#include <string>


inline constexpr int WIDTH = 20;
inline constexpr int HEIGHT = 11;
inline constexpr int NUM_FLOORS = 3;

enum Tile
{
    FLOOR = '.',
    SUNDIAL = 'b',
    CHINAGARDEN = 'G',
    LIFT = '|',
    ESCALATOR_ABOVE = 'E',
    ESCALATOR_BELOW = 'e',
    STAIRS_ABOVE = 'S',
    STAIRS_BELOW = 's',
    WALL = '#',
    CONCOURSE = 'c',
    MUSHROOM = 'm',
    LIBRARY = 'L',
    BUS_STATION = 'B',
    SECURITY_STATION = '*',
    CLASSROOM = 'C',
    LTA = 'A',
    SCIHOME = 'H',
    EMPTY = ' ',
    ARR = 'R',
    TOILET = 'T',
};

// Map.
namespace M
{
    void l();             // Load.
    const char* g(int f); // Get.
} // namespace M

// Path
namespace P
{
    void l();             // Load.
    const char* g(int f); // Get.
} // namespace P

namespace monosodiumglutamate
{
    namespace move
    {
        std::string_view get_msg(Tile from, Tile to);
        std::string_view get_msg(char from, char to);
    } // namespace move

    namespace tile
    {
        std::pair<std::string_view, std::string_view> get_name(Tile tile);
        std::pair<std::string_view, std::string_view> get_name(char tile);
    } // namespace tile

    namespace interact
    {
        std::string_view get_msg(Tile tile);
        std::string_view get_msg(char tile);
    } // namespace interact
} // namespace monosodiumglutamate

#endif