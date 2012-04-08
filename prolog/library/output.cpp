#include "../prolog.hpp"
#include "output.hpp"
namespace output {

  void Init() {
    using namespace PlgStdLib;
    static SReference &Nil = *PTheEmptyList;
    static PlgVariable _var_A("A");
    AssertWithoutExpansion((write(_var_A)<<print(_var_A)));
  }

}
