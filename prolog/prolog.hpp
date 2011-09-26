#ifndef INTELIB_PROLOG_HPP_SENTRY
#define INTELIB_PROLOG_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/custmem.hpp"

class IntelibX_not_a_prolog_object : public IntelibX
{
public:
    IntelibX_not_a_prolog_object(SReference a_param);
};

template <class T>
class GenericPlgReference : public GenericSReference<T, IntelibX_not_a_prolog_object> {
public:
    typedef GenericSReference<T, IntelibX_not_a_prolog_object> Super;

    GenericPlgReference(T* ptr) : Super(ptr) {}

    template <class Y>
    GenericPlgReference(const GenericPlgReference<Y> &ref) : Super(ref) {}
};

class PlgResultImpl;
class PlgResult : public GenericPlgReference<PlgResultImpl> {
public:
    void Next();
};

class PlgTermImpl;
class PlgTerm : public GenericPlgReference<PlgTermImpl> {
public:
    PlgResult Solve() const;
};

class PlgPredicateImpl;
class PlgPredicate : public GenericPlgReference<PlgPredicateImpl> {
public:
};

class PlgSymbolImpl;
class PlgSymbol : public GenericPlgReference<PlgSymbolImpl> {
public:
    PlgSymbol(const char *name);
    PlgPredicate GetPredicate(int arity);
};

class PlgVariableNameImpl;
class PlgVariableName : public GenericPlgReference<PlgVariableNameImpl> {
public:
    PlgVariableName(const char *name);
};

#endif //INTELIB_PROLOG_HPP_SENTRY
