// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
// | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




#include <stdio.h>
#include "tests.hpp"

#include "sexpress/iexcept.hpp"
#include "sexpress/sstring.hpp"
#include "tools/sreader.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

static SListConstructor L;
static SReference proc_1(const SReference &r)
{
    return (L|1, r);
}
static SReference proc_2(const SReference &r)
{
    return (L|2, r);
}
static SReference proc_3(const SReference &r)
{
    return (L|3, r);
}

static SReference proc_4(const SReference &r)
{
    return (L|4, r);
}
static SReference proc_5(const SReference &r)
{
    return (L|5, r);
}
static SReference proc_6(const SReference &r)
{
    return (L|6, r);
}

static SReference proc_6ch(const char* str)
{
    return (L|6, str);
}

int main()
{
    poc();
    try {
        TestSection("Intelib Reader");
        TestSubsection("Reader");
        {
            IntelibReader reader;
            reader.FeedString("(A B)");
            reader.FeedEof();
            TESTB("read_list_ready", reader.IsReady());
            SReference res = reader.Get();
            TESTTR("read_list", res, "(A B)");
        }
        {
            IntelibReader reader;
            reader.FeedString("()");
            reader.FeedEof();
            TESTB("read_empty_list_ready", reader.IsReady());
            SReference res = reader.Get();
            TESTTR("read_empty_list", res, "()");
        }
        TestSubsection("AncientReader");
        {
            SStreamCharbuf str("25");
            IntelibReader reader;
            SReference res = reader.Read(str);
            TESTTR("read_number_result", res, "25");
            TESTB("read_number_type",res->TermType()==SExpressionInt::TypeId);
        }
        {
            SStreamCharbuf str("25 36");
            IntelibReader reader;
            reader.Read(str);
            SReference res = reader.Read(str);
            //TESTB("read_consequent_number_success", reader.Success());
            TESTTR("read_number_result", res, "36");
            TESTB("read_number_type",res->TermType()==SExpressionInt::TypeId);
        }
        {
            SStreamCharbuf str("(25)");
            IntelibReader reader;
            SReference res = reader.Read(str);
            //TESTB("read_single_item_list_success", reader.Success());
            TESTTR("read_single_item_list_result", res, "(25)");
            TESTB("read_single_item_list_type",
                 res->TermType() == SExpressionCons::TypeId);
            //printf("%s\n", reader.LastErrorDescription());
        }
        {
            SStreamCharbuf str("(25 36)");
            IntelibReader reader;
            SReference res = reader.Read(str);
            //TESTB("read_list_success", reader.Success());
            TESTTR("read_list_result", res, "(25 36)");
            TESTB("read_list_type", res->TermType() == SExpressionCons::TypeId);
            //printf("%s\n", reader.LastErrorDescription());
        }
        {
            SStreamCharbuf str("(25 36 49)");
            IntelibReader reader;
            SReference res = reader.Read(str);
            //TESTB("read_list3_success", reader.Success());
            TESTTR("read_list3_result", res, "(25 36 49)");
            TESTB("read_list3_type", res->TermType() == SExpressionCons::TypeId);
            //printf("%s\n", reader.LastErrorDescription());
            res = reader.Read(str);
            TESTTR("nothing_after_list", res, "#<END OF FILE>");
        }
        {
            SStreamCharbuf str("(25 (6 36) (7 49))");
            IntelibReader reader;
            SReference res = reader.Read(str);
            //TESTB("read_list3_2_success", reader.Success());
            TESTTR("read_list3_2_result", res, "(25 (6 36) (7 49))");
            TESTB("read_list3_2_type", res->TermType() == SExpressionCons::TypeId);
            //printf("%s\n", reader.LastErrorDescription());
            res = reader.Read(str);
            TESTTR("nothing_after_list", res, "#<END OF FILE>");
        }
        TestSubsection("PostponedBug");
        {
            SStreamCharbuf str("(25 (6 36)(77 49))(ABC)");
            IntelibReader reader;
            SReference res = reader.Read(str);
            TESTTR("read_postponed_bug", res, "(25 (6 36) (77 49))");
            res = reader.Read(str);
            TESTTR("after_bug", res, "(ABC)");
        }
        TestSubsection("CharMacroBug");
        {
            SStreamCharbuf str("(25 #\\( 36)");
            IntelibReader reader;
            SReference res = reader.Read(str);
            TESTTR("read_postponed_bug", res, "(25 #\\( 36)");
        }
        TestSubsection("Quoters");
        {
            IntelibReader reader;
            reader.AddQuoter("'", proc_1);
            reader.AddQuoter("#'", proc_2);
            reader.AddQuoter("#&", proc_3);
            reader.AddQuoter("`", proc_4);
            reader.AddQuoter(",", proc_5);
            reader.AddQuoter("@", proc_6);

            SStreamCharbuf str("('a #'b #&c `d ,e @f)");
            SReference res = reader.Read(str);
            TESTTR("quoters", res, "((1 A) (2 B) (3 C) (4 D) (5 E) (6 F))");

        }
        TestSubsection("Tokens");
        {
            IntelibReader reader;
            reader.AddTokenType("#&", proc_6ch);

            SStreamCharbuf str("(#&abc #&123 #&---)");
            SReference res = reader.Read(str);
            TESTTR("quoters", res, "((6 \"abc\") (6 \"123\") (6 \"---\"))");

        }
        TestSubsection("Delimiters");
        {
            IntelibReader reader;
            reader.AddDelimiter("#F", 777);
            reader.AddDelimiter("#T", 888);

            SStreamCharbuf str("(#T #F #T #F)");
            SReference res = reader.Read(str);
            TESTTR("truefalse", res, "(888 777 888 777)");
        }
        TestSubsection("Prefixes and suffixes");
        {
            IntelibReader reader;
            reader.AddQuoter(",@", proc_6);
            reader.AddQuoter(",", proc_5);
            SStreamCharbuf str("(,25 ,@36 ,(49) ,@(64))");
            SReference res = reader.Read(str);
            TESTTR("semiquotations", res, "((5 25) (6 36) (5 (49)) (6 (64)))");
        }
        TestScore();
    }
    catch(const IntelibX &ex) {
        printf("Caught IntelibX: %s\n%s\n", ex.Description(),
                ex.Parameter()->TextRepresentation().c_str());
    }
    catch(...) {
        printf("Something strange caught\n");
    }
    poc();
    return 0;
}


