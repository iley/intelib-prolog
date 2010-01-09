//   InteLib                                    http://www.intelib.org
//   The file tests/sexpress/t_lexer.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include <stdio.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "tools/slexer.hpp"

void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

SReference string_dasher_fun(const char *str)
{
    SString s("--");
    s += str;
    s += "--";
    return s;
}

SReference char_esc_tran(const char *str)
{
    if(str[0] && !str[1]) return SReference(*str);
    SString s(str);
    if(s == "Tab") return SReference('\t');
    if(s == "Newline") return SReference('\n');
    if(s == "Space") return SReference(' ');
    return SReference('?');
}

SLabel UnreadableItem("#<->");
SReference unreadable_tran(const char */*str*/)
{
    return UnreadableItem;
}

SReference extvar_tran(const char *str)
{
    SString s("#&");
    s += str;
    s += "";
    return new SExpressionLabel(s.c_str());
}

int main()
{
    poc();
    try {
        TestSection("IntelibSLexAnalyser");
        TestSubsection("creation errors");
        {
            IntelibSLexAnalyser lexer;
            TESTB("created", true);
            TESTB("adding_lex", lexer.AddDelimiter("abc", "-abc-"));
            TESTB("no_adding_after_extendable_prefix",
                lexer.AddTokenStarter("def", string_dasher_fun) &&
                !lexer.AddDelimiter("defgh", "-defgh-"));
        }
        TestSubsection("normal operation");
        {
            IntelibSLexAnalyser lexer;

            TESTB("feed_symbol1", lexer.FeedChar('A') == lexer.res_continue);
            TESTB("feed_symbol2", lexer.FeedChar('B') == lexer.res_continue);
            TESTB("feed_symbol3", lexer.FeedChar('C') == lexer.res_continue);
            TESTB("feed_symbol4", lexer.FeedChar('D') == lexer.res_continue);
            TESTB("feed_symbol_spc", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_symbol", lexer.Get(), "(ABCD . 1)");

        }
        TestSubsection("delimiters");
        {
            IntelibSLexAnalyser lexer;
            SLabel OpenPar("#(#");
            lexer.AddDelimiter("(", OpenPar);

            TESTB("feed_symbol21", lexer.FeedChar('A') == lexer.res_continue);
            TESTB("feed_symbol22", lexer.FeedChar('B') == lexer.res_continue);
            TESTB("feed_symbol23", lexer.FeedChar('C') == lexer.res_continue);
            TESTB("feed_symbol_dlm", lexer.FeedChar('(') == lexer.res_ready);
            TESTTR("get_symbol", lexer.Get(), "(ABC . 1)");
            TESTB("space_after_dlm", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_dlm", lexer.Get(), "(#(# . 1)");

            TESTB("feed_lower1", lexer.FeedChar('a') == lexer.res_continue);
            TESTB("feed_lower2", lexer.FeedChar('b') == lexer.res_continue);
            TESTB("feed_lower3", lexer.FeedChar('c') == lexer.res_continue);
            TESTB("feed_lower_spc", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_lower_symbol", lexer.Get(), "(abc . 1)");

            for(int i=0; i<25; i++) lexer.FeedChar('\n');
            lexer.FeedChar('A');
            lexer.FeedChar(' ');
            TESTTR("line_counting", lexer.Get(), "(A . 26)");
        }
        TestSubsection("strings"); // TODO
        TestSubsection("numbers"); // TODO
        TestSubsection("dot");     // TODO
        TestSubsection("usual char escapes");
        {
            IntelibSLexAnalyser lexer;
            lexer.AddTokenStarter("#\\", char_esc_tran);
            SLabel OpenPar("#(#");
            lexer.AddDelimiter("(", OpenPar);

            TESTB("feed_cesc1", lexer.FeedChar('#') == lexer.res_continue);
            TESTB("feed_cesc2", lexer.FeedChar('\\') == lexer.res_continue);
            TESTB("feed_cesc3", lexer.FeedChar('(') == lexer.res_continue);
            TESTB("feed_cesc_spc", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_cesc", lexer.Get(), "(#\\( . 1)");

            TESTB("feed_spcesc1", lexer.FeedChar('#') == lexer.res_continue);
            TESTB("feed_spcesc2", lexer.FeedChar('\\') == lexer.res_continue);
            TESTB("feed_spcesc3", lexer.FeedChar(' ') == lexer.res_continue);
            TESTB("feed_spcesc_spc", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_spcesc", lexer.Get(), "(#\\Space . 1)");

            TESTB("feed_tab1", lexer.FeedChar('#') == lexer.res_continue);
            TESTB("feed_tab2", lexer.FeedChar('\\') == lexer.res_continue);
            TESTB("feed_tab3", lexer.FeedChar('T') == lexer.res_continue);
            TESTB("feed_tab4", lexer.FeedChar('a') == lexer.res_continue);
            TESTB("feed_tab5", lexer.FeedChar('b') == lexer.res_continue);
            TESTB("feed_tab_spc", lexer.FeedChar(' ') == lexer.res_ready);
            TEST("get_tab", lexer.Get().Car().GetSingleChar(), '\t');

            // besides Tab, there are:
            //  Newline, Space, Backspace, Rubout, Linefeed, Page, Return
            // not tested here to save space   (TODO?)
        }
        TestSubsection("lisp lexic");
        {
            IntelibSLexAnalyser lexer;

            SLabel OpenPar("#(#");
            SLabel ClosePar("#)#");
            SLabel TheDot("#.#");
            SLabel TheFunesc("##'#");
            SLabel TheVecstart("##(#");
            SLabel TheVecstart2("#~(");
            SLabel TheHStart("##S(#");
            SLabel TheQuoter("\'");
            SLabel TheBackQuoter("`");
            SLabel TheComma(",");
            SLabel TheSegment(",@");
            SLabel TheTrue("#T");
            SLabel TheFalse("#F");

            TESTB("add_par",    lexer.AddDelimiter("(", OpenPar));
            TESTB("add_par2",   lexer.AddDelimiter(")", ClosePar));
            TESTB("add_dot",    lexer.AddNonDelimiter(".", TheDot));

            TESTB("add_string", lexer.AddStringStarter("\"", '\"'));
            TESTB("add_unrdbl", lexer.AddStringStarter("#<", '>',
                                                  unreadable_tran));

            TESTB("add_spec2",  lexer.AddDelimiter("#'", TheFunesc));

            TESTB("add_spec3",  lexer.AddDelimiter("#(", TheVecstart));
            TESTB("add_spec4",  lexer.AddDelimiter("#~(", TheVecstart2));

            TESTB("add_spec5",  lexer.AddDelimiter("#S(", TheHStart));
            TESTB("add_spec6",  lexer.AddDelimiter("#s(", TheHStart));

            TESTB("add_spec7",  lexer.AddTokenStarter("#&", extvar_tran));
            TESTB("add_spec8",  lexer.AddDelimiter("'", TheQuoter));
            TESTB("add_spec9",  lexer.AddDelimiter("`", TheBackQuoter));
            TESTB("add_spec10", lexer.AddDelimiter(",", TheComma));
            TESTB("add_spec11", lexer.AddDelimiter(",@", TheSegment));
            TESTB("add_spec12", lexer.AddNonDelimiter("#F", TheFalse));
            TESTB("add_spec13", lexer.AddNonDelimiter("#T", TheTrue));
            TESTB("add_spec14", lexer.AddNonDelimiter("#f", TheFalse));
            TESTB("add_spec15", lexer.AddNonDelimiter("#t", TheTrue));

            TESTB("quote_feed", lexer.FeedChar('\'') == lexer.res_ready);
            TESTTR("quote", lexer.Get(), "(' . 1)");
            TESTB("backquote_feed", lexer.FeedChar('`') == lexer.res_ready);
            TESTTR("backquote", lexer.Get(), "(` . 1)");

            TESTB("seq_feed1", lexer.FeedChar('#') == lexer.res_continue);
            TESTB("seq_feed2", lexer.FeedChar('~') == lexer.res_continue);
            TESTB("seq_feed3", lexer.FeedChar('(') == lexer.res_ready);
            TESTTR("seq_get", lexer.Get(), "(#~( . 1)");

            TESTB("spectok_feed1", lexer.FeedChar('#') == lexer.res_continue);
            TESTB("spectok_feed2", lexer.FeedChar('&') == lexer.res_continue);
            TESTB("spectok_feed3", lexer.FeedChar('a') == lexer.res_continue);
            TESTB("spectok_feed4", lexer.FeedChar('b') == lexer.res_continue);
            TESTB("spectok_feed_dlm", lexer.FeedChar('(') == lexer.res_ready);
            TESTTR("spectok_get", lexer.Get(), "(#&ab . 1)");
            TESTB("spectok_feed_dlm2", lexer.FeedChar(')') == lexer.res_ready);
            TESTTR("spectok_get", lexer.Get(), "(#(# . 1)");

            lexer.FeedChar(' ');

            TEST("feed_symbol1", lexer.FeedChar('a'), lexer.res_continue);
            TESTB("feed_symbol2", lexer.FeedChar('b') == lexer.res_continue);
            TESTB("feed_symbol3", lexer.FeedChar('c') == lexer.res_continue);
            TESTB("feed_symbol_spc", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_symbol_symbol", lexer.Get(), "(abc . 1)");

            lexer.FeedChar(' ');
            TESTB("feed_false1", lexer.FeedChar('#') == lexer.res_continue);
            TESTB("feed_false2", lexer.FeedChar('F') == lexer.res_continue);
            TESTB("feed_false3", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_false", lexer.Get(), "(#F . 1)");

            lexer.FeedChar(' ');
            TESTB("feed_true1", lexer.FeedChar('#') == lexer.res_continue);
            TESTB("feed_true2", lexer.FeedChar('T') == lexer.res_continue);
            TESTB("feed_true", lexer.FeedChar('(') == lexer.res_ready);
            TESTTR("get_true", lexer.Get(), "(#T . 1)");
            TESTB("feed_true4", lexer.FeedChar(' ') == lexer.res_ready);
            TESTTR("get_true", lexer.Get(), "(#(# . 1)");
        }
        TestScore();
    }
    catch(const IntelibX &ex) {
        printf("Caught IntelibX: %s\n%s\n",
            ex.Description(), ex.Parameter()->TextRepresentation().c_str());
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


