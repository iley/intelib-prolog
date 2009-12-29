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




#ifndef SENTRY_TESTS_HPP___
#define SENTRY_TESTS_HPP___


void TestSection(const char *name);

void TestSubsection(const char *name);

bool Testfl(const char *name, const char *file, int line,
            bool result);
bool Testfl(const char *name, const char *file, int line,
            const char *tst, const char *expected);
bool Testfl(const char *name, const char *file, int line,
            long tst, long expected);

#define TESTB(name, res) \
      Testfl(name, __FILE__, __LINE__, res)

#define TEST(name, res, pattern) \
      Testfl(name, __FILE__, __LINE__, res, pattern)

#define TESTTR(name, res, pattern) \
      Testfl(name, __FILE__, __LINE__, \
          (res)->TextRepresentation().c_str(), pattern)

#define TESTC(name, res, pattern) \
      Testfl(name, __FILE__, __LINE__, (res) == (pattern))


void TestScore();



#endif
