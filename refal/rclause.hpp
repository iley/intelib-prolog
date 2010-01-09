//   InteLib                                    http://www.intelib.org
//   The file refal/rclause.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




// Modified by Igor Bronstein (<igor.bronstein_AT_gmail.com>)

#ifndef INTELIB_RCLAUSE_HPP_SENTRY
#define INTELIB_RCLAUSE_HPP_SENTRY

#include <stdio.h>

#include "refal.hpp"
#include "rfmatch.hpp"


/*
    Structure representing each Refal function is as follows:

    1) Each clause

    pat[0],
    [expr[0] : pat[1], ... , expr[n-1] : pat[n]] =
    expr[n]

    where n is 0 (usual clause) or natural (where-clause)
    
    is stored as a TWO-way list of RfClause's:

    pattern = pat[i],
    expression = expr[i+1],
    left = to previous pair (pattern, expression)
    additional = to next pair (pattern, expression)
    where = false

    2) If there is a with-clause, i.e.

    pat[0],
    expr[0] : pat[1], ... , expr[n-1] : { ... }

    everything is the same but there is the last RfClause where

    pattern = points to an anonymous function representing {...}
    expression = NULL
    where = true

    (*)

    3) First RfClause's in these two way lists are connected into
    a ONE-way list representing the whole Refal function
*/
class RfClause
{
    RfReference pattern;
    RfReference result;
    RfClause *additional;
    RfClause *left;
    RfClause *next;
    bool where;

    public:

    RfClause()
        : pattern(), result(), additional(0), 
          left(0), next(0), where(false)
        {}
    RfClause(const RfReference &a_pattern, const RfReference &a_result)
        : pattern(a_pattern), result(a_result), 
          additional(0), left(0), next(0), where(false)
        {}
    RfClause(const RfReference &a_pattern,
             const RfReference &a_result, bool w)
        : pattern(a_pattern), result(a_result), 
          additional(0), left(0), next(0), where(w)
        {}
    RfClause(const RfClause& a)
        : pattern(a.pattern), result(a.result), 
          additional(a.additional?new RfClause(this, *a.additional) : 0),
	  left(a.left?new RfClause(*a.left, this) : 0),
          next(a.next?new RfClause(*a.next) : 0),
	  where(a.where)
        {}
    RfClause(RfClause *l, const RfClause& a)
        : pattern(a.pattern), result(a.result), 
          additional(a.additional?new RfClause(this, *a.additional) : 0),
	  left(l),
          next(a.next?new RfClause(*a.next) : 0),
	  where(a.where)
        {}
    RfClause(const RfClause& a, RfClause *r)
        : pattern(a.pattern), result(a.result), 
          additional(r),
	  left(a.left ? new RfClause(*a.left, this) : 0),
          next(a.next ? new RfClause(*a.next) : 0),
	  where(a.where)
        {}
    ~RfClause()
    {
        if (next) delete next;
        if (additional) delete additional;
    }
    RfClause& operator=(const RfClause &other)
    {
        pattern = other.pattern;
        result = other.result;
        if (additional) delete additional;
        additional =
            other.additional ? new RfClause(this, *other.additional) :0;
        if (next) delete next;
        next = other.next ? new RfClause(*other.next) : 0;
        if (left) delete left;
        left = other.left ? new RfClause(*other.left, this) : 0;
	where = other.where;
        return *this;
    }

    //! Add additional pattern/result pair
    RfClause& operator|(const RfClause& a);

    //! Add next clause to the block
    RfClause& operator||(const RfClause& a);    

    //! Evaluate
    void Apply(RfReference &arg, RfContext &varlist) const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    SString TextRepresentation() const;
#endif

    private:

    bool EvaluateClause(RfReference &arg, 
                        RfReference &res, RfContext &varlist) const;
};


inline RfClause operator&(const RfReference &pattern,
                          const RfReference &result)
{ return RfClause(pattern, result); }

inline RfClause operator^(const RfReference &pattern,
                          const RfReference &result)
{ return operator&(pattern, result); }



class RfExpressionSymbol : public SExpressionLabel
{
    RfClause associated_function;

    public:

    static IntelibTypeId TypeId;

    RfExpressionSymbol(const char *a_name) 
        : SExpressionLabel(TypeId, a_name), 
          associated_function()
        {}

    ~RfExpressionSymbol() {}

    //! Get the associated functional object
    const RfClause& GetFunction() const
        { return associated_function; } 

    RfClause& SetFunction(const RfClause &c)
    { 
        associated_function = c;
        return associated_function; 
    } 

    private:

    RfExpressionSymbol(const RfExpressionSymbol &) 
         : SExpressionLabel(TypeId, 0)
         {} // ensure no copying

    void operator=(const RfExpressionSymbol &)
        {}
};


