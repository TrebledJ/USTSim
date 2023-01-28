#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include "defines.hpp"
#include "map.hpp"

#include <optional>
#include <ostream>
#include <string>
#include <vector>


struct Vec3
{
    int x = 0, y = 0, z = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v)
    {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};


enum Direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT
};

class GameMap
{
    std::vector<const char*> levels;
    Vec3 m_pos;

public:
    template <typename... Ts>
    GameMap(Ts&&... ls) : levels{std::forward<Ts>(ls)...}
    {}

    GameMap(const GameMap&) = delete;
    GameMap(GameMap&&) = delete;
    GameMap& operator=(const GameMap&) = delete;
    GameMap& operator=(GameMap&&) = delete;

    bool set_pos(const Vec3& pos)
    {
        if (!is_valid_pos(pos))
            return false;
        m_pos = pos;
        return true;
    }

    char get(int x, int y, int z) const
    {
        if (!is_valid_pos(x, y, z))
            return '\0';

        return unsafe_get(x, y, z);
    }

    char get(const Vec3& v) const
    {
        return get(v.x, v.y, v.z);
    }

    char unsafe_get(int x, int y, int z) const
    {
        return levels[z][y * WIDTH + x];
    }

    bool is_valid_pos(int x, int y, int z) const
    {
        return !(z < 0 || z >= levels.size() || x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT);
    }

    bool is_valid_pos(const Vec3& v) const
    {
        return is_valid_pos(v.x, v.y, v.z);
    }

    const Vec3& pos() const
    {
        return m_pos;
    }

    std::optional<std::pair<char, char>> move(Direction dir)
    {
        char curr = get(m_pos);
        Vec3 next = move_in(m_pos, dir);
        if (char c = get(next); c) {
            m_pos = next;
            return {{curr, c}};
        } else {
            return std::nullopt;
        }
    }

    bool up()
    {
        if (m_pos.z + 1 >= levels.size())
            return false;
        m_pos.z += 1;
        return true;
    }

    bool down()
    {
        if (m_pos.z == 0)
            return false;
        m_pos.z -= 1;
        return true;
    }

    bool go_to_floor(int floor)
    {
        if (floor < 0 || floor >= levels.size())
            return false;

        if (unsafe_get(m_pos.x, m_pos.y, m_pos.z) != Tile::LIFT || unsafe_get(m_pos.x, m_pos.y, floor) != Tile::LIFT) {
            // No lifts at either end. Can't go to floor via lift.
            return false;
        }

        m_pos.z = floor;
        return true;
    }

    static Vec3 move_in(const Vec3& m_pos, Direction dir)
    {
        static Vec3 dirs[] = {Vec3{0, -1, 0}, Vec3{-1, 0, 0}, Vec3{0, 1, 0}, Vec3{1, 0, 0}};
        auto [x, y, z] = dirs[dir];
        return Vec3{m_pos.x + x, m_pos.y + y, m_pos.z + z};
    }
};

#endif