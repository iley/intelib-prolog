//   InteLib                                    http://www.intelib.org
//   The file sexpress/svector.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file svector.hpp
    \brief Vectors and matrices of S-expressions

    This file defines the SExpressionVector class along with
    tools to operate it.
*/
#ifndef INTELIB_SVECTOR_HPP_SENTRY
#define INTELIB_SVECTOR_HPP_SENTRY

#include "iexcept.hpp"
#include "sexpress.hpp"
#include "gensref.hpp"

//! Vector of SExpressions
/*! Implements an optionally-resizeable vector of S-expressions. 
    \note The vector is also an S-expression so it can be a 
    member of a Lisp-like list and/or a member of another vector, etc.
    \warning Do not use the objects of SExpressionVector directly. 
    There are SVectorRef and SVector classes which are designed to be
    the interface to the S-expression vector facility. 
 */
class SExpressionVector : public SExpression { 
    SReference *vector;
    unsigned int size;
    unsigned int real_size;
    bool resizeable;
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! Constructor
        /*! \param n is the vector's length. 0 (the default) means
            the vector will be dynamically resized. If a positive
            size is given, it means the size is strict and resizing
            is prohibited. If n<0 is given, then the vector will 
            be dynamically resized, and -n is a hint for the constructor
            how much slots to allocate during initialization.
         */    
    SExpressionVector(int n = 0);

protected:
        //! Constructor form for derived classes
    SExpressionVector(const IntelibTypeId &tid, int n = 0);
        //! Destructor
    ~SExpressionVector() { if(vector) delete [] vector; } 

public:
        //! constant version of the indexing operator
    const SReference& operator[](unsigned int n) const;
        //! non-constant version of the indexing operator
    SReference& operator[](unsigned int n);

        //! Get the current slots count
    int Size() const { return size; }
        //! Force resizing the vector
    void SetSize(unsigned int n);
        //! Is it allowed to resize the vector?
    bool IsResizeable() const { return resizeable; }

    virtual SExpression* Clone() const;

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

private:
    void Resize(unsigned int n);
};

//! Exception: vector expected
class IntelibX_not_a_vector : public IntelibX {
public:
    IntelibX_not_a_vector(SReference a_param);
};

//! Basic reference to a SExpressionVector
typedef GenericSReference<SExpressionVector,IntelibX_not_a_vector>
SVectorRefBase;

//! SExpressionVector reference
/*! This class allows to easily handle S-expressions of the Vector 
    type. What it really does is making sure it always points to 
    an SExpressionVector object and providing some vector-specific 
    operations.
    \warning Please be aware that objects of this class do not assume
    'ownership' over the referenced S-vectors; that is, making a copy 
    of the SVectorRef object doesn't make a copy of the vector; the 
    SExpressionVector object is shared between two SVectorRef objects.
    You might want to use SVector class instead of this one if you 
    need a heterogenous container which duplicates the underlying 
    vector on copyings and assignments. 
 */
class SVectorRef : public SVectorRefBase {
public:
        //! Default constructor
        /*! Makes an unbound reference (surprize!) */
    SVectorRef() {}

        //! Raw vector creation constructor
        /*! New vector is created. 
            \par n is the vector's length; it is passed to the 
            SExpressionVector's constructor.
            \note The constructor is made explicit because vector of N 
            elements has nothing to do with the integer N, that is, such a
            cast would be absolute nonsense.
         */
    explicit SVectorRef(int n) : SVectorRefBase(new SExpressionVector(n)) {}

        //! 'Take care' constructor
    SVectorRef(SExpressionVector *ts) : SVectorRefBase(ts) {}

        //! Copy constructor
        /*! \warning The underlying vector is NOT copied */
    SVectorRef(const SVectorRef &other) : SVectorRefBase(other) {}

        //! Cast from SReference
        /*! If the argument doesn't reference to an SExpressionVector 
            object, exception is thrown. Be careful!
         */
    SVectorRef(const SReference &other) : SVectorRefBase(other) {}

        //! Strange constructor
        /*! This form is used by SMatrix to allocate vectors as needed into 
            empty slots. 
            \param ref is the slot to be allocated. It must either 
                reference an SExpressionVector or be a null reference. 
                In the former case a new SExpressionVector will be 
                allocated, thus the ref'erenced object will change!
            \param fake is unused. It is only to tell this form from the 
                SVectorRef(const SReference &ref).
         */
    SVectorRef(SReference &ref, int fake); 

        //! The destructor
    ~SVectorRef() {}

        //! The index operator
    const SReference& operator[](int index) const 
        { return (*GetPtr())[index]; }

