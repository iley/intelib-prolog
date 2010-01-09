//   InteLib                                    http://www.intelib.org
//   The file sexpress/sbacklnk.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file sbacklnk.hpp
    \brief Heterogenous doubly-linked lists
  
    See the SExpressionBacklink class description 
*/

#ifndef INTELIB_SBACKLNK_HPP_SENTRY
#define INTELIB_SBACKLNK_HPP_SENTRY

#include "sexpress.hpp"
#include "iexcept.hpp"

class SBacklinkRef;

//! An element of a doubly-linked list
/*! Effectively this class is just a cons with a pointer for backwards
    direction, intended to point to the previous node of the list.
    \warning Generic doubly-linked list is a data structure with oriented
    cycles of references, therefore it can NOT be garbage-collected with
    the usual reference-counting technique. To avoid this problem, the 
    backwards reference is made a plain pointer instead of a reference.
    Thus it sometimes can be unsafe to use it. That's why there's no
    direct access to it; the methods of the class change the field
    only together with changing Cdr of the pointed object. You can
    destroy this consistensy changing Cdr manually; the behaviour is
    undefined for this case. DO NOT assign to Cdr() if you want to be safe.
 */
class SExpressionBacklink : public SExpressionCons {
    SExpressionBacklink* back;
public:
        //! Type identifier
    static IntelibTypeId TypeId;

        //! The constructor
        /*! Backward reference is set to NULL */
    SExpressionBacklink(const SReference &acar,
                        const SReference &acdr)
           : SExpressionCons(acar, acdr, TypeId), back(0) {}
protected:
    SExpressionBacklink(const SReference &acar,
                        const SReference &acdr, 
                        const IntelibTypeId &t)
           : SExpressionCons(acar, acdr, t), back(0) {}
    ~SExpressionBacklink(); 

public:
        /*! \warning In this version cloning 2l-lists is not implemented */
    virtual SExpression* Clone() const;

    // TextRepresentation is just the same as for SExpressionCons

        //! Connect this element to the end of another one
        /*! If this element has something before the back or 
            the where_to item has something after it, 
            the function returns false. Otherwise, it connects
            them (thus changing our backlink and where_to's Cdr), 
            returning true.
         */  
    bool ConnectBack(const SBacklinkRef &where_to);
        //! Destroy the connection between us and the item right back to us
        /*! If there's no item backwards to us, simply returns false;
            Otherwise, destroys the link between us (thus changing our 
            backlink and where_to's Cdr), returning true.
         */
    bool DisconnectBack();

        //! Insert new item into the list right after this one
    void InsertAfter(const SReference &ref);
        //! Remove the next item from the list
    bool RemoveNext();

        //! Get the previous item reference
    class SBacklinkRef Prev() const;
        //! Get the next item reference
    class SBacklinkRef Next() const;
};

//! Reference to a SExpressionBacklink
/*! \internal This class is not derived from GenericSReference because
    it can point to PTheEmptyList which might be of a wrong type
 */
class SBacklinkRef : public SReference {
public:
        //! Default constructor (makes unbound reference)
    SBacklinkRef();
        //! Constructor from a plain pointer
    SBacklinkRef(SExpressionBacklink *ex);
        //! Constructor from a SReference with type checking
    SBacklinkRef(const SReference &ref);
        //! Copy constructor
    SBacklinkRef(const SBacklinkRef &other);
        //! The destructor
    ~SBacklinkRef();

        //! Assignment
    SBacklinkRef& operator=(SExpressionBacklink *ex)
        { this->SReference::operator=(ex); return *this; }
        //! Assignment
    SBacklinkRef& operator=(const SBacklinkRef &other)
        { this->SReference::operator=(other); return *this; }
        //! Assignment
    SBacklinkRef& operator=(const SReference &ref);


        //! Insert after the current
        /*! For an empty list, creates a list of one element.
            Else, just calls SExpressionBacklink::InsertAfter(ref)
         */
    void InsertAfter(const SReference &ref);

        //! Remove the current element from the list
        /*! For empty list, returns false; otherwise, tries to remove 
            the current element and to advance backwards. If there's no 
            previous element, advances forwards. If it is impossible 
            as well, becomes EmptyList (this includes the case of removing 
            the last node from a dotted backlinked list).
         */
    bool Remove();

        //! Get the pointer to the referenced object.
        /*! \warning Please note that the smart pointer SBacklinkRef
            may be pointing to the EmptyList object instead of an object
            of the SExpressionBacklink class. GetPtr() will simply return
            0 (null pointer) in this case because the address of EmptyList
            can not be cast to SExpressionBacklink.
         */
    SExpressionBacklink *GetPtr() const;

        //! Field/method selector
    SExpressionBacklink *operator->() const { return GetPtr(); }

        //! Append another element to the list
    SBacklinkRef operator,(const SReference &s);

        //! Get the first element of the list
    SBacklinkRef Begin() const;
        //! Get the last element of the list
    SBacklinkRef End() const;
    
        //! Move forward
    SBacklinkRef& operator++();
        //! Move backward
    SBacklinkRef& operator--();
        //! Move forward
    SBacklinkRef operator++(int);
        //! Move backward
    SBacklinkRef operator--(int);
};


//! Exception: backlinked cons expected
class IntelibX_not_a_backlink : public IntelibX {
public:
    IntelibX_not_a_backlink(SReference a_param);
};

#endif
