//   InteLib                                    http://www.intelib.org
//   The file tools/slexer.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <ctype.h>
#include "slexer.hpp"

#ifndef EOF
#define EOF (-1)
#endif

extern SReference Charp2LispNumber(const char* p);

SLabel LexemaEof("#<END OF FILE>");

IntelibSLexAnalyser::IntelibSLexAnalyser()
    : specchars(0), special_state(0), state(home),
      postponed_char(-1), line(1), error_message(0)
{}

IntelibSLexAnalyser::~IntelibSLexAnalyser()
{
    while(specchars) {
        SpecChar *tmp = specchars->next;
        delete specchars;
        specchars = tmp;
    }
}

bool IntelibSLexAnalyser::AddDelimiter(const char *prefix,
                                       const SReference &token)
{
    SpecChar *spc = AddSpecial(prefix, SpecChar::term);
    if(!spc) return false;
    delimiter_chars += prefix[0];
    spc->token = token;
    return true;
}

bool IntelibSLexAnalyser::AddNonDelimiter(const char *prefix,
                                          const SReference &token)
{
    SpecChar *spc = AddSpecial(prefix, SpecChar::non_delim);
    if(!spc) return false;
    spc->token = token;
    return true;
}

bool IntelibSLexAnalyser::AddTokenStarter(const char *prefix,
                                          SReference (*fun)(const char *))
{
    SpecChar *spc = AddSpecial(prefix, SpecChar::read_rest);
    if(!spc) return false;
    // in this case, we don't put the first char into delimiter_chars
    spc->makestring = fun;
    return true;
}

bool IntelibSLexAnalyser::AddQuotingToken(const char *prefix,
                                          SReference (*fun)(const char *))
{
    SpecChar *spc = AddSpecial(prefix, SpecChar::read_rest_spec);
    if(!spc) return false;
    // in this case, we don't put the first char into delimiter_chars
    spc->makestring = fun;
    return true;
}

bool IntelibSLexAnalyser::AddStringStarter(const char *prefix,
                                           int closer_char,
                                           SReference (*fun)(const char *))
{
    SpecChar *spc = AddSpecial(prefix, SpecChar::read_string);
    if(!spc) return false;
    delimiter_chars += prefix[0];
    spc->closer[0] = closer_char;
    spc->closer[1] = 0;
    spc->makestring = fun;
    return true;
}

bool IntelibSLexAnalyser::AddCommentStarter(const char *prefix,
                                            const char *closer)
{
    SpecChar *spc = AddSpecial(prefix, SpecChar::ignore_until);
    if(!spc) return false;
    delimiter_chars += prefix[0];
    spc->status = SpecChar::ignore_until;
    for(unsigned int i=0; i<sizeof(spc->closer)-1; i++) {
        if((spc->closer[i] = closer[i]) == 0) return true;
    }
#if INTELIB_RUNTIME_CHECKS == 1
    error_message = "comment closer is too long";
    return false; 
#else
    spc->closer[sizeof(spc->closer)-1] = 0;
#endif
}

IntelibSLexAnalyser::SpecChar*
IntelibSLexAnalyser::DoAddSpecial(SpecChar **p, const char *str,
                                  SpecChar::TokenKind tk)
{
    // str can't be empty here! that is, str[0] is definitely a char
    // let's see if the char is at this level
    if(!*p) {
        // we must place it right here!
        *p = new SpecChar;
        (*p)->ch = str[0];
        (*p)->sub = 0;
        (*p)->next = 0;
        if(str[1]) { // there must be a subtree
            (*p)->status = SpecChar::non_term;
            return DoAddSpecial(&((*p)->sub), str+1, tk);
        } else { // no subtree, terminate here
            (*p)->status = tk;
            return *p;
        }
    } else {
        // okay, the level exists (or we're actually traversing it
        // let's see what char is there
        if((*p)->ch == str[0]) {
            // here is it!
            if(str[1]) { // there must be a subtree
                if((*p)->status == SpecChar::read_rest ||
                   (*p)->status == SpecChar::read_rest_spec
                ) {
                    error_message =
                        "Previously-added extendable special lexem is "
                        "a strict prefix of the new lexem";
                    return 0;
                }
                return DoAddSpecial(&((*p)->sub), str+1, tk);
            } else { // no subtree, terminate here
                if((*p)->status != SpecChar::non_term) {
                    // lexem already exists!
                    error_message = "duplicate or conflicting special lexems";
                    return 0;
                }
                (*p)->status = tk;
                return *p;
            }
        } else {
            // just try the next position
            return DoAddSpecial(&((*p)->next), str, tk);
        }
    }
}

