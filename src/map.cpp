#include "map.hpp"

#include <map>
#include <random>


namespace random_
{
    std::random_device rd;
    std::mt19937 gen(rd());

    int randint(int min, int max)
    {
        return std::uniform_int_distribution<>(min, max)(gen);
    }
} // namespace random_

/**
 * Select and choose different options.
 */
struct options
{
    std::vector<std::string_view> texts;

    std::string_view choose() const
    {
        if (texts.empty())
            return "";
        return texts[random_::randint(0, texts.size() - 1)];
    }
};


namespace M
{
    namespace comp
    {
        // Compress path map into run-length encoded byte strings.
        const char F[NUM_FLOORS][256] = {
            "\x28\x23\x03\x47\x11\x23\x03\x47\x01\x2e\x02\x7c\x01\x2e\x01\x54\x0b\x23\x01\x2a\x03\x47\x02\x2e\x01\x53"
            "\x01\x2e\x02\x45\x02\x53\x09\x2e\x03\x47\x04\x2e\x02\x45\x02\x53\x04\x2e\x01\x62\x04\x2e\x01\x6d\x0c\x63"
            "\x01\x2e\x01\x62\x05\x2e\x01\x6d\x04\x63\x02\x65\x06\x63\x01\x2e\x03\x62\x03\x2e\x01\x6d\x0c\x63\x02\x2e"
            "\x01\x62\x04\x2e\x01\x7c\x0c\x63\x07\x2e\x08\x4c\x0b\x2e\x01\x42",
            "\x04\x43\x03\x48\x06\x2e\x07\x41\x04\x43\x02\x23\x01\x52\x06\x2e\x07\x41\x04\x43\x02\x23\x01\x52\x11\x2e"
            "\x02\x7c\x01\x54\x06\x2e\x01\x43\x01\x54\x01\x43\x01\x54\x03\x43\x05\x2e\x01\x73\x01\x2e\x02\x65\x02\x73"
            "\x02\x2e\x07\x43\x07\x23\x02\x65\x02\x73\x0a\x23\x13\x20\x01\x23\x13\x20\x01\x23\x13\x20\x01\x23\x13\x20"
            "\x07\x4c\x0d\x23",
            "\x03\x43\x01\x2e\x13\x43\x01\x2e\x02\x43\x07\x2e\x0a\x43\x04\x2e\x05\x23\x08\x2e\x03\x43\x01\x2e\x02\x7c"
            "\x01\x54\x05\x23\x01\x2e\x07\x43\x0d\x2e\x1b\x43\x01\x23\x13\x20\x01\x23\x13\x20\x01\x23\x13\x20\x01\x23"
            "\x13\x20\x14\x23",
        };

        void load(char* fill, const char* src)
        {
            while (*src) {
                uint8_t n = *src++;
                char c = *src++;
                while (n--) {
                    *fill++ = c;
                }
            }
        }

        void store(uint8_t* fill, const char* src)
        {
            // Alternating count and char.
            uint8_t count = 1;
            char prev = *src, curr;
            while (*(src + 1)) {
                while ((curr = *(src + 1)) && prev == curr)
                    src++, count++;

                *fill++ = count;
                *fill++ = prev;
                prev = curr;
                count = 0;
            }
            *fill++ = 0;
        }
    } // namespace comp


#if USE_ENCRYPTED_M
    char F[NUM_FLOORS][WIDTH * HEIGHT + 1];
#else
    char F[NUM_FLOORS][WIDTH * HEIGHT + 1] = {{"####################"
                                               "####################"
                                               "GGG#################"
                                               "GGG.||.T###########*"
                                               "GGG..S.EESS........."
                                               "GGG....EESS....b...."
                                               "mcccccccccccc.b....."
                                               "mcccceecccccc.bbb..."
                                               "mcccccccccccc..b...."
                                               "|cccccccccccc......."
                                               "LLLLLLLL...........B"},
                                              {"CCCCHHH......AAAAAAA"
                                               "CCCC##R......AAAAAAA"
                                               "CCCC##R............."
                                               "....||T......CTCTCCC"
                                               ".....s.eess..CCCCCCC"
                                               "#######eess#########"
                                               "#                   "
                                               "#                   "
                                               "#                   "
                                               "#                   "
                                               "LLLLLLL#############"},
                                              {"CCC.CCCCCCCCCCCCCCCC"
                                               "CCC.CC.......CCCCCCC"
                                               "CCC....#####........"
                                               "CCC.||T#####.CCCCCCC"
                                               ".............CCCCCCC"
                                               "CCCCCCCCCCCCCCCCCCCC"
                                               "#                   "
                                               "#                   "
                                               "#                   "
                                               "#                   "
                                               "####################"}};
#endif

