//   InteLib                                    http://www.intelib.org
//   The file tests/tests.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
