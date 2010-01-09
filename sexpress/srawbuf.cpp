//   InteLib                                    http://www.intelib.org
//   The file sexpress/srawbuf.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#include "srawbuf.hpp"
#if INTELIB_TEXT_REPRESENTATIONS == 1
#include "sstring.hpp"
#endif

IntelibTypeId SExpressionRawBuffer::TypeId(&SExpression::TypeId, true);

SExpressionRawBuffer::SExpressionRawBuffer(int size)
    : SExpression(TypeId)
{
    if(size > 0) {
        buf = new char[size];
        len = size;
    } else {
        buf = 0;
        len = 0;
    }
}

SExpressionRawBuffer::~SExpressionRawBuffer()
{
    if(buf) delete[] buf;
}


#if INTELIB_TEXT_REPRESENTATIONS == 1
SString SExpressionRawBuffer::TextRepresentation() const
{
    return "#<RAW-MEMORY-BUFFER>";
}
#endif

SExpression* SExpressionRawBuffer::Clone() const
{
    SExpressionRawBuffer *res = new SExpressionRawBuffer(len);
    for(int i=0; i<len; i++)
        res->buf[i] = buf[i];
    return res;
}

void SExpressionRawBuffer::Resize(int newsize)
{
    int cplen = (len > newsize) ? newsize : len;
    char *newbuf = new char[newsize];
    for(int i=0; i<cplen; i++)
        newbuf[i] = buf[i];
    delete[] buf;
    buf = newbuf;
    len = newsize;
}

int SExpressionRawBuffer::Append(void* nbuf, int buflen)
{
    int oldlen = len;
    Resize(len+buflen);
    for(int i=0; i<buflen; i++)
        buf[i+oldlen] = ((char*)nbuf)[i];
    return len;
}
