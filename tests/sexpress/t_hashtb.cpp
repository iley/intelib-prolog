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
#include <string.h>
#include "tests.hpp"

#include "sexpress/sexpress.hpp"
#include "sexpress/sstring.hpp"
#include "sexpress/iexcept.hpp"
#include "sexpress/shashtbl.hpp"


void poc()
{
#ifdef INTELIB_DEBUG_COUNTERS
    printf("       Object counter: %ld\n", SExpression::object_counter);
#endif
}

const char *itoa(int i)
{
    static char buf[20];
    snprintf(buf, sizeof(buf), "%d", i);
    return buf;
}


int main()
{
    poc();
    try {
        TestSection("SHashTableRef");
        TestSubsection("Creation");
        {
            SHashTable t;
            TEST("hash_table_0", t->Count(), 0);
            t->AddItem(0, itoa(0));
            TEST("hash_table_1", t->Count(), 1);
            for(int i=1; i<10; i++) {
                t->AddItem(i, itoa(i));
            }
            TEST("hash_table_10", t->Count(), 10);
            for(int i=10; i<10000; i++) {
                t->AddItem(i, itoa(i));
            }
            TEST("hash_table_10000", t->Count(), 10000);
        }
        TestSubsection("Eq");
        {
            SHashTable t(SExprsAreEq);
            SString key1("key");
            SString key2("key");

            t->AddItem(key1, "value");

            TESTTR("hash_eq_ok", t->FindItem(key1, "none"), "\"value\"");
            TESTTR("hash_eq_not_eql", t->FindItem(key2, "none"), "\"none\"");
        }
        TestSubsection("Eql");
        {
            SHashTable t(SExprsAreEql);
            SString key1("key");
            SString key2("key");

            t->AddItem(key1, "value");

            TESTTR("hash_eql_ok_for_eq", t->FindItem(key1, "none"),
                   "\"value\"");
            TESTTR("hash_eql_ok", t->FindItem(key2, "none"), "\"value\"");

            SListConstructor L;
            SReference key3((L|1,2));
            SReference key4((L|1,2));

            t->AddItem(key3, "value");
            TESTTR("hash_eql_not_equal", t->FindItem(key4, "none"),
                   "\"none\"");
        }
        TestSubsection("Equal");
        {
            SHashTable t(SExprsAreEqual);
            SString key1("key");
            SString key2("key");

            t->AddItem(key1, "value");

            TESTTR("hash_equal_ok_for_eq", t->FindItem(key1, "none"),
                   "\"value\"");
            TESTTR("hash_equal_ok_for_eql", t->FindItem(key2, "none"),
                   "\"value\"");

            SListConstructor L;
            SReference key3((L|1,2));
            SReference key4((L|1,2));

            t->AddItem(key3, "value");
            TESTTR("hash_equal_ok", t->FindItem(key4, "none"), "\"value\"");
        }
        TestSubsection("DefaultIsEqual");
        {
            SHashTable t;
            SString key1("key");
            SString key2("key");

            t->AddItem(key1, "value");

            TESTTR("hash_default_eq_ok", t->FindItem(key1, "none"),
                   "\"value\"");
            TESTTR("hash_default_eql_ok", t->FindItem(key2, "none"),
                   "\"value\"");

            SListConstructor L;
            SReference key3((L|1,2));
            SReference key4((L|1,2));

            t->AddItem(key3, "value");
            TESTTR("hash_default_equal_ok", t->FindItem(key4, "none"),
                   "\"value\"");
        }
        TestSubsection("Iteration");
        {
            SHashTable t;
            int i;
            for(i=0; i<10000; i++) {
                t->AddItem(i, itoa(i));
            }

            {
                SExpressionHashTable::Iterator it(*t.GetPtr());
                i=0;
                while(it.GetNext().GetPtr()) i++;
            }
            TEST("hash_table_iterator_10000", i, 10000);


            {
                SExpressionHashTable::Iterator it(*t.GetPtr());
                t->GetItemPosition(-5); // unexistent!
                // this makes us an entry in the table which
                // must not be counted because it doesn't have
                // the value
                i=0;
                while(it.GetNext().GetPtr()) i++;
            }
            TEST("hash_table_iterator_bug_fixed", i, 10000);
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


