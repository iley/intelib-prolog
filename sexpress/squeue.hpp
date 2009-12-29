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




/*! \file squeue.hpp
    \brief S-expression-based queue

    This file invents a class derived from SReference which acts as
    a queue-like container based on S-expressions.  See the SQueue
    class description for details.
 */

#ifndef INTELIB_SQUEUE_HPP_SENTRY
#define INTELIB_SQUEUE_HPP_SENTRY

#include "sexpress.hpp"
#include "iexcept.hpp"


//! Heterogenous queue
/*! This is in fact just a list for which a pointer to its last item
    is stored. The only gain is that the comma operation works faster
    for this class than for SReference. This class has to be 8 bytes 
    long so you wouldn't pass it by value (heh, you CAN'T do it in 
    this version because the copy constructor was intentionally made 
    private).
    \note There's no separate SExpression for this. If you try to 
    make such an object a member of another S-expression then a plain
    list will become the member, and it will share the data structure
    with the container. 
    \warning Modification of the underlying list is possible, but 
    the results will be undefined if you break the connectivity of
    the list and/or do something to its head and the last cons. 
    \warning In this version, direct assigning and copying of queues
    are prohibited. Copying can be emulated, however, by constructing 
    SReference object and then an SQueue object from it.  Rationale 
    is that the user should realize what does he REALLY do.
 */
class SQueue : public SReference {
    SExpressionCons *last;
public:

        //! Just an empty queue
    SQueue();

        //! Construct from existing list
        /*! This uses SReference::CopyList() to make a copy of the list.
            Be sure you are not copying too long data structures
         */
    SQueue(const SReference &ref);

        //! Destructor
    ~SQueue() { /* the parent will do all the job */ }

        //! Insert an item before all the resting queue
    void InsertFirst(const SReference& item);
        //! Append another item to the queue (at the end)
    void Append(const SReference& item);
        //! Remove the first item
    bool RemoveFirst();
        //! Is the queue currently empty
    bool IsEmpty() const { return IsEmptyList(); }
    
        //! Append another item
    template <class Tp> SQueue& operator|(const Tp &t) 
        { Append(SReference(t)); return *this; }

        //! Append another item
    template <class Tp> SQueue& operator,(const Tp &t) 
        { Append(SReference(t)); return *this; }

        //! Get the address of the first cons in the queue
    SExpressionCons *GetPtr() const
        { return static_cast<SExpressionCons*>(SReference::GetPtr()); }

private:
    SQueue(const SQueue &) {} // no copying!
    void operator=(const SQueue &) {} // no assigning!
};

#endif // sentry
