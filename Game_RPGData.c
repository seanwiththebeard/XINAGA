#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LOWCODE")
//#pragma code-name (push, "CODE")
#endif

#if defined (__NES__)
//#pragma code-name (push, "XINAGA_CONSOLE")
//#pragma rodata-name (push, "XINAGA_CONSOLE")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
//#pragma code-name (push, "XINAGA")
#pragma rodata-name (push, "GAME_RPGDATA")
#endif

const RaceDescriptionDef const RaceDescription[8] =
{
  {"Human", 255},
  {"Elf", 6},
  {"Dwarf", 255},
  {"Frobit", 6},
  {"Orc", 6},
  {"Fuzzer", 6},
  {"Dracon", 6},
  {"Construct", 6}
  //{"Construct", 8} // Never levels up, strong early on but expensive to upgrade stats
};


const ClassDescriptionDef const ClassDescription[8]=
{
  {"Fighter", 8},
  {"Ranger", 8},
  {"Wizard", 4},
  {"Paladin", 6},
  {"Monk", 6},
  {"Bard", 4},
  {"Thief", 4},
  {"Cleric", 6}
};

const sbyte const AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};

const char namePrefixA[][8][4] =
{
        {//Human
        "Hur",
        "Dur",
        "Ran",
        "Mal",
        "Fal",
        "Lir",
        "Gaf",
        "Jex"
        },
        {//Elf
        "Lis",
        "Kes",
        "Sz'",
        "Th'",
        "Yli",
        "Lwi",
        "Eol",
        "Rys"
        },
        {//Dwarf
        "Muh",
        "Dov",
        "Sha",
        "Ste",
        "Sfo",
        "Yeg",
        "Gos",
        "Woz"
        },
        {//Halfling
        "Flo",
        "Bli",
        "Man",
        "Tem",
        "Huw",
        "Jle",
        "Nel",
        "Sul"
        },
        {//Orc
        "Mor",
        "Far",
        "Hir",
        "Gha",
        "Mal",
        "Ral",
        "Dan",
        "Pol"
        },
        {//Fuzzer
        "Uwe",
        "Reu",
        "Bes",
        "Kle",
        "Uye",
        "Xes",
        "Ztu",
        "Hsa"
        },
        {//Dracon
        "Fah",
        "Sza",
        "Oha",
        "Wah",
        "Lah",
        "Rah",
        "Qua",
        "Pla"
        },
        {//Construct
        "AP",
        "ZI",
        "OP",
        "UO",
        "GH",
        "QK",
        "MN",
        "ZY"
        }
};
const char nameSuffixA[][8][6] =
{
        {//Human
        "kar@",
        "roth@",
        "mar@",
        "par@",
        "dak@",
        "eos@",
        "nast@",
        "stle@"
        },
        {//Elf
        "leu@",
        "ious@",
        "yir@",
        "thel@",
        "aeu@",
        "ndir@",
        "brae@",
        "owas@"
        },
        {//Dwarf
        "yen@",
        "ama@",
        "ral@",
        "apa@",
        "oru@",
        "eke@",
        "hest@",
        "julo@"
        },
        {//Halfling
        "bo@",
        "blib@",
        "mak@",
        "bar@",
        "est@",
        "nlo@",
        "kle@",
        "hoh@"
        },
        {//Orc
        "gen@",
        "nir@",
        "aar@",
        "air@",
        "olen@",
        "geth@",
        "menk@",
        "fhan@"
        },
        {//Fuzzer
        "papa@",
        "riga@",
        "gisa@",
        "huer@",
        "fath@",
        "mner@",
        "uiop@",
        "fust@"
        },
        {//Dracon
        "fir@",
        "nir@",
        "lir@",
        "kir@",
        "hir@",
        "mir@",
        "wir@",
        "zir@"
        },
        {//Construct
        "-A@",
        "-B@",
        "-C@",
        "-D@",
        "-E@",
        "-F@",
        "-G@",
        "-H@"
        }
};
