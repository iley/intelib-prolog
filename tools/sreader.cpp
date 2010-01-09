//   InteLib                                    http://www.intelib.org
//   The file tools/sreader.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <string.h>
#include <ctype.h>

#include "slexer.hpp"
#include "sreader.hpp"

static SLabel LexemaQuote("#'#");
SLabel& IntelibGenericReader::EofMarker = LexemaEof;

IntelibGenericReader::IntelibGenericReader()
{ 
    the_package = &TheDummyPackage;
    uppercase = true;
    lexer = new IntelibSLexAnalyser;
    first_sl = 0;
    unclosed_brackets = 0;
}

IntelibGenericReader::~IntelibGenericReader()
{
    while(first_sl) {
        SpecialLexic *tmp = first_sl->next;
        delete first_sl;
        first_sl = tmp;
    }
    delete lexer;
}

IntelibGenericReader::SpecialLexic*
IntelibGenericReader::AddSpecialLexic(const char *str,
                                      SpecialLexic::sl_type t,
                                      process_function proc)
{
    SpecialLexic *tmp = new SpecialLexic;
    tmp->id = new SExpressionString(str);
    tmp->t = t;
    tmp->process = proc;
    tmp->next = first_sl;
    first_sl = tmp;
    return tmp;
}

#if 0
void IntelibGenericReader::AddInvalidLexem(const char *str)
{
    AddSpecialLexic(str, SpecialLexic::invalid, 0, false);
}
#endif

void IntelibGenericReader::
AddDelimiter(const char *str, const SReference &tok)
{
    bool res = lexer->AddDelimiter(str, tok);
    INTELIB_ASSERT(res, IntelibX_reader_error(lexer->GetErrorMessage(), -1));
}

void IntelibGenericReader::
AddToken(const char *str, const SReference &tok)
{
    bool res = lexer->AddNonDelimiter(str, tok);
    INTELIB_ASSERT(res, IntelibX_reader_error(lexer->GetErrorMessage(), -1));
}

void IntelibGenericReader::
AddTokenType(const char *str, SReference (*fun)(const char*))
{
    bool res = lexer->AddTokenStarter(str, fun);
    INTELIB_ASSERT(res, IntelibX_reader_error(lexer->GetErrorMessage(), -1));
}

void IntelibGenericReader::
AddStringLiteral(const char *str, int closing_char,
                 SReference (*fun)(const char*))
{
    bool res = lexer->AddStringStarter(str, closing_char, fun);
    INTELIB_ASSERT(res, IntelibX_reader_error(lexer->GetErrorMessage(), -1));
}

void IntelibGenericReader::AddQuoter(const char *str, process_function proc)
{
    SpecialLexic *sl = AddSpecialLexic(str, SpecialLexic::quoter, proc);
    bool res = lexer->AddDelimiter(str, sl->id);
    INTELIB_ASSERT(res, IntelibX_reader_error(lexer->GetErrorMessage(), -1));
}

void IntelibGenericReader::AddSequenceOpener(const char *str,
                                             process_function proc,
                                             const char *closer,
                                             const char *cons_sign,
                                             bool cons_sign_delimiter)
{
    SReference closer_id;
    // first, check if the closer is already there
    for(SpecialLexic *p = first_sl; p; p = p->next) {
        if(p->id == closer) {
            closer_id = p->id;
            break;
        }
    }
    if(!closer_id.GetPtr()) {
        SpecialLexic *cls =
            AddSpecialLexic(closer, SpecialLexic::seq_closer, 0);
        lexer->AddDelimiter(closer, cls->id);
        closer_id = cls->id;
    }

    // next, if there's a cons_sign...
    SReference cons_sign_id;
    if(cons_sign) {
        for(SpecialLexic *p = first_sl; p; p = p->next) {
            if(p->id == cons_sign) {
                cons_sign_id = p->id;
                break;
            }
        }
        if(!cons_sign_id.GetPtr()) {
            SpecialLexic *cls =
                AddSpecialLexic(cons_sign, SpecialLexic::cons_sign, 0);
            if(cons_sign_delimiter)
                lexer->AddDelimiter(cons_sign, cls->id);
            else
                lexer->AddNonDelimiter(cons_sign, cls->id);
            cons_sign_id = cls->id;
        }
    }

    // now add the opener
    SpecialLexic *sl = AddSpecialLexic(str, SpecialLexic::sequence, proc);
    lexer->AddDelimiter(str, sl->id);
    sl->the_closer = closer_id;
    sl->the_cons_sign = cons_sign_id;
}

void IntelibGenericReader::AddComment(const char *starter, const char *closer)
{
    lexer->AddCommentStarter(starter, closer);
}

void IntelibGenericReader::SetLine(int l, const SString &afn)
{
    lexer->SetLine(l);
    file_name = afn;
}

