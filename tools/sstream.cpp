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




#include "sstream.hpp"


/////////////////////////////////////////////////////
// SExpressionStream

IntelibTypeId
SExpressionStream::TypeId(&SExpression::TypeId, false);

SString SExpressionStream::TextRepresentation() const
{
    char buf[15];
    sprintf(buf, "#<STREAM %d>", Fileno());
    return SString(buf);
}



/////////////////////////////////////////////////////
// SExpressionStreamFile

IntelibTypeId
SExpressionStreamFile::TypeId(&SExpressionStream::TypeId, false);

SExpressionStreamFile::SExpressionStreamFile(FILE *af)
        : SExpressionStream(TypeId)
{
    f = af;
}
SExpressionStreamFile::SExpressionStreamFile(
    const IntelibTypeId& id, FILE *af)
        : SExpressionStream(id)
{
    f = af;
}
SExpressionStreamFile::~SExpressionStreamFile()
{
}
int SExpressionStreamFile::Getc()
{
    return fgetc(f);
}
int SExpressionStreamFile::Ungetc(int c)
{
    return ungetc(c, f);
}
int SExpressionStreamFile::Putc(int c)
{
    return fputc(c, f);
}
int SExpressionStreamFile::Puts(const char *s)
{
    return fputs(s, f);
}
char* SExpressionStreamFile::Gets(char *buf, int bufsize)
{
    return fgets(buf, bufsize, f);
}
int SExpressionStreamFile::Seek(long pos)
{
    return fseek(f, pos, SEEK_SET);
}
long SExpressionStreamFile::Tell() const
{
    return ftell(f);
}
int SExpressionStreamFile::Flush()
{
    return fflush(f);
}
int SExpressionStreamFile::Close()
{
    int r = f ? fclose(f) : -1;
    f = 0;
    return r;
}
int SExpressionStreamFile::Fileno() const
{
    return fileno(f);
}

/////////////////////////////////////////////////////
// SExpressionStreamCharbuf

SExpressionStreamCharbuf::
SExpressionStreamCharbuf(const char *initbuf, bool mpass)
{
    if(initbuf) buf = initbuf;
    multipass = mpass;
    pos = 0;
}

SExpressionStreamCharbuf::~SExpressionStreamCharbuf()
{}

void SExpressionStreamCharbuf::AddChar(char c)
{
    buf += c;
}

void SExpressionStreamCharbuf::AddString(const char *s)
{
    buf += s;
}

void SExpressionStreamCharbuf::Cleanup()
{
    //buf.erase(0, pos);
    buf = buf.c_str() + pos;
    pos = 0;
}

int SExpressionStreamCharbuf::Getc()
{
    if(buf.length() <= pos) {
        return EOF;
    } else {
        int ret = (int)(unsigned char)(buf[pos]);
        pos++;
        return ret;
    }
}

int SExpressionStreamCharbuf::Ungetc(int c)
{
    if(pos) {
        pos--;
    } else {
        char tmp[2]; tmp[0] = c; tmp[1] = 0;
        buf = SString(tmp) + buf.c_str();
    }
    return c;
}

int SExpressionStreamCharbuf::Putc(int c)
{
    char tmp[2]; tmp[0] = c; tmp[1] = 0;
    buf += tmp;
    return c;
}

int SExpressionStreamCharbuf::Puts(const char *s)
{
    buf += s;
    return 1;
}

char* SExpressionStreamCharbuf::Gets(char *outbuf, int outbuflen)
{
    unsigned int l = buf.length();
    if(l <= pos) return 0;
    if((signed)(l-pos) < outbuflen-1) outbuflen = l-pos+1;
    int i;
    for(i=0; i<outbuflen-1; i++) {
        outbuf[i] = buf[i+pos];
        if(outbuf[i] == '\n') {
            i++;
            break;
        }
    }
    pos += i;
    outbuf[i] = 0;
    return outbuf;
}

int SExpressionStreamCharbuf::Seek(long npos)
{
    if(multipass) {
        if(buf.length() < (unsigned long) npos) {
            return -1;
        } else {
            pos = npos;
            return 1;
        }
    } else
        return -1;
}

long SExpressionStreamCharbuf::Tell() const
{
    return pos;
}

int SExpressionStreamCharbuf::Close()
{
    buf = "";
    return 0;
}

int SExpressionStreamCharbuf::Flush()
{
    return 0;
}

int SExpressionStreamCharbuf::Fileno() const
{
    return -1;
}

/////////////////////////////////////////////////////
// SExpressionStreamTextInput

IntelibTypeId
SExpressionStreamTextInput::TypeId(&SExpressionStreamFile::TypeId, false);

int SExpressionStreamTextInput::Getc()
{
    int ret = SExpressionStreamFile::Getc();
    if(ret == '\n') line++;
    return ret;
}

int SExpressionStreamTextInput::Ungetc(int c)
{
    if(c == '\n') line--;
    return SExpressionStreamFile::Ungetc(c);
}

char* SExpressionStreamTextInput::Gets(char *buf, int bufsize)
{
    char *ret = SExpressionStreamFile::Gets(buf, bufsize);
    if(!ret) return 0;
    for(int i=0; i<bufsize && buf[i]; i++)
    if(buf[i]=='\n') { line++; break; }
    return ret;
}

/////////////////////////////////////////////////////
// exceptions

IntelibX_not_a_stream::
IntelibX_not_a_stream(SReference a_param)
        : IntelibX("Not a stream or wrong stream type", a_param) {}


