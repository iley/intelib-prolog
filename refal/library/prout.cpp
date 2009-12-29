// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




// Modified by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#include <stdio.h>
#include <string.h>


#include "../../sexpress/sstring.hpp"

#include "../refal.hpp"
#include "../rvars.hpp"
#include "../rclause.hpp"

#include "piggybox.hpp"

#define BUFSIZE 1024



extern RfPiggyBox box;



RfFile::RfFile(RfReference &key, FILE *fd)
{
    this->key = key;
    this->fd = fd;
    this->next = 0;
}

RfFile::~RfFile()
{
    if (fd) {
        fclose(fd);
        fd = 0;
    }
    if (next) {
        delete next;
        next = 0;
    }
}

void RfPiggyBox::OpenFile(RfReference &key, char *name, char *mode)
{
    FILE *fd = fopen(name, mode);

    if (!fd)
        throw IntelibX_refal_failure(0);
    
    RfFile *start = files;
    while (start) {
        if (are_equal(start->key, key))
            throw IntelibX_refal_failure(0);
        start = start->next;
    }

    if (!files) {
        files = new RfFile(key, fd);
    }
    else {
        RfFile *temp = new RfFile(key, fd);
        temp->next = files;
        files = temp;
    }
}

FILE *RfPiggyBox::GetDesc(RfReference &key)
{
    RfFile *temp = files;
    while (temp) {
        if (are_equal(temp->key, key))
            return temp->fd;
        temp = temp->next;
    }
    return 0;
}

void RfPiggyBox::CloseFile(RfReference &key)
{
    if (!files)
        throw IntelibX_refal_failure(key);
    RfFile *temp;
    if (are_equal(files->key, key)) {
        temp = files;
        files = files->next;
        temp->next = 0;
        delete temp;
        return;
    }
    RfFile *start = files;
    while (start->next) {
        if (are_equal(start->next->key, key)) {
            temp = start->next;
            start->next = start->next->next;
            temp->next = 0;
            delete temp;
            return;
        }
        start = start->next;
    }
    throw IntelibX_refal_failure(key);
}

void RfPiggyBox::InitArg(int argc, char **argv)
{
    this->argc = argc;
    this->argv = argv;
}

int RfPiggyBox::GetArgc()
{
    return argc;
}

char *RfPiggyBox::GetArg(int num)
{
    return (num < argc) ? argv[num] : 0;
}

static void output_function(RfReference &ref, FILE *fd)
{
    for (RfListItem *i = ref->GetFirst(); i; i = i->next)
    {
        if (i->IsChar())
        {
            if ((i->charcode & 0xFFFFFF00) == 0)
                putc(i->charcode, fd);
            else {
                signed char c[4] = {0, 0, 0, 0};
                int temp = i->charcode;
                for (int i = 3; i >= 0; i--) {
                    c[i] = temp & 0xFF;
                    if (c[i] == 0)
                        break;
                    temp >>= 8;
                }
                for (int i = 0; i < 4; i++) {
		    if (c[i])
                        putc(c[i], fd);
                }
            }
        }
        else
        if (i->symb_val == RfListItem::left_par)
        {
            putc('(', fd);
        }
        else
        if (i->symb_val == RfListItem::right_par)
        {
            putc(')', fd);
        }
        else
        if (i->symb_val == RfListItem::left_call)
        {
            putc('<', fd);
        }
        else
        if (i->symb_val == RfListItem::right_call)
        {
            putc('>', fd);
        }
        else
        {
            fputs(i->symb_val->TextRepresentation().c_str(), fd);
            putc(' ', fd);
        }
    }
    putc('\n', fd);
}

static void print_function(RfReference &ref)
{
    output_function(ref, stdout);
}

static void prout_function(RfReference &ref)
{
    print_function(ref);
    ref = new RfExpression; /* that is, just kill it */ 
}

static void input_function(RfReference &ref, FILE *inp)
{
    RfFormConstructor R;
    char *buf;
    buf = new char[BUFSIZE];
    char *temp;
    int size = BUFSIZE;
    int i = 0;
    int c;
    do
    {
        c = getc(inp);
        if (i == size-1)
        {
            temp = new char[size * 2];
            for (int j = 0; j < i; j++)
                temp[j] = buf[j];
            delete []buf;
            buf = temp;
            size *= 2;
        }
        if (c != EOF && c != '\n')
            buf[i] = c;
        else
            buf[i] = '\0';
        i++;
    } while (c != EOF && c != '\n');
    ref = (c != EOF) ? (R|buf) : (R|buf, 0);
    delete []buf;
}

