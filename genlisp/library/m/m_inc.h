#ifdef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

#include <math.h>
#include "../../../sexpress/sstring.hpp"
#include "../../../genlisp/lispform.hpp"
#include "../../../genlisp/conteval.hpp"

#endif



#ifndef INTELIB_GENLISP_LIBRARY_IMPLEMENTATION

  #define DECLARE_MATHFUNCTION(classname, textrep, fun)       \
       DECLARE_CFUNCTION(classname, 1, 1, textrep)

#else

  #define DECLARE_MATHFUNCTION(classname, textrep, fun)       \
       DECLARE_CFUNCTION(classname, 1, 1, textrep)            \
       void classname::                                                 \
       DoApply(int paramsc, const SReference paramsv[],                 \
               IntelibContinuation& lf) const                           \
       {                                                                \
            lf.RegularReturn(SReference(fun(paramsv[0].GetFloat())));   \
       }                         

#endif
