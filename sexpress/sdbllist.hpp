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




/*! \file sdbllist.hpp
    \brief Doubly-linked lists

    This file invents an S-expression which itself is a container for
    S-expressions, organized as a doubly-linked list.  See the
    SExpressionDoubleList class description.
 */

#ifndef INTELIB_SDBLLIST_HPP_SENTRY
#define INTELIB_SDBLLIST_HPP_SENTRY

#include "iexcept.hpp"
#include "sbacklnk.hpp"
#include "gensref.hpp"

//! Exception: double list expected
class IntelibX_not_a_doublelist : public IntelibX {
public:
    IntelibX_not_a_doublelist(SReference a_param);
};

//! S-expression containig a doubly-linked list
/*! The list is built off objects of the SExpressionBacklink class.
    This property of the implementation is visible and can even be
    useful in some cases.
 */ 
class SExpressionDoubleList : public SExpression {
    SBacklinkRef begin, end;
protected:
        //! Constructor for derived types
    SExpressionDoubleList(const IntelibTypeId &t) : SExpression(t) {}
    SExpressionDoubleList(const SBacklinkRef &a_begin,
                          const SBacklinkRef &a_end,
                          const IntelibTypeId &t) 
        : SExpression(t), begin(a_begin), end(a_end) {}
        //! The destructor
    ~SExpressionDoubleList() {}
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! The default constructor creates an empty list
    SExpressionDoubleList() : SExpression(TypeId) {}

        //! Construct the object from an existing chain of items
        /*! \warning The structure will be shared, so you'd better
            not use the list you pass. It's the caller's duty to 
            make sure a_begin and a_end are the ends of the same list
            and there are no elements before and after them. No 
            checks are performed. Chaos guaranteed if you don't comply.
         */
    SExpressionDoubleList(const SBacklinkRef &a_begin,
                          const SBacklinkRef &a_end)
        : SExpression(TypeId), begin(a_begin), end(a_end) {}

        //! Add an item to the head of the list
    const SBacklinkRef& AddToBegin(const SReference &ref);
        //! Add an item to the tail of the list
    const SBacklinkRef& AddToEnd(const SReference &ref);

        //! Get the first element's location
    SBacklinkRef GetBegin() const { return begin; }
        //! Get the last element's location
    SBacklinkRef GetEnd() const { return end; }

        //! Exclude a sublist from the list
        /*! Parameters from and thru must point to somewhere inside the 
            list, and the from must precede the thru, or else the 
            result is undefined.
            \par The excluded part remains to be a linked chain, but
            the first element doesn't have a backlink, and the last 
            one doesn't have a Cdr link. The remaining part is 
            updates as to remain consistent.
            \param from points to the first element to be excluded
            \param thru points to the last element to be excluded
         */
    void Exclude(const SBacklinkRef &from, const SBacklinkRef &thru);
        //! Insert a linked chain of elements into the list
        /*! Parameters left and right must point to ends of a 
            linked chani (e.g., a chain excluded from somewhere with
            the Exclude() method). The chain is inserted into the list
            after the element pointed by the from parameter. If from is
            unbound, the chain is inserted into the head of the list.
            \param from determines the position where to insert the chain
            \param left points to the left end of the chain to be inserted
            \param right points to the right end of the chain to be inserted
         */
    void InsertListAfter(const SBacklinkRef &from, 
                         const SBacklinkRef &left, 
                         const SBacklinkRef &right);

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

    class Iterator;
    friend class Iterator;

private:
    void SetTheOnlyItem(const SReference &ref);
};


//! A base for a type-specific smartpointer
typedef GenericSReference<SExpressionDoubleList,IntelibX_not_a_doublelist>
SDoubleListRefBase;


