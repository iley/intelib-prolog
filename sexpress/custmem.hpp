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




/*!  \file custmem.hpp
     \brief InteLib custom memory management

     This file defines a template class which implements a kind of
     a custom memory management.  It is used to be notably faster for
     classes for which lots of objects (of the same size) are made.
 */

#ifndef INTELIB_CUSTMEM_HPP_SENTRY
#define INTELIB_CUSTMEM_HPP_SENTRY


#ifndef INTELIB_CUSTOM_MEMORY_MANAGEMENT
    //! Set to 1 to use the custom memory management, 0 to disable it
#define INTELIB_CUSTOM_MEMORY_MANAGEMENT 1
#endif

#if INTELIB_CUSTOM_MEMORY_MANAGEMENT == 1

#ifndef INTELIB_CMM_MAX_MEM_BLOCK
#define INTELIB_CMM_MAX_MEM_BLOCK (256*1024)
#endif



#include <stdlib.h>   // for size_t  // yes, this is ugly!

#ifndef INTELIB_CUSTOM_MEMORY_MANAGEMENT
#error smemory.hpp must not be included when there is no memory management
#endif 

//! an ugly generic implementation of custom memory management
template <int unit_size, int block_size>
class CustomMemoryManager {
    union memoryunit {
        char raw[unit_size];
        memoryunit *next;    // Used in free unit chains
    };
    struct blockhandler {
        int firstfreeunit;
        blockhandler *next;
        memoryunit block[block_size];
        blockhandler(blockhandler *nx = 0)
        { firstfreeunit = 0; next = nx; }
        void *getunit()
        { return (void*) ((firstfreeunit < block_size) ?
                              block + firstfreeunit++ : 0 ) ;
        }
    };
    blockhandler *firstblock;
    memoryunit *freechain;
public:
    //! Constructor
    CustomMemoryManager() {
        firstblock = new blockhandler;
        freechain = 0;
    }
    //! Destructor
    ~CustomMemoryManager() {
        while(firstblock) {
            blockhandler *tmp = firstblock;
            firstblock = firstblock->next;
            delete tmp;
        }
    }
    //! Memory allocation
    void *GetUnit(size_t s) {
        void *r;
        if(freechain) {   // If there's a freed unit, reuse it
            r = freechain;
            freechain = freechain->next;
        } else {          // otherwise, hope there's still some free space
            // in the block last allocated
            r = firstblock->getunit();
            if(!r) {        // alas, we need to allocate another one
                firstblock = new blockhandler(firstblock);
                r = firstblock->getunit();
                // this MUST succeed 'cause block is empty
            }
        }
        return r;
    }
    //! Memory disposal
    void ReleaseUnit(void *unit, size_t s) {
        // Just add it to the free units chain
        ((memoryunit*)unit) -> next = freechain;
        freechain = (memoryunit*)unit;
    }
};


//! Macro to easily make any class to be allocated on a custom manner
/*! Just call this macro within the class definition
    \note It leaves the protection mode 'public'. You might want to
          insert it right before the end of your class' definition.
    \note Be sure to use INTELIB_IMPLEMENT_CUSTOM_MEMORY_MANAGEMENT
          somewhere in your implementation (.cpp) file so that all
          the necessary methods get implemented.
 */
#define INTELIB_DECLARE_CUSTOM_MEMORY_MANAGEMENT(classname)         \
  private:                                                          \
  static class classname##MemoryManager TheMemoryManager;           \
  public:                                                           \
  void* operator new(size_t s);                                     \
  void operator delete(void *p, size_t s);

//! Implementation of a custom memory management
/*! This macro is to be inserted into the .cpp file along with the
    class definition for every class which is made to be allocated
    on the custom manner with the INTELIB_DECLARE_CUSTOM_MEMORY_MANAGEMENT
    macro
 */
#define INTELIB_IMPLEMENT_CUSTOM_MEMORY_MANAGEMENT(classname)       \
  class classname##MemoryManager :                                  \
     public CustomMemoryManager<sizeof (classname),                 \
           INTELIB_CMM_MAX_MEM_BLOCK / sizeof(classname)> {}        \
       classname::TheMemoryManager;                                 \
  void* classname::operator new(size_t s)                           \
     { return TheMemoryManager.GetUnit(s); }                        \
  void classname::operator delete(void *p, size_t s)                \
     { return TheMemoryManager.ReleaseUnit(p, s); }                 



#endif       // intelib_custom_memory_management
#endif       // sentry