    void l()
    {
        for (int i = 0; i < NUM_FLOORS; i++)
            comp::load(F[i], comp::F[i]);
    }


    const char* g(int f)
    {
        return F[f];
    }

} // namespace M


// char G[WIDTH * HEIGHT + 1] = {
//     "####################"
//     "####################"
//     "GGG#################"
//     "GGG.||.T###########*"
//     "GGG..S.EESS........."
//     "XXXXXXXXESS....b...."
//     "Xcccccccc......b....."
//     "XcccccXXXXXXXXXXb..."
//     "XcctccXcc......X...."
//     "X.....X........X...."
//     "XXXXXXX........XXXXX"};

// char ONE[WIDTH * HEIGHT + 1] = {
//     "CCCCHHH......AAAAAAA"
//     "CCCC##R......AAAAAAA"
//     "CCCC##R............."
//     "...._XT......CTCTCCC"
//     ".....XXXess..CCCCCCC"
//     "#######Xess#########"
//     "#                   "
//     "#                   "
//     "#                   "
//     "#                   "
//     "LLLLLLL#############"};

// char TWO[WIDTH * HEIGHT + 1] = {
//     "CCC.CCCCCCCCCCCCCCCC"
//     "CCC.CC.......CCCCCCC"
//     "CCC....#####........"
//     "CCC.XXXXXXXXXXXXXCCC"
//     ".............CCCXCCC"
//     "CCCCCCCCCCCCCCCCXCCC"
//     "#               X   "
//     "#                   "
//     "#                   "
//     "#                   "
//     "####################"};

//

// char G[WIDTH * HEIGHT + 1] = {
//     "####################"
//     "####################"
//     "GGG#################"
//     "GGG.||.T###########*"
//     "GGG..S.EESS........."
//     "0lyglo7.ESS....b...."
//     "pcccccccccccc.b....."
//     "_ccccc3rA-1l3w{db..."
//     "acctccncccccc..r...."
//     "-ccccc'cccccc..i...."
//     "uwuoy_t........berif"};

// char ONE[WIDTH * HEIGHT + 1] = {
//     "CCCCHHH......AAAAAAA"
//     "CCCC##R......AAAAAAA"
//     "CCCC##R............."
//     "...u_dT......CTCTCCC"
//     "....._1Dess..CCCCCCC"
//     "#######_ess#########"
//     "#                   "
//     "#                   "
//     "#                   "
//     "#                   "
//     "LLLLLLL#############"};

// char TWO[WIDTH * HEIGHT + 1] = {
//     "CCC.CCCCCCCCCCCCCCCC"
//     "CCC.CC.......CCCCCCC"
//     "CCC....#####........"
//     "CCC.-r3aD-7He-t3xCCC"
//     ".............CCCTCCC"
//     "CCCCCCCCCCCCCCCC?CCC"
//     "#               }   "
//     "#                   "
//     "#                   "
//     "#                   "
//     "####################"};


