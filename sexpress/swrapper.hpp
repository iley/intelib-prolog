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




/*! \file swrapper.hpp
    \brief SExpressionWrapper class definition

    The SExpressionWrapper is a template to create a wrapping S-expressions
    for any (compound) data type.
 */

#ifndef INTELIB_SWRAPPER_HPP_SENTRY
#define INTELIB_SWRAPPER_HPP_SENTRY

#include "sexpress.hpp"
#include "iexcept.hpp"
#include "gensref.hpp"

//! An S-expression wrapper for an arbitrary compound 
/*! The wrapper is actually a subclass of a given compound,
    derived from it and from SExpression as well.  So, all
    public fields and methods available in the copmound, are
    available in the wrapper.
    \note You might want to provide a copy constructor and
          a default constructor in the compound you wish to
          wrap.
    \note The wrapped part of the object is only initialized
          by the default or the copy constructor, so think
          carefully is it what you want.
 */
template <class Data>
class SExpressionWrapper : public SExpression, public Data {
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! The default constructor
        /*! The data part remains uninitialized, that is,
            the default constructor is used for it.
            \warning This won't compile in case Data has
            no default constructor
         */
    SExpressionWrapper() 
        : SExpression(SExpressionWrapper<Data>::TypeId) {}

        //! The data-initializing constructor
        /*! For Data, its copy constructor is used
         */
    SExpressionWrapper(const Data &t) 
        : SExpression(SExpressionWrapper<Data>::TypeId), Data(t) {}

#if INTELIB_TEXT_REPRESENTATIONS == 1
    SString TextRepresentation() const { return "#<WRAPPER>"; }
#endif

protected:
    ~SExpressionWrapper() {}
};

//! The custom S-expression type identifier for each particular instantiation
template <class Data>
IntelibTypeId SExpressionWrapper<Data>::TypeId(&SExpression::TypeId, true);


//! Reference to a wrapper S-expression
template <class Data>
class SWrapperRef
    : public GenericSReference<SExpressionWrapper<Data>,
                               IntelibX_wrong_expression_type>
{
public:
        //! Default constructor
    SWrapperRef()
        : GenericSReference<SExpressionWrapper<Data>,
                            IntelibX_wrong_expression_type>
              (new SExpressionWrapper<Data>) {}

        //! Initialize the payload part as a copy of the given object
    SWrapperRef(const Data &t) 
        : GenericSReference<SExpressionWrapper<Data>,
                            IntelibX_wrong_expression_type>
              (new SExpressionWrapper<Data>(t)) {}

        //! The copy constructor
    SWrapperRef(const SWrapperRef<Data>& o)
        : GenericSReference<SExpressionWrapper<Data>,
                            IntelibX_wrong_expression_type>(o) {}

        //! Type-checking constructor
    SWrapperRef(const SReference &t)
        : GenericSReference<SExpressionWrapper<Data>,
                            IntelibX_wrong_expression_type>(t) {}

        //! The destructor
    ~SWrapperRef() {}
};


#endif
