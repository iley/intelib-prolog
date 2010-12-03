//   InteLib                                    http://www.intelib.org
//   The file tools/sreader.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file sreader.hpp
    \brief Intelib Reader

    The file invents a flexible syntax analyzer (the IntelibGenericReader
    class), which can be used for reading in different kinds of heterogenous
    data.  If you just want to read S-expressions, use the IntelibReader
    class; it is derived from IntelibGenericReader and tunes its ancestor
    to fit the traditional syntax of Lisp S-expressions.
 */
#ifndef INTELIB_SREADER_HPP_SENTRY
#define INTELIB_SREADER_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"
#include "../sexpress/squeue.hpp"
#include "sstream.hpp"

//! Abstract package 
/*! This is just the interface to a mechanism which allows to
    transform an unquoted string to an appropriate S-expression
    (e.g. label)
 */
class IntelibPackage {
public:
        //! The destructor, just to make it virtual
    virtual ~IntelibPackage() {}
        //! This function is THE thing for which the class is created
        /*! Must be overridden in the child class to provide the
            desired conversion method
         */
    virtual SReference MakeSymbol(const char *name) = 0;
};

//! Dummy package
/*! The dummy version just transforms the string into a
    SExpressionClassicAtom. 
    \note The objects produced on the same name are NOT equal in the sense
    of EQ, but are equal in the sense of EQL
 */
class IntelibDummyPackage : public IntelibPackage {
public:
    IntelibDummyPackage() {}
        //! Returns SExpressionClassicAtom with the given name
    virtual SReference MakeSymbol(const char *name);
};

extern IntelibDummyPackage TheDummyPackage;


//! Generic Reader
/*! This class is a base for implementing various readers for
    S-expression-based languages such as Lisp, Prolog, Refal, Planner etc.
    This reader is flexibly reprogrammable using Add*() functions; e.g.,
    to create a plain Lisp reader you've got to add the list, the
    character escapes, the string literal, the comment and may be
    some quoters, like this:

        AddQuotingToken("#\\", process_char_escape);
        AddStringLiteral("\"", '\"');
        AddSequenceOpener("(", process_plain_list, ")", ".", false);
        AddComment(";");
        AddQuoter("\'", process_lisp_quote);

    This is done (in parts) by constructors of IntelibReader, LispReader
    and SchemeReader classes.
 */
class IntelibGenericReader {
public:
        //! A function used to process some syntax constructs
    typedef SReference (*process_function)(const SReference&, void*);
private:
    class IntelibSLexAnalyser *lexer;
    struct SpecialLexic {
        SString id; //!< used both as a label and to store the string
        enum sl_type { quoter, sequence, seq_closer, cons_sign } t;
            /* for quoter, process is given the expression which follows
                 the quoter lexem 
               for sequence, process is given the list of members of the
                 sequence
               for seq_closer and cons_sign, process is ignored (should be 0)
             */
        process_function process;
        void *userdata;
        SReference the_closer; /*!< used for sequence only,
                                    stores the id of the closer */
        SReference the_cons_sign; /*!< used for sequence only,
                                    stores the id of cons sign, if any */
        SpecialLexic *next;
    } *first_sl;
    SpecialLexic* AddSpecialLexic(const char *str,
                                  SpecialLexic::sl_type t,
                                  process_function proc,
                                  void *userdata);

    IntelibPackage *the_package;
    bool uppercase;

    SQueue parts;
    SQueue ready;
    int unclosed_brackets;
    SString file_name;
public:
        //! The constructor
    IntelibGenericReader();
        //! The destructor
    ~IntelibGenericReader();

        //! lexem to be considered a delimiter in itself
    void AddDelimiter(const char *str, const SReference &tok);

        //! lexem that isn't a delimiter but nevertheless special
    void AddToken(const char *str, const SReference &tok);

        //! lexem to be read until a delimiter, such as : or &
    void AddTokenType(const char *str,
                      SReference (*fun)(const char*, void*),
                      void *userdata = 0);

        //! read until a delimiter, force the first char to be read
    void AddQuotingToken(const char *str,
                         SReference (*fun)(const char*, void*),
                         void *userdata = 0);

        //! lexem to be read until a delimiter, such as : or &
    void AddStringLiteral(const char *str, int closing_char,
                          SReference (*fun)(const char*, void*) = 0,
                          void *userdata = 0);

        //! lexem that modifies the following expression, such as ', #', `
    void AddQuoter(const char *str, process_function proc, void *userdata = 0);

        //! lexem which opens a sequence, such as (, #(, #S(
    void AddSequenceOpener(const char *str,
                           process_function proc,
                           void *userdata,
                           const char *closer,
                           const char *cons_sign = 0,
                           bool cons_sign_delimiter = false);

        //! lexem for separating a cons, such as . in lisp, or | in prolog
    void AddTailSeparator(const char *str, bool is_delimiter = false);

        //! comment
        /*! by default, it is a line-based comment; just specify your
            desired comment closing token to create a block comment
         */
    void AddComment(const char *starter, const char *closer = "\n");

        //! install a package
        /*! \warning The package object is not deleted by
            the destructor, so it is the caller's duty to
            take care about it
         */
    IntelibPackage* SetPackage(IntelibPackage *a_package)
    {
        IntelibPackage *old = the_package;
        the_package = a_package;
        return old;
    } 

        //! Should the identifiers be converted into the upper case
    void SetUppercase(bool u) { uppercase = u; }

