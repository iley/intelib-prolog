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




#ifndef INTELIB_REFAL_HPP_SENTRY
#define INTELIB_REFAL_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/gensref.hpp"
#include "../sexpress/custmem.hpp"



struct RfListItem
{
    /*
        1) Markup:
                     symb_val.GetPtr() == {left,right}_{par,call}.GetPtr()
                     charcode is undefined, pair points to the pair
        2) Char:
                     symb_val.GetPtr() == 0
                     charcode is the code, pair is undefined
        3) Non-char token:
                     symb_val == the token
                     charcode and pair are both undefined
    */

    SReference symb_val;
    RfListItem *prev;
    RfListItem *next;

    union
    {
        int charcode;
        RfListItem *pair;
    };

    // Special labels representing markup elements
    static SLabel left_par;
    static SLabel right_par;
    static SLabel left_call;
    static SLabel right_call;

    // Constructing non-char
    RfListItem(int i, const SReference &av) // i must be 0
        : symb_val(av), prev(0), next(0)
        {}
    // Constructing char
    explicit RfListItem(int a_charcode)
        : symb_val(), prev(0), next(0), charcode(a_charcode)
        {}
    // Copying RfListItem (depeding on whether it is non-char or char)
    RfListItem(const RfListItem &a)
        : symb_val(), prev(0), next(0), pair(0)
    {
        if (a.symb_val.GetPtr())
            symb_val = a.symb_val;
        else
            charcode = a.charcode;
    }

    ~RfListItem()
        {}

    bool IsLeftMarkup() const
    {
        return symb_val == left_par || 
               symb_val == left_call;
    }
    bool IsRightMarkup() const
    {
        return symb_val == right_par || 
               symb_val == right_call;
    }
    bool IsMarkup() const
        { return IsLeftMarkup() || IsRightMarkup(); }
    bool IsChar() const
        { return symb_val.GetPtr() == 0; }

    void Pair(RfListItem *another);

    /*
        Are two RfListItem's equal,
        depending on where they are both chars or non-chars.
    */
    bool IsEql(const RfListItem &other) const
    {
        if (IsChar())
        {
           return (other.IsChar() && charcode == other.charcode);
        }
        return (!other.IsChar() && symb_val.IsEql(other.symb_val));
    }

    RfListItem *PrevTerm() const;
    RfListItem *NextTerm() const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    SString TextRepresentation() const;
    SString TextRepresentationThis() const;
#endif

    INTELIB_DECLARE_CUSTOM_MEMORY_MANAGEMENT(RfListItem)
};

class RfExpression : public SExpression {
    bool enclosed; // parentheses enforced unless it is the toplevel
    RfListItem *first, *last;
    RfListItem *last_unpaired;
public:
    static IntelibTypeId TypeId;

    RfExpression() 
        : SExpression(TypeId), enclosed(false),
          first(0), last(0), last_unpaired(0) {}
    RfExpression(const SReference &a_first) 
        : SExpression(TypeId), enclosed(false),
          first(0), last(0), last_unpaired(0) 
        { Add(a_first); }
    RfExpression(RfListItem *a_beg, RfListItem *a_end) 
        : SExpression(TypeId), enclosed(false), first(a_beg),
          last(a_end), last_unpaired(0)
        {}
    ~RfExpression();

    RfListItem* GetFirst() const { return first; }
    RfListItem* GetLast() const { return last; }

    void Exclude(RfListItem *from, RfListItem *thru);
    void InsertListAfter(RfListItem* from, 
                         RfListItem* left, 
                         RfListItem* right);

    void Add(const SReference &elem); 
    void BeginSubcall(const SReference &elem);
    void BeginSubcall();
    void EndSubcall(const SReference &elem);
    void EndSubcall();

    void QuickAdd(RfListItem *elem); 

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

    void SetEnclosed() { enclosed = true; }
    bool ShallBeEnclosed() const { return enclosed; }

    SReference ConvertToLisp() const;

    void AddToEndSmart(RfListItem *elem1, RfListItem *elem2);

private:
    RfListItem* AddToEnd(const SReference &elem);
    void AddToEnd(RfListItem *elem);
}; 

class RfReference;

class RfFormConstructor
{
    public:

    RfFormConstructor()
        {}
    ~RfFormConstructor()
        {}

    class CallEnd : public SReference
    {
        int i;

        public:

        CallEnd(const SReference &r) : SReference(r)
            { i = 0; }
        CallEnd& operator>(RfFormConstructor)
            { i++; return *this; }
        int ExtraCloses() const
            { return i; }
    };

    class CallBegin : public SReference
    {
        public:

        CallBegin(const SReference &r) : SReference(r)
            {}

        RfReference operator>(RfFormConstructor);
        RfReference operator,(const SReference& ref);
        RfReference operator,(const RfFormConstructor::CallBegin& ref);
        RfReference operator,(const RfFormConstructor::CallEnd& ref);
    };

    CallBegin operator<(const SReference &r) const
        { return CallBegin(r); }
    CallBegin operator<(const CallBegin &r) const; 

