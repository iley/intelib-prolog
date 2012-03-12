#include "prolog.hpp"
#include "output.hpp"
namespace output {

  void InitDatabase(PlgDatabase &db) {
    using namespace PlgStdLib;
    static SReference &Nil = *PTheEmptyList;
    static PlgVariable _var_A("A");
    db.AddWithoutExpansion((write(_var_A)<<=print(_var_A)));
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
