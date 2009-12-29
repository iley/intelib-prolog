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