static void card_function(RfReference &ref)
{
    input_function(ref, stdin);
}

static void open_function(RfReference &ref)
{
    RfFormConstructor R;
    RfContext context;

    RfVariable_S s_Mode("s.Mode");
    RfVariable_S s_Desc("s.Desc");
    RfVariable_E e_Name("e.Name");

    bool res_flag = RefalMatchProcess(ref,
                                      (R|s_Mode, s_Desc, e_Name),
                                      context);

    if (!res_flag)
        throw IntelibX_refal_failure(ref);

    RfReference mode = RefalSubstitute((R|s_Mode), context, false);
    RfReference desc = RefalSubstitute((R|s_Desc), context, false);
    RfReference name = RefalSubstitute((R|e_Name), context, true);

    if (!name->GetFirst())
        throw IntelibX_refal_failure(ref);

    box.OpenFile(desc,
                 (char *)name->GetFirst()->TextRepresentation().c_str(),
                 (char *)mode->GetFirst()->TextRepresentation().c_str());

    ref = new RfExpression;
}

static void close_or_get_function(RfReference &ref, bool flag)
{
    RfFormConstructor R;
    RfContext context;

    RfVariable_S s_Desc("s.Desc");

    bool res_flag = RefalMatchProcess(ref, (R|s_Desc), context);

    if (!res_flag)
        throw IntelibX_refal_failure(ref);

    RfReference desc = RefalSubstitute((R|s_Desc), context, false);
    FILE *fd = box.GetDesc(desc);
    if (!fd)
        throw IntelibX_refal_failure(ref);
    if (flag)
    {
        box.CloseFile(desc);
        ref = new RfExpression;
    }
    else
        input_function(ref, fd);
}

static void get_function(RfReference &ref)
{
    close_or_get_function(ref, false);
}

static void put_genfunction(RfReference &ref, bool flag)
{
    RfFormConstructor R;
    RfContext context;

    RfVariable_S s_Desc("s.Desc");
    RfVariable_E e_Expr("e.Expr");

    bool res_flag = RefalMatchProcess(ref, (R|s_Desc, e_Expr), context);

    if (!res_flag)
        throw IntelibX_refal_failure(ref);

    RfReference desc = RefalSubstitute((R|s_Desc), context, false);
    RfReference expr = RefalSubstitute((R|e_Expr), context, true);

    FILE *fd = box.GetDesc(desc);
    if (!fd)
        throw IntelibX_refal_failure(ref);

    output_function(expr, fd);    
    ref = flag ? new RfExpression : expr;
}

static void put_function(RfReference &ref)
{
    put_genfunction(ref, false);
}

static void putout_function(RfReference &ref)
{
    put_genfunction(ref, true);
}

static void close_function(RfReference &ref)
{
    close_or_get_function(ref, true);
}

void initarg(int argc, char **argv)
{
    box.InitArg(argc, argv);
}

static void argc_function(RfReference &ref)
{
    if (ref->GetFirst())
        throw IntelibX_refal_failure(ref);
    RfFormConstructor R;
    ref = (R|box.GetArgc());
}

static void arg_function(RfReference &ref)
{
    RfListItem *temp = ref->GetFirst();
    if (!temp || temp->next)
        throw IntelibX_refal_failure(ref);
    SExpressionInt *ti;
    ti = temp->symb_val.DynamicCastGetPtr<SExpressionInt>();
    if (!ti)
        throw IntelibX_refal_failure(ref);
    RfFormConstructor R;
    char *res = box.GetArg(ti->GetValue());
    ref = res ? (R|res) : new RfExpression();
}

RfLibSymbol Print("Print", print_function);
RfLibSymbol Prout("Prout", prout_function);
RfLibSymbol Card("Card", card_function);
RfLibSymbol Open("Open", open_function);
RfLibSymbol Get("Get", get_function);
RfLibSymbol Put("Put", put_function);
RfLibSymbol Putout("Putout", putout_function);
RfLibSymbol Close("Close", close_function);
RfLibSymbol Arg("Arg", arg_function);
RfLibSymbol Argc("Argc", argc_function);

