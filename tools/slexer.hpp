//   InteLib                                    http://www.intelib.org
//   The file tools/slexer.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2010
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*! \file slexer.hpp
    \brief The InteLib lexical analyser

    See the IntelibSLexAnalyser class description.
 */

#ifndef INTELIB_SLEXER_HPP_SENTRY
#define INTELIB_SLEXER_HPP_SENTRY

#include "../sexpress/sexpress.hpp"
#include "../sexpress/sstring.hpp"

extern SLabel LexemaEof;

    /* this paranoia check should in fact never be positive, but... */
#if defined(EOF) && EOF != -1
#error This version assumes EOF is -1 and that is not true for your situation
    /* in case it is, edit slexer.cpp, remove the definition of EOF and
       add ``#include <stdio.h>'' instead... and, well, be sure to 
       report this :) */
#endif


//! Lexical analyzer of S-expressions
/*! The lexer accepts chars one by one until a whole lexem is ready.
    The lexer is relatively flexible; it allows to add different
    keywords, both delimiting or non-delimiting, string literals
    with any symbol used as quoter, tokens such as #\\Newline or
    the like, and any kind of comments.
    The lexer is also capable of counting lines on its input.
 */
class IntelibSLexAnalyser {

    struct SpecChar {
        char ch;
        enum TokenKind {
            non_term, term, non_delim, read_rest, read_rest_spec,
            read_string, ignore_until
        } status : 8;
        SpecChar *sub;     //!< List of possible chars to be the next
        SpecChar *next;    //!< Next possible char on the same level
        char closer[4];    //!< for read_string and ignore_until
                           //!< NB: read_string uses only one char
        SReference token;  //!< For term, what to return
        SReference (*makestring)(const char *str, void *userdata);
                           //!< For read_string, read_rest and read_rest_spec 
                           //!< what to make or leave it NULL if you want
                           //!< just a string
        void *userdata;    //!< Passed to makestring, if any

        SpecChar() : ch(0), status(non_term), sub(0), next(0),
                     token(), makestring(0), userdata(0)
            { closer[0] = 0; }
    } *specchars, **special_state;

    SString delimiter_chars;

    enum State { 
        home,        // the home state
        string,      // reading a string constant until a closer
        stringq,     // within a string, reading escaped char
        token,       // reading a token (either a number or a symbol)
        token_force, // force reading one char, then go on as for a token
        special,     // special lexic element
        special2,    // check the next char, there can be longer delimiter
        special3,    // check the next char, make the non-delim token if
                     //    and only if the next char is a space or
                     //    delimiter
        comment      // ignore everything until the closer is found
    } state;
    const char *closer;    //!< for string and comment states
    SReference (*string_finalizer)(const char*, void*);
                           //!< for string and (special) token state
    void *string_finalizer_userdata;
    int comment_closer_index; //!< for comment state only
    int postponed_char;
    SString buf;

    int line, lexline;
    SReference lex;
    const char *error_message;
public:
        //! The constructor
    IntelibSLexAnalyser();
        //! The destructor
    ~IntelibSLexAnalyser();

        //! Add a delimiter
    bool AddDelimiter(const char *prefix, const SReference &token);
        //! Add a keyword
    bool AddNonDelimiter(const char *prefix, const SReference &token);
        //! Add a sequence which starts a token
        /*! Token is being read until whitespace or a delimiter */
    bool AddTokenStarter(const char *prefix,
                         SReference (*fun)(const char*, void*) = 0,
                         void *userdata = 0);
        //! Add a sequence which starts a token of at least one char
        /*! Token is being read until whitespace or a delimiter,
            and the first char of it is NOT considered neither
            whitespace nor delimiter (such as for #\ in Lisp)
         */
    bool AddQuotingToken(const char *prefix,
                         SReference (*fun)(const char *, void*) = 0,
                         void *userdata = 0);
        //! Add a string literal
    bool AddStringStarter(const char *prefix, int closer_char,
                          SReference (*fun)(const char *, void*) = 0,
                          void *userdata = 0);
        //! Add a comment
    bool AddCommentStarter(const char *prefix, const char *closer = "\n");

        //! The possible results of feeding another character
    enum FeedResult { 
        res_empty,    //!< Nothing but whitespace or comments yet
        res_ready,    //!< Lexem is ready
        res_continue, //!< Keep feeding chars, lexem is being built
        res_eof,      //!< End of file reached
        res_error     //!< Lexical error encountered
    };

        //! Give the lexer another character
        /*! See the FeedResult enum explanation for possible results */
    FeedResult FeedChar(int c);

        //! Get the lexem when it is ready (FeedChar returned res_ready)
    SReference Get() const;

        //! Get the current line number
    int GetLine() const { return line; }
        //! Set the current line number
    void SetLine(int l) { line = l; }
        //! Get the error message in case FeedChar returns res_error
    const char *GetErrorMessage() const { return error_message; }

        //! Drop and clear all internal data
    void Drop();

private:
    SpecChar *AddSpecial(const char *prefix, SpecChar::TokenKind tk);
    SpecChar *DoAddSpecial(SpecChar **p, const char *str,
                                             SpecChar::TokenKind tk);

    FeedResult Home(int c);
    FeedResult String(int c);
    FeedResult Stringq(int c);
    FeedResult Token(int c);
    FeedResult Special(int c);
    FeedResult Special3(int c);
    FeedResult Comment(int c);

    SReference ProcessToken(const char *s);

    bool IsDelimiterChar(int c);
};

#endif
