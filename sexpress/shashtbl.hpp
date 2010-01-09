//   InteLib                                    http://www.intelib.org
//   The file sexpress/shashtbl.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file shashtbl.hpp
    \brief Hash tables

    This file invents S-expressions which act as hash tables, with
    both the key and the value being S-expressions themselves.
    See the SExpressionHashTable class description.  In case you
    need something like a container, you might want to use the
    SHashTable class.

 */

#ifndef INTELIB_SHASHTBL_HPP_SENTRY
#define INTELIB_SHASHTBL_HPP_SENTRY

#include "sexpress.hpp"
#include "iexcept.hpp"
#include "gensref.hpp"

    //! The type the hash functions are to return
typedef unsigned long intelib_hash_t;

    //! Generic hash function (for an integer value)
intelib_hash_t UniversalHash(intelib_hash_t l);
    //! Hash function for an arbitry S-expression
intelib_hash_t LispHash(const SReference &ref);

    //! Type definition for equality predicate
typedef bool (*SEqualityPredicate) (SReference, SReference);
    //! Equality predicate 'EQ' (pointer comparision)
bool SExprsAreEq(SReference l1, SReference l2);
    //! Equality predicate 'EQL'
bool SExprsAreEql(SReference l1, SReference l2);
    //! Equality predicate 'EQUAL' (recursive)
bool SExprsAreEqual(SReference l1, SReference l2);


//! Hash table
/*! The class implements hash tables similar to those from
    Common Lisp
 */
class SExpressionHashTable : public SExpression {
    unsigned long dim;
    SReference *table; //!< Stores items of (key . value) 
    SEqualityPredicate comp;
    unsigned long itemcount;
    long lastfoundpos;
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! Empty slot mark
        /*! This object is returned as a search result when
            the given key is not found in the table
         */
    static SLabel EmptySlotMark; 

        //! The constructor
        /*! \param eqp is the equality predicate;
                   by default, EQUAL is used
         */
    SExpressionHashTable(SEqualityPredicate eqp = 0); 
protected:
    SExpressionHashTable(const IntelibTypeId &id, 
                         SEqualityPredicate eqp = 0); 
    ~SExpressionHashTable();
    
public:

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual class SString TextRepresentation() const;
#endif

    virtual SExpression* Clone() const;
    
      //! Add a new item
      /*! If there's already an item with the given key, it is replaced
       */
    void AddItem(SReference key, SReference val);

      //! Add a new item safely
      /*! If there was no such item, add one and returns true. 
          If an item with the same key already exists, do nothing 
          and return false.
       */
    bool SafeAddItem(SReference key, SReference val);

      //! Find item
      /*! Find an item with the given key. Return defval value 
          if none found, or else return the found value (key is 
          assumed to be the same)
       */ 
    SReference FindItem(SReference key, 
                const SReference &defval = EmptySlotMark) const; 

      //! Get the position where to store an item
      /*! Gets reference to the value position. If there's no such key,
          new entry is created. If the entry remains unbound, the 
          table removes it at the next call to GetItemPosition()
          \warning You must use the value returned by this method
          BEFORE another call to this method. The subsequent call
          may cause rehashing so that your reference become
          invalid.
       */
    SReference& GetItemPosition(SReference key);

      //! Remove item 
      /*! Removes the item. 
          Returns true if there was one, false othervise
       */
    bool RemoveItem(SReference key);

      //! Items count
      /*! Return total amount of items in the table */ 
    long Count() const { return itemcount; }

      //! Clear the table
      /*! Remove all existing items from the table */
    void Clear();

    //! Iterator to walk through the hash table
    class Iterator {
        SReference *tbl;
        int idx;
        int lim;
    public:
            //! The constructor
        Iterator(const SExpressionHashTable &tbl);
            //! Iterate
            /*! Returns a reference to the pair (cons)
                of the key and the value; returns unbound
                reference in case the table is over.
                \warning The behaviour might be undefined
                if you change the car (that is, the key)
             */
        SReference& GetNext();        
    };

    friend class Iterator::Iterator;
  
private:
    void DoRemoveItem(long pos);
    void ResizeTable();
    void HandleLastFoundPos() const;

};    

//! Hash table expected
class IntelibX_not_a_hash_table : public IntelibX {
public:
    IntelibX_not_a_hash_table(SReference a_param);
};

//! Reference to a SExpressionHashTable
typedef GenericSReference<SExpressionHashTable,IntelibX_not_a_hash_table>
SHashTableRef;

//! Smart pointer to a SExpressionHashTable
/*! This class is to be used as a 'container'. It differs form it's
    superclass in that is creates a table right on constructing, 
    and disallows assignments.
 */
class SHashTable : public SHashTableRef {
public:
    SHashTable(SEqualityPredicate pred = SExprsAreEqual)
        : SHashTableRef(new SExpressionHashTable(pred)) {}
};

//! Hash table too big
class IntelibX_hash_table_too_big : public IntelibX {
public:
    IntelibX_hash_table_too_big(SReference a_param);
};

#endif // sentry