    RfReference operator|(const SReference &r) const;
    RfReference operator|(const CallBegin &r) const;

    RfReference operator~() const; 
};

class RfMultyCallBegin;

template <int n> 
class RfMultyCallConstructor
{
    public:

    RfMultyCallConstructor()
        {}

    RfMultyCallConstructor<n+1> operator<(RfFormConstructor) 
        { return RfMultyCallConstructor<n+1>(); }

    RfMultyCallBegin operator<(const SReference &r);
};

class IntelibX_not_a_refal_expression : public IntelibX
{
    public:

    IntelibX_not_a_refal_expression(SReference a_param);
};

typedef GenericSReference<RfExpression, IntelibX_not_a_refal_expression>
RfRef;

class RfReference : public RfRef
{
    public:

    RfReference()
        {}
    RfReference(RfExpression *x) : RfRef(x)
        {}
    RfReference(const SReference &a) : RfRef(a)
        {}
    ~RfReference()
        {}

    void Ensure()
        { if (!GetPtr()) RfRef::operator=(new RfExpression); }

    void Evaluate(); 
    bool EvaluationStep(RfListItem **from);

    RfReference& operator,(const SReference& ref)
        { GetPtr()->Add(ref); return *this; }
    RfReference& operator,(const RfFormConstructor::CallBegin& ref)
        { GetPtr()->BeginSubcall(ref); return *this; }
    RfReference& operator,(const RfFormConstructor::CallEnd& ref)
    { 
        GetPtr()->EndSubcall(ref); 
        for (int i=0; i<ref.ExtraCloses(); i++) 
            GetPtr()->EndSubcall();
        return *this; 
    }

    private:

    bool SearchForCall(RfListItem **from,
                       RfListItem **left,
                       RfListItem **right);
};

class RfMultyCallBegin : public RfReference
{
    public:

    RfMultyCallBegin(const SReference &val, int begins=1) 
        : RfReference(new RfExpression())
    { 
        for (int i=0; i<=begins; i++) /* begins+1 iterations */
             (*this)->BeginSubcall();
        (*this)->Add(val); 
    }

    RfMultyCallBegin& operator>(RfFormConstructor)
    { 
        (*this)->EndSubcall(); 
        return *this; 
    }
};

inline RfReference 
RfFormConstructor::operator|(const SReference &r) const
{
    RfReference res(new RfExpression());
    res->SetEnclosed();
    res->Add(r);
    return res;
}

inline RfReference 
RfFormConstructor::operator|(const RfFormConstructor::CallBegin &r) const
{
    RfReference res(new RfExpression);
    res->SetEnclosed();
    res->BeginSubcall(r);
    return res;
}

inline RfFormConstructor::CallBegin
RfFormConstructor::operator<(const RfFormConstructor::CallBegin &r) const
{
    RfReference res(new RfExpression(true));
    res->BeginSubcall(r);
    return res;
}

inline RfReference RfFormConstructor::operator~()  const
{
    RfReference res(new RfExpression);
    res->SetEnclosed();
    return res;
}

inline RfReference RfFormConstructor::CallBegin::
operator,(const SReference& ref)
{   // R<1, 2     without outer structure
    RfReference res(new RfExpression);
    res->BeginSubcall(*this);
    res->Add(ref);
    return res;
}

inline RfReference RfFormConstructor::CallBegin::
operator,(const RfFormConstructor::CallBegin& ref)
{   // R<1, R<2     without outer structure
    RfReference res = new RfExpression();
    res->BeginSubcall(*this);
    res->BeginSubcall(ref);
    return res;
}

inline RfReference RfFormConstructor::CallBegin::
operator,(const RfFormConstructor::CallEnd& ref)
{   // R<1, 2>R     without outer structure
    // assert(ref.ExtraCloses() == 0) // because no outer structure
    RfReference res(new RfExpression);
    res->BeginSubcall(*this);
    res->Add(ref);
    res->EndSubcall();
    return res;
}

inline RfReference RfFormConstructor::CallBegin::
operator>(RfFormConstructor)
{   // R<1>R
    RfReference res(new RfExpression);
    res->BeginSubcall(*this);
    res->EndSubcall();
    return res; 
}

template<int n>
class RfMultyCallBegin 
RfMultyCallConstructor<n>::operator<(const SReference &x)
{
    return RfMultyCallBegin(x, n);
}

inline
RfMultyCallConstructor<1> operator<(RfFormConstructor,RfFormConstructor)
{
    return RfMultyCallConstructor<1>();
}

inline RfFormConstructor::CallEnd 
operator>(const SReference &r, RfFormConstructor)
{ 
    return RfFormConstructor::CallEnd(r); 
}

class IntelibX_not_a_refal_function : public IntelibX {
public:
    IntelibX_not_a_refal_function(SReference a_param);
};

class IntelibX_not_a_refal_call : public IntelibX {
public:
    IntelibX_not_a_refal_call(SReference a_param);
};

#endif // sentry