namespace P
{
    namespace comp
    {
        // Compress path map into run-length encoded byte strings.
        const char F[NUM_FLOORS][256] = {
            "\x0a\x01\x11\x01\x01\x02\x05\x01\x09\x01\x06\x03\x06\x0a\x01\x01\x03\x01\x0f\x01\x01\x01\x03\x08\x07\x01"
            "\x03\x02\x0c\x02\x01\x02\x02\x01\x05\x0f\x05\x01\x04\x01\x03\x01\x03\x02\x05\x01\x03\x02\x03\x01\x03\x08"
            "\x08\x05",
            "\x04\x0b\x09\x01\x06\x03\x01\x03\x04\x03\x05\x01\x03\x01\x03\x01\x05\x02\x05\x01\x05\x01\x07\x03\x01\x01"
            "\x02\x02\x01\x02\x05\x01\x04\x01\x06\x01\x07\x01\x04\x01\x08\x04\x02\x01\x04\x01\x02\x01\x05\x01\x02\x01"
            "\x07\x01\x04\x01\x06\x15\x12\x02",
            "\x05\x03\x0d\x03\x03\x06\x04\x01\x09\x01\x10\x0d\x06\x01\x01\x02\x01\x01\x06\x03\x06\x02\x01\x01\x08\x01"
            "\x09\x02\x07\x02\x05\x03\x01\x01\x09\x01\x03\x03\x01\x03\x03\x04\x06\x01\x05\x05\x06\x01\x02\x01\x07\x01"
            "\x0b",
        };

        void load(char* fill, const char* src)
        {
            bool targetX = false;
            while (*src) {
                uint8_t n = *src++;
                while (n--) {
                    *fill++ = (targetX ? 'X' : '.');
                }
                targetX = !targetX;
            }
        }

        void store(uint8_t* fill, const char* src)
        {
            // P maps store only X or . (i.e. 2 states).
            bool targetX = false;
            uint8_t count = 0;
            while (*src) {
                while (*src == (targetX ? 'X' : '.'))
                    src++, count++;

                *fill++ = count;
                targetX = !targetX; // Flip target.
                count = 0;
            }
            *fill = 0;
        }
    } // namespace comp

#if USE_ENCRYPTED_P
    char F[NUM_FLOORS][WIDTH * HEIGHT + 1];

#else
    char F[NUM_FLOORS][WIDTH * HEIGHT + 1] = {{"..........X........."
                                               "........X.XX.....X.."
                                               ".......X......XXX..."
                                               "...XXXXXXXXXX.X...X."
                                               "..............X.X..."
                                               "XXXXXXXX.......X...X"
                                               "X............XX.XX.."
                                               "X.....XXXXXXXXXXXXXX"
                                               "X.....X....X...X...X"
                                               "X.....X...XX...X...X"
                                               "XXXXXXX........XXXXX"},
                                              {"....XXXXXXXXXXX....."
                                               "....X......XXX.XXX.."
                                               "..XXX.....X...X...X."
                                               "....XX.....X.....X.."
                                               ".....XXX.X..XX.XX..."
                                               "..X....X......X....."
                                               "..X....X........XXXX"
                                               "..X....X..X.....X..X"
                                               ".......X....X......X"
                                               "XXXXXXXXXXXXXXXXXXXX"
                                               "..................XX"},
                                              {".....XXX............"
                                               ".XXX...XXXXXX....X.."
                                               ".......X............"
                                               "....XXXXXXXXXXXXX..."
                                               "...X.XX.X......XXX.."
                                               "....XX.X........X..."
                                               "......XX.......XX..."
                                               "..XXX.X.........X..."
                                               "XXX.XXX...XXXX......"
                                               "X.....XXXXX......X.."
                                               "X.......X..........."}};
#endif

    void l()
    {
        for (int i = 0; i < NUM_FLOORS; i++)
            comp::load(F[i], comp::F[i]);
    }

    const char* g(int f)
    {
        return F[f];
    }

} // namespace P

