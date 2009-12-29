// +-------------------------------------------------------------------------+
// |               I__n__t__e__L__i__b           0.6.21 development          |
// | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
// |                                                                         |
// | This is free software. The library part is available under              |
// |                               GNU LESSER GENERAL PUBLIC LICENSE v.2.1.  |
// | GNU LGPL v2.1 is found in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
// |     Please see also docs/readme.txt and visit http://www.intelib.org    |
// |                                                                         |
// | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
// +-------------------------------------------------------------------------+




/*! \file rvars.hpp
    \brief Refal variables abstraction representation
    
    This file invents several classes which server to represent 
    Refal variables and their behaviour (e.g., bindings). 
*/


#ifndef INTELIB_RVARS_HPP_SENTRY
#define INTELIB_RVARS_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "refal.hpp"


/*
   We have three derived from RfExpressionVariable types,
   but the only difference is a constructor and there are actually
   nothing virtual at all. So there is (as for now) no possibility
   to create non-standard types.
*/
class RfExpressionVariable : public SExpressionLabel
{
    public:

    static IntelibTypeId TypeId;
    enum RfVarType {var_E, var_S, var_T};
    RfVarType var_type;

    RfExpressionVariable(RfVarType avar_type, const char *aname = "")
        : SExpressionLabel(TypeId, aname),
          var_type(avar_type)
        {}
    ~RfExpressionVariable()
        {}
    inline bool IsMultiMatch() const 
        { return (var_type == var_E); }
    inline bool IsT() const 
        { return (var_type == var_T); }
};

//! Refal S-variable
/*! Refal variable which matches any single symbol */
class RfExpressionVariable_S : public RfExpressionVariable
{
    public:

    RfExpressionVariable_S(const char *name = 0)
       : RfExpressionVariable(RfExpressionVariable::var_S, name)
       {}
    ~RfExpressionVariable_S()
       {}
};

//! Refal T-variable
/*! Refal variable which matches any single term (symbol or a
    expression enclosed with parentheses
*/
class RfExpressionVariable_T : public RfExpressionVariable
{
    public:

    RfExpressionVariable_T(const char *name = 0)
        : RfExpressionVariable(RfExpressionVariable::var_T, name)
        {}
    ~RfExpressionVariable_T() {}
};

//! Refal E-variable
/*! Refal variable which matches any part (possibly empty) of any
    Refal expression. The only standard multimatch variable.
*/
class RfExpressionVariable_E : public RfExpressionVariable
{
    public:

    RfExpressionVariable_E(const char *name = 0)
        : RfExpressionVariable(RfExpressionVariable::var_E, name)
        {}
    ~RfExpressionVariable_E()
        {}
};

template <class T> // T must be RfExpressionVariable's subclass
class RfVariable : public SReference
{
    public:

    RfVariable(const char *name = 0) : SReference(new T(name))
        {}
    T* GetPtr() const
        { return static_cast<T*>(SReference::GetPtr()); }
    T* operator->()
        { return GetPtr(); }
};

typedef RfVariable<RfExpressionVariable_S> RfVariable_S;
typedef RfVariable<RfExpressionVariable_T> RfVariable_T;
typedef RfVariable<RfExpressionVariable_E> RfVariable_E;

#endif // sentry


