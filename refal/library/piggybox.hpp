// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#define PIGGYBOXSIZE 1003



bool are_equal(RfReference &ref1, RfReference &ref2);

int hash_function(RfReference &key);

void initarg(int argc);

struct RfPiggyBoxValue {
    RfReference value;
    RfPiggyBoxValue *next;

    RfPiggyBoxValue(RfReference &value);
    ~RfPiggyBoxValue();
    
    void Dgall(RfReference &ref, RfReference &key, bool flag);
};

struct RfPiggyBoxKey {
    RfPiggyBoxValue *values;
    RfReference key;
    RfPiggyBoxKey *next;

    RfPiggyBoxKey(RfReference &key, RfReference &value);
    ~RfPiggyBoxKey();

    void BuryOrReplaceExpression(RfReference &value, bool bury_flag);
    void Dgall(RfReference &ref);
    void DgValues(RfReference &ref);
};

struct RfFile {
    RfReference key;
    FILE *fd;
    RfFile *next;

    RfFile(RfReference &key, FILE *fd);
    ~RfFile();
};

class RfPiggyBox {
    RfPiggyBoxKey *elems[PIGGYBOXSIZE];
    RfFile *files;
    int argc;
    char **argv;

    public:

    RfPiggyBox();
    ~RfPiggyBox();

    void BuryOrReplaceExpression(RfReference &key, RfReference &value,
                                 int hash_fun, bool flag);
    RfPiggyBoxKey *GetKey(RfReference &key);
    RfReference DigValue(RfPiggyBoxKey *key);
    RfReference CopyValue(RfPiggyBoxKey *key);
    RfReference DigExpression(RfReference &key);
    RfReference CopyExpression(RfReference &key);
    void Clear();
    void Dgall(RfReference &ref);
    void OpenFile(RfReference &key, char *name, char *mode);
    void CloseFile(RfReference &key);
    FILE *GetDesc(RfReference &key);
    void InitArg(int argc, char **argv);
    int GetArgc();
    char *GetArg(int num);
};


