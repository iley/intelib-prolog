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




/*! \file sexpress.hpp
    \brief The basic S-expression types and methods

    The most notable things defined in this file are probably the
    SReference class which is the main interface to the S-expression
    model provided by InteLib; the SExpression abstract class which
    is the common ancestor of all the S-expressions; and the classes
    SExpressionInt, SExpressionFloat, SExpressionString, 
    SExpressionLabel and SExpressionCons, which represent basic
    atomic S-expression types as well as the Cons (from which all
    lists are built).
*/

#ifndef INTELIB_SEXPRESS_HPP_SENTRY
#define INTELIB_SEXPRESS_HPP_SENTRY

#include "refcount.hpp"


//! InteLib product name and version
extern const char the_intelib_title[];

//! InteLib copyright notice 
extern const char the_intelib_copyright[];

//! InteLib brief licensing information
extern const char the_intelib_copyright[];

//! An integer identifying the InteLib version
extern const int the_intelib_version_id;

/* ------------------------------------------------------- */
/* The main defaults                                       */

#ifndef INTELIB_INTEGER_T 
    //! Integer type to be used by InteLib
#define INTELIB_INTEGER_T int
#endif 
 
#ifndef INTELIB_FLOAT_T
    //! Floating point number type to be used by InteLib
#define INTELIB_FLOAT_T float
#endif 

#ifndef INTELIB_SIMPLE_STRING_LIMIT
    //! String has room for this many chars; arrays are allocated for longer
#define INTELIB_SIMPLE_STRING_LIMIT sizeof(char*)
#endif
 
#ifndef INTELIB_TEXT_REPRESENTATIONS
    //! Whether InteLib supports text representations of S-expressions
#define INTELIB_TEXT_REPRESENTATIONS 1
#endif

#if INTELIB_TEXT_REPRESENTATIONS == 1
#  ifndef INTELIB_INTEGER_FORMAT
    //! How should sprintf() be called to make represetnation of an integer
#  define INTELIB_INTEGER_FORMAT "%d"
#  endif
#  ifndef INTELIB_FLOAT_FORMAT
    //! How should sprintf() be called to make represetnation of a float
#  define INTELIB_FLOAT_FORMAT "%f"
#  endif
#endif

#ifndef INTELIB_RUNTIME_CHECKS
    //! Should InteLib perform some sanity cheks. Should be 1 during debug
#define INTELIB_RUNTIME_CHECKS 1
#endif

#ifndef INTELIB_DEBUG_COUNTERS
    //! Enables a global counter of allocated SExpression objects.
    /*! Should normally be off, unless to test for memory leaks */
#define INTELIB_DEBUG_COUNTERS 0
#endif

    //! Integer type used by InteLib
typedef INTELIB_INTEGER_T intelib_integer_t;

    //! Floating point number type used by InteLib
typedef INTELIB_FLOAT_T intelib_float_t;

    //! "Simple" string max. length
    /*! Strings of this many chars are stored inside the
        SExpressionString object, without creating dynamic 
        strings
     */
const unsigned int intelib_simple_string_limit 
                          = INTELIB_SIMPLE_STRING_LIMIT;

/* ------------------------------------------------------- */

//! S-expression type identificators
/*! The class is used to identify types of S-expressions at 
    runtime. Each descender of SExpression declares a public static 
    member of the IntelibTypeId type to be the identifier of the
    given type of S-expression. It is named TypeId. 
    So SExpressionInt::TypeId identifies integer S-expressions, 
    SExpressionCons::TypeId - dotted pairs etc.
 */
class IntelibTypeId {
        //! Identifier of the supertype
    const IntelibTypeId *prev;
    bool is_mutable;
public:
        //! The default constructor
        /*! Only used for SExpression::TypeId */
    IntelibTypeId() 
        : is_mutable(false) { prev = 0; }
        //! This for inherits changeability from the base class
    IntelibTypeId(const IntelibTypeId *previous) 
        : is_mutable(previous->is_mutable) { prev = previous; }
        //! The most general constructor
    IntelibTypeId(const IntelibTypeId *previous, bool a_is_mutable) 
        : is_mutable(a_is_mutable) { prev = previous; }
        //! The destructor
    ~IntelibTypeId() {}

        //! Get the pointer to the superclass' identifier
    const IntelibTypeId* Prev() const { return prev; } 

        //! Searches the chain of supertypes 
    bool IsSubtypeOf(const IntelibTypeId &op) const; 
        //! Comparision
    bool operator==(const IntelibTypeId &op) const
       { return &op == this; } 
        //! Comparision
    bool operator!=(const IntelibTypeId &op) const
       { return &op != this; } 