IntelibSLexAnalyser::SpecChar*
IntelibSLexAnalyser::AddSpecial(const char *str, SpecChar::TokenKind tk)
{
    if(!*str) {
        error_message = "can't add empty special lexem";
        return false;
    }
    return DoAddSpecial(&specchars, str, tk);
}

SReference IntelibSLexAnalyser::Get() const
{
    return SReference(lex, lexline);
}

void IntelibSLexAnalyser::Drop() { 
    state = home;
    buf = "";
    lex = SReference();
    error_message = 0;
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::FeedChar(int c)
{
    if(postponed_char != -1) {
        int pp = postponed_char;
        postponed_char = -1;
        FeedResult res = FeedChar(pp);
        switch(res) {
            case res_eof:
            case res_error:
            case res_ready:
                postponed_char = c;
                return res;
            case res_empty: 
            case res_continue:
                return FeedChar(c);
            default:
                return res_error;
        }
        throw IntelibX_bug();
    }
    if(c == '\n') line++;
    switch(state) {
        case home:
            return Home(c);
        case special:
        case special2:
            return Special(c);
        case special3:
            return Special3(c);
        case string:
            return String(c);
        case stringq:
            return Stringq(c);
        case token:
            return Token(c);
        case token_force:
            buf+=c;
            state = token;
            return res_continue;
        case comment:
#if 0
            if(c == EOF) {
                state = home;
                return res_eof;
            }
            if(c == '\n')
                state = home;
            return res_empty;
#endif
            return Comment(c);
        default:
            throw IntelibX_bug();
    }
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::Home(int c)
{
    lexline = line;
    switch(c) {
        case EOF: 
            return res_eof;
        case ' ':
        case '\n':
        case '\t':
        case '\r':
        case '\f':
        case '\v':
            return res_continue;
        default:
            buf = "";
            // check whether it's special
            special_state = &specchars;
            FeedResult res = Special(c);
            if(res != res_error) {
                return res;
            }
            // being here means it isn't special
            buf += c;
            state = token;
            string_finalizer = 0;
            return res_continue;
    }
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::Special(int c)
{
    if(c == EOF) {
        error_message = "eof within or right after a special lexem";
        return res_error;
    }
    for(SpecChar *p = *special_state; p; p = p->next) {
        if(p->ch == c) {
            buf += c;
            if(p->status == SpecChar::term) {
                if(p->sub) { // could be longer
                    state = special2;
                    lex = p->token;
                    special_state = &(p->sub);
                    return res_continue;
                } else {
                    lex = p->token;
                    buf = "";
                    state = home;
                    return res_ready; 
                }
            }
            if(p->status == SpecChar::non_delim) {
                state = special3;
                lex = p->token;
                special_state = &(p->sub);
                return res_continue;
            }
            if(p->status == SpecChar::read_rest) {
                state = token;
                buf = "";
                string_finalizer = p->makestring;
                return res_continue; 
            }
            if(p->status == SpecChar::read_rest_spec) {
                state = token_force;
                buf = "";
                string_finalizer = p->makestring;
                return res_continue; 
            }
            if(p->status == SpecChar::read_string) {
                state = string;
                buf = "";
                closer = p->closer;
                string_finalizer = p->makestring;
                return res_continue; 
            }
            if(p->status == SpecChar::ignore_until) {
                state = comment;
                buf = "";
                closer = p->closer;
                comment_closer_index = 0;
                return res_continue; 
            }
            if(!p->sub) // it's impossible for non_term
                throw IntelibX_bug();
            state = special;
            special_state = &(p->sub);
            return res_continue;
        }
    }
    if(state == special2) {
        // this means the previous char was actually the last of the lexem
        // this->lex is already assigned, by the way
        buf = "";
        state = home;
        postponed_char = c;
        return res_ready; 
    } else if(state == special3) {
        // this means that, well, it looks like a regular token
        state = token;
        return res_continue;
    } else {
        error_message = "unexpected special lexem";
        return res_error;
    }
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::Special3(int c)
{
    if(c==EOF || IsDelimiterChar(c) || isspace(c))
    {
        state = home;
        buf = "";
        if(!isspace(c)) {
            postponed_char = c;
        }
        return res_ready;
    } else {
        return Special(c);
    }
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::String(int c)
{
    switch(c) {
        case EOF:
            error_message = "eof in string";
            return res_error;
        case '\\':
            state = stringq;
            return res_continue;
        default:
            if(c == closer[0]) {
                lex = string_finalizer ?
                    string_finalizer(buf.c_str()) : SReference(buf);
                buf = "";
                state = home;
                return res_ready;
            } else {
                buf += c;
                return res_continue;
            }
    }
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::Stringq(int c)
{
    switch(c) {
        case EOF:
            error_message = "quoted eof";
            return res_error;
        case 'a': buf += '\a'; break;
        case 'b': buf += '\b'; break;
        case 'f': buf += '\f'; break;
        case 'n': buf += '\n'; break;
        case 'r': buf += '\r'; break;
        case 't': buf += '\t'; break;
        case 'v': buf += '\v'; break;
             // No, codes like \012 and \0xA are not implemented. Sorry.
             // Do you need them? If so, you're encouraged to
             // do this job and contribute it to me... or just
             // let me know, heh. In fact, the thing will require
             // at least one more state in this machine.
        default:
            buf += c;
    }
    state = string;
    return res_continue;
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::Token(int c)
{
    if(c==EOF || IsDelimiterChar(c) || isspace(c))
    {
        state = home;
        if(!isspace(c)) {
            postponed_char = c;
        }
        lex = string_finalizer ?
            string_finalizer(buf.c_str()) : ProcessToken(buf.c_str());
        buf = "";
        if(!lex.GetPtr()) {
            return res_error;
        }
        return res_ready;
    } else {
        buf += c;
        return res_continue;
    }
}

IntelibSLexAnalyser::FeedResult IntelibSLexAnalyser::Comment(int c)
{
    if(closer[comment_closer_index] == c) {
        comment_closer_index++;
        if(closer[comment_closer_index] == 0) {
            // end of comment
            state = home;
            return res_empty;
        }
    } else {
        comment_closer_index = 0;
    }
    return res_continue;
}

static bool IsNumber(const char *s, bool sign_ok = true, bool dot_ok = true)
{
    if(!s || s[0] == 0) {
        return true;
    } else if(isdigit(s[0])) {
        return IsNumber(s+1, false, dot_ok);
    } else if(dot_ok && s[0] == '.') {
        return IsNumber(s+1, false, false);
    } else if(sign_ok && (s[0] == '-' || s[0] == '+') && s[1] != 0) {
        return IsNumber(s+1, false, dot_ok);
    } else 
        return false;
}

SReference IntelibSLexAnalyser::ProcessToken(const char *s)
{
#if 0
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
    if(s[0] == '.' && s[1] == 0)
        return LexemaDot;
    if(s[0] == '#') {
        if(s[1] == '\\') {
            if(s[3] == '\0') {
                // single char, such as #\a
                return SReference(s[2]);
            } else {
                int i;
                for(i = 0; charnames[i].name; i++) {
                    if(strcasecmp(s+2, charnames[i].name) == 0)
                        return SReference(charnames[i].val);
                }
                error_message = "invalid character name";
                return SReference(); // error
            }
        } else {
            // just return it as it is, it might be another special
            return SReference(new SExpressionClassicAtom(s));
        }
    } else
#endif
    if(IsNumber(s)) {
        return Charp2LispNumber(s);
    } else {
        return SReference(new SExpressionClassicAtom(s));
    }
}

bool IntelibSLexAnalyser::IsDelimiterChar(int ch)
{
    for(const char *tmp = delimiter_chars.c_str(); *tmp; tmp++)
        if(ch == (int)*tmp) return true;
    return false;
}
