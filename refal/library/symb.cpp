// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#include <sys/param.h>
#include <sys/stat.h>

#include "../win_port/win_port.hpp"

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


#include "../../sexpress/sstring.hpp"

#include "../rvars.hpp"
#include "../rclause.hpp"

#include "symb.hpp"



static void symb_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp || temp->next || temp->IsChar() || temp->IsMarkup())
        throw IntelibX_refal_failure(ref);
    ref = new RfExpression(temp->symb_val->TextRepresentation().c_str());
}

static void numb_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp)
        throw IntelibX_refal_failure(ref);
   SString str = temp->TextRepresentation();
    while (temp) {
        if (!temp->IsChar())
            throw IntelibX_refal_failure(ref);
        temp = temp->next;
    }
    char *check;
    int res_int = strtol(str.c_str(),
                         &check, 0);
    if (*check == '\0') {
        ref = new RfExpression(res_int);
        return;
    }
    float res_float = strtod(str.c_str() + 1,
                             &check);
    if (*check == '\0') {
        ref = new RfExpression(res_float);
        return;
    }
    throw IntelibX_refal_failure(ref);
}

static void ord_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp || temp->next || !temp->IsChar())
        throw IntelibX_refal_failure(ref);
    ref = new RfExpression(temp->charcode);
}

static void chr_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp || temp->next)
        throw IntelibX_refal_failure(ref);
    SExpressionInt *ti;
    ti = temp->symb_val.DynamicCastGetPtr<SExpressionInt>();
    if (!ti)
        throw IntelibX_refal_failure(ref);
    char str[2];
    str[0] = (char)(ti->GetValue() % 256);
    str[1] = '\0';
    ref = new RfExpression(str);
}

bool is_capital(int charcode)
{
    return (charcode >= 'A' && charcode <= 'Z');
}

bool is_lower(int charcode)
{
    return (charcode >= 'a' && charcode <= 'z');
}

bool good_first(int charcode)
{
    return (charcode == '_' || is_capital(charcode) || is_lower(charcode));
}

bool good_next(int charcode)
{
    return (good_first(charcode) || charcode == '-');
}

bool is_ident(char *str)
{
    if (!good_first(*str))
        return false;
    str++;
    while (*str) {
        if (!good_next(*str))
            return false;
        str++;
    }
    return true;
}

const char *get_type(RfListItem *typed)
{
    if (!typed)
        return "*0";
    if (typed->IsChar()) {
        if (is_capital(typed->charcode))
            return "Lu";
        if (is_lower(typed->charcode))
            return "Ll";
        return "Ol";
    }
    if (typed->IsLeftMarkup())
        return "B0";
    SExpressionInt *ti;
    ti = typed->symb_val.DynamicCastGetPtr<SExpressionInt>();
    if (ti)
        return "N0";
    SExpressionFloat *tf;
    tf = typed->symb_val.DynamicCastGetPtr<SExpressionFloat>();
    if (tf)
        return "D0";
    SString repr = typed->symb_val->TextRepresentation();
    char *str = (char *)repr.c_str();
    if (is_ident(str))
        return "Wi";
    return "Wq";
}

static void type_function(RfReference &ref)
{
    const char *type_char = get_type(ref->GetFirst());
    RfFormConstructor R;
    RfContext context;
    RfVariable_E e_Foo("e.Foo");
    RefalMatchProcess(ref, (R|e_Foo), context);
    ref = RefalSubstitute((R|type_char, e_Foo), context, true);
}

int count_len(RfListItem *start)
{
    int len = 0;
    for (RfListItem *i = start; i; i = i->next) {
        len++;
        if (i->IsMarkup())
            i = i->pair;
    }
    return len;
}

static void lenw_function(RfReference &ref)
{
    int len = count_len(ref->GetFirst());
    RfFormConstructor R;
    RfContext context;
    RfVariable_E e_Foo("e.Foo");
    RefalMatchProcess(ref, (R|e_Foo), context);
    ref = RefalSubstitute((R|len, e_Foo), context, true);
}

bool is_symbol(RfListItem *typed) {
    if (!typed || typed->IsLeftMarkup())
        return false;
    SExpressionLabel *tl;
    tl = typed->symb_val.DynamicCastGetPtr<SExpressionLabel>();
    return (tl != 0);
}

static void explode_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!is_symbol(temp) || temp->next)
        throw IntelibX_refal_failure(ref);
    ref = new RfExpression(temp->symb_val->TextRepresentation().c_str());
}