        //! Changeability
    bool IsMutable() const { return is_mutable; }

#if INTELIB_DEBUG_COUNTERS == 1
    mutable long object_counter; 
#endif
private:
    // disallow copying
    IntelibTypeId(const IntelibTypeId &) {} 
};

#if INTELIB_TEXT_REPRESENTATIONS == 1
class SString; // we need it before it is defined, for text representations
#endif

//! Generic S-expression
/*! The class represents the most common properties of all possible
    S-expressions. 
 */
class SExpression : public GarbageSafe {
        //! Run-time S-expr. type id
    const IntelibTypeId *term_type_id;
public:
        //! Identifier for the set of all possible S-expressions
        /*! \note It never can be the value of term_type_id 
            because SExpression is an abstract class. 
            SExpression::TypeId is used for unification, to identify
            the whole world of S-expressions. 
            \note IntelibTypeId::IsSubtypeOf(SExpression::TypeId) is 
            true for all objects of IntelibTypeId class. Guess why.
         */
    static IntelibTypeId TypeId;

        //! Constructor
        /*! \param the_type intended for the child classes to 
                   pass their (real) type ids
         */
    SExpression(const IntelibTypeId &the_type);

protected:
        //! The destructor
        /*! \note The destructor is intentionally made protected.
                  This doesn't make much sense for the abstract class,
                  but demonstrates the required practice for all the
                  derived classes.  Destructors of all SExpression
                  subclasses MUST be kept protected to ensure that
                  the S-expression objects are only created in the
                  dynamic memory.
         */
    virtual ~SExpression();
public:
        //! Actual S-expression type
    const IntelibTypeId& TermType() const { return *term_type_id; }

        //! Can it be changed during the lifetime of the object?
    bool IsMutable() const { return term_type_id->IsMutable(); }

        //! Clone a mutable object
        /*! The method should return the pointer created with new
            \note It can (and should) return null pointer 
            for non-mutables 
            \warning This method will hang your program being called
            for a looped data structure, including looped lists,
            a hash table which references a list in which it is a 
            member, etc.
            \warning If you create a mutable S-expression, 
            you MUST make sure this method duplicates your object 
            in such a way that nothing is shared between the old 
            and new objects, that is, changing the new
            object can not in any way change the old object. 
         */
    virtual SExpression* Clone() const { return 0; }
     

#if INTELIB_TEXT_REPRESENTATIONS == 1
        //! Text representation of the S-expression
        /*! Returns the text string which represents the given 
            S-expression as for real Lisp (for example, 
            (25 36 49))
         */
    virtual class SString TextRepresentation() const = 0;
#endif

        //! Are the two objects EQL (while not EQ)
        /*! This function is overriden by those SExpressions able to be EQL
            while being not EQ. 
            \warning It is not demanded that this function return true 
            if it's arguments are the same SExpression 
            (that is, they're EQ). 
            It is only called by the EQL predicate implementation when
            it has been checked that the operands are not the same object,
            but ARE of the exactly same type.  This means you can assume
            the parameter points to an object of the same class and use
            static_cast.
         */
    virtual bool SpecificEql(const SExpression* /*other*/) const 
        { return false; }
private:
        //! Copying and assigning SExpressions disallowed
    SExpression(const SExpression&) {}
        //! Copying and assigning SExpressions disallowed
    void operator = (const SExpression&) {}

#if INTELIB_DEBUG_COUNTERS == 1
public:
    static long object_counter; 
#endif
};

//! Integer S-expression
/*! The class reresents an S-expression of an integer number. 
    The actual number has the type intelib_integer_t which 
    must be short, int, long or long long. Using unsigned 
    integers is not recommended.
 */
class SExpressionInt : public SExpression {
    intelib_integer_t data;
public:
    static IntelibTypeId TypeId;
    SExpressionInt() : SExpression(TypeId) { data = 0; }
    SExpressionInt(char d) : SExpression(TypeId) { data = d; }
    SExpressionInt(short int d) : SExpression(TypeId) { data = d; }
    SExpressionInt(int d) : SExpression(TypeId) { data = d; }
    SExpressionInt(long int d) : SExpression(TypeId) { data = d; }
    SExpressionInt(long long int d) : SExpression(TypeId) { data = d; }
    SExpressionInt(unsigned char d) : SExpression(TypeId) { data = d; }
    SExpressionInt(unsigned short int d) : SExpression(TypeId) { data = d; }
    SExpressionInt(unsigned int d) : SExpression(TypeId) { data = d; }
    SExpressionInt(unsigned long int d) : SExpression(TypeId) { data = d; }
    SExpressionInt(unsigned long long int d) 
        : SExpression(TypeId) { data = d; }
 