        //! The index operator
    SReference& operator[](int index) { return (*GetPtr())[index]; }
};

//! Subvector
/*! This class provides the possibility to select a part 
    within a vector, which can be removed or replaced by anoter
    vector or even copied.
 */
class SVectorRange : public SVectorRef {
    int idx, len;
public:
        //! The constructor
        /*! \param vec is the vector
            \param a_idx is the starting index for the range
            \param a_len is the length for the range
         */
    SVectorRange(const SVectorRef& vec, int a_idx = 0, int a_len = -1)
        : SVectorRef(vec), idx(a_idx), len(a_len) 
    {
        if(len==-1 || len > (*this)->Size()-idx ) 
            len = (*this)->Size()-idx; 
    }

        //! The destructor
    ~SVectorRange() {}

        //! Make a vector as a copy of the range
        /*! \param resizeable specifies if the new vector
                   should be resizeable
         */
    SVectorRef Copy(bool resizeable) const;
   
        //! Make a vector as a copy of the range
        /*! \note Resizeability of the newly-created vector
                  is inherited from the source vector
         */
    SVectorRef Copy() const;   

        //! Exclude the range from the source vector
    void Erase();

        //! Replace the range with another vector
    void Replace(const SVectorRange& other);
};


//! S-expression vector container
/*! This class differs from the SVectorRef class in that copying and
    assigning objects of SVector one really makes copies of vectors,
    not just references. Thus, this class is convenient to be used as
    a heterogenous container, in contrast to a reference to an S-expression
    of a particular (vector) type.
 */
class SVector : public SVectorRef {
public:
        //! Raw vector creation constructor
        /*! New vector is created. 
            \par n is the vector's length; if it is left zero (the 
            default), the constructed vector is autoresized. Thus, 
            by default we create an empty zero-length vector to be 
            auto-resized.
            \note The constructor is made explicit because vector of N 
            elements has nothing to do with the integer N, that is, such a
            cast would be absolute nonsense.
         */
    explicit SVector(unsigned int n = 0) : SVectorRef(n) {}
        //! 'Take care' constructor
        /*! This form is intended to be used like this: 
            SVectorRef(new SExpressionVector(...))
         */
    SVector(SExpressionVector *ts) : SVectorRef(ts) {}
        //! Copy constructor
        /*! \note The underlying vector is Clone()'d, 
            see SExpressionVector::Clone() for details 
         */
    SVector(const SVector &other) : SVectorRef(other.Clone()) {}
        //! Cast from SVectorRef constructor
        /*! \note The underlying vector is Clone()'d, 
            see SExpressionVector::Clone() for details 
         */
    SVector(const SVectorRef &other) : SVectorRef(other.Clone()) {}
    ~SVector() {}

    SVector& operator =(const SVectorRef& ref)
      { SReference::operator=(ref.Clone()); return *this; }
    SVector& operator =(SExpressionVector *lt)
      { SReference::operator=(lt); return *this; }

private:
    // made private to prevent the user from mistakes and
    //   to make sure this always points to SExpressionVector object.
    void operator =(const SReference& ref) {}

};

//! Reference to a multidimensional matrix of S-references
/*! This class references to an SExpressionVector object, whose 
    elements are in turn vectors (may be, of vectors of vectors...).
    The class provides operator[] so that you can address an element
    of such a matrix using as many indexations as there are dimensions.
    \param dim is the count of such dimentions (2 or more)
 */
template<int dim>
class SMatrixRef : public SVectorRef {
public:
    SMatrixRef() : SVectorRef() {}
    SMatrixRef(const SReference &ref) : SVectorRef(ref) {}
    SMatrixRef<dim-1> operator[](int i) 
        { return SVectorRef(SVectorRef::operator[](i), 1/*unused*/); }
};

//! The basis for recursively-defined SMatrixRef
template<>
class SMatrixRef<2> : public SVectorRef {
public:
    SMatrixRef() : SVectorRef() {}
    SMatrixRef(const SReference &ref) : SVectorRef(ref) {}
    SVectorRef operator[](int i) 
        { return SVectorRef(SVectorRef::operator[](i), 1/*unused*/); }
};

template <int dim>
class SMatrix : public SMatrixRef<dim> {
public:
    SMatrix() : SMatrixRef<dim>(SVector()) {}
};

//! Exception: vector is not resizeable
/*! Attempt to resize (that is, range violation) a vector which was 
    created as a fixed size one
 */
class IntelibX_vector_out_of_range : public IntelibX {
public:
    IntelibX_vector_out_of_range(SReference a_param);
};

#endif