static void mu_function(RfReference &ref)
{
    RfFormConstructor R;
    RfContext context;

    RfExpressionVariable_E *e_Foo = new RfExpressionVariable_E("e.Foo");
    context.AddVariable(e_Foo, ref->GetFirst(),
                        ref->GetLast(), ref->GetLast());

    RfReference expr =
	RefalSubstitute((R|R<e_Foo>R), context, true);

    expr.Evaluate();

    ref = expr;
}

static void exit_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    int ret_value = 0;
    if (temp) {
    	if (temp->next)
            throw IntelibX_refal_failure(ref);
        SExpressionInt *ti;
        ti = temp->symb_val.DynamicCastGetPtr<SExpressionInt>();
        if (!ti)
            throw IntelibX_refal_failure(ref);
        ret_value = ti->GetValue();
    }
    exit(ret_value);
}

static void compress_function(RfReference &ref)
{
    RfListItem *copy_start = 0;
    int bytes_left = 0;
    int value = 0;
    RfReference temp = new RfExpression;
    for (RfListItem *i = ref->GetFirst(); i; i = i->next) {
        if (!i->IsChar())
            throw IntelibX_refal_failure(ref);
	if ((i->charcode & 0xFFFFFF00) != 0)
	    continue;
	if ((i->charcode & 0x80) == 0) {
            if (i->prev != 0) {
                if (!copy_start) {
                    copy_start = i;
                }
            }
            else {
	        temp->QuickAdd(new RfListItem(i->charcode));
            }
	    continue;
	}
        if (copy_start) {
            CopyAll(temp, copy_start, i->prev);
            copy_start = 0;
        }
	if ((i->charcode & 0x40) == 0) {
	    if (bytes_left <= 0)
	        continue;
	    value <<= 8;
	    value += i->charcode;
	    bytes_left--;
	    if (bytes_left == 0) {
	        temp->QuickAdd(new RfListItem(value));
            }
	    continue;
	}
	if ((i->charcode & 0x20) == 0) {
	    value = i->charcode;
            bytes_left = 1;
            continue;
	}
	if ((i->charcode & 0x10) == 0) {
            value = i->charcode;
            bytes_left = 2;
            continue;
        }
        if ((i->charcode & 0x8) == 0) {
            value = i->charcode;
            bytes_left = 3;
            continue;
        }
    }
    if (copy_start) {
        CopyAll(temp, copy_start, ref->GetLast());
    }
    ref = temp;
}

static void decompress_function(RfReference &ref)
{
    RfListItem *copy_start = 0;
    RfReference res = new RfExpression;
    for (RfListItem *elem = ref->GetFirst(); elem; elem = elem->next) {
        if (!elem->IsChar())
	    throw IntelibX_refal_failure(ref);
        if ((elem->charcode & 0xFFFFFF00) == 0) {
            if (elem->prev != 0) {
                if (!copy_start) {
                    copy_start = elem;
                }
            }
            else {
                res->QuickAdd(new RfListItem(elem->charcode));
            }
            continue;
        }
        if (copy_start) {
            CopyAll(res, copy_start, elem->prev);
            copy_start = 0;
        }
        int c[4] = {0, 0, 0, 0};
        int temp = elem->charcode;
        for (int i = 3; i >= 0; i--) {
            c[i] = temp & 0xFF;
            if (c[i] == 0)
                break;
            temp >>= 8;
        }
        for (int i = 0; i < 4; i++) {
	    if (c[i]) {
                res->QuickAdd(new RfListItem(c[i]));
            }
        }
    }
    if (copy_start) {
        CopyAll(res, copy_start, ref->GetLast());
    }
    ref = res;
}

static void getpid_function(RfReference &ref)
{
    if (ref->GetFirst())
        throw IntelibX_refal_failure(ref);
    RfFormConstructor R;
    ref = (R|getpid());
}

static void getppid_function(RfReference &ref)
{
    if (ref->GetFirst())
        throw IntelibX_refal_failure(ref);
    RfFormConstructor R;
    ref = (R|getppid());
}

static void getcurrentdirectory_function(RfReference &ref)
{
    if (ref->GetFirst())
        throw IntelibX_refal_failure(ref);
    char buf[MAXPATHLEN];
    char *wd = getcwd(buf, sizeof(buf));
    RfFormConstructor R;
    ref = (R|wd);
}

static void getenv_function(RfReference &ref)
{
    if (!ref->GetFirst()) {
        ref = new RfExpression;
        return;
    }
    char *value = getenv((char *)ref->GetFirst()->TextRepresentation().c_str());
    RfFormConstructor R;
    ref = value ? (R|value) : new RfExpression;
}