        //! Set the fule name and line number information
        /*! \param l is the line number
            \param afn is the file name, none by default
         */
    void SetLine(int l, const SString &afn = "");
        //! Get the current line number
    int GetLine() const;
        //! Get the current file name
    const SString& GetFileName() const { return file_name; }

        //! Pass another character to the reader
    void FeedChar(int c);
        //! Pass a string to the reader, char by char
    void FeedString(const char *s);
        //! Pass the End Of File condition to the reader
    void FeedEof() { FeedChar(-1); }

        //! Is the buffer totally empty?
        /*! Returns true if there are neither complete expressions
            nor incomplete lexem chains stored within the reader.
            Generally speaking, this can only be true if only whitespace
            and comments were read so far (or since the last sequence of
            Get() operations which emptied the buffer, or since the last
            Drop() operation).
         */
    bool IsEmpty() const { return parts.IsEmpty() && ready.IsEmpty(); }
        //! Is there an expression to get?
    bool IsReady() const { return !ready.IsEmpty(); }

        //! Get the next expression constructed by the reader
        /*! If there's an expression (that is, if IsReady() method
            returns true), Get() returns the first expression and
            removes it from the internal buffer.  The behaviour is
            undefined if you call Get() when IsReady() returns false.
         */
    SReference Get();
        //! Drop all the stored information
        /*! Both complete expressions and incomplete lexem sequences
            are dropped, the lexer is reset.  This can be useful, for
            example, if the user decides to cancel the interactive input
         */
    void Drop();

        //! Read one expression from the given stream
        /*! Feeds the reader with chars read from the stream until
            IsReady() returns true, then returns the expression.
            \warning This can actually take more chars from the stream
            than it is necessary to form a complete expression, and thus
            leave the reader object in an unempty state.  For example,
            if the stream contains smth. like ``abc('', then a token will
            be built off the 'abc', but the char '(' will be read as well,
            transformed into the lexem and stored in ther incomplete
            expression queue.
         */
    SReference Read(const SStreamRef& stream);

        //! The end of file marker
        /*! The object returned as an expression when the end of file
            is reached  
         */
    static SLabel& EofMarker;

private:
    void HandleLexema(const SReference &lexpair); // lexpair is (lex . line)
    void HandleSpecialLex(SpecialLexic *sp, int line);
    SReference DoMakeSymbol(const char *s) const;
    void HandleEof();
    void FinishParts();
    SReference DoFinishParts();
    SReference DoFinishPartsSeq(const SReference& closer,
                                const SReference& conser,
                                int begline);
};

//! The reader preprogramed for Lisp S-expressions
/*! This object differs from IntelibGenericReader only in the
    constructor, which sets up the reader to recognize the
    parentheses, the dot, the double-quotes, the character escapes
    and the comment.
    \warning It doesn't set up any quoters such as ', #', ,@ etc;
    use the appropriate child objects instead if you need the quoters:
    LispReader introduces ', #', #& etc; SchemeReader doesn't introduce
    #', but introduces #T and #F. 
 */
class IntelibReader : public IntelibGenericReader {
public:
    IntelibReader();
};


//! Exception class for reader errors capable of keeping line numbers
class IntelibX_reader_error : public IntelibX {
    int line, beg_line;
    SString fname;
public:
        //! The constructor
        /*! \param a_param is an expression,
                   typically just a diagnostic message
            \param a_line is the line number (or -1 if not available)
            \param a_begline is the number of the line where the
                   expression has started (e.g., where the left
                   parenthesis was found, in case smth. wrong with
                   the list
            \param afn is the name of the file, or an empty string if
                   the name is not available
         */
    IntelibX_reader_error(SReference a_param,
                          int a_line,
                          int a_begline = -1,
                          const SString &afn = "");
    int Line() const { return line; }
    int ExpBegLine() const { return beg_line; }
    const SString& FileName() const { return fname; }
};

//! Exception "Reader Not Ready"
/*! This exception is thrown by IntelibGenericReader in case of attempt to 
    get the resulting (read) expression when it's not available
 */
class IntelibX_reader_not_ready : public IntelibX {
public:
    IntelibX_reader_not_ready();
};

//! Exception 'Unexpected Eof'
/*! It's never thrown by the reader itself, but the lisp function READ 
    throws it when no EOF value is specified
 */ 
class IntelibX_read_eof : public IntelibX {
public:
    IntelibX_read_eof(SReference a_param);
};


//! The global IntelibGenericReader pointer
/*! The library functions READ, SAFEREAD and READFROMSTRING
    use this pointer to determine whether to use a custom
    (possibly fine-tuned) IntelibGenericReader. If the pointer remains
    NULL (0), the functions create an object locally each time
    they're called. The library doesn't assign this pointer to
    any object, so it remains zero unless you explicitly change it. 
    \note If you decide to change the pointer, it might be not a bad 
    idea to save its value and restore it after you finish with your
    operation.
 */
extern IntelibGenericReader *PTheIntelibReader;


    //! Tune a reader to recognize vectors
    /*! Make the given reader object recognize constructs
        like #(1 2 3) or #~(1 2 3) and return the appropriate
        vectors, respectively, of fixed or variable size
     */
void add_vectors_to_reader(class IntelibReader &reader);

    //! Tune a reader to recognize hash tables
    /*! Make the given reader object recognize constructs
        like #S(EQL (KEY . VAL) ...) and return the appropriately
        constructed object of the SExpressionHashTable class
     */
void add_hashtables_to_reader(class IntelibReader &reader);

#endif // sentry
