//   InteLib                                    http://www.intelib.org
//   The file scheme/library/schemlib.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




#if defined(INTELIB_SCHEME_TRANSLATOR_INFORMATION)

    #define DECLARE_CFUNCTION(classname, min, max, textrepres, name) \
        (DECLARE-FUNCTION #classname name) 
          
    #define DECLARE_SPECFORM(classname, textrepres, name)  \
        (DECLARE-FORM #classname name) 

    #define DECLARE_GENFUNCTION(classname, nickname, name) \
              (DECLARE-FUNCTION #classname name)
    
    #define DECLARE_SYMBOL(symb, name) \
        (DECLARE-SYMBOL symb name)
    
    #define DECLARE_LIBHEADER(name) \
        (ADD-LIB-HEADERS name)


#elif defined(INTELIB_SCHEME_LIBRARY_HEADER_GENERATION)

    ////////////////////////////////////////////////////////////
    // The section for DECLARATIONS (no string constants) 
    
    #undef DECLARE_CFUNCTION
    #undef DECLARE_SPECFORM
    #undef DECLARE_GENFUNCTION
    #undef DECLARE_SYMBOL
    #undef DECLARE_LIBHEADER
    
    #define DECLARE_CFUNCTION(classname, min, max, textrepres, name) \
        class classname : public SExpressionFunction {               \
        public:                                                      \
            classname() : SExpressionFunction(min, max) {}           \
            SString TextRepresentation() const;                      \
            void DoApply(int paramsc,                                \
                         const SReference *paramsv,                  \
                         IntelibContinuation &cont) const;           \
        };
          
    #define DECLARE_SPECFORM(classname, textrepres, name)  \
        class classname : public SExpressionForm {         \
        public:                                            \
            SString TextRepresentation() const;            \
            void Call(const SReference& params,            \
                      IntelibContinuation &cont) const;    \
        };

    #define DECLARE_GENFUNCTION(classname, nickname, name) \
      typedef classname nickname;

    #define DECLARE_LIBHEADER(name)
    #define DECLARE_SYMBOL(symb, name)
    
    // The section for DECLARATIONS (no string constants) ends here
    ////////////////////////////////////////////////////////////


#elif defined(INTELIB_SCHEME_LIBRARY_IMPLEMENTATION)

    ////////////////////////////////////////////////////////////
    // The section of implementation of text representations goes here
    
    #undef DECLARE_CFUNCTION
    #undef DECLARE_SPECFORM
    #undef DECLARE_GENFUNCTION
    #undef DECLARE_SYMBOL
    #undef DECLARE_LIBHEADER
    
    #define DECLARE_CFUNCTION(classname, min, max, textrepres, name) \
        class classname : public SExpressionFunction {               \
        public:                                                      \
            classname() : SExpressionFunction(min, max) {}           \
            SString TextRepresentation() const;                      \
            void DoApply(int paramsc,                                \
                         const SReference *paramsv,                  \
                         IntelibContinuation &cont) const;           \
        };                                                           \
        SString classname :: TextRepresentation() const              \
            { return SString(textrepres); }
          
    #define DECLARE_SPECFORM(classname, textrepres, name)  \
        class classname : public SExpressionForm {         \
        public:                                            \
            SString TextRepresentation() const;            \
            void Call(const SReference& params,            \
                      IntelibContinuation &cont) const;    \
        };                                                 \
        SString classname :: TextRepresentation() const    \
            { return SString(textrepres); }
    
    #define DECLARE_GENFUNCTION(classname, nickname, name)
    #define DECLARE_LIBHEADER(name)
    #define DECLARE_SYMBOL(symb, name)
    // The section ends here 
    ////////////////////////////////////////////////////////////

#else
#  error schemlib.hpp must be invoked in one of the modes
#endif /* main generation mode switch */