namespace monosodiumglutamate
{
    namespace move
    {
        std::string_view get_msg(Tile from, Tile to)
        {
            if (from != to) {
                switch (to) {
                    case SUNDIAL:
                        return "You visited the sundial--also known as the firebird in certain contexts (such "
                               "as--cough cough--in the context of this challenge).";
                    case CHINAGARDEN: return "You entered a Chinese restaurant.";
                    case LIFT: return "You entered a lift.";
                    case ESCALATOR_ABOVE: return "You arrived at an escalator. You can use it to go up.";
                    case ESCALATOR_BELOW: return "You arrived at an escalator. You can use it to go down.";
                    case STAIRS_ABOVE:
                    case STAIRS_BELOW: return "You took the stairs timidly.";
                    case WALL: return "You ghosted through a wall.";
                    case MUSHROOM: return "You visited the mushroom.";
                    case LIBRARY: return "You entered the library.";
                    case BUS_STATION: return "You visited the bus station.";
                    case SECURITY_STATION: return "You visited the security station.";
                    case CLASSROOM: return "You entered a classroom.";
                    case LTA:
                        return "You entered a lecture theatre. A professor was lecturing on information systems and "
                               "networking.";
                    case SCIHOME: return "You entered the science commons.";
                    case EMPTY: return "You activated your levitation repulsors and glided through empty air.";
                    case ARR: return "You entered the academic registry office. Things look empty in here.";
                    case TOILET: return "You entered a toilet. Did you enter the right one though...";
                    default: break;
                }
            }

            switch (to) {
                case FLOOR: return "You placed one foot in front of the other, then did the same with the other foot.";
                case SUNDIAL:
                    return "You looked at the sun gleaming off the round edges of the sundial and pondered the meaning "
                           "of life.";
                case CHINAGARDEN: return "You walked around the restaurant.";
                case LIFT:
                    return "You punched a hole in the side of the lift and jumped across to the other one. Now why "
                           "would you do that?";
                case ESCALATOR_ABOVE:
                case ESCALATOR_BELOW: return "You arrived at... another escalator.";
                case STAIRS_ABOVE:
                case STAIRS_BELOW: return "You arrived at another set of stairs.";
                case WALL:
                    return "You hulk-smashed your way through the wall. Your fists hurt, but at least your strength "
                           "increased. Are there even stats in this game?";
                case CONCOURSE: return "You cantered around the concourse.";
                case MUSHROOM: return "You walked around the mushroom and felt the crisp winter breeze.";
                case LIBRARY: return "You lumbered around the library.";
                case CLASSROOM: return "You walked around the classroom.";
                case LTA:
                    return "You loitered around the lecture theatre. The professor giving a talk was not pleased.";
                case SCIHOME: return "You shuffled around the science commons.";
                case EMPTY: return "You levitated across thin air with your repulsors.";
                case ARR: return "You ambled around the academic registry office.";
                case TOILET: return "You traipsed around the toilet. Since you're alone here, you might as well fart.";
                default: return "";
            }
        }

        std::string_view get_msg(char from, char to)
        {
            return get_msg(static_cast<Tile>(from), static_cast<Tile>(to));
        }
    } // namespace move

    namespace tile
    {
        std::pair<std::string_view, std::string_view> get_name(Tile tile)
        {
            switch (tile) {
                case FLOOR: return {"standing on", "solid ground"};
                case SUNDIAL: return {"at the", "sundial"};
                case CHINAGARDEN: return {"at the", "Chinese restaurant"};
                case LIFT: return {"at an", "elevator"};
                case ESCALATOR_ABOVE:
                case ESCALATOR_BELOW: return {"at an", "escalator"};
                case STAIRS_ABOVE:
                case STAIRS_BELOW: return {"at a", "stairwell"};
                case WALL: return {"inside a", "wall"};
                case CONCOURSE: return {"at the", "concourse"};
                case MUSHROOM: return {"at the", "mushroom"};
                case LIBRARY: return {"at the", "library"};
                case BUS_STATION: return {"at the", "bus station"};
                case SECURITY_STATION: return {"at the", "security centre"};
                case CLASSROOM: return {"inside a", "classroom"};
                case LTA: return {"inside", "Lecture Theatre A"};
                case SCIHOME: return {"inside the", "science commons"};
                case EMPTY: return {"floating in", "thin air"};
                case ARR: return {"at the", "academic registry"};
                case TOILET: return {"in the", "loo"};
                default: return {"mulling over life", "somewhere"};
            }
        }

        std::pair<std::string_view, std::string_view> get_name(char tile)
        {
            return get_name(static_cast<Tile>(tile));
        }
    } // namespace tile

