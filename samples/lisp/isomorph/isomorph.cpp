//   InteLib                                    http://www.intelib.org
//   The file samples/lisp/isomorph/isomorph.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU GPL v.2
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




//       File isomorph.cpp
#include <intelib/lisp/lisp.hpp>
#include <intelib/lisp/lsymbol.hpp>
#include <intelib/lfun_std.hpp>
#include <intelib/lfun_sel.hpp>

LSymbol ISOMORPHIC("ISOMORPHIC");

static LFunctionalSymbol<LFunctionDefun> DEFUN("DEFUN");
static LFunctionalSymbol<LFunctionCond> COND("COND");
static LFunctionalSymbol<LFunctionAtom> ATOM("ATOM");
static LFunctionalSymbol<LFunctionAnd> AND("AND");
static LFunctionalSymbol<LFunctionCar> CAR("CAR");
static LFunctionalSymbol<LFunctionCdr> CDR("CDR");

LListConstructor L;

void LispInit_isomorphic() {
  static LSymbol TREE1("TREE1");
  static LSymbol TREE2("TREE2");
  ////////////////////////////////////////////////
  //
  (L|DEFUN, ISOMORPHIC, (L|TREE1, TREE2),
    (L|COND, 
      (L|(L|ATOM, TREE1), (L|ATOM, TREE2)),
      (L|(L|ATOM, TREE2), NIL),
      (L|T, (L|AND,
        (L|ISOMORPHIC, (L|CAR, TREE1), 
                       (L|CAR, TREE2)),
        (L|ISOMORPHIC, (L|CDR, TREE1), 
                       (L|CDR, TREE2))
  )))).Evaluate();
  //
  ////////////////////////////////////////////////
} 
//      end of file