//! Smart pointer to RfExpressionSymbol
/*! This class derived from RfReference is dedicated to handle 
    RfExpressionSymbols and has the appropriate functionality
    \warning It is assumed that RfSymbol persists all the period
    the corresponding symbol is in use. In order to prevent cyclic
    dynamic structures (like F { ... <F ..> ...}) from surviving 
    in the garbage, this object removes the associated function 
    from it's referenced object (RfExpressionSymbol) before passing
    away -- that is, in the destructor.
 */
class RfSymbol : public SReference
{
    public:

    RfSymbol(const char *s)
       : SReference(new RfExpressionSymbol(s))
       {}
    ~RfSymbol()
        { (*this)->SetFunction(RfClause()); }
    RfExpressionSymbol* GetPtr() const
        { return (RfExpressionSymbol*)(SReference::GetPtr()); }
    RfExpressionSymbol& operator*() const
        { return *GetPtr(); }
    RfExpressionSymbol* operator->() const
        { return GetPtr(); }

    RfClause& operator||(const RfClause &cla)
        { return GetPtr()->SetFunction(cla); }

    private:

    // made private to prevent the user from mistakes and
    //   to make sure this always points to RfExpressionSymbol object.
    void operator =(const RfReference& ref)
        {}
    RfSymbol(const RfSymbol&)
        {}
};

class RfExpressionLibSymbol : public SExpressionLabel
{
    void (*fun)(RfReference&);

    public:

    static IntelibTypeId TypeId;

    RfExpressionLibSymbol(const char *a_name,
                          void (*a_fun)(RfReference&)) 
        : SExpressionLabel(TypeId, a_name), fun(a_fun)
        {}
    ~RfExpressionLibSymbol()
        {}

    //! Evaluate
    void Apply(RfReference &arg) const
        { fun(arg); }

    private:

    RfExpressionLibSymbol(const RfExpressionLibSymbol &) 
         : SExpressionLabel(TypeId, 0)
         {} // ensure no copying
    void operator=(const RfExpressionSymbol &) {}
};

class RfLibSymbol : public SReference {
    public:

    RfLibSymbol(const char *s, void (*fun)(RfReference&))
       : SReference(new RfExpressionLibSymbol(s, fun))
       {}
    ~RfLibSymbol()
       {}
    RfExpressionLibSymbol* GetPtr() const
      { return (RfExpressionLibSymbol*)(SReference::GetPtr()); }
    RfExpressionLibSymbol& operator*() const
      { return *GetPtr(); }
    RfExpressionLibSymbol* operator->() const
      { return GetPtr(); }

    private:

    // made private to prevent the user from mistakes and
    //   to make sure this always points to RfExpressionSymbol object.
    void operator =(const RfReference& ref)
        {}
    RfLibSymbol(const RfLibSymbol&)
        {}
};

/*
    Element of the list with anonymous symbols assosiated with
    anonymous functions. These will be needed to implement
    with-clauses.
*/
struct RfWithItem
{
    RfSymbol symb;
    RfWithItem *next;

    RfWithItem()
        : symb(""), next(0)
        {}

    ~RfWithItem()
    {
        if (next)
        {
            delete next;
            next = 0;
        }
    }
};

/*
    Class which implements with-clauses.
    To do this we need two lists of anonymous symbols.
    InteLib syntax of with-clauses is as follows:

    Go {
        s.1 s.2, <f s.1> : { True = s.1; False = s.2 }
    }

    is represented as

    Go
    ||
    (R|s_1, s_2) & (R|R<file1_f, s_1>R) |  RWITH(
    RWITH
    ||
    (R|True)        ^ (R|s_1)
    ||
    (R|False)       ^ (R|s_2)
    );

    As you see, RfWith has two operations:

    1) ||:
    
        creating new item in the head of the first list and, as usual,
        assosiating it with the following clauses

    2) overloaded ():
        
        moving head of the first list to the second list and
        creating the last clause (see (*))

    We need two lists to implement nested with-clauses.
    
*/
class RfWith 
{
    RfWithItem *first;
    RfWithItem *second;

    public:

    RfWith()
        : first(0), second(0)
        {}

    ~RfWith()
    {
        if (first)
        {
            delete first;
            first = 0;
        }
        if (second)
        {
            delete second;
            second = 0;
        }
    }
    void AddWithItem();
    void MoveToSecond();
    RfClause& operator||(const RfClause &cla)
    {
        AddWithItem();
        return first->symb.GetPtr()->SetFunction(cla);
    }
    RfClause operator()(const RfClause &cla)
    {
        MoveToSecond();
        RfReference ref1 = RfReference(new RfExpression(second->symb));
        ref1->SetEnclosed();
        RfReference ref2 = RfReference(new RfExpression);
        ref2->SetEnclosed();
        return RfClause(ref1, ref2, true);
    }
};

class IntelibX_refal_failure : public IntelibX {
public:
    IntelibX_refal_failure(SReference a_param);
};

#endif


