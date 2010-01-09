//   InteLib                                    http://www.intelib.org
//   The file lisp/library/rdr/generics_.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




DECLARE_GENFUNCTION(SFunctionClassicatomname, LFunctionClassicatomname,
                                                      "CLASSICATOMNAME")
DECLARE_GENFUNCTION(SFunctionClassicatomp, LFunctionClassicatomp,
                                                      "CLASSICATOMP")
DECLARE_GENFUNCTION(SFunctionMakeclassicatom, LFunctionMakeclassicatom,
                                                      "MAKECLASSICATOM")

DECLARE_GENFUNCTION(SFunctionImport, LFunctionImport, "IMPORT")
DECLARE_GENFUNCTION(SFunctionIntern, LFunctionIntern, "INTERN")
DECLARE_GENFUNCTION(SFunctionSymbolname, LFunctionSymbolname, "SYMBOLNAME")

DECLARE_GENFUNCTION(SFunctionPrin1, LFunctionPrin1, "PRIN1")
DECLARE_GENFUNCTION(SFunctionPrint, LFunctionPrint, "PRINT")

DECLARE_GENFUNCTION(SFunctionRead, LFunctionRead, "READ")
DECLARE_GENFUNCTION(SFunctionReadfromstring, LFunctionReadfromstring,
                                                      "READFROMSTRING")
DECLARE_GENFUNCTION(SFunctionSaferead, LFunctionSaferead, "SAFEREAD")
DECLARE_GENFUNCTION(SFunctionReaderror, LFunctionReaderror,"READERROR")
