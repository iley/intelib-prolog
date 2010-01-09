//   InteLib                                    http://www.intelib.org
//   The file genlisp/binds.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#ifndef INTELIB_BINDS_HPP_SENTRY
#define INTELIB_BINDS_HPP_SENTRY

#include "../sexpress/sexpress.hpp"

class IntelibBindingsSet {
    struct Binding {
        const SExpressionLabel *var;
        SReference val;
        Binding *next;
    };
    mutable Binding *first;

    Binding* Find(const SExpressionLabel *var) const;
public:
    IntelibBindingsSet();
    ~IntelibBindingsSet();

    SReference* AddBinding(const SExpressionLabel* var);
    SReference* GetBinding(const SExpressionLabel* var) const;


    class Iterator;
    friend class IntelibBindingsSet::Iterator;

        //! Iterator for the table
    class Iterator {
        Binding *cur;
    public:
            //! Constructor
        Iterator(const IntelibBindingsSet &master)
            : cur(master.first) {}
            //! Get the next item
        bool GetNext(const SExpressionLabel* &key, SReference &ret) {
            if(!cur) return false;
            key = cur->var;
            ret = cur->val;
            cur = cur->next;
            return true;
        }
    };

};

#endif