static void removefile_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp)
        throw IntelibX_refal_failure(ref);
    SString str = temp->TextRepresentation();
    const char *filename = str.c_str();
    RfFormConstructor R;
    if (unlink(filename) == 0) {
        ref = (R|True, (~R));
        return;
    }
    else {
        ref = (R|False, (R|strerror(errno)));
        return;
    }
}

static void existfile_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp)
        throw IntelibX_refal_failure(ref);
    SString str = temp->TextRepresentation();
    const char *filename = str.c_str();
    RfFormConstructor R;
    struct stat file_info;
    ref = (R|(stat(filename, &file_info) ? False : True));
}

static void time_function(RfReference &ref)
{
    long temp = time(0);
    char *time_string = ctime(&temp);    
    ref = new RfExpression;
    if (!time_string)
        return;
    char c[2] = " ";
    while (*time_string != '\n') {
        c[0] = *time_string;
        ref = ref, c;
        time_string++;
    }
}

static void listofbuiltin_function(RfReference &ref);

RfLibSymbol Symb("Symb", symb_function);
RfLibSymbol Numb("Numb", numb_function);
RfLibSymbol Ord("Ord", ord_function);
RfLibSymbol Chr("Chr", chr_function);
RfLibSymbol Type("Type", type_function);
RfLibSymbol Lenw("Lenw", lenw_function);
RfLibSymbol Explode("Explode", explode_function);
RfLibSymbol Mu("Mu", mu_function);
RfLibSymbol Exit("Exit", exit_function);
RfLibSymbol Compress("Compress", compress_function);
RfLibSymbol Decompress("Decompress", decompress_function);
RfLibSymbol GetPID("GetPID", getpid_function);
RfLibSymbol GetPPID("GetPPID", getppid_function);
RfLibSymbol GetCurrentDirectory("GetCurrentDirectory",
                                getcurrentdirectory_function);
RfLibSymbol GetEnv("GetEnv", getenv_function);
RfLibSymbol RemoveFile("RemoveFile", removefile_function);
RfLibSymbol ExistFile("ExistFile", existfile_function);
RfLibSymbol ListOfBuiltin("ListOfBuiltin", listofbuiltin_function);
RfLibSymbol Time("Time", time_function);

extern RfLibSymbol Print;
extern RfLibSymbol Prout;
extern RfLibSymbol Card;
extern RfLibSymbol Open;
extern RfLibSymbol Get;
extern RfLibSymbol Put;
extern RfLibSymbol Putout;
extern RfLibSymbol Close;
extern RfLibSymbol Arg;
extern RfLibSymbol Argc;

extern RfLibSymbol Add;
extern RfLibSymbol Sub;
extern RfLibSymbol Mul;
extern RfLibSymbol Div;
extern RfLibSymbol Mod;
extern RfLibSymbol Real;
extern RfLibSymbol Trunc;
extern RfLibSymbol Compare;
extern RfLibSymbol Sin;
extern RfLibSymbol Cos;
extern RfLibSymbol Tan;
extern RfLibSymbol Asin;
extern RfLibSymbol Acos;
extern RfLibSymbol Atan;
extern RfLibSymbol Pi;
extern RfLibSymbol Sqrt;
extern RfLibSymbol Exp;
extern RfLibSymbol Log;
extern RfLibSymbol Log10;
extern RfLibSymbol Pow;

extern RfLibSymbol Br;
extern RfLibSymbol Dg;
extern RfLibSymbol Rp;
extern RfLibSymbol Cp;
extern RfLibSymbol Dgall;
extern RfLibSymbol Clear;

RfReference namesofbuiltin()
{
    RfFormConstructor R;
    return (R|Mu,
              Add, Arg, Br, Card, Chr, Cp, Dg, Dgall,
              Div, Explode, Get, Lenw, Mod, Mul, Numb,
              Open, Ord, Print, Prout, Put, Putout,
              Rp, Sub, Symb, Time, Type, GetEnv, Exit, Close,
              ExistFile, GetCurrentDirectory, RemoveFile,
              Compare, ListOfBuiltin, GetPID, GetPPID,
              //Unimplemented in standard Refal-5:
              Argc, Real, Trunc, Sin, Cos, Tan,
              Asin, Acos, Atan, Pi, Sqrt, Exp, Log, Log10,
              Pow, Compress, Decompress,
              Clear);
}

static void listofbuiltin_function(RfReference &ref)
{
    RfFormConstructor R;
    RfReference names = namesofbuiltin();
    RfListItem *temp = names->GetFirst();
    int i = 1;
    ref = new RfExpression;
    while (temp) {
        //temp->next checking is a cheat
        //it works only because we have only one special function
	//(Mu) implemented
        ref = ref, (R|i, temp->symb_val, temp->prev ? regular : special);
        i++;
        temp = temp->next;
    }
}
