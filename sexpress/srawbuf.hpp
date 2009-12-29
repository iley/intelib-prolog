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




/*! \file srawbuf.hpp
    \brief Raw memory store as an S-expression
*/
#ifndef INTELIB_SRAWBUF_HPP_SENTRY
#define INTELIB_SRAWBUF_HPP_SENTRY

#include "sexpress.hpp"

//! Raw memory buffer
/*! The class implements an S-expression which carries memory buffer */
class SExpressionRawBuffer : public SExpression {
    char *buf;
    int len;
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! The constructor
    SExpressionRawBuffer(int size); 

protected:
    ~SExpressionRawBuffer();
    
public:

#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual class SString TextRepresentation() const;
#endif
    virtual SExpression* Clone() const;

        //! Just get the buffer's location
    void* GetBuffer() const { return buf; }
        //! Get the size of the buffer
    int GetSize() const { return len; }
        //! Change the size of the buffer
        /*! The data is copied; if you shrink the buffer,
            the remaining data is lost
         */
    void Resize(int newsize);

        //! Append another buffer, extending the length accordingly
    int Append(void* buf, int buflen);
};    

#endif  // sentry