    intelib_integer_t GetValue() const { return data; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
        /*! INTELIB_INTEGER_FORMAT controls how the
            integer is converted to text
         */
    virtual SString TextRepresentation() const;
#endif

protected:
    virtual ~SExpressionInt() {}
    
    virtual bool SpecificEql(const SExpression* other) const;
};

//! Floating point number S-expression
/*! The class represents a floating point numberic constant 
    S-expression. The actual type of the value is intelib_float_t 
    which must be float, double or long double.
 */
class SExpressionFloat : public SExpression {
    intelib_float_t data;
public:
    static IntelibTypeId TypeId;
    SExpressionFloat() : SExpression(TypeId) { data = 0; }
    SExpressionFloat(float d) : SExpression(TypeId) { data = d; }
    SExpressionFloat(double d) : SExpression(TypeId) { data = d; }
    SExpressionFloat(long double d) : SExpression(TypeId) { data = d; }
 
    intelib_float_t GetValue() const { return data; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
    /*! INTELIB_FLOAT_FORMAT controls how the
        number is converted to text
     */
    virtual SString TextRepresentation() const;
#endif

protected:
    virtual ~SExpressionFloat() {}
    
    virtual bool SpecificEql(const SExpression* other) const;
};

//! Character constant
/*! The class represents a single character, such as #\\a or #\\Newline
    \warning Normally, you never need to create objects of this class,
    as there are already an internal vector containing objects
    for all 256 possible chars.
 */
class SExpressionChar : public SExpression {
    char s[2];
public: 
    static IntelibTypeId TypeId;

    SExpressionChar(int c) : SExpression(TypeId) { s[0] = c; s[1] = 0; }

protected:
    ~SExpressionChar() {}
    virtual bool SpecificEql(const SExpression* other) const;

public:
    char GetValue() const { return s[0]; }
    const char *GetString() const { return s; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
        /*! The representation includes double quotes. 
            All special characters are converted to standard C 
            string escapes
         */
    virtual SString TextRepresentation() const;
#endif
};


//! String literal constant
/*! The class represents a string constant. Note the library doesn't
    support character vectors. That is, the string constant is considered
    atomic constant in all sences. 
    \see SString class
 */
class SExpressionString : public SExpression {
        //! Storage mode
        /*! - true means str.p is used
            - false means str.s is used and the string is shorter than 
              intelib_simple_string_limit
         */
    bool is_indirect;
    union {
        char *p;
        char s[intelib_simple_string_limit];
    } str;
        /*! Determine the appropriate storage mode and set up the object */
    void StringSetup(const char *s); 
public: 
    static IntelibTypeId TypeId;

    SExpressionString() : SExpression(TypeId) 
             { is_indirect = 0; str.s[0] = 0; } // just empty string
    SExpressionString(const char *s); 
        //! Concatenating constructor
        /*! This form of the constructor builds concatenation of the 
            two strings */
    SExpressionString(const char *s1, const char *s2);

protected:
        //! Protected
        /*! This form of the constructor is intended for derived classes */
    SExpressionString(const IntelibTypeId &tid, const char *s);
    ~SExpressionString();    

public:
    const char* GetValue() const
      { return is_indirect ? str.p : str.s; }

#if INTELIB_TEXT_REPRESENTATIONS == 1
        /*! The representation includes double quotes. 
            All special characters are converted to standard C 
            string escapes
         */
    virtual SString TextRepresentation() const;
#endif

protected:
    virtual bool SpecificEql(const SExpression* other) const;
};

//! Classic atom
/*! The purpose of this class is just to represent the difference between
    `CAT' and `"CAT"' being read by the IntelibReader in case no real
    package is given. You can use it for your own purposes, too. 
    \note This class is declared here because slexer.cpp needs it, but
    it is no good to declare it in slexer.hpp because lexer.hpp is not
    intended to be included by user at all.
 */
class SExpressionClassicAtom : public SExpressionString {
public:
    static IntelibTypeId TypeId;
    SExpressionClassicAtom(const char *s) : SExpressionString(TypeId, s) {}
    virtual SString TextRepresentation() const; 
      // Note we don't need to write specific SpecificEql function. 
      // It's version of SExpressionString will perfectly serve us.
protected:
    ~SExpressionClassicAtom() {}
};


//! Label (S-expression whose address is the only thing important)
/*! SExpressionLabel can be used for:
     - Scheme-flavor empty list representation
     - Scheme-flavor booleans
     - CommonLisp-flavor keywords
     - etc, 
    that is, it is suggested to be used when there's an S-expression which 
    exists one and only one of it's kind and plays a special role just 
    because it is that one, not because of it's type, state, values etc.
    \par Besides that, it could be convenient to represent the notion of a 
    Lisp Symbol with a class derived from SExpressionLabel.
 */
#if INTELIB_TEXT_REPRESENTATIONS == 1
class SExpressionLabel : public SExpression {
    char *name;
protected:
    SExpressionLabel(const IntelibTypeId &the_type, const char *a_name);
public:
    static IntelibTypeId TypeId;
    SExpressionLabel(const char *a_name);
    virtual SString TextRepresentation() const;
    const char* GetName() const { return name; }
protected:
    ~SExpressionLabel();
};
#else
class SExpressionLabel : public SExpression {
protected:
    SExpressionLabel(const IntelibTypeId &the_type, const char*)
        : SExpression(the_type) {}
    ~SExpressionLabel() {}
public:
    static IntelibTypeId TypeId;
    SExpressionLabel(const char* /*a_name*/) : SExpression(TypeId) {}
    const char* GetName() const { return ""; }
};
#endif


//! 'Smart pointer' to an S-expression
/*! The class acts just like a smart pointer to an object of
    SExpression class. SReference has no virtual table so it is 
    guaranteed that sizeof(SReference) == sizeof(SExpression*).
    This means it's no problem to pass SReferences by 
    value. 
      SReference is intended to be the primary API to the 
    invented S-expression representation.  
 */
class SReference : public GenericReference<SExpression> {
public:
        //! Default constructor
        /*! Creates NULL (unbound) reference */
    SReference() {}       