    namespace interact
    {
        auto escalator_options = options{{"You waited patiently on the escalator.", "You held the handrail.",
                                          "You walked in the opposite direction for a minute. Boy, was that fun.",
                                          "You observed social norms by standing on the right."}};
        auto stair_options = options{{"You hummed a tune and heard the sounds reverberate in the stairwell.",
                                      "You stared at the stairs in silence."}};
        std::map<Tile, options> msgs = {
            {FLOOR, options{{"You felt the floor. It looks partially clean.",
                             "You responded to some messages on your phone.", "You stood there... like a statue..."}}},
            {SUNDIAL, options{{"You touched the sundial and felt a burst of energy.",
                               "A student in graduation robes is taking photos nearby. You considered photo bombing, "
                               "but decided against it.",
                               "You tried to tell the time using the sundial, but you don't know how it works.",
                               "You peered at the small pool underneath the sundial. The faucets are off."}}},
            {CHINAGARDEN, options{{"You enjoyed some local cuisine.", "You partook in some coffee pudding. Exquisite!",
                                   "A service robot stopped by to deliver some lotus cakes. You took the cakes and "
                                   "watched as it pathfound its way to the kitchen."}}},
            {LIFT, options{{"As no one else was in the lift, you pressed all the buttons--y'know, just for fun--except "
                            "for the alarm button, of course.",
                            "You passed the time staring at your reflection.",
                            "A few university staff entered the lift. You stepped aside to make space for them."}}},
            {ESCALATOR_ABOVE, escalator_options},
            {ESCALATOR_BELOW, escalator_options},
            {STAIRS_ABOVE, stair_options},
            {STAIRS_BELOW, stair_options},
            {WALL, options{{"Well, what *can* you do inside a wall? Hide?", "You took a light nap inside the wall.",
                            "A professor passed by and pretended not to see you. \"Kids these days!\"",
                            "You sniffed the concrete powder and tasted a bit. Blech!"}}},
            {CONCOURSE, options{{"You looked around at the massive area.",
                                 "You watched a staff worker drive around on a cleaning vehicle.",
                                 "A couple walked past, chatting amiably."}}},
            {MUSHROOM, options{{"You looked out towards Clear Water Bay and the sun's reflection on the sea.",
                                "You pondered the meaning of life.",
                                "You watched black kites circle over treetops in search of prey.",
                                "You watched puffy clouds gently float in the air."}}},
            {LIBRARY, options{{"You passed the time reading books.",
                               "You took a look at the exhibition on Israeli developments and achievements.",
                               "You found a book on reverse engineering. One of the chapters compared dynamic analysis "
                               "against static analysis. You decided to borrow the book, cos why not?"}}},
            {BUS_STATION,
             options{{"You waited patiently for the bus, but you don't know what your destination is. Anyhow, "
                      "shouldn't you be looking for a flag?",
                      "A 91M red KMB bus stops over.", "An 11M minibus stops by before speeding away.",
                      "You watch as people queue for the rides to Hang Hau.",
                      "You notice a bunch of promotional posters on the walls. Looks like the University Philharmonic "
                      "Orchestra will be playing.",
                      "You notice a bunch of promotional posters on the walls. Looks like the fencing team is "
                      "recruiting."}}},
            {SECURITY_STATION, options{{"You looked in and saw some security guards manning the front desk."}}},
            {CLASSROOM, options{{"The classroom looks empty during winter. You goofed around with the whiteboard.",
                                 "You chilled on the desks.",
                                 "A section of the whiteboard was left unerased. Looks like the previous class was on "
                                 "linear algebra."}}},
            {LTA, options{{"You heckled during class. The professor was not impressed by your shenanigans.",
                           "You yawned. Loudly. The professor glanced inquisitively in your direction."}}},
            {SCIHOME, options{{"You sat in the science commons.\nAnd sat.\nAnd sat."}}},
            {EMPTY, options{{"You floated in space. Well, not space space. But just pure-\nempty-\nmindless-\nspace.",
                             "You checked the energy levels of your repulsors. Looks good."}}},
            {ARR, options{{"The academic registry looks empty."}}},
            {TOILET,
             options{{"The toilet looks somewhat clean. But then again, it's the winter term--hardly anyone uses "
                      "the toilet.",
                      "This is a good place to fart."}}},
        };

        std::string_view get_msg(Tile tile)
        {
            if (auto it = msgs.find(tile); it != msgs.end())
                return it->second.choose();

            return "Welp, looks like the developer forgot to add text for this tile.";
        }

        std::string_view get_msg(char tile)
        {
            return get_msg(static_cast<Tile>(tile));
        }
    } // namespace interact
} // namespace monosodiumglutamate
