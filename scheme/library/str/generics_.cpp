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

