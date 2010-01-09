//   InteLib                                    http://www.intelib.org
//   The file sexpress/refcount.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file refcount.hpp
    \brief This file implements a reference-counting garbage collection.
    It consists of the following classes: 
     - GarbageSafe which is just an object which counts references on it; 
     - PGarbageSafe which behaves just like a pointer to GarbageSafe 
    except it notifies the referenced object whenever a reference is 
    established or disposed 
*/


#ifndef INTELIB_REFCOUNT_HPP_SENTRY
#define INTELIB_REFCOUNT_HPP_SENTRY

//! Generic object that needs to be garbage-collected
/*! Generic object that maintains a counter of references to it
    and automatically deletes itself when the counter becomes zero.
    \note The objects of this class are to be created 
          IN DYNAMIC MEMORY ONLY 
*/
class GarbageSafe {
    friend class GarbageSafePointer;
    friend class GarbageSafeConstPointer;
        //! Counter of existing references to the object
        /*! The counter is hidden from the user so it is no problem
            to modify it in constant objects. 
         */
    mutable int RefCount;
        //! Notification about a new established reference
    void AddRef() const { RefCount++; }
        //! Notification about disposion of a reference
    void Unlink() const { if((--RefCount)<=0) delete this; }
protected:
        //! Is it OK to delete the object now?
        /*! Our children have no access to the private attribute
            RefCount and should never need it except when checking
            if the destruction is not an error. This solves the problem.
         */ 
    bool CanDie() { return RefCount <= 1; }
public:
        //! Constructor
        /*! Constructor sets the counter to 1 assuming there will
            be at least one reference (otherwise we couldn't do any
            garbage collection anyway). If the object is allocated 
            statically, then the reference means reference by name 
            (so if everything works Ok, the counter never reaches zero).
            If the object is allocated dynamically, then we've got a
            pointer and must take care about it.
         */
    GarbageSafe() { RefCount = 0; }
protected:
        /*! Our destructor is virtual. Yes, it is. 
            And, BTW, it is intentionally made protected so that it is
            impossible to create such an object on stack or as a global.
         */
    virtual ~GarbageSafe() {}
private:
        /*! "=" made private to prevent a user from assigning these objects.
            It is senseless to assign this kind of objects...
         */
    void operator =(const GarbageSafe &other) {}
};

//! The smart pointer which performs reference-counting garbage collection
/*! The GarbageSafePointer is the class which encapsulates a pointer
    to an GarbageSafe object (constant or non-constant) and notifies 
    the object about creation and disposion of the reference.
      \note It is assumed that a plain pointer to an GarbageSafe object 
    can exist only in one situation - when the object is created with 
    operator new. 
      \warning User is strongly discouraged do declare pointers to 
    GarbageSafe objects. PGarbageSafe should be used instead. 
 */
class GarbageSafePointer {
    GarbageSafe *p;
public:
        //! Default constructor simply creates a NULL pointer
    GarbageSafePointer() { p = 0; }

        //! Construct from pointer
    GarbageSafePointer(const GarbageSafe *pp) 
    { if((p = (GarbageSafe*) pp)) p->AddRef(); }

        //! Simply a copy constructor
    GarbageSafePointer(const GarbageSafePointer &other)
    { if((p = other.p)) p->AddRef(); }

        //! Destructor
    ~GarbageSafePointer() { if(p) { p->Unlink(); p = 0; } }


        //! Assign a value from another GarbageSafePointer
        /*! We must increment the counter in this situation */
    GarbageSafePointer& operator =(const GarbageSafePointer &other)
    {
        GarbageSafe *tmp = p; // this is a workaround for the case
        if((p = other.p)) p->AddRef();  // when unlinking p we destroy
        if(tmp) tmp->Unlink(); // the structure our operand belongs to
        return *this;
    }

        //! Assign a value of a plain pointer
    GarbageSafePointer& operator =(GarbageSafe *pp)
    {
        GarbageSafe *tmp = p; 
        if((p = pp)) p->AddRef();
        if(tmp) tmp->Unlink(); 
        return *this;
    }

        //! Get the pointer to the referenced object
    GarbageSafe* GetPtr() const { return p; }
};


//! Pointer comparision operation
inline bool
operator==(const GarbageSafePointer& op1, const GarbageSafePointer& op2)
{ return op1.GetPtr() == op2.GetPtr(); }

//! Pointer comparision operation
inline bool
operator!=(const GarbageSafePointer& op1, const GarbageSafePointer& op2)
{ return op1.GetPtr() != op2.GetPtr(); }

//! Template for creating application-defined references to g/c classes
/*! This template implements some common methods to deal with
    garbage collecting pointers invented above.  
    \note T must be a descender of GarbageSafe class
 */
template <class Tp>
class GenericReference : public GarbageSafePointer {
public:
        //! Creates NULL reference
    GenericReference() {}
        //! Creates a reference from a plain pointer
    GenericReference(const Tp *tp) : GarbageSafePointer(tp) {}
        //! Copy constructor
    GenericReference(const GenericReference &other)
            : GarbageSafePointer(other) {}
        //! Destructor
    ~GenericReference() {}

        //! Assignment
    GenericReference& operator=(Tp *tp)
    {
        GarbageSafePointer::operator=(tp);
        return *this;
    }
        //! Assignment
    GenericReference& operator=(const GenericReference &other)
    {
        GarbageSafePointer::operator=(other);
        return *this;
    }
        //! Get a plain pointer
    Tp* GetPtr() const
        { return static_cast<Tp*>(GarbageSafePointer::GetPtr()); }

        //! Field/method selection
    Tp* operator ->() const
        { return GetPtr(); }

        //! Dereference
    Tp& operator *() const
        { return *(GetPtr()); }

};

#endif //sentry
