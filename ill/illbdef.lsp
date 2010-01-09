;   InteLib                                    http://www.intelib.org
;   The file ill/illbdef.lsp
; 
;   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
; 
; 
;   This is free software, licensed under GNU GPL v.2
;   See the file COPYING for further details.
; 
;   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
;   Please see the file WARRANTY for the detailed explanation.





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
(DECLARE-NAME "/" "QUOTIENT")
(DECLARE-NAME "<" "LESSP")
(DECLARE-NAME ">" "GREATERP")
(DECLARE-NAME "^" "Not")
(DECLARE-NAME "&" "And")

                   ; This is for "PLUS" version (in contrast with "+"
                   ; which is defined by the library)
(DECLARE-FUNCTION "LFunctionPlus" "PLUS")

                   ; library defines "<" which becomes "lESSP"
                   ; we want LESSP as well 
(DECLARE-FUNCTION "LFunctionLessp" "LESSP")

;;                    ; library doesn't define "SORT" because there are
;;                    ; different implementation of it. We'd like to use
;;                    ; the "Sort-merge algorythm" one. 
;; (DECLARE-FUNCTION "LFunctionSortmerge" "SORT")

(DECLARE-SYMBOL NIL "NIL")
(DECLARE-SYMBOL T "T")
(DECLARE-SYMBOL QUOTE "QUOTE")
(DECLARE-SYMBOL FUNCTION "FUNCTION")
(DECLARE-SYMBOL LAMBDA "LAMBDA")

(ADD-LIB-HEADERS 
	"sexpress/sexpress.hpp"
	"sexpress/iexcept.hpp"
	"sexpress/sstring.hpp"
	"genlisp/lispform.hpp"
	"lisp/lisp.hpp" 
	"lisp/lsymbol.hpp" 
)