        //! Construct from a pointer
    SReference(const SExpression *tp) : GenericReference<SExpression>(tp) {} 

        //! The copy constructor
    SReference(const SReference &other) 
        : GenericReference<SExpression>
          (static_cast<const GenericReference<SExpression>&>(other)){}
    ~SReference() {}
   
         //@{
         /*! These constructors allow to cast any standard scalar type to 
             the appropriate S-expression 
          */
    SReference(char c);
    SReference(signed char c);
    SReference(unsigned char c);
    SReference(signed short i);
    SReference(unsigned short i);
    SReference(signed int i);
    SReference(unsigned int i);
    SReference(signed long i);
    SReference(unsigned long i);
    SReference(signed long long i);
    SReference(unsigned long long i);
    SReference(float f);
    SReference(double f);
    SReference(long double f);
    SReference(const char *s);
    SReference(const unsigned char *s);
         //@}
        //! Creates empty list repesentation
    SReference(const class SListConstructor &); 
        //! Creates a dotted pair
    SReference(const SReference &car, const SReference &cdr);

    SReference& operator=(const SReference &other)
      { GenericReference<SExpression>::operator=(other); return *this; }
    void operator=(SExpression* newterm)
      { GenericReference<SExpression>::operator=(newterm); }

    SExpression* GetPtr() const 
      { return (SExpression*)GenericReference<SExpression>::GetPtr(); }

        //! Add item to list
        /*! For a list, adds another dotted pair to its end.
            For non-lists, the result is undefined.
         */
    SReference& AddAnotherItemToList(const SReference &right); 
        //! Make/change dotted list
        /*! - For a proper (non-dotted) list, replaces its 
              terminating () with the given argument, 
              therefore producing a dotted list.
            - for a dotted list, changes it's terminating value
              (the value which is after the dot)
            - for other types, the result is undefined
         */
    SReference& ChangeListEnd(const SReference &new_last); 
        //! Make a cons
        /*! Produces a cons (dotted pair) using *this as the
            CAR and the given argument as the CDR
         */
    SReference MakeCons(const SReference &right) const;

        //! List composing operation
        /*! Operator "," is used to construct lists naturally, like this:
            (L| 1, 2, 3) 
            \see LListConstructor class
         */
    SReference& operator,(const SReference &r) 
        { return AddAnotherItemToList(r); }
        //! dotted list composing operation
        /*! Operator "||" is used to construct dotted lists, like this:
            ((L| 1, 2, 3) || 4)    // (1 2 3 . 4)
            \see LListConstructor class
         */
    SReference& operator||(const SReference &t) 
        { return ChangeListEnd(t); }
        //! CONS operation
        /*! Operator "^" is used to produce conses */
    SReference operator^(const SReference &t) const 
        { return MakeCons(t); }

        //! Equality is just pointer comparision
    bool operator == (const SReference &other) const
      { return GetPtr() == other.GetPtr(); }
        //! Non-equality is just pointer comparision
    bool operator != (const SReference &other) const 
      { return GetPtr() != other.GetPtr(); }
        //! Equality is just pointer comparision
    bool operator == (const SExpression *other) const
      { return GetPtr() == other; }
        //! Non-equality is just pointer comparision
    bool operator != (const SExpression *other) const
      { return GetPtr() != other; }

