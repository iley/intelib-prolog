#ifndef INTELIB_PLGREFTYPES_HPP_SENTRY
#define INTELIB_PLGREFTYPES_HPP_SENTRY

template <class T>
class GenericPlgReference : public GenericSReference<T, IntelibX_not_a_prolog_object>
{
public:
    typedef GenericSReference<T, IntelibX_not_a_prolog_object> Super;

    GenericPlgReference(T* ptr) : Super(ptr) {}

    template <class Y>
    GenericPlgReference(const GenericPlgReference<Y> &ref) : Super((const GenericPlgReference<T>&)ref) {}
    //TODO: check statically if Y is subtype of T
};

// Reference types

class PlgExpressionImpl;
class PlgExpression : public GenericPlgReference<PlgExpressionImpl> 
{
public:
    typedef GenericPlgReference<PlgExpressionImpl> Super;

    //PlgResult Solve() const;
};

class PlgDisjunctionImpl;
class PlgDisjunction : public GenericPlgReference<PlgDisjunctionImpl>
{
public:
    typedef GenericPlgReference<PlgDisjunctionImpl> Super;
};

class PlgConjunctionImpl;
class PlgConjunction : public GenericPlgReference<PlgConjunctionImpl>
{
public:
    typedef GenericPlgReference<PlgConjunctionImpl> Super;
};

class PlgTermImpl;
class PlgTerm : public GenericPlgReference<PlgTermImpl>
{
public:
    typedef GenericPlgReference<PlgTermImpl> Super;

    template <class Y>
    PlgTerm(const GenericPlgReference<Y> &ref) : Super(ref) {}
};

class PlgAtom;
class PlgCompoundTermImpl;
class PlgCompoundTerm : public GenericPlgReference<PlgCompoundTermImpl> 
{
public:
    typedef GenericPlgReference<PlgCompoundTermImpl> Super;

    PlgCompoundTerm(const PlgAtom &functor, const SReference &args);
    const PlgAtom &GetFunctor() const;
    const SReference &GetArguments() const;
};

class PlgAtomImpl;
class PlgAtom : public GenericPlgReference<PlgAtomImpl> 
{
public:
    typedef GenericPlgReference<PlgAtomImpl> Super;

    PlgAtom(const char *name);
    const char *GetName() const;

    PlgCompoundTerm operator() (const PlgTerm &term1);
    PlgCompoundTerm operator() (const PlgTerm &term1, const PlgTerm &term2);
    PlgCompoundTerm operator() (const PlgTerm &term1, const PlgTerm &term2, const PlgTerm &term3);
    //TODO: continue
};

class PlgVarImpl;
class PlgVar : public GenericPlgReference<PlgVarImpl> 
{
public:
    typedef GenericPlgReference<PlgVarImpl> Super;

    PlgVar(const char *name);
    const char *GetName() const;
};

class PlgClauseImpl;
class PlgClause : public GenericPlgReference<PlgClauseImpl> 
{
public:
    typedef GenericPlgReference<PlgClauseImpl> Super;

    PlgClause(const PlgCompoundTerm &head, const PlgDisjunction &body);

    const PlgCompoundTerm &GetHead() const;
    const PlgDisjunction &GetBody() const;
};

inline PlgClause operator <<= (const PlgCompoundTerm &head, const PlgDisjunction &body) {
    return PlgClause(head, body);
}

#endif
