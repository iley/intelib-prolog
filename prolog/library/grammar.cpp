#include "../prolog.hpp"
#include "grammar.hpp"
namespace grammar {
  PlgAtom C36t_head("$t_head", PlgLibraryPredicate, false);
  PlgAtom C36t_body("$t_body", PlgLibraryPredicate, false);
  PlgAtom last("last", PlgLibraryPredicate, false);
  PlgAtom C36t_tidy("$t_tidy", PlgLibraryPredicate, false);
  PlgAtom instantiation_error("instantiation_error", PlgLibraryPredicate, false);
  PlgAtom C36t_hgoal("$t_hgoal", PlgLibraryPredicate, false);
  PlgAtom C36t_hlist("$t_hlist", PlgLibraryPredicate, false);
  PlgAtom C58(":", PlgLibraryPredicate, false);
  PlgAtom C36extend("$extend", PlgLibraryPredicate, false);
  PlgAtom C("C", PlgLibraryPredicate, false);
  PlgAtom type_error("type_error", PlgLibraryPredicate, false);
  PlgAtom list("list", PlgLibraryPredicate, false);
  PlgAtom filled_in("filled_in", PlgLibraryPredicate, false);
  PlgAtom to_fill("to_fill", PlgLibraryPredicate, false);
  PlgAtom not_last("not_last", PlgLibraryPredicate, false);
  PlgAtom C45C62("->", PlgLibraryPredicate, false);
  PlgAtom callable("callable", PlgLibraryPredicate, false);