//! Reference to a doubly linked list
class SDoubleListRef : public SDoubleListRefBase {
public:
        //! The default constructor
    SDoubleListRef() {}
        //! Constructor from a pointer
    SDoubleListRef(SExpressionDoubleList *e) : SDoubleListRefBase(e){}
        //! A copy constructor
    SDoubleListRef(const SDoubleListRef &e) : SDoubleListRefBase(e){}
        //! A cast constructor with typechecking
    SDoubleListRef(const SReference &e) : SDoubleListRefBase(e){}
        //! The destructor
    ~SDoubleListRef() {}

        //! Append another element to the list
    SDoubleListRef& operator,(const SReference &s);
};


//! Iterator for a doubly-linked list
class SExpressionDoubleList::Iterator : public SBacklinkRef {
    SDoubleListRef master;
public:
        //! Construct by a reference
    Iterator(const SDoubleListRef &m, bool from_end = false)
        : SBacklinkRef(
            m.GetPtr() ? (from_end ? m->end : m->begin) : SBacklinkRef()
          ), master(m) {}
        //! Construct by a pointer to the master object
    Iterator(SExpressionDoubleList *m, bool from_end = false)
        : SBacklinkRef(from_end ? m->end : m->begin), master(m) {}

        //! Move the iterator to the first item of the list
    void SetBegin() { SBacklinkRef::operator=(master->begin); }
        //! Move the iterator to the last item of the list
    void SetEnd() { SBacklinkRef::operator=(master->end); }

        //! Insert a new item into the list before the current one
    void InsertBefore(const SReference& r);
        //! Insert a new item into the list after the current one
    void InsertAfter(const SReference& r);
        //! Remove the current item
    bool Remove();

        //! Are we at the first element of the list?
    bool IsBegin() const { return GetPtr() && (*this)->Prev().IsEmptyList(); }
        //! Are we at the last element of the list?
    bool IsEnd() const { return GetPtr() && (*this)->Next().IsEmptyList(); }
        //! Either the list is empty or we stepped past the last item
    bool Exhausted() const { return !GetPtr() || IsEmptyList(); }

        //! Get the list obhect we are operating on
    const SDoubleListRef& GetMaster() { return master; }
};

inline SDoubleListRef& SDoubleListRef::operator,(const SReference &s)
{ (*this)->AddToEnd(s); return *this; }

//! Doubly-linked list container
/*! This class differs from SDoubleListRef in that it creates the list 
    to reference to it right on construction, and disallows assignments.
    It should be considered as a 'container' with operations typical
    for doubly-linked lists.
 */
class SDoubleList : public SDoubleListRef {
public:
        //! The constructor creates the object of empty list
    SDoubleList() : SDoubleListRef(new SExpressionDoubleList) {}
private:
    SDoubleList(const SDoubleList&) {}
    void operator=(const SDoubleList&) {}
};


//! The Doubly Linked List Constructor
/*! This class is much like the SListConstructor class, except it creates 
    a doubly-linked list. 
 */
class SDoubleListConstructor {
public:
    SDoubleListConstructor() {}   //!< Constructor
    ~SDoubleListConstructor() {}  //!< Destructor
    
        //! This operator makes a doubly-linked list of one element
        /*! \param t is the item of the list being created */
    SDoubleListRef operator |(const SReference &t) const
    {
        SDoubleListRef r(new SExpressionDoubleList);
        r->AddToBegin(t);
        return r;
    }
};


//! A part of a doubly-linked list
class SDoubleListRange {
    SDoubleListRef master;
    SBacklinkRef first, follow;
public:
        //! Default constructor
        /*! \warning No methods will work until the object is assigned
            to something useful! Using operator= is mandatory if the object
            was created with the default constructor.
         */
    SDoubleListRange() {}
    
        //! Range to match the whole list
    SDoubleListRange(const SDoubleListRef& a_master) 
        : master(a_master), first(master->GetBegin()), follow(*PTheEmptyList)
        {}