int IntelibGenericReader::GetLine() const
{
    return lexer->GetLine();
}

void IntelibGenericReader::FeedChar(int c)
{
    SListConstructor L;
    switch(lexer->FeedChar(c)) {
        case IntelibSLexAnalyser::res_empty: 
        case IntelibSLexAnalyser::res_continue:
            break; /* nothing to do */
        case IntelibSLexAnalyser::res_ready:
            HandleLexema(lexer->Get());
            break;
        case IntelibSLexAnalyser::res_eof:
            HandleEof();
            break;
        case IntelibSLexAnalyser::res_error:
            throw IntelibX_reader_error(
                SString("lexical error: ") + lexer->GetErrorMessage(),
                lexer->GetLine(),
                -1,
                file_name
            );
    }
}

void IntelibGenericReader::FeedString(const char *s)
{
    while(*s) FeedChar(*s++);
}

SReference IntelibGenericReader::Get()
{
    INTELIB_ASSERT(IsReady(), IntelibX_reader_not_ready());
    SReference res = ready.Car();
    ready.RemoveFirst();
    return res;
}

void IntelibGenericReader::Drop()
{
    lexer->Drop();
    while(ready.RemoveFirst()) {}
    while(parts.RemoveFirst()) {}
    unclosed_brackets = 0;
}

SReference IntelibGenericReader::Read(const SStreamRef& stream)
{
    SExpressionStreamTextInput *sti =
        stream.DynamicCastGetPtr<SExpressionStreamTextInput>();
    if(sti)
        lexer->SetLine(sti->TellLine());
    int c;
    do {
        c = stream->Getc();
        FeedChar(c);
    } while(!IsReady() && c != EOF);
    if(IsReady())
        return Get();
    else
        return EofMarker;
}

#if 0
static bool is_prefix(const char *pr, const char *w)
{
    for(;;pr++,w++) {
        if(!*pr) return true;
        if(*pr != *w) return false; // this covers w shorter that pr
    }
}
#endif

static SLabel special_modifier("#<SM>");

void IntelibGenericReader::HandleSpecialLex(SpecialLexic *sp, int line)
{
    SListConstructor L;
    switch(sp->t) {
        case SpecialLexic::quoter:
            parts.Append((L|sp->id, line)||special_modifier);
            return;
        case SpecialLexic::sequence:
            parts.Append((L|sp->id, line)||special_modifier);
            unclosed_brackets++;
            return;
        case SpecialLexic::seq_closer:
            if(parts.IsEmpty() || unclosed_brackets<1) {
                throw IntelibX_reader_error("unexpected sequence closer",
                                            line, -1, file_name);
            }
            parts.Append((L|sp->id, line)||special_modifier);
            unclosed_brackets--;
            if(unclosed_brackets == 0) 
                FinishParts();
            return;
        case SpecialLexic::cons_sign:
            if(parts.IsEmpty() || unclosed_brackets<1) {
                throw IntelibX_reader_error("unexpected pair constructor",
                                            line, -1, file_name);
            }
            parts.Append((L|sp->id, line)||special_modifier);
            return;
    }
}

void IntelibGenericReader::HandleLexema(const SReference &lexpair)
{
    SListConstructor L;
    SReference lex = lexpair.Car();
    int line = lexpair.Cdr().GetInt();

    // first of all, determine whether it is special
    for(SpecialLexic *p = first_sl; p; p = p->next) {
        if(p->id.GetPtr() == lex.GetPtr()) {
            HandleSpecialLex(p, line);
            return;
        }
    }

   SExpressionClassicAtom *ca = 
       lex.SimpleCastGetPtr<SExpressionClassicAtom>();
   if(ca) { 
       lex = DoMakeSymbol(ca->GetValue());
   }
   if(parts.IsEmpty()) {
       ready.Append(lex);
   } else {
       parts.Append((L|lex, lexpair.Cdr()));
       if(unclosed_brackets == 0) FinishParts();
   }
}

SReference IntelibGenericReader::DoMakeSymbol(const char *s) const
{
    if(uppercase) {
        char *str = new char[strlen(s)+1];
        int i;
        for(i = 0; s[i]!='\0'; i++) str[i] = toupper(s[i]);
        str[i] = 0;
        SReference tmp = the_package->MakeSymbol(str);
        delete[] str;
        return tmp;
    } else {
        return the_package->MakeSymbol(s);
    }
}

void IntelibGenericReader::HandleEof()
{
    if(!parts.IsEmpty()) {
        throw IntelibX_reader_error("unexpected eof", 
                                    lexer->GetLine(),
                                    parts.Car().Cdr().Car().GetInt(),
                                    file_name);
    }
    ready.Append(EofMarker);
}

void IntelibGenericReader::FinishParts()
{
    ready.Append(DoFinishParts());
    while(parts.RemoveFirst()) {}
    unclosed_brackets = 0;
}

