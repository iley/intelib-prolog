// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_intelib.org>)

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "sexpress/sstring.hpp"

#include "refal/refal.hpp"
#include "refal/rvars.hpp"
#include "refal/rclause.hpp"

#include "irina.hxx"

#define BUFSIZE 1024
#define DEFAULT_DIRECTIVES "directives.ref"

#include "win_port.hpp"

RfFormConstructor R;

bool wasbflag = false, waslflag = false, wasoflag = false;
const char *binary_name = 0;

enum etype {DIRMISS, BADFILENAME, WASLFLAG, WASBFLAG, L_AND_B,
            NOTEXISTS, CANTCREATE, INVALID_FLAG, CANTCOMPILE,
            NOBFLAG, WASOFLAG, BINNAMEMISS};
enum state {STATE_S, STATE_L, STATE_O};

bool is_minus_h(char *str) {
    return !strcmp(str, "-h") || !strcmp(str, "--help");
}

bool is_minus_l(char *str) {
    return !strcmp(str, "-l");
}

bool is_minus_a(char *str) {
    return !strcmp(str, "-a") ||  !strcmp(str, "--about");
}

bool is_minus_b(char *str) {
    return !strcmp(str, "-b") ||  !strcmp(str, "--binary");
}

bool is_minus_V(char *str) {
    return !strcmp(str, "-V") || !strcmp(str, "--version");
}

bool is_minus_o(char *str) {
    return !strcmp(str, "-o");
}

void print_help()
{
    printf(
    "Command line arguments:\n"\
    "-h (--help) --- print this message.\n"\
    "-a (--about) --- print information about this program.\n"\
    "-V (--version) --- print version.\n"\
    "[-l dirfilename] filename1 ... filenameN --- translate files using d"\
    "irectives from "\
    "\"dirfilename\"\n(if this argument is missing --- from \"directives."\
    "ref\") file.\n"\
    "-b [-o binaryname] filename1 ... filenameN --- compile files into "\
    "\"binaryname\"\nbinary file (if \"-o\" flag is missing --- into "\
    "\"a.out\" file)\n"\
    "(using default directives --- not compatible with \"-l\" flag).\n"\
    "\n"\
    "Directives:\n"\
    "1. DECLARE_PUBLIC_FUNCTION = Symbol.\n"\
    "2. DECLARE_PUBLIC_SYMBOL = Symbol.\n"\
    "3. DECLARE_EXTERNAL_FUNCTION = Symbol.\n"\
    "4. DECLARE_EXTERNAL_SYMBOL = Symbol.\n"\
    "5. DECLARE_NAME Symbol or Variable = Symbol --- replace the left par"\
    "t with the symbol "\
    "Symbol during code generation.\n"\
    "6. DECLARE_CHAR \'c\' = Symbol --- replace char \'c\' with the symbo"\
    "l "\
    "Symbol during code generation.\n"\
    "7. MODULE_NAME = Symbol --- generate \'Symbol.[ch]xx\' files. "\
    "By default \'string\' = \'refalpart\'.\n"\
    "8. CXX_FILE_PATH = \'string\' --- generate cxx file \'string\'. "\
    "By default \'string\' = MODULE_NAME + \'.cxx\'.\n"\
    "9. HXX_FILE_PATH = \'string\' --- generate hxx file \'string\'. "\
    "By default \'string\' = MODULE_NAME + \'.hxx\'.\n"\
    "10. DECLARE_USED_MODULE = \'string\' --- add line "\
    "\'#include \"string\"\' to cxx file.\n"\
    "11. INIT_FUNCTION_NAME = Symbol --- by default "\
    "Symbol = RefalInit.\n"
    );
    exit(1);
}

void get_version()
{
    printf(IRINA_VERSION);
}

void print_version()
{
    printf("IRINA ");
    get_version();
    putchar('\n');
    exit(1);
}