  void InitDatabase(PlgDatabase &db) {
    using namespace PlgStdLib;
    static SReference &Nil = *PTheEmptyList;
    static PlgVariable _var_A("A");
    static PlgVariable _var_B("B");
    static PlgVariable _var_C("C");
    static PlgVariable _var_D("D");
    static PlgVariable _var_E("E");
    static PlgVariable _var_F("F");
    static PlgVariable _var_G("G");
    static PlgVariable _var_H("H");
    static PlgVariable _var_I("I");
    static PlgVariable _var_J("J");
    db.AddWithoutExpansion((dcg_translate_rule((_var_A>>=_var_B),(_var_C<<=_var_D))<<=(C36t_head(_var_A,_var_C,_var_E,_var_F,_var_G,(_var_A>>=_var_B))&(((var(_var_E)&(cut&C36t_body(_var_B,_var_H,last,_var_F,_var_G,_var_I)))|C36t_body((_var_B&action(_var_E)),_var_J,last,_var_F,_var_G,_var_I))&C36t_tidy(_var_I,_var_D)))));
    db.AddWithoutExpansion((C36t_head(_var_A,_var_B,_var_C,_var_D,_var_E,_var_F)<<=(var(_var_A)&(cut&error(instantiation_error,_var_F)))));
    db.AddWithoutExpansion((C36t_head((_var_A&_var_B),_var_C,_var_D,_var_E,_var_F,_var_G)<<=(cut&(C36t_hgoal(_var_A,_var_C,_var_E,_var_H,_var_G)&C36t_hlist(_var_B,_var_F,_var_H,_var_D,_var_G)))));
    db.AddWithoutExpansion((C36t_head(_var_A,_var_B,_var_C,_var_D,_var_E,_var_F)<<=C36t_hgoal(_var_A,_var_B,_var_D,_var_E,_var_F)));
    db.AddWithoutExpansion((C36t_hgoal(_var_A,_var_B,_var_C,_var_D,_var_E)<<=(var(_var_A)&(cut&error(instantiation_error,_var_E)))));
    db.AddWithoutExpansion((C36t_hgoal(C58(_var_A,_var_B),C58(_var_A,_var_C),_var_D,_var_E,_var_F)<<=(cut&C36t_hgoal(_var_B,_var_C,_var_D,_var_E,_var_F))));
    db.AddWithoutExpansion((C36t_hgoal(_var_A,_var_B,_var_C,_var_D,_var_E)<<=C36extend((_var_C)^((_var_D)^(Nil)),_var_A,_var_B)));
    db.AddWithoutExpansion((C36t_hlist(_var_A,_var_B,_var_C,_var_D,_var_E)<<=(var(_var_A)&(cut&error(instantiation_error,_var_E)))));
    db.AddWithoutExpansion(C36t_hlist(Nil,_var_A,_var_B,truth,_var_C));
    db.AddWithoutExpansion((C36t_hlist((_var_A)^(Nil),_var_B,_var_C,C(_var_C,_var_A,_var_B),_var_D)<<=cut));
    db.AddWithoutExpansion((C36t_hlist((_var_A)^(_var_B),_var_C,_var_D,(C(_var_D,_var_A,_var_E)&_var_F),_var_G)<<=(cut&C36t_hlist(_var_B,_var_C,_var_E,_var_F,_var_G))));
    db.AddWithoutExpansion((C36t_hlist(_var_A,_var_B,_var_C,_var_D,_var_E)<<=error(type_error(list,_var_A),_var_E)));
    db.AddWithoutExpansion((C36t_body(_var_A,filled_in,_var_B,_var_C,_var_D,phrase(_var_A,_var_C,_var_D))<<=(var(_var_A)&cut)));
    db.AddWithoutExpansion((C36t_body(cut,to_fill,last,_var_A,_var_B,(cut&(_var_B^=_var_A)))<<=cut));
    db.AddWithoutExpansion((C36t_body(cut,_var_A,_var_B,_var_C,_var_C,cut)<<=cut));
    db.AddWithoutExpansion((C36t_body(Nil,to_fill,last,_var_A,_var_B,(_var_B^=_var_A))<<=cut));
    db.AddWithoutExpansion((C36t_body(Nil,_var_A,_var_B,_var_C,_var_C,truth)<<=cut));
    db.AddWithoutExpansion((C36t_body((_var_A)^(Nil),filled_in,_var_B,_var_C,_var_D,C(_var_C,_var_A,_var_D))<<=cut));
    db.AddWithoutExpansion((C36t_body((_var_A)^(_var_B),filled_in,_var_C,_var_D,_var_E,(C(_var_D,_var_A,_var_F)&_var_G))<<=(cut&C36t_body(_var_B,filled_in,_var_C,_var_F,_var_E,_var_G))));
    db.AddWithoutExpansion((C36t_body(action(_var_A),to_fill,last,_var_B,_var_C,(_var_A&(_var_C^=_var_B)))<<=cut));
    db.AddWithoutExpansion((C36t_body(action(_var_A),_var_B,_var_C,_var_D,_var_D,_var_A)<<=cut));
    db.AddWithoutExpansion((C36t_body((_var_A&_var_B),_var_C,_var_D,_var_E,_var_F,(_var_G&_var_H))<<=(cut&(C36t_body(_var_A,_var_C,not_last,_var_E,_var_I,_var_G)&C36t_body(_var_B,_var_C,_var_D,_var_I,_var_F,_var_H)))));
    db.AddWithoutExpansion((C36t_body(C45C62(_var_A,_var_B),_var_C,_var_D,_var_E,_var_F,C45C62(_var_G,_var_H))<<=(cut&(C36t_body(_var_A,_var_C,not_last,_var_E,_var_I,_var_G)&C36t_body(_var_B,_var_C,_var_D,_var_I,_var_F,_var_H)))));
    db.AddWithoutExpansion((C36t_body(nope(_var_A),_var_B,_var_C,_var_D,_var_E,(C45C62(_var_F,fail)|(_var_D^=_var_E)))<<=(cut&C36t_body(_var_A,_var_B,not_last,_var_D,_var_G,_var_F))));
    db.AddWithoutExpansion((C36t_body((_var_A|_var_B),_var_C,_var_D,_var_E,_var_F,(_var_G|_var_H))<<=(cut&(C36t_body(_var_A,_var_I,last,_var_E,_var_F,_var_G)&C36t_body(_var_B,_var_J,last,_var_E,_var_F,_var_H)))));
    db.AddWithoutExpansion((C36t_body((_var_A^_var_B),_var_C,_var_D,_var_E,_var_F,(_var_G|_var_H))<<=(cut&(C36t_body(_var_A,_var_I,last,_var_E,_var_F,_var_G)&C36t_body(_var_B,_var_J,last,_var_E,_var_F,_var_H)))));
    db.AddWithoutExpansion((C36t_body(C58(_var_A,_var_B),_var_C,_var_D,_var_E,_var_F,C58(_var_A,_var_G))<<=(cut&C36t_body(_var_B,_var_C,_var_D,_var_E,_var_F,_var_G))));
    db.AddWithoutExpansion((C36t_body(_var_A,filled_in,_var_B,_var_C,_var_D,_var_E)<<=C36extend((_var_C)^((_var_D)^(Nil)),_var_A,_var_E)));
    db.AddWithoutExpansion((C36extend(_var_A,_var_B,_var_C)<<=(univ(_var_B,_var_D)&(append(_var_D,_var_A,_var_E)&univ(_var_C,_var_E)))));
    db.AddWithoutExpansion((C36t_tidy(_var_A,_var_A)<<=(var(_var_A)&cut)));
    db.AddWithoutExpansion((C36t_tidy((_var_A|_var_B),(_var_C|_var_D))<<=(cut&(C36t_tidy(_var_A,_var_C)&C36t_tidy(_var_B,_var_D)))));
    db.AddWithoutExpansion((C36t_tidy(C45C62(_var_A,_var_B),C45C62(_var_C,_var_D))<<=(cut&(C36t_tidy(_var_A,_var_C)&C36t_tidy(_var_B,_var_D)))));
    db.AddWithoutExpansion((C36t_tidy(((_var_A&_var_B)&_var_C),_var_D)<<=C36t_tidy((_var_A&(_var_B&_var_C)),_var_D)));
    db.AddWithoutExpansion((C36t_tidy((truth&_var_A),_var_B)<<=(cut&C36t_tidy(_var_A,_var_B))));
    db.AddWithoutExpansion((C36t_tidy((_var_A&truth),_var_B)<<=(cut&C36t_tidy(_var_A,_var_B))));
    db.AddWithoutExpansion((C36t_tidy((_var_A&_var_B),(_var_C&_var_D))<<=(cut&(C36t_tidy(_var_A,_var_C)&C36t_tidy(_var_B,_var_D)))));
    db.AddWithoutExpansion(C36t_tidy(_var_A,_var_A));
    db.AddWithoutExpansion(C((_var_A)^(_var_B),_var_A,_var_B));
    db.AddWithoutExpansion((phrase(_var_A,_var_B)<<=phrase(_var_A,_var_B,Nil)));
    db.AddWithoutExpansion((phrase(_var_A,_var_B,_var_C)<<=(var(_var_A)&(cut&error(instantiation_error,phrase(_var_A,_var_B,_var_C))))));
    db.AddWithoutExpansion((phrase(_var_A,_var_B,_var_C)<<=((atomic(_var_A)&nope(atom(_var_A)))&(cut&error(type_error(callable,_var_A),phrase(_var_A,_var_B,_var_C))))));
    db.AddWithoutExpansion((phrase(Nil,_var_A,_var_B)<<=(cut&(_var_A^=_var_B))));
    db.AddWithoutExpansion((phrase((_var_A)^(_var_B),_var_C,_var_D)<<=(cut&append((_var_A)^(_var_B),_var_D,_var_C))));
    db.AddWithoutExpansion((phrase(_var_A,_var_B,_var_C)<<=(C36t_body(_var_A,_var_D,last,_var_B,_var_C,_var_E)&(cut&_var_E))));
  }

  PlgDatabase &Database() {
    static PlgDatabase db;
    static bool needsInit = true;
    if (needsInit) {
      InitDatabase(db);
      needsInit = false;
    }
    return db;
  }
}
