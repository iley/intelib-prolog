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




#ifndef PERM_HPP_SENTRY
#define PERM_HPP_SENTRY


#include <intelib/sexpress/sexpress.hpp>
#include <intelib/genlisp/lispform.hpp>
#include <intelib/scheme/scheme.hpp>
#include <intelib/scheme/schsymb.hpp>

class SExpressionPermGen : public SExpression {
    class PermutationGenerator *pg;
public:
    static IntelibTypeId TypeId;

    SExpressionPermGen(int n);
    ~SExpressionPermGen();

    virtual SString TextRepresentation() const;

    class PermutationGenerator *Get() const;
};

SReference create_permutation_generator(int, const SReference *);
SReference next_permutation(int, const SReference *);

#endif
