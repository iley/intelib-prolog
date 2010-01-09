//   InteLib                                    http://www.intelib.org
//   The file genlisp/library/genlslib.hpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




/*!
  \file genlslib.hpp 
  \brief Macros for generic Lisp library functions declaration.
  
  The file contains macros which allow to use a single 
    information source  to generate 
      - header information about supported lisp library functions,
        that is, class definitions without any text constants
      - the C++ code (implementation)
*/ 


#if defined(INTELIB_GENLISP_LIBRARY_HEADER_GENERATION)

    ////////////////////////////////////////////////////////////
    // The section for DECLARATIONS (no string constants) 
    
    #undef DECLARE_CFUNCTION
    #undef DECLARE_SPECFORM
    #undef DECLARE_SYMBOL
    #undef DECLARE_LIBHEADER
    
    #define DECLARE_CFUNCTION(classname, min, max, textrepres)       \
      class classname : public SExpressionFunction {                 \
      public:                                                        \
          classname() : SExpressionFunction(min,max) {}              \
          SString TextRepresentation() const;                        \
          void DoApply(int paramsc,                                  \
                       const SReference *paramsv,                    \
                       IntelibContinuation &lf) const;               \
      };
          
    #define DECLARE_SPECFORM(classname, textrepres)        \
      class classname : public SExpressionForm {           \
      public:                                              \
          classname() : SExpressionForm() {}               \
          SString TextRepresentation() const;              \
          void Call(const SReference& params,              \
                    IntelibContinuation &lf) const;        \
      };

    #define DECLARE_LIBHEADER(name)
    
    // The section for DECLARATIONS (no string constants) ends here
    ////////////////////////////////////////////////////////////


#elif defined(INTELIB_GENLISP_LIBRARY_IMPLEMENTATION)

    ////////////////////////////////////////////////////////////
    // The section of implementation of text representations goes here
    
    #undef DECLARE_CFUNCTION
    #undef DECLARE_SPECFORM
    #undef DECLARE_LIBHEADER
    
    #define DECLARE_CFUNCTION(classname, min, max, textrepres)       \
      class classname : public SExpressionFunction {                 \
      public:                                                        \
          classname() : SExpressionFunction(min,max) {}              \
          virtual SString TextRepresentation() const;                \
          void DoApply(int paramsc,                                  \
                       const SReference *paramsv,                    \
                       IntelibContinuation &lf) const;               \
      };                                                             \
      SString classname :: TextRepresentation() const                \
        { return SString(textrepres); }
          
    #define DECLARE_SPECFORM(classname, textrepres)                  \
      class classname : public SExpressionForm {                     \
      public:                                                        \
          classname() : SExpressionForm() {}                         \
          SString TextRepresentation() const;                        \
          void Call(const SReference& params,                        \
                    IntelibContinuation &lf) const;                  \
      };                                                             \
      SString classname :: TextRepresentation() const                \
        { return SString(textrepres); }
    
    #define DECLARE_LIBHEADER(name)
    // The section ends here 
    ////////////////////////////////////////////////////////////


#else
#  error genlslib.hpp must be invoked in one of the modes
#endif /* main generation mode switch */
