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