        //! Arbitrary range
        /*! \param a_master is the doubly linked list object
            \param a_first is the first element in the range
            \param a_follow is the element which FOLLOWS next to 
                   the last one in the range. Thus, if a_follow 
                   points to the same position as a_first, then
                   the range is assumed empty. The empty list 
                   means that the last item in the range is the 
                   last item in the list.
         */
    SDoubleListRange(const SDoubleListRef& a_master,
                     const SBacklinkRef& a_first,
                     const SBacklinkRef& a_follow)
        : master(a_master), first(a_first), follow(a_follow) 
        {}

        //! Copy constructor
    SDoubleListRange(const SDoubleListRange& o)
        : master(o.master), first(o.first), follow(o.follow) 
        {}
 
        //! Assignment
    void operator=(const SDoubleListRange &a) 
        { master = a.master; first = a.first; follow = a.follow; }

        /*! Is the range empty, that is, do its ends point to
            the same position
         */
    bool IsEmpty() const { return first.GetPtr() == follow.GetPtr(); }
        /*! Is the range 'exhausted', that is, it starts nowhere? 
         */
    bool Exhausted() const { return first.IsEmptyList(); }

        //! Make a copy of the range
        /*! The items are not copied, but the list cells are
            copied so that no SExpressionBacklink's are shared
         */
    SDoubleListRef Copy() const;

        //! Slave class which represents the left end of the range
    class LeftEndMover { 
        friend class SDoubleListRange;
        SDoubleListRange *master;
        LeftEndMover(SDoubleListRange *a_master) : master(a_master) {}
    public:
        operator SBacklinkRef() const;     //!< Cast to the item reference
        SBacklinkRef operator++() const;   //!< Move right
        SBacklinkRef operator++(int) const;//!< Move right
        SBacklinkRef operator--() const;   //!< Move left
        SBacklinkRef operator--(int) const;//!< Move left
        void JumpToFirst() const; //!< Move to the firts item of the list
        void JumpToLast() const;  //!< Move to the last item of the list
        void JumpToRightEnd() const;
            //!< Move to the last item of the range thus making it empty
    };
    friend class LeftEndMover;

        //! Slave class which represents the right end of the range
    class RightEndMover { 
        friend class SDoubleListRange;
        SDoubleListRange *master;
        RightEndMover(SDoubleListRange *a_master) : master(a_master) {}
    public:
        operator SBacklinkRef() const;     //!< Cast to the item reference
        SBacklinkRef operator++() const;   //!< Move right
        SBacklinkRef operator++(int) const;//!< Move right
        SBacklinkRef operator--() const;   //!< Move left
        SBacklinkRef operator--(int) const;//!< Move left
        void JumpToFirst() const; //!< Move to the first item of the list
        void JumpToLast() const;  //!< Move to the last item of the list
        void JumpToLeftEnd() const;
            //!< Move to the first item of the range thus making it empty
    };
    friend class RightEndMover;

        //! Get the left end of the range
    LeftEndMover LeftEnd() { return LeftEndMover(this); }
        //! Get the right end of the range
    RightEndMover RightEnd() { return RightEndMover(this); }

        //! Get the object we're operating on
    const SDoubleListRef& GetMaster() const { return master; }
 
        //! Get the first item of the range
    SBacklinkRef GetFirst() const;
        //! Get the item which immediately follows the range
    const SBacklinkRef& GetFollow() const;
        //! Get the last item of the range
    SBacklinkRef GetLast() const;

        //! Move the left end to the given position
        /*! \warning No integrity checking is performed, be careful */
    void SetFirst(const SBacklinkRef& r) { first = r; }
        //! Move the right end to the given position
        /*! \warning No integrity checking is performed, be careful */
    void SetFollow(const SBacklinkRef& r) { follow = r; }

        //! Get the part of the list before the range
    SDoubleListRange WhatsBefore() const
        { return SDoubleListRange(master, master->GetBegin(), first); }
        //! Get the part of the list after the range
    SDoubleListRange WhatsAfter() const
        { return SDoubleListRange(master, follow, *PTheEmptyList); }
};




#endif