    bool IsEql(const SReference &other) const;
    bool IsEqual(const SReference &other) const;

    SReference& Car() const; 
    SReference& Cdr() const;
    SReference& CCar() const; 
    SReference& CCdr() const;
    intelib_float_t GetFloat() const;
    intelib_integer_t GetInt() const;
    const char* GetString() const;
    char GetSingleChar() const;

    template<class Tp> Tp* DynamicCastGetPtr() const;
    template<class Tp> Tp* SimpleCastGetPtr() const;

    SReference Clone() const;
    SReference CopyList() const;
    SReference CopyTree() const;

    bool IsEmptyList() const; 

};


//! Label handler
class SLabel : public SReference {
public:
    SLabel(const char *name) : SReference(new SExpressionLabel(name)) {}
    SExpressionLabel *GetPtr() const
        { return (SExpressionLabel*) SReference::GetPtr(); }
private: /* note all operations prohibited */
    SLabel(const SLabel &) {}
    void operator=(const SLabel &) {}
    void operator=(const SReference &) {}
    void operator=(const SExpression*) {}
};

//! A cons (a dotted pair) 
/*! The class represents a dotted pair ("cons")
 */
class SExpressionCons : public SExpression {
    //! the car of the cons
    SReference car;
    //! the cdr of the cons
    SReference cdr;
protected:
    SExpressionCons(const SReference &acar, const SReference &acdr,
                    const IntelibTypeId &tid)
           : SExpression(tid), car(acar), cdr(acdr) {}
public:
    static IntelibTypeId TypeId;

    SExpressionCons(const SReference &acar, const SReference &acdr)
           : SExpression(TypeId), car(acar), cdr(acdr) {}
    ~SExpressionCons(){} 

    SReference& Car() { return car; }
    SReference& Cdr() { return cdr; }
    const SReference& Car() const { return car; }
    const SReference& Cdr() const { return cdr; }

    virtual SExpression* Clone() const; 

#if INTELIB_TEXT_REPRESENTATIONS == 1
        /*! Used as a subroutine of TextRepresentation() to implement
            a representation of a part of a list 
            \par delim is the string to delimit the list's elements
                 (" " is used by TextRepresentation())
            \par dotdelim is the string to delimit an irregular (dotted) 
                 list's termination element
                 (" . " is used by TextRepresentation())
            \par repfun If given, this function is used to create 
                 representation of each element. If repfun remains 
                 null, then generic TextRepresentation is used for 
                 all but unbound SReferences, and the stirng "#<UNBOUND>"
                 is used for unbounds.
         */
    SString CoreTextRepresentation(const char *delim, 
                                   const char *dotdelim,
                                   SString (*repfun)(const SReference &)=0) 
            const;
        /*! List and dotted lists are represented just like in 
            a real Lisp, that is, with traditional Lisp syntax.  
         */
    virtual SString TextRepresentation() const;
#endif
};


//! Empty List
/*! This global pointer points to the SExpression object used as the
    empty list representation. If only the sexpress/ part of the 
    library is used, the pointer remains 0 (NULL), that is, unbound
    reference is used to mark the end of a list. The lisp/ part of
    the library changes this pointer during initialization so that
    it points to the symbol NIL
 */
extern SReference *PTheEmptyList;


//! The List Constructor
/*! This pseudoclass is not a data structure at all. It is created for
    its set of operations, primarily for operator|().
      Only one instance of the class is needed, and it is usually
    named L.
      This allows to create lists naturally, like 
        (L| 1, 2, 3)
    \see SReference::operator,()
 */
class SListConstructor {
public:
    SListConstructor() {}
    ~SListConstructor() {}
    
    SReference operator |(const SReference &t) const
        { return SReference(new SExpressionCons(t, *PTheEmptyList)); } 
    SReference operator ^(const SReference &other) const
        { return SReference(*PTheEmptyList, other); }
};


template<class Tp> Tp* SReference::DynamicCastGetPtr() const
{
    if(GetPtr() && GetPtr()->TermType().IsSubtypeOf(Tp::TypeId)) 
        return (Tp*)GetPtr(); 
    else
        return 0;
}

template<class Tp> Tp* SReference::SimpleCastGetPtr() const
{
    if(GetPtr() && &(GetPtr()->TermType()) == &(Tp::TypeId)) 
        return (Tp*)GetPtr(); 
    else
        return 0;
}



#endif
