#include "loot.hpp"

#include "defines.hpp"

#include <map>
#include <string>


using namespace std;


// For lack of a better name, this is a class that pukes out chars. It's
// supplied a string to work with and will output characters from that string,
// cycling it.
class char_puker
{
    string_view s;
    size_t i = 0;

public:
    char_puker() : s{""} {}
    char_puker(const string_view& s) : s{s} {}

    char get()
    {
        char c = s[i];
        advance();
        return c;
    }

    void reset()
    {
        i = 0;
    }

private:
    void advance()
    {
        i = (i + 1 == s.size() ? 0 : i + 1);
    }
};


// map<char, char_puker> m {
//     {'B', char_puker{"f"}},
//     {'.', char_puker{"irebiD_rev1i"}},
//     {'b', char_puker{"rd{"}},
//     {'c', char_puker{"1d_y0ub0"}},
//     {'e', char_puker{"_rs"}},
//     {'L', char_puker{"rroW_a_"}},
//     {'|', char_puker{"8g_in"}},
//     {'m', char_puker{"0ok1ok2ok3ok4ok5ok6okAok
//     {'G', char_puker{"_on"}},
//     {'E', char_puker{"e"}},
//     {'s', char_puker{"n"}},
//     {'T', char_puker{"_"}},
//     {'#', char_puker{"th3_l"}},
//     {'C', char_puker{"br42y?"}},
//     {' ', char_puker{"}"}},
// };


map<char, char_puker> lootbox = {
    {'B', char_puker{"firebird{l00k_d33p3r"}},
    {'.', char_puker{"irebiD_rev1i"}},
    {'b', char_puker{"rd{H0w}N3VER_G0NN4_l3t-YU-DWN"}},
    {'c', char_puker{"1d_y0ub0bird{s_FlY_1n_7H3_skY"}},
    {'e', char_puker{"_rsfirewaterearthlightning"}},
    {'L', char_puker{"rroW_a_r3ver5e-fl4g"}},
    {'|', char_puker{"8g_ind{d0_y0U-Kn0w_th3_wa3"}},
    {'m', char_puker{"0ok1ok2ok3ok4ok5ok6okAok"}},
    {'G', char_puker{"_on3_s3cre7_t0-kNoW}"}},
    {'E', char_puker{"e...;;;''']]]===}}}_U_UP"}},
    {'s', char_puker{"n_r3ver5e}_0"}},
    {'T', char_puker{"_N3V3R_G0NN4_G1V3"}},
    {'#', char_puker{"th3_l4se2_c4ts_are_0u7_to_g3t_y0u}"}},
    {'C', char_puker{"br42y?_sC4ry?_sh4dy?_p3rry?_0f_CoUr53}"}},
    {' ', char_puker{"}{o}_{o}{}{<}_{<}{"}},
    {'*', char_puker{"alj18gq]as.df-10ajo1f}{}"}},
    {'A', char_puker{"Th3_pr0f3ss0r_l00ks_at_y0u_with_a_d3adp4n_f4ce."}},
    {'H', char_puker{"fibrd{Br0k3n_t3xt?"}},
    {'R', char_puker{"r0w_r0w_r0w_y0ur_b0at_g3ntly_d0wn_th3_s3a"}},
    {'S', char_puker{"reird{wh3r3_d03s_th3_p4th_go?"}},
};

auto default_loot =
    char_puker{R"(hw!S6s/gW>7)GoX+BC@(n3<D*e0UmEH^TFL"%c8v?}Ox,4r-_|`V[NtK2{Rdkq\\QjP.a]yzf&=YM$~;:ZIi 51lJApb\'9#u)"};

namespace loot
{
    char get(char tile)
    {
        if (auto it = lootbox.find(tile); it != lootbox.end())
            return it->second.get();

        return default_loot.get();
    }

    void reset()
    {
        for (auto& [_, puke] : lootbox)
            puke.reset();
        default_loot.reset();
    }
} // namespace loot
