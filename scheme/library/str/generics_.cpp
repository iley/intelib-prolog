//   InteLib                                    http://www.intelib.org
//   The file scheme/library/str/generics_.cpp
// 
//   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
// 
// 
//   This is free software, licensed under GNU LGPL v.2.1
//   See the file COPYING for further details.
// 
//   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
//   Please see the file WARRANTY for the detailed explanation.




DECLARE_GENFUNCTION(SFunctionCharacterp, SchFunctionCharp, "CHAR?")
DECLARE_GENFUNCTION(SFunctionList2string,SchFunctionList2string,"LIST->STRING")
DECLARE_GENFUNCTION(SFunctionString2c, SchFunctionString2c, "STRING2C")
DECLARE_GENFUNCTION(SFunctionChar2c, SchFunctionChar2c, "CHAR2C")
DECLARE_GENFUNCTION(SFunctionString2list,SchFunctionString2list,"STRING->LIST")
DECLARE_GENFUNCTION(SFunctionString2num, SchFunctionString2num, "STRING2NUM")
DECLARE_GENFUNCTION(SFunctionStringlength, SchFunctionStringlength,
                                      "STRING-LENGTH")
DECLARE_GENFUNCTION(SFunctionStringp, SchFunctionStringp, "STRING?") 
DECLARE_GENFUNCTION(SFunctionSubstring, SchFunctionSubstring, "SUBSTRING")
DECLARE_GENFUNCTION(SFunctionToupper, SchFunctionToupper, "TOUPPER")
DECLARE_GENFUNCTION(SFunctionTolower, SchFunctionTolower, "TOLOWER")


// commented out; see the explanation of STRING-REF in r5rs
// DECLARE_GENFUNCTION(SFunctionChar, SchFunctionChar, "CHAR")

