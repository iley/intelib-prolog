;   InteLib                                    http://www.intelib.org
;   The file ils/ilsbdef.scm
; 
;   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
; 
; 
;   This is free software, licensed under GNU GPL v.2
;   See the file COPYING for further details.
; 
;   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
;   Please see the file WARRANTY for the detailed explanation.




; ilsbdef.scm -- BASIC definitions for ils

(DECLARE-NAME "NULL" "lNULL")      ; to avoid conflicts with system macro
(DECLARE-NAME "SYMBOL" "lSYMBOL")  ; some systems have SYMBOL
(DECLARE-NAME "L" "lL")            ; to avoid conflicts with
(DECLARE-NAME "F" "lF")            ; L{List,Function}Constructor instances
(DECLARE-NAME "=" "Mathequal")
(DECLARE-NAME "/=" "Mathnotequal")
(DECLARE-NAME ">=" "Mathgeq")
(DECLARE-NAME "<=" "Mathleq")
(DECLARE-NAME "PLUS" "pLUS")
(DECLARE-NAME "LESSP" "lESSP")
(DECLARE-NAME "+" "PLUS")   ; we want both "+" and "PLUS" to work
(DECLARE-NAME "-" "DIFFERENCE")
(DECLARE-NAME "*" "TIMES")
(DECLARE-NAME "/" "DIVIDE")
(DECLARE-NAME "<" "LESSP")
(DECLARE-NAME ">" "GREATERP")
(DECLARE-NAME "^" "Not")
(DECLARE-NAME "&" "And")

                   ; This is for "PLUS" version (in contrast with "+"
                   ; which is defined by the library)
(DECLARE-FUNCTION "SchFunctionPlus" "PLUS")

                   ; library defines "<" which becomes "lESSP"
                   ; we want LESSP as well 
(DECLARE-FUNCTION "SchFunctionLessp" "LESSP")

(DECLARE-SYMBOL QUOTE "QUOTE")

(ADD-CORE-LIB-HEADERS 
	"sexpress/sexpress.hpp"
	"sexpress/sstring.hpp"
	"scheme/scheme.hpp" 
	"scheme/schsymb.hpp" 
)

(ADD-LIB-HEADERS 
	"sexpress/iexcept.hpp"
	"scheme/schfun.hpp"
	"scheme/schcont.hpp"
)
