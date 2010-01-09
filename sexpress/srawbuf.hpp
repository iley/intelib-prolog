//   InteLib                                    http://www.intelib.org
//   The file sexpress/srawbuf.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




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