SReference IntelibGenericReader::DoFinishParts()
{
    SReference lex = parts.Car().Car();
    int line = parts.Car().Cdr().Car().GetInt();
    SReference modifier = parts.Car().Cdr().Cdr();
    parts.RemoveFirst();

    if(modifier.IsEmptyList()) {
        // nothing special...
        return lex;
    }

    // okay, it's definitely something special
    for(SpecialLexic *p = first_sl; p; p = p->next) {
        if(p->id.GetPtr() == lex.GetPtr()) {
            switch(p->t) {
                case SpecialLexic::quoter:
                    return p->process(DoFinishParts());
                case SpecialLexic::sequence:
                    return p->process(
                        DoFinishPartsSeq(p->the_closer,
                                         p->the_cons_sign, line));
                case SpecialLexic::seq_closer:
                    throw IntelibX_reader_error("unexpected sequence closer", 
                                    line, -1, file_name);
                case SpecialLexic::cons_sign:
                    throw IntelibX_reader_error("unexpected pair constructor", 
                                    line, -1, file_name);
            }
        }
    }

    // being here means the lex is marked as special but not found in the
    // list of special lexems
    throw IntelibX_bug();
}

SReference IntelibGenericReader::
DoFinishPartsSeq(const SReference& closer,
                 const SReference& conser,
                 int begline)
{
    SQueue res;
    for(;;) {
        SReference lex = parts.Car().Car();
        //int line = parts.Car().Cdr().Car().GetInt();
        SReference modifier = parts.Car().Cdr().Cdr();

        if(lex.GetPtr() == closer.GetPtr()) { // end of the seq
            parts.RemoveFirst();
            if(res.IsEmpty()) {
                return *PTheEmptyList;
            } else {
                return SReference(res);
            }
        } else 
        if(conser.GetPtr() && (lex.GetPtr() == conser.GetPtr())) { 
            parts.RemoveFirst();
            SReference last = DoFinishParts();
            if(parts.Car().Car().GetPtr() != closer.GetPtr()) {
                throw IntelibX_reader_error("sequence closer expected",
                                     parts.Car().Cdr().Car().GetInt(),
                                     begline, file_name);
            }
            parts.RemoveFirst();
            if(res.IsEmpty()) {
                throw IntelibX_reader_error("incorrect dotted sequence",
                                     parts.Car().Cdr().Car().GetInt(),
                                     begline, file_name);
            }
            SReference res_r(res);
            res_r||last;
            return res_r;
        } else 
        if(modifier.IsEmptyList()) {
            res.Append(lex); 
            parts.RemoveFirst();
        } else {
            // this is in turn something special
            res.Append(DoFinishParts());   
        }
    }    
}

////////////////////////////////////////////////////////
// IntelibReader 

static SReference process_char_escape(const char *name)
{
    static const struct {
        const char *name;
        char val;
    } charnames[] = {
        { "NEWLINE", '\n' },
        { "SPACE", ' ' },
        { "TAB", '\t' },
        { "BACKSPACE", '\010' },
        { "LINEFEED", '\012' },
        { "PAGE", '\014' },
        { "RETURN", '\015' },
        { "RUBOUT", '\177' },
        { 0, 0 }
    };
    if(name[1] == 0) { // single char
        return SReference((char)(name[0]));
    }
    int i;
    for(i = 0; charnames[i].name; i++) {
        if(strcasecmp(name, charnames[i].name) == 0)
            return SReference(charnames[i].val);
    }
    throw IntelibX_reader_error("unknown_char_name", -1, -1, "");
}

static SReference process_plain_list(const SReference &a)
{
    return a;
}

IntelibReader::IntelibReader()
{
    AddTokenType("#\\", process_char_escape);
    AddStringLiteral("\"", '\"');
    AddSequenceOpener("(", process_plain_list, ")", ".", false);
    AddComment(";");
}

////////////////////////////////////////////////////////
// Dummy package

SReference IntelibDummyPackage::MakeSymbol(const char *s)
{
    return SReference(new SExpressionClassicAtom(s));
}

IntelibDummyPackage TheDummyPackage;


/////////////////////////////
//

IntelibGenericReader *PTheIntelibReader = 0;

/////////////////////
// exceptions


IntelibX_reader_error::
IntelibX_reader_error(SReference a_param, int a_line, int a_b_line,
                      const SString &afn) 
    : IntelibX("Reader encountered an error", a_param), fname(afn)
 
{ line = a_line; beg_line = a_b_line; }

IntelibX_reader_not_ready::
IntelibX_reader_not_ready() 
    : IntelibX("IntelibGenericReader::Get() called when IsReady() is false") 
{}

IntelibX_read_eof::
IntelibX_read_eof(SReference a_param) 
    : IntelibX("Unexpected end of file", a_param) 
{}
