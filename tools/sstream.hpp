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




/*!
    \file sstream.hpp
    \brief InteLib stream support

    This file provides classes needed for InteLib i/o support.
    In the tools/ part they're only needed for the IntelibReader class,
    while in the lisp/ and scheme/ parts they are used by all the
    i/o Lisp/Scheme functions.
*/

#ifndef INTELIB_LSTREAM_HPP_SENTRY
#define INTELIB_LSTREAM_HPP_SENTRY

#include <stdio.h>
#include "../sexpress/refcount.hpp"
#include "../sexpress/iexcept.hpp"
#include "../sexpress/sstring.hpp"
#include "../sexpress/gensref.hpp"

//! Abstract (generic) stream
/*! This class just provides interface for streams, that is, declares 
    operations common for all existing streams.
 */
class SExpressionStream : public SExpression {
public:
        //! The type identifier
    static IntelibTypeId TypeId;

        //! The constructor
    SExpressionStream() : SExpression(TypeId) {}
protected:
    SExpressionStream(const IntelibTypeId &id) : SExpression(id) {}
    ~SExpressionStream() {}

public:
#if INTELIB_TEXT_REPRESENTATIONS == 1
    virtual SString TextRepresentation() const;
#endif

        //! read one char like fgetc(3)
    virtual int Getc() = 0;
        //! return a char to the input buffer just like ungetc(3) 
    virtual int Ungetc(int c) = 0;
        //! output a single char
    virtual int Putc(int c) = 0;
        //! output a string
    virtual int Puts(const char *s) = 0;
        //! read a string just like fgets(3)
    virtual char* Gets(char *buf, int bufsize) = 0;
        //! perform positioning operation (like lseek(fd, pos, SEEK_SET))
    virtual int Seek(long pos) = 0;
        //! return the current position
    virtual long Tell() const = 0;
        //! close the stream
    virtual int Close() = 0;
        //! perform flush operation if it is supported
    virtual int Flush() = 0;
        //! returns the corresponding file descriptor, or -1 if none
    virtual int Fileno() const = 0;
};

//! Exception: stream expected
class IntelibX_not_a_stream : public IntelibX {
public:
    IntelibX_not_a_stream(SReference a_param);
};

//! Smart pointer to an SExpressionStream
typedef GenericSReference<SExpressionStream,IntelibX_not_a_stream>
SStreamRef;


//! Stream which encapsulates a FILE* from stdio.h
class SExpressionStreamFile : public SExpressionStream {
    FILE *f;
public:
    static IntelibTypeId TypeId;

    SExpressionStreamFile(FILE *); 

protected:
    SExpressionStreamFile(const IntelibTypeId &id, FILE *); 
    virtual ~SExpressionStreamFile(); 
public:

    virtual int Getc();
    virtual int Ungetc(int c);
    virtual int Putc(int c);
    virtual int Puts(const char *s);
    virtual char* Gets(char *buf, int bufsize);
    virtual int Seek(long pos);
    virtual long Tell() const;
    virtual int Close();
    virtual int Flush();
    virtual int Fileno() const; 
    
    FILE* GetFile() const { return f; }
};

//! A reference which creates a stream by given file
class SStreamFile : public SStreamRef {
public:
    SStreamFile(FILE *f) : SStreamRef(new SExpressionStreamFile(f)) {}
};

//! A reference for standard input
class SStreamStdin : public SStreamFile {
public:
    SStreamStdin() : SStreamFile(stdin) {}
};

//! A reference for standard output
class SStreamStdout : public SStreamFile {
public:
    SStreamStdout() : SStreamFile(stdout) {}
};

//! A reference for standard error stream
class SStreamStderr : public SStreamFile {
public:
    SStreamStderr() : SStreamFile(stderr) {}
};

//! Stream formed as a queue of characters
class SExpressionStreamCharbuf : public SExpressionStream {
    SString buf;
    unsigned long pos;
    bool multipass;
public:
    SExpressionStreamCharbuf(const char *initbuf = NULL, bool mpass = false);
protected:
    ~SExpressionStreamCharbuf();
public:
    void AddChar(char);
    void AddString(const char*);
    void Cleanup(); /* removes the processed part of the buffer */
    virtual int Getc();
    virtual int Ungetc(int c);
    virtual int Putc(int c);
    virtual int Puts(const char *s);
    virtual char* Gets(char *buf, int bufsize);
    virtual int Seek(long pos);
    virtual long Tell() const;
    virtual int Close();
    virtual int Flush();
    virtual int Fileno() const;
};

//! Reference for a character-buffer stream
class SStreamCharbuf : public SStreamRef {
public:
    SStreamCharbuf(const char *str, int mpass = 1)
        : SStreamRef(new SExpressionStreamCharbuf(str, mpass)) {}
    SExpressionStreamCharbuf* operator->() const
        { return static_cast<SExpressionStreamCharbuf*>(GetPtr()); }
};


//! File-based stream that counts lines of input
class SExpressionStreamTextInput : public SExpressionStreamFile {
    int line;
public:
    static IntelibTypeId TypeId;

    SExpressionStreamTextInput(FILE *f) 
        : SExpressionStreamFile(TypeId, f), line(1) {}
    virtual int Getc();
    virtual int Ungetc(int c);
    virtual char* Gets(char *buf, int bufsize);

        //! Returns the current line counter value
    int TellLine() const { return line; } 
protected:
    ~SExpressionStreamTextInput() {}
};

//! Reference to a SExpressionStreamTextInput
class SStreamTextInput : public SStreamFile {
public:
    SStreamTextInput(FILE *f) : SStreamFile(f) {}
    SExpressionStreamTextInput* operator->() const
        { return static_cast<SExpressionStreamTextInput*>(GetPtr()); }
};


#endif
