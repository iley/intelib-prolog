//   InteLib                                    http://www.intelib.org
//   The file sexpress/squeue.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