void print_about()
{
    printf(
    ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,~+$$8DDDD88O$::,,,,,,,,,,,,,,,,,.,,,,"\
    ",,,,,,,.....\n"\
    ",,,,,,,,,,,,,,,,,,,,,,,,,,,,~Z88DNDNNNNDNNNNND7:,,,,,,,,,,,,,,,,,,,,"\
    ",,,,,,,,,,..\n"\
    ",,,,,,,,,,,,,,,,,,,,,,,,,,=O8DNNNNDNNNNNNNDNNNDN+,,,,,,,,,,,,,,,,,,,"\
    ",,,,,,,,,,,.\n"\
    ",,,,,,,,,,,,,,,,,,,,,,,,:$DNNNNNNDDNNNDNNNNNNNNNDN:,,,,,,,,,,,,,,,,,"\
    ",,,,,,,,,,,.\n"\
    ",,,,,,,,,,,,,,,,,,,,,,:~8D8DDNNND8DNNDNDDNMMMNNNNDDO,,,,,,,,,,,,,,,,"\
    ",,,,,,,,,,,.\n"\
    ",,,,,,,,,,,,,,,,,,,,,:7ODNDNNNND8888DNNNDNNNMNNNNNNND~,,,,,,,,,,,,,,"\
    ",,,,,,,,,,,.\n"\
    ",,,,,,,,,,,,,,,,,,,,:ZDDDD8DDD8Z$7$$O88NDDNNMNNNNNNND8=,,,,,,,,,,,,,"\
    ",,,,,,,,,,,,\n"\
    ",,,,,,,,,,,,,,,,,,,:8DDN8$77IIIII77ZZO8DDDNNNMNNNNNNNNO=:,,,,,,,,,,,"\
    ",,,,,,,,,,,,\n"\
    ",,,,,,,,,,,,,,,,,,:DNDDN7I????III7$ZOO8DNNDNNMNNNNNNNDNO~,,,,,,,,,,,"\
    ",,,,,,,,,,,,\n"\
    ",,,,,,,,,,,,,,,,,:DNN8D8?I?++??II$ZOOO8DDNNDNNMMNNNNNNND$:,,,,,,,,,,"\
    ",,,,,,,,,,,.\n"\
    ",,,,,,,,,,,,,,,,,+NNN8D8+?++++I77$$ZZOODDNNNNNMMNNNNNNND8=:,:,,,,,,,"\
    ",,,,,,,,,,,,\n"\
    ",,,,,,,,,,,,,,,,,?DDMDN8=7~=+I77IZM7$7$MNNNNNNMNNNNDNNDNDZ=:,,,,,,,,"\
    ",,,,,,,,,,,.\n"\
    ",,,,,,,,,,,,,,,,,?NDNNNOI?==+?I$$DMMMMMMMMMMMMMMNNNND8DDDD7~,,,,,,,,"\
    ",,,,,,,,,,,.\n"\
    ",,,,,,,,,,,,,,,,,+NNNMMZ7Z$Z7D$7I$NNNNNNNMNMMNMNZ$77$I~IDDOI~:,,,,,,"\
    ",,,,,,,,,,..\n"\
    ",,,,,,,,,,,,,,,,,NNNNNM++ZNMMII7DMNNNMNNDNNNNMN88ZIND$I=DDDO+~:,,,,,"\
    ",,,,,,,,,,..\n"\
    ",,,,,,,,,,,,,,,,,~NNNNM++I8OI?ZNMNNMMMMMM8NMDM8ZZZ?++$7==NDDO+~:,,,,"\
    ",,,,,,,,,,..\n"\
    ",,,,,,,,,,,,,,:,~DNNMMM==?M??+DMNDMMMMMMMNNMM8OZ$DOO?+++?$DD8$=~::,,"\
    ",,,,,,,,,,..\n"\
    ",,,.::,,,,,,,:~IDNNMMNN~~+Z???NMNNNNNNNNMMNMMN8DZ8DOZZ7I?+N88Z7I::,,"\
    ",,,,,,,,,,..\n"\
    ",,,,,:,,,,,,~~$8NNMMMMMI=IN++I?MNNMNNNMMMMNMMNNDZ$77I777+??DOOZ7+:,,"\
    ",,,,,,,,,...\n"\
    ",,,,,,,,,,,:~I$DNNNMNNNM+NNO?7?MNNNNMNMMMMMDDND8NDN88$I?I=ID8OZ8$+:,"\
    ",,,,,,,,,,..\n"\
    ",::::::::::~+ODNNNMMMMMMDMN7OI7+NMMMMMMMMMD8NNDNDOZZZ$=+?=+ND8ZOZ8:,"\
    ",,,,,,,,,...\n"\
    "::::::::::~=IDNNNMNNNMMMMDNN$?II??DMMMMMDD8DMNNDD88O7Z++?+INN8OI787:"\
    ",,,,,,,,,...\n"\
    ":::::::::~=IZNNNNNNNNMMMMMMMNNNNNMMMN8DN8O8MMNNNNNMD8DMNNNNNDDO$+$O:"\
    ",,,,,,,,,...\n"\
    "::::::::~+IZDNNNNNNMMMMMMMMMNNMMNMNNNNMMO8MMMMNNMMMMMMNNNNNNNDO$I7O:"\
    ",,,,,,,,,...\n"\
    ":::::~:~I$ONNNNNMNNMMMMMMMMMMNNMMMNNNNNNNNMMMMMMMMMMMMNMNNMNDD$ZI8I:"\
    ",,,,,,,,,...\n"\
    "::::~~=+O8NNNNNNMNMMMMMMMMMMMMNMMNNNNNNMNMMMMMMNMMMMMNMMMMMNND$88I=:"\
    ":,,,,,,,,...\n"\
    "::::~~=ZODNNNMNMNMNMNMMMMMMMMNMNMNNNNMNMMMMNMMMMMMMMMMMNMNNNNDOOZ=::"\
    ":,:,,,,,,,..\n"\
    "::::~=I$8NNNMMMMMMNNNMMMMMMMMMMNMMMMMNMNMMNMMMMMMMMMMMMMMMNNNNNNNDN~"\
    ":::,,,,,,,..\n"\
    ":::~~=ZONNNMMMMMMMMMMMMMNNMMMMNNMMMNMNMMMMMMMMMMMMMMMMMMMMNMNNMNNNNN"\
    "+~:,,,,,,...\n"\
    ":~~~?$8NNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNMMMMMMMMMMMMMMMMMMNNNNNNNNN"\
    "NNM,:,,,,,..\n"\
    "~~==OO8NMNNNMMMMMMMMMMMMMMMMMMMMMMMMNNMMMMMNMMMMMMMMMMMMMMMNNNNNNMMN"\
    "NMNN?,,,,,,.\n"\
    "~~~=Z8NNNNNMNMMMMMMMMMMMMMMMMMMMMMMMNMMMMMMMMMMMNMMMMMMMMMNMNNMNMMMN"\
    "NNMNN~::,,,.\n"\
    ":~~?DDNNMMNMMNMMMMMMMMMMNNNMMMMMMMMMMNMMMMMMMMMMNMMMMMMMMMMNNMNMNMNM"\
    "NMMNNN,,,,..\n"\
    "~~~OONNMMMNMMNMMMMMMMMMNMMNMMMMMMMMMMMMMMMMMMMMNMMMMMMMMMNMNNNNNMNMM"\
    "MNNNNN:,,,..\n"\
    "~~~OONNMMMMMMMMMMMMMMMMMMMNNMMMMMNMMMMMMNNMMMMMNMMMMMMMMMMMNNNMNMNNN"\
    "NNMNNN=,,,..\n"\
    "~:~+ONNNMMMMMMMMMMMMMMNMMMMMNMMMMMMMMMMMMMMMMMMNNMMMMMMMMMNNNNNMNMNM"\
    "NNNNNN7,,,..\n"\
    "~~~?8NNNMMMMMMMMMMMMMNNMMMMNMMMMMMMMMMMMMMMMMMMNMMMNMMMNMMMMNNNNMMNN"\
    "MNNNNND,,,..\n"\
    "~+7$MNMMMMMMMMMMMMMMMMNMMMMNMMMMMMMMMMMMMMMMMMMNMMMMMMMMMNMNNNNMNMNN"\
    "NMNNNNM,,,..\n"\
    "==O87=NMMMMMMMMMMMMMMMNMMMMNMMMMMMMMMMMMMMMMMMMNMMMMMMMMNMNNNNNNMNMN"\
    "NNNNNNN,,,,,\n"\
    "+O===~NMMMMMMMMMMMMMMMNMMMMNMMMMMMMMMMMMMMMMMMNNMMMMMMMMMMNMNNNNNMNN"\
    "NNMMNNN::,,.\n"\
    "I~~=I=IMMNMMMMMMMMMMMMMNNMMNMMMMMMMMMMMMMMMMMMNNMMMMMMMMMNNNNNNNMNNN"\
    "NMNNNNN~:,,.\n"\
    "~=~~=~~NNNMNMMMMMMMMMMMDO7MNMMMMMMMMMMMMMMMMMNNNMMMMMMMMMMMNMNNMNMMN"\
    "NMMMNNNI::,,\n"\
    "~~~=~~~NNNNMMMMMMMMMMMN8M8NNMMMMMMMMMMMMMMMMNNNNMMMMMMMMMMMMNNNNMMNM"\
    "NMNNNNNZ~:,,\n"\
    "~~=~~~~DNNNMMMMMMMMMMMMDMIMMMMMMMMMMMMMMMMMMNNNNMMMMMMMMMMMMMMNNMMMN"\
    "NNMMNNNZ~~:,\n"\
    "~=~~~~~~NMMMMMMMMMMMMMNNM7NMMMMMMMMMMMMMMMMNNNNMMMMMMMMMMMMMMMNNNMMN"\
    "NMNMNNN$~~::\n"\
    "~==~~~~~MMMMMMMMMMMMMMMMD8NMMMMMMMMMMMMMMMMNNNNMMMMMMMMMMMMMMNNNMNNM"\
    "NNMMMNMM$=~~\n"\
    "========MMMMMMMMMMMMMMN+M8NMMMMMMMMMMMMMMMNNNNNMMMMMMMMMMMMMMNMMMMMN"\
    "MMMMMMNMM7=+\n"\
    "====~~=+NMMMMNMMMMMMMMM==NNMMMMMMMMMMMMMMNNNNNNMMMMMMMMMMMMMNMMNMMMM"\
    "NNMMMMMMMM++\n"\
    "=======?MMMMMMMMMMMMMNN+?MDMMMMMMMMMMMMMMNNNNNMMMMMMMMMMMMMMMMMMMNMM"\
    "MMMMMNMMMNI?\n"\
    "=======+MMMMMMMMMMMMMMND+NDMMMMMMMMMMMMMMNNNNNMMMMMMMMMMMMMMMMMMMMMM"\
    "MMMMMNMMMNN?\n"\
    "++===+++MMMMMMMMMMMMMMNDMMNMMMMMMMMMMMMMMNNNNNMMMMMMMMMMMMMMMMMNMNMM"\
    "MMMMNMMMNNN?\n"\
    "+++====+MMMMMMMMMMMMMMN+?ONMMMMMMMMMMMMMMNNNNNMMMMMMMMMMMMMMMMNMMMMN"\
    "MNMNNNMNNNII\n"\
    "++++++=+DMMMMMMMMMMMMMM=~MMMMMMMMMMMMMMMMMNNNMMMMMMMMMMMMMMMMMMMMMMM"\
    "NMMMMMNNNI?I\n"\
    "\n"\
    "..............                             . ..,,~I$OMNDNMMMMMMMMMMM"\
    "MMMMMMMNNDD8\n"\
    ".................                       .....,~+I7DMMD8NMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "..................                      ....,+?+$NMMDMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "..........................          ......,~++ZMMMNNNNMMMMMMMNMMMNNM"\
    "MMMMMMMMMMMM\n"\
    "...........................    . ........~+IZNNNNMMMMMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    ".............................   ........:+78NNNNMMMMMMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "....................................,,~I88NNNMMMMMMMMMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "....................,,,,...........,~=78DNMMMMMMMMMMMMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    " ..................,,,,,,,......,:+$Z8NNMMMMMMMMMMMMMMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "     ..............,,,::::,,,,,,~OMNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "     .............,,,,::::::,,,~7NMNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "        ..........,,,:::~~=+???$NMMMMMMMMMN8O$77$$ZO8NMMMMMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "          ........,,::::~~+?7ZODMMMMMMMMNO$7I??II777$O8DNNMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "            ......,,:::~~=7ZODMMMMNNNDOI++++++++???II7ZO8DNMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "               ...,,,::~=+$ODNMMMNDZI+=======+++???II$ZODNNMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "               ....,,::~=+Z8NNMMMD$?=~~~~~====+++??II$Z8DNMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "               . ...,,:~=?8NMMMMZ=~~~~~~~~~====++++?I7$ODNMMMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "                  ....,~+7DNMMMZ::~~~~~~~~~~===++++??I7$ZODNMMMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "               .  ....,~+7DNMMN+::::~~~~~~~~===+++++?II7$ZO8DNMMMMMM"\
    "MMMMMMMMMMMM\n"\
    "     .    ...........,,:~?DMMM$,:::::~~~~~~~=====+++?II77$$ZZOOO8NMM"\
    "MMMMMMMMMMMM\n"\
    "       ......,,::::::::::~DMMM7,::::::~~~~~~~===+++??II77$$$ZZZOO8NM"\
    "MMMMMMMMMMMM\n"\
    "    . .,~+I7ZODNNMMMMMMMMMDNMMO:::::::::::~~~~~~=+?I77$$$777$$$$ZOOD"\
    "MMMMMMMMMMMM\n"\
    " ,=$DMMMMMMMMMMMMMMMMMMMMMMMMMD~::,,,,,,,,::::~+?+==~==++?III7777$ZO"\
    "8NMMMMMMMMMM\n"\
    "I8MMMMMMMMMMMMMMMMMMMMMMMMMMMMN=,,,,,,,,,,,::~+=~::::~~===?7IIII7$$Z"\
    "ODMMMMMMMMND\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7,,,,,,,,,,::~~:::::~=++++??????II77$"\
    "ZODMMMMMN8ND\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ,,,..,...,::::::::~=+??I777I????I77$"\
    "$ZODNNND8O8O\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN~.,,....,,,:~==::,~=+ON87?+?????II77"\
    "$$ZO88D888NM\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7=:::,,,,:~+??+I$Z888$$?++++++???III"\
    "7$$$ZOOO$$DN\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ~::,,,,,:~+???$8O7II+++==++++????II"\
    "77$$$ZZZ$ZNN\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM+.,::::,:=+???++===~~~~~~==+++???II"\
    "I77$$ZZZ$Z$I\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMD::,:~+~~++???+==~~~~~~~~==+++????I"\
    "I77$$ZZZZ$$7\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ:7Z=:=++??++=~~::~~~~~===++++???"\
    "II7$$ZZZOO$$\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMO~:::~=+???+=~:,:::~~~=====+++??"\
    "?I77$$ZZODDO\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM+,:::~=+????+~:,,:::~~~~====++??"\
    "?II77$$ZODDM\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN:,,::=++???III=,,,::::~~~====++?"\
    "?III77$ZODDN\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM~,,,:~==~::~+++:,,::::~~=====+++"\
    "???I77$ZOD8N\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM?..,,:~~~I7~:==~:,::::~~~==+++++"\
    "???I77$ZO88D\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM=...,:I7=~~+=~~~:~~~~~~~===++??"\
    "+??II7$ZOO8D\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ...,,::,~==~~~~~~~~~~~~===++??"\
    "???II7$ZOO8D\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM: .,,,,,~=~~~~~~~~~~~~~==++++"\
    "+??II7ZOZOO8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8,.,,,,::::~~?$~,:~~~=~===++"\
    "+??I$$$$$ZO8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ.,,,:::::=?O$=::~~~~~==+++"\
    "+?I7$777$ZO8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM~:++?77I?I+~~~~~~~=====++"\
    "?I$777II7ZO8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMZ.=+=~~++I~~~~~~~~~====+?"\
    "?77IIIII7$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM~,,,:::::::~~~~~~===++?"\
    "I???IIII7$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7.,,,,,,::::~~====+?"\
    "?=+??????I7$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM=.,,,,,:::~~~===+++=="\
    "=+?????I7$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$...,,,::~~~~==~::~~"\
    "==+?????I$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8: ..,,,::~~:,,,::~"\
    "~=++++??7$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN~ .,,:~~:,,,,,,:~"\
    "~=+++++?I$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMMNZ$$ZZZZ8MMMMMMMMMMMMMMMMMMMMM7+=~~~::::::::"\
    "~=+++++?I$O8\n"\
    "MMMMMMMMMMMMMMMMMMMMMMMN$III7777$NMMMMMMMMMMMMMMMMMMMMZ++===~~:::::~"\
    "~==++++?7ZO8\n"\
    "MMMMMMMMMMMMMMMMMMMMMN$?+=++??III8MMMMMNNMMMMMMMMMMMMMNZI?++++===~~~"\
    "==++++?I$O8O\n"\
    "MMMMMMMMMMMMMMMMMMMMD?=~~~=+?77ZZDMMMMMMMMMMMMMMMMMMMMMNZ$7II???++=="\
    "==++++?7Z8OO\n"\
    "MMMMMMMMMMMMMMMMMMMM$=~:~~+IZ8DNNMMMMMMMMMMMMMMMMMMMMMMMDOZ$7II???++"\
    "++++???7O8OO\n"\
    "MMMMMMMMMMMMMMMMMMM$=:,,,:==+?I7NMMMMMMMMMMMMMMMMMMMMMMMMN8OZZ$77II?"\
    "??????IZ8OOO\n"\
    "MMMMMMMMMMMMMMMMMMMI~,,,,,~~=+I$NMMMMMMMMMMMMMMMMMMMMMMMMMD8OOZ$777I"\
    "II????7O8OZO\n"\
    "MMMMMMMMMMMMMMMMMMO=:,....::~=+$MMMMMMMMMMMMMMMMMMMMMMMMMMND888OZZZ$"\
    "7777II$88OO8\n"\
    "MMMMMMMMMMMMMMMMMMI~,,.....,::~7MMMMNNMMMMMMMMMMMMMMMMMMMMMND888OOOO"\
    "ZZZZZ$Z88888\n"\
    "MMMMMMMMMMMMMMMMMN?:,,.....,,:~7MMMM8NMMMMMMMMMMMMMMMMMMMMMMD88D8OOO"\
    "OOOOZZO88888\n"\
    "MMMMMMMMMMMMMMMMMO=:,,,.....,,,ZMMMDZDMMMMMMMMMMMMMMMMMMMMMMND888888"\
    "OOOOOZOOZZOO\n"\
    "MMMMMMMMMMMMMMMMM$~,,,,,....,,,OMMM8ZDMMMMMMMMMMMMMMMMMMMMMMND888888"\
    "OOOOOZZZ$$$Z\n"\
    "\n"\
    "(For Irina Nazarova\'s photos please visit "\
    "http://flickr.com/photos/nonconstant/)\n"
    );
    printf(
    "\n"\
    "\n"\
    "This is IRINA, _I_ nteLib _R_ efal to C++\n"\
    "(_I_ ntellectual, _N_ ew & _A_ ll-inclusive) translator "\
    "(version "
    );
    get_version();
    printf(
    ").\n"\
    "\n"\
    "It was written by Igor Bronstein <igor.bronstein_AT_intelib.org>.\n"\
    "\n"\
    "My greatest acknowledgment to all Irinas\n"\
    "who have influenced my life very much (in alphabetical order):\n"\
    "\n"\
    "Irina Bronstein (née Goykhberg),\n"\
    "Irina Gaynanova,\n"\
    "Irina Mikheyeva,\n"\
    "Irina Nazarova,\n"\
    "Irina Red\'kina\n"\
    "\n"\
    "and many others.\n"\
    "\n"\
    "And what concerns you, I wish you good luck and a nice day!\n"\
    "Be happy!\n"
    );
    exit(1);
}

void print_error(int &errnum, etype errtype, const char *edesc)
{
    if (errnum == 0)
    {
        printf("\nFollowing errors were found:\n\n");
    }
    errnum++;
    printf("%d. ", errnum);
    switch (errtype)
    {
        case DIRMISS:

        printf("Directives file name missing.\n");
        break;

        case BADFILENAME:

        printf("\"%s\" is a bad file name.\n", edesc);
        break;

        case INVALID_FLAG:

        printf("\"%s\" is an invalid flag.\n", edesc);
        break;

        case WASLFLAG:

        printf("\"-l\" flag can be used only once.\n");
        break;

        case WASBFLAG:

        printf("\"-b\" flag can be used only once.\n");
        break;

        case L_AND_B:

        printf("\"-l\" and \"-b\" flags can not be used together.\n");
        break;

        case NOTEXISTS:

        printf("\"%s\" file does not exist.\n", edesc);
        break;

        case CANTCREATE:

        printf("\"%s\" file cannot be created.\n", edesc);
        break;

        case CANTCOMPILE:

        printf("Compiling failed.\n");
        break;

        case NOBFLAG:

        printf("\"-o\" flag can be used only with \"-b\" one.\n");
        break;

        case WASOFLAG:

        printf("\"-o\" flag can be used only once.\n");
        break;

        case BINNAMEMISS:

        printf("Binary file name missing.\n");
        break;
    }
}

void is_stateS(char **&filenames, int argc, char *argv[],
               int &filenum, int &errnum, state &cur_state, int i) {
    if (is_minus_o(argv[i])) {
        cur_state = STATE_O;
        if (wasoflag) {
            print_error(errnum, WASOFLAG, 0);
            return;
        }
        else
        if (!wasbflag) {
            print_error(errnum, NOBFLAG, 0);
            wasoflag = true;
            return;
        }
        else {
            wasoflag = true;
            return;
        }
    }
    if (is_minus_l(argv[i])) {
        cur_state = STATE_L;
        if (waslflag) {
            print_error(errnum, WASLFLAG, 0);
            return;
        }
        else
        if (wasbflag) {
            print_error(errnum, L_AND_B, 0);
            waslflag = true;
            return;
        }
        else {
            waslflag = true;
            return;
        }
    }
    if (is_minus_b(argv[i])) {
        if (wasbflag) {
            print_error(errnum, WASBFLAG, 0);
            return;
        }
        else
        if (waslflag) {
            print_error(errnum, L_AND_B, 0);
            wasbflag = true;
            return;
        }
        else {
            wasbflag = true;
            return;
        }
    }
    if (argv[i][0] == '\0') {
        print_error(errnum, BADFILENAME, argv[i]);
        return;
    }
    if (argv[i][0] == '-') {
        print_error(errnum, INVALID_FLAG, argv[i]);
        return;
    }
    filenames[filenum] = new char[strlen(argv[i])+1];
    strcpy(filenames[filenum], argv[i]);
    filenum++;
}

void is_stateL(char **&filenames, int argc, char *argv[],
               int &filenum, int &errnum, state &cur_state, int i) {
    if (is_minus_o(argv[i])) {
        print_error(errnum, DIRMISS, 0);
        cur_state = STATE_O;
        if (wasoflag) {
            print_error(errnum, WASOFLAG, 0);
            return;
        }
        else
        if (!wasbflag) {
            print_error(errnum, NOBFLAG, 0);
            wasoflag = true;
            return;
        }
        else {
            wasoflag = true;
            return;
        }
    }
    if (is_minus_l(argv[i])) {
        print_error(errnum, DIRMISS, 0);
        print_error(errnum, WASLFLAG, 0);
        return;
    }
    if (is_minus_b(argv[i])) {
        print_error(errnum, DIRMISS, 0);
        cur_state = STATE_S;
        if (wasbflag) {
            print_error(errnum, WASBFLAG, 0);
            return;
        }
        else {
            print_error(errnum, L_AND_B, 0);
            wasbflag = true;
            return;
        }
    }
    if (argv[i][0] == '\0') {
        print_error(errnum, BADFILENAME, argv[i]);
        cur_state = STATE_S;
        return;
    }
    if (argv[i][0] == '-') {
        print_error(errnum, INVALID_FLAG, argv[i]);
        print_error(errnum, DIRMISS, 0);
        cur_state = STATE_S;
        return;
    }
    delete []filenames[0];
    filenames[0] = new char[strlen(argv[i])+1];
    strcpy(filenames[0], argv[i]);
    cur_state = STATE_S;
}

void is_stateO(char **&filenames, int argc, char *argv[],
               int &filenum, int &errnum, state &cur_state, int i) {
    if (is_minus_o(argv[i])) {
        print_error(errnum, BINNAMEMISS, 0);
        print_error(errnum, WASOFLAG, 0);
        return;
    }
    if (is_minus_l(argv[i])) {
        print_error(errnum, BINNAMEMISS, 0);
        cur_state = STATE_L;
        if (waslflag) {
            print_error(errnum, WASLFLAG, 0);
            return;
        }
        else
        if (wasbflag) {
            print_error(errnum, L_AND_B, 0);
            waslflag = true;
            return;
        }
        else {
            waslflag = true;
            return;
        }
    }
    if (is_minus_b(argv[i])) {
        cur_state = STATE_S;
        print_error(errnum, BINNAMEMISS, 0);
        if (wasbflag) {
            print_error(errnum, WASBFLAG, 0);
            return;
        }
        else
        if (waslflag) {
            print_error(errnum, L_AND_B, 0);
            wasbflag = true;
            return;
        }
        else {
            wasbflag = true;
            return;
        }
    }
    if (argv[i][0] == '\0') {
        print_error(errnum, BADFILENAME, argv[i]);
        cur_state = STATE_S;
        return;
    }
    if (argv[i][0] == '-') {
        print_error(errnum, INVALID_FLAG, argv[i]);
        cur_state = STATE_S;
        return;
    }
    binary_name = argv[i];
    cur_state = STATE_S;
}

void readfilenames(char **&filenames, int argc, char *argv[],
                   int &filenum, int &errnum) {
    state cur_state = STATE_S;
    for (int i = 1; i < argc; i++) {
        switch (cur_state) {
            case STATE_S:
            
            is_stateS(filenames, argc, argv, filenum,
                      errnum, cur_state, i);
            break;
 
            case STATE_L:
  
            is_stateL(filenames, argc, argv, filenum,
                      errnum, cur_state, i);
            break;

            case STATE_O:

            is_stateO(filenames, argc, argv, filenum,
                      errnum, cur_state, i);
        }
    }
    switch (cur_state) {
        case STATE_S:

        break;

        case STATE_L:
        
        print_error(errnum, DIRMISS, 0);
        break;

        case STATE_O:
        
        print_error(errnum, BINNAMEMISS, 0);
        break;
    }
}

void checkfile(char *filename, int &errnum)
{
    struct stat file_info;
    if (stat(filename, &file_info) != 0)
    {
        print_error(errnum, NOTEXISTS, filename);
        return;
    }
}

void checkfiles(char **filenames, int filenum, int &errnum)
{
    for (int i=0; i<filenum; i++)
        checkfile(filenames[i], errnum);
}

void free_everything(char **&filenames, int filenum)
{
    for (int i=0; i<filenum; i++)
    {
        delete []filenames[i];
        filenames[i] = 0;
    }
    delete []filenames;
}

void print_advice(char **&filenames, int filenum,
                  int errnum)
{
    if (errnum != 0)
    {
        printf("\nUse \"-h\" flag for help.\n\n");
        free_everything(filenames, filenum);
        exit(1);
    }
}

void lstring2file(SReference ref, int &errnum)
{
    FILE* fl = fopen(ref.Car().Car().GetString(), "w");
    if (!fl)
    {
        print_error(errnum, CANTCREATE, ref.Car().Car().GetString());
        return;
    }
    fputs(ref.Cdr().Car().Car().GetString(), fl);
    fclose(fl);
}

void compile(int &errnum) {
    int result;

	result = execute_cmd_lp(CXX, CXX, "-Wall", "-Wno-parentheses",
		"-ggdb", "-I..", "-O0",	"-c", "refalpart.cxx", NULL);

    if (result == -1)
    {
        print_error(errnum, CANTCOMPILE, 0);
        exit(1);
    }

	result = execute_cmd_lp(CXX, CXX, "-Wall", "-ggdb", "-I..", "-O3",
		"refalpart.o", "def_run.o", "-Lbin_lib", "-lsexpr",	"-lirefal",
		"-o", (binary_name != 0) ? binary_name : "a.out", NULL);

    if (result == -1)
    {
        print_error(errnum, CANTCOMPILE, 0);
        exit(1);
    }
}

void callrefal(char **&filenames, int filenum,
               int &errnum)
{
    RefalInit();
    RfReference res;
    res = R|R<VeryStart, (R|filenames[0]);
    for (int i=1; i<filenum-1; i++)
        res = res, (R|filenames[i]);
    res = res, (R|filenames[filenum-1])>R;
    free_everything(filenames, filenum);
    res.Evaluate();
    if (!res->GetFirst()) {
        res = 0;
        exit(1);
    }
    SReference result = res->ConvertToLisp();
    lstring2file(result.Car(), errnum);
    lstring2file(result.Cdr().Car(), errnum);
    if (errnum) {
        putchar('\n');
        exit(1);
    }
    if (wasbflag) {
        compile(errnum);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1 || (argc == 2 && is_minus_h(argv[1])))
        print_help();
    if (argc == 2 && is_minus_a(argv[1]))
        print_about();
    if (argc == 2 && is_minus_V(argv[1]))
        print_version();
    char **filenames;
    filenames = new char *[argc];
    for (int i=0; i<argc; i++)
    {
        filenames[i] = 0;
    }
    filenames[0] = new char[strlen(DEFAULT_DIRECTIVES)+1];
    strcpy(filenames[0], DEFAULT_DIRECTIVES);
    int filenum = 1;
    int errnum = 0;
    readfilenames(filenames, argc, argv, filenum, errnum);
    checkfiles(filenames, filenum, errnum);
    print_advice(filenames, filenum, errnum);
    callrefal(filenames, filenum, errnum);
}

