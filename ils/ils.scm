;   InteLib                                    http://www.intelib.org
;   The file ils/ils.scm
; 
;   Copyright (c) Andrey Vikt. Stolyarov, 2000-2009
; 
; 
;   This is free software, licensed under GNU GPL v.2
;   See the file COPYING for further details.
; 
;   THERE IS NO WARRANTY OF ANY KIND, EXPRESSED, IMPLIED OR WHATEVER!
;   Please see the file WARRANTY for the detailed explanation.




(%%%
    ; Module name
    (MODULE-NAME "ils")
    (INIT-FUNCTION-NAME "TheSchemeProgram")
    ; Interface to the module
    (DECLARE-PUBLIC ils_main)
    (DECLARE-PUBLIC cmdlinedirectives)
)


(define chartrantable (make-eqv-hashtable)) ; Character translation rules

(define symbtable (make-equal-hashtable)) ; Symbols
       ; name is that in Scheme, translated to uppercase 
       ; (name . 'cppname)   -> <name in c++>
       ; (name . 'package)   -> 'pkg_intelib, 'pkg_scheme, 'pkg_user
       ; (name . 'funclass)  -> name of the function class, if any
       ; (name . 'external)  -> #t/#f, #t means it is already defined 
       ; (name . 'public)    -> #t/#f, #t means it must be public
       ; (name . 'used)      -> #t/#f, #t means it is in use
       ;                        (this might be #f if there was a directive
       ;                        for it, but it never appeared in the text)


(define theprogram ())     ; Program text to be translated
(define othermodules ())   ; header files of other modules to be included 
(define libheaders ())     ; header files of InteLib to be included
                           ; into the implementation part
(define corelibheaders ()) ; header files of InteLib to be included
                           ; into the generated header 


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;     global parameters
; default values
(define definitfunctionname "SchemeInit_") 
(define defschemepackagename "PackageScheme_") 
(define defuserpackagename "PackageUser_") 
(define defcxxfilepath ".cxx") 
(define defhxxfilepath ".hxx")
(define defhxxfilename ".hxx")
(define defdebuggerfilename "_debugger.cxx")
(define defmodulename "schemepart")
;
; variables for actual values
(define initfunctionname #f) ; name of the initialization function
(define schemepackagename #f) ; name of the object of package
(define userpackagename #f) ; name of the object of package
(define cxxfilepath #f) ; path of CXX file 
(define hxxfilepath #f) ; path of HXX file
(define hxxfilename #f) ; name of HXX file for the #include directive
(define debuggerfilename #f) ; name for debugger module
(define modulename #f) ; Module Id 

(define rightmargin 75) ; Max number of symbols per line
(define identstep 4) ; How far an inner construct is indented to the right

(define atomquote (car ''(1)))
(define directivesatom (makeclassicatom "%%%"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (symtabset name category value)
    (setf! (gethash (cons (toupper name) category) symbtable) value)
)

(define (symtabget name category)
    (gethash
        (cons (toupper name) category)
        symbtable
        (symtabdefault (toupper name) category)
    )
)

(define (symtabdefault name category)
    (case category
        ((cppname) (chartran name))
        ((package) 'pkg_user)
        ((funclass) #f)
        ((used public external) #f)
        (else (error 'err_bug))
    )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; getting/analysing sources
;;

(define (chartran name) ; character translation of a symbol name
    (list->string (chartran_d (chartran_l (string->list name))))
)
(define (chartran_l name)
    (cond
        ((null? name) ())
        (#t
            (let ((chr (gethash (car name) chartrantable ())))
                (if (null? chr) 
                    (cons (car name) (chartran_l (cdr name)))
                    (append (string->list chr) (chartran_l (cdr name)))
                )
            )
        )
    )
)
(define (chartran_d name)
  (cond ((member? (car name) '(#\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9))
           (cons #\d name)
        )
        (#t name)
  )
)

(define (interpretedirective directive infile infilename)
    ;(write directive) (newline)
    (case (classicatomname (first directive)) 
        (("DECLARE-CHAR")
            ; declares character translation, e.g. (D-C #\@ "at")
            ; NOTE character declarations should go _before_ 
            ; everything else to make the chartran function work
            ;(display (list (second directive) (third directive))) (newline)
            (setf!
                 (gethash (second directive) chartrantable) 
                 (third directive)
            )
        )
        (("DECLARE-FUNCTION" "DECLARE-FORM")
            ; declares function available with the library
            ; (D-F class scheme-name) e.g. (D-F "SchFunctionIf" "IF") 
            (let ((d3 (third directive)))
                (if (string? d3) (begin
                    (symtabset d3 'package 'pkg_scheme) 
                    (symtabset d3 'funclass (second directive)) 
                ))
            )
        )
        (("DECLARE-SYMBOL")
            ; declares symbol available from the library
            ; (D-S <C++-obj> <schemename>), e.g. (D-S QUOTE "QUOTE")
            ; normally, Scheme part provides QUOTE only
            (let ((name (third directive)))
                (symtabset name 'cppname (classicatomname (second directive))) 
                (symtabset name 'package 'pkg_intelib) 
                (symtabset name 'external #t) 
                ;(symtabset name 'public #t) 
                ;(symtabset name 'used #t) 
            )
        )
        (("DECLARE-NAME")
            ; declares name translation, e.g. (D-N "NULL" "lNULL") 
            ;         or (D-N null? "lNULL")
            (let 
                ((name
                    (if (classicatom? (second directive))
                        (classicatomname (second directive))
                        (second directive)
                    )
                ))
                (symtabset name 'cppname (third directive)) 
            )
        )
        (("DECLARE-PUBLIC")
            ; declares a symbol which must be made public 
            ; (that is, available for other modules 
            ; including the main program)
            ; (D-P MY-SYMBOL)
            (let ((name (classicatomname (second directive))))
                (symtabset name 'public #t) 
                (symtabset name 'used #t) 
            )
        )
        (("DECLARE-EXTERNAL")
            ; declares a symbol which is public in another module 
            ; so there's no need to define it
            ; (D-E MY-SYMBOL)
            (let ((name (classicatomname (second directive))))
                (symtabset name 'external #t) 
            )
        )
        (("MODULE-NAME")
            ; sets the name of the module 
            ; (M-N "mymodule")
            (if (not modulename)
                (set! modulename (second directive))   
            )
        )
        (("CXX-FILE-PATH")
            ; sets the .cxx file to generate
            ; (C-F-N "myfile.cxx")
            (if (not cxxfilepath)
                (set! cxxfilepath (second directive))   
            )
        )
        (("HXX-FILE-PATH")
            ; sets the .hxx file to generate
            ; (H-F-N "myfile.hxx")
            (if (not hxxfilepath)
                (set! hxxfilepath (second directive))   
            )       
        )
        (("HXX-FILE-NAME")
            ; sets the name of the .hxx file as it appears in the #include
            ; (H-F-N "myfile.hxx")
            (if (not hxxfilename)
                (set! hxxfilename (second directive))   
            )       
        )
        (("DEBUGGER-FILE")
            ; sets the name of the .hxx file as it appears in the #include
            ; (H-F-N "myfile.hxx")
            (set! debuggerfilename
                (if (null? (second directive)) #t (second directive))       
            )
        )
        (("SCHEME-PACKAGE-NAME")
            ; sets the name of the package which contains 
            ;   symbols bound to Scheme library functions
            ; (S-P-N "MySchemePackage")
            (if (not schemepackagename)
                (set! schemepackagename (second directive))   
            )
        )
        (("USER-PACKAGE-NAME")
            ; sets the name of the package which contains 
            ;   symbols invented by the user 
            ; (L-P-N "MyUserPackage")
            (if (not userpackagename)
                (set! userpackagename (second directive))   
            )
        )
        (("INIT-FUNCTION-NAME")
            ; sets the name of the initialization function name
            ; (I-F-N "mymodule")
            (if (not initfunctionname)
                (set! initfunctionname (second directive))   
            )
        )
        (("DECLARE-USED-MODULE")
            ; makes the compiler to insert an #include directive
            ; at the beginning of the .cxx file
            ; (D-U-M "othermdl.hxx")
            (set! othermodules 
                 (append othermodules (list (second directive))))
        )
        (("ADD-CORE-LIB-HEADERS")
            ; makes the compiler to insert an #include directive(s)
            ; at the beginning of the .hxx file
            ; the directives are transformed into the "angle" form
            ;    e.g., "sexpress/sexpress.hpp" becomes
            ;    <intelib/sexpress/sexpress.hpp> 
            ; (A-C-L-H "header1.hpp" "header2.hpp" ...)
            (set! corelibheaders
                 (append corelibheaders (cdr directive)))
        )
        (("ADD-LIB-HEADERS")
            ; makes the compiler to insert an #include directive(s)
            ; at the beginning of the .cxx file
            ; _before_ those required by DECLARE-USED-MODULE directive
            ; the directives are transformed into the "angle" form
            ;    e.g., "sexpress/sexpress.hpp" becomes
            ;    <intelib/sexpress/sexpress.hpp> 
            ; (A-L-H "header1.hpp" "header2.hpp" ...)
            (set! libheaders
                 (append libheaders (cdr directive)))
        )
        (else
            (error
                'err_unknowndirective "unknown directive" dirname
                infilename
                (if infile (getcurrentline infile) -1)
            )
        )
    )
)

(define (interpretedirectives directives infile infilename)
    (cond
        ((null? directives) ())
        ((or (atom? directives) (atom? (car directives)))
            (error 
                'err_atomdirective "strange directive" directives
                infilename 
                (if infile (getcurrentline infile) -1)
            )
        )
        (#t 
            (interpretedirective (car directives) infile infilename) 
            (interpretedirectives (cdr directives) infile infilename)
        )
    )
)

(define (processfile infile filename)
    (let ((form (saferead infile '%ERROR '%EOF ())))
        (cond
            ((eq? form '%ERROR)       
                (error 'err_readfile "reading file" filename (readerror))
            )
            ((eq? form '%EOF)
                ()
            )
            ((atom? form)
                (cons form (processfile infile filename))
            ) 
            ((eqv? (car form) directivesatom)
                (interpretedirectives (cdr form) infile filename)
                (processfile infile filename)
            )
            (#t
                ; leave the form as is and continue
                (cons form (processfile infile filename))
            )
        )
    )
)

(define (readsrcfile name)
    (let ((infile (lfopen name))) 
             ; "r" is not specified here to make line num info available
        (cond
            ((null? infile)
                 (error 'err_nofile "Couldn't open file" name)
            )
            (#t
                 (set!
                     theprogram
                     (append theprogram (processfile infile name))
                 )
                 (lfclose infile)
            )
        )
    )
) 

(define (getsources srcfiles) (map readsrcfile srcfiles))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; register all symbols that aren't registered yet.
(define (parsesources text)
    (cond
        ((null? text) ())
        ((atom? text) 
            (if (classicatom? text)
                (let ((theatomname (classicatomname text)))
                    (symtabset theatomname 'used #t) 
                           ; it definitely IS used
                    theatomname
                ) 
                text
            )
        )
        (#t
            (cons
                (parsesources (car text)) 
                (parsesources (cdr text))
            )
        ) 
    )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (makeglobalnames )
    (if (not modulename)
        (set! modulename defmodulename)
    )
    (if (not initfunctionname)
        (set! initfunctionname (plus definitfunctionname modulename))
    ) 
    (if (not schemepackagename)
        (set! schemepackagename (plus defschemepackagename modulename))
    ) 
    (if (not userpackagename)
        (set! userpackagename (plus defuserpackagename modulename))
    ) 
    (if (not cxxfilepath)
        (set! cxxfilepath (plus modulename defcxxfilepath))
    )
    (if (not hxxfilename)
        (set! hxxfilename (plus modulename defhxxfilename))
        ; else, that is, hxxfilename explicitly defined
        (if (not hxxfilepath)
            (set! hxxfilepath hxxfilename)
        )
    )
    (if (not hxxfilepath)
        (set! hxxfilepath (plus modulename defhxxfilepath))
    )
    (if (eq? debuggerfilename #t)
        (set! debuggerfilename (plus modulename defdebuggerfilename))
    )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (stringsort_split splitval rest lower upper)
    (cond
        ((null? rest)
            (append (stringsort lower) (cons splitval (stringsort upper)))
        )
        ((< (car rest) splitval)
            (stringsort_split splitval (cdr rest)
                (cons (car rest) lower) upper)
        )
        (#t
            (stringsort_split splitval (cdr rest)
                lower (cons (car rest) upper))
        )
    )
)

(define (stringsort lst)
    (cond
        ((null? lst) ())
        ((null? (cdr lst)) lst)
        ((null? (cddr lst))
            (if (< (car lst) (cadr lst))
                lst
                (list (cadr lst) (car lst))
            )
        )
        (#t (stringsort_split (car lst) (cdr lst) () ()))
    )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; code generation
;;

(define (makehxx name)
    (let ((outfile (lfopen name "w")))
        (cond 
            ((not outfile) (error 'err_nofile "Couldn't open file" name))
            (#t
                (hxxdescription outfile)
                (hxxheading outfile)
                (hxxdefinitions outfile)
                (hxxending outfile)
                (lfclose outfile)
            )
        )
    )
)

(define (hxxdescription outfile) 
    (display "/* Header file GENERATED with ILS */" outfile) (newline outfile)
)

(define (makesysincludesection headers outfile)
    (map 
        (lambda (name)
            (display "#include <intelib/" outfile)
            (display name outfile)
            (display ">" outfile)
            (newline outfile)
        )
        headers
    )
)

(define (hxxheading outfile) 
    (newline outfile)
    (newline outfile)
    (display
        (plus "#ifndef ILL_GENERATED_SENTRY_" modulename " // sentry")
        outfile)
    (newline outfile)
    (display
         (plus "#define ILL_GENERATED_SENTRY_" modulename)
         outfile)
    (newline outfile)
    (newline outfile)
    (makesysincludesection corelibheaders outfile)
    (newline outfile)
    (newline outfile)
    (newline outfile)
)

(define (packagegeninits startn finishn outfile)
    (display "   void PackageInitFunction" outfile)
    (display startn outfile)
    (display "();" outfile)
    (newline outfile)
    (if (< startn finishn) (packagegeninits (+ startn 1) finishn outfile))
)

(define (select_members predicate lst)
    (cond
        ((null? lst) ())
        ((predicate (car lst))
            (cons (car lst) (select_members predicate (cdr lst)))
        )
        (#t (select_members predicate (cdr lst)))
    )
)

(define (getsymbolsbypredicate pred)
    (let ((lst ()))
        (maphash
            (lambda (key val)
                ;(write (list "***MAPHASH*** " key val)) (newline)
                (if
                    (and
                        (eq? (cdr key) 'used)
                        val                   ; val == #t
                    )
                    (set! lst (cons (car key) lst))
                )
            )
            symbtable
        )
        (stringsort (select_members pred lst))
        ;(select_members pred lst)
    )
)

(define (getsymbolsforpackage pkgid)
    (getsymbolsbypredicate
        (lambda (name)
            (and
                (eq? (symtabget name 'package) pkgid)
                (not (symtabget name 'external))
                (not (symtabget name 'public))
            )
        )
    )
)

(define (getsymbolspublic)
    (getsymbolsbypredicate
        (lambda (name)
            (and
                (not (symtabget name 'external))
                (symtabget name 'public)
            )
        )
    )
)

(define (packagedefinition pkgid classname parentstr numinits outfile)
    (display (plus "class " classname parentstr " { public: ") outfile) 
    (newline outfile)
    (map  
        (lambda (name)
            (display
                (plus "    SchSymbol " (symtabget name 'cppname) ";") outfile)
            (newline outfile)
        )
        (getsymbolsforpackage pkgid)
    )    
    (display (plus "    " classname "();") outfile)
    (newline outfile)
    (display (plus "    ~" classname "(){}") outfile)
    (newline outfile)
    (cond
        ((> numinits 0)
            (display "private:" outfile)
            (newline outfile)
            (packagegeninits 1 numinits outfile)
        ) 
        (#t ())
    )
    (display "}; " outfile)
    (newline outfile)
    (newline outfile)
    (newline outfile)
)

(define (hxxpublicsymbols outfile)
    (newline outfile)
    (display "// Now public symbols go..." outfile)
    (newline outfile)
    (newline outfile)
    (map
        (lambda (name)
            (display
                 (plus "extern SchSymbol " name ";" ) 
                 outfile
            )
            (newline outfile)
        )
        (getsymbolspublic)
    )
    (newline outfile)
    (display "// End of public symbols section" outfile)
    (newline outfile)
    (newline outfile)
)

(define (hxxdefinitions outfile) 
    ; First, generate the pkg_scheme definition
    (packagedefinition 'pkg_scheme schemepackagename "" 0 outfile)
    ; Second, generate the pkg_user definition
    (packagedefinition
        'pkg_user 
        userpackagename 
        (plus " : public " schemepackagename " ")
        (length theprogram)
        outfile
    )
    ; Declare a pointer to the instance of User package
    (display
        (plus "extern " userpackagename " *p" userpackagename ";")
        outfile
    )
    (newline outfile)
    ; Now, public symbols are to go...
    (hxxpublicsymbols outfile)
    ; Finally, declare the initialization function
    (display "// The initialization function" outfile)
    (newline outfile)
    (display (plus "void " initfunctionname "();") outfile) 
    (newline outfile)
    (newline outfile)
    (newline outfile)
)

(define (hxxending outfile) 
    (newline outfile)
    (newline outfile)
    (display "#endif // sentry" outfile)
    (newline outfile)
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; CPP (.cxx) FILE GENERATION

(define (makecxx name)
    (let ((outfile (lfopen name "w")))
        (cond
            ((null? outfile) (error 'err_nofile "Couldn't open file" name))
            (#t
                (cxxdescription outfile)
                (cxxheading outfile)
                (cxxdefinitions outfile)
                (cxxtoplevels outfile)
                (cxxinitfunction outfile)
                (cxxending outfile)
                (lfclose outfile)
            )
        )
    )
)

(define (cxxdescription outfile) 
    (display "/* C++ file GENERATED with ILL */" outfile)
    (newline outfile)
    (newline outfile)
    (newline outfile)
)

(define (makeincludesection headers outfile)
    (map
        (lambda (name)
            (display "#include \"" outfile)
            (display name outfile)
            (display "\"" outfile)
            (newline outfile)
        )
        headers
    )
)

(define (makesysincludesection headers outfile)
    (map 
        (lambda (name)
            (display "#include <intelib/" outfile)
            (display name outfile)
            (display ">" outfile)
            (newline outfile)
        )
        headers
    )
)

(define (cxxheading outfile) 
    (makeincludesection (list hxxfilename) outfile)
    (makesysincludesection libheaders outfile)
    (makeincludesection othermodules outfile)
    (newline outfile)
    (newline outfile)
    (newline outfile)
    (display "// The pointer to the User package instance" outfile)
    (newline outfile)
    (display (plus userpackagename " *p" userpackagename " = 0;") outfile)
    (newline outfile)
    (newline outfile)
    (newline outfile)
    (display "// Helper variables" outfile)
    (newline outfile)
    (display "static SchListConstructor L;" outfile)
    (newline outfile)
    ;(display "static SReference &schT = *PTheSchemeBooleanTrue;" outfile)
    (display "#define schT (*PTheSchemeBooleanTrue)" outfile)
    (newline outfile)
    ;(display "static SReference &schF = *PTheSchemeBooleanFalse;" outfile)
    (display "#define schF (*PTheSchemeBooleanFalse)" outfile)
    (newline outfile)
    (newline outfile)
    (newline outfile)
)

(define (symbolinitializer name)
    (let ((funclass (symtabget name 'funclass)))
        (if funclass
            (plus
                ", SReference(new "
                funclass 
                ")"
            )
            ""
        )
    )
)

(define (packageimplementation pkgid classname numinits outfile)
    (display (plus classname "::" classname "()") outfile)
    (let ((firstone #t))
        (map  
            (lambda (name)
                (cond
                    (firstone
                        (set! firstone #f)
                        (display " :" outfile)
                        (newline outfile)
                    )
                    (#t
                        (display "," outfile)
                        (newline outfile)
                    )
                )
                (display
                    (plus
                        "    "
                        (symtabget name 'cppname) 
                        "(\"" name "\""
                        (symbolinitializer name)
                        ")"
                    ) 
                    outfile
                )
            )
            (getsymbolsforpackage pkgid)
        )    
    )    
    (cond 
        ((> numinits 0) 
            (newline outfile)
            (display "{" outfile)
            (newline outfile)
            (geninitsinconstructor 1 numinits outfile)
            (display "}" outfile)
            (newline outfile)
        )
        (#t (newline outfile) (display "{}" outfile))
    )
    (newline outfile)
    (newline outfile)
    (newline outfile)
)

(define (geninitsinconstructor startn finishn outfile)
    (display "   PackageInitFunction" outfile)
    (display startn outfile)
    (display "(); " outfile)
    (newline outfile)
    (if (< startn finishn) 
        (geninitsinconstructor (+ startn 1) finishn outfile)
    )
)

(define (cxxpublicsymbols outfile)
    (newline outfile)
    (display "// Now public symbols go..." outfile)
    (newline outfile)
    (newline outfile)
    (map
        (lambda (name)
            (display
                (plus "SchSymbol " name "(\"" name "\")" ";") 
                outfile
            )
            (newline outfile)
        )
        (getsymbolspublic)
    )
    (newline outfile)
    (display "// End of public symbols section" outfile)
    (newline outfile)
    (newline outfile)
)

(define (cxxdefinitions outfile) 
    (packageimplementation 'pkg_scheme schemepackagename 0 outfile)
    (packageimplementation 'pkg_user userpackagename 
                           (length theprogram) outfile)
    (cxxpublicsymbols outfile)
    (newline outfile)
    (newline outfile)
)

(define (cxxtoplevels outfile) 
    (outtoplevels
        (plus "void " userpackagename "::PackageInitFunction")
        1
        theprogram
        outfile
    ) 
)

(define (cxxinitfunction outfile) 
    (newline outfile)
    (newline outfile)
    (display "// The initialization function" outfile)
    (newline outfile)
    (display (plus "void " initfunctionname "()" #\Newline "{" #\Newline) 
           outfile) 
    (display
        (plus "    p" userpackagename " = new " userpackagename ";")
        outfile
    )
    (newline outfile)
    (display "}" outfile)
    (newline outfile)
)

(define (cxxending outfile) 
    (newline outfile)
    (newline outfile)
    (display "// END OF GENERATED FILE" outfile)
    (newline outfile)
)

(define (outtoplevels funprefix startnum formsleft outfile)
    (cond
        ((null? formsleft) #t)
        (#t
             (display funprefix outfile)
             (display startnum outfile) 
             (display "() { " outfile)
             (doouttoplevel (car formsleft) 0 #t outfile)
             (newline outfile)
             (display "  .Evaluate();" outfile)
             (newline outfile)
             (display "}" outfile)
             (newline outfile)
             (newline outfile)
             (outtoplevels funprefix (+ startnum 1) (cdr formsleft) outfile)
        )
    )
)

(define (atomprint a outfile)
    (cond
        ((eq? a #t) (display "schT" outfile))
        ((eq? a #f) (display "schF" outfile))
        ((string? a) (display (string2c a) outfile))
        ((char? a) (display (char2c a) outfile))
        (#t (write a outfile))
    )
)

(define (doouttoplevel form ident isnewline outfile)
    (if isnewline (outident ident outfile))
    (cond
        ((null? form) (display "(L)" outfile)) 
        ((classicatom? form) 
            (display (symtabget (classicatomname form) 'cppname) outfile)
        )
;       ((extvarnamep form) 
;           (display (plus "F&" form) outfile)
;       )
        ((atom? form) (atomprint form outfile))
        (
            (and                              ; quote form? 
                (eqv? atomquote (car form))
                (eqv? 2 (length form))
            )
            (let ((form2 (second form)))
                (if (or (number? form2) (string? form2))
                    #f ; don't do ~2 or ~"some_string", it will be no good
                    (display "~" outfile)
                )
                (doouttoplevel form2 ident #f outfile)
            )
        )  ; Otherwise, it's a list...
        ((< (+ (printinglength form) ident) rightmargin) ; looks like will fit
            (outlisthere form outfile)
        )
        (#t ; doesnt fit in a single line - let's print one item per line
            (outlonglist form ident outfile)
        )
    )
)

(define (outlisthere form outfile)
    (dooutlist form (not (isdottedlist form)) #f 0 outfile) 
)

(define (outlonglist form ident outfile)
    (dooutlist form (not (isdottedlist form)) #t (+ ident identstep) outfile)
)

(define (dooutlist form isnormal islong ident outfile)
    (if isnormal 
        (display "(L| " outfile)
        (display "((L| " outfile)
    )
           ; first element's always at the same line, 
           ; but it might be a long list itself...
    (if (and islong   ; if the list is not long, it's element cannot be long
             (not (null? (car form)))
             (pair? (car form)) ; only a non-empty list can be a long list
             (> (+ (printinglength (car form)) ident 5) rightmargin) ; long
        )
        (outlonglist (car form) ident outfile)
        (doouttoplevel (car form) 0 #f outfile)
    ) 
           ; The rest of elements...
    (dooutrestoflist (cdr form) islong ident outfile)
)

(define (dooutrestoflist form islong ident outfile)
    (cond 
        ((null? form) ; normal list's ending
          (display ")" outfile)
        )
        ((atom? form) ; dotted list's ending
          (display ")|| " outfile)
          (doouttoplevel form ident islong outfile)
          (display ")" outfile)
        )
        (#t           ; middle of the list
          (display ", " outfile)
          (doouttoplevel (car form) ident islong outfile)
          (dooutrestoflist (cdr form) islong ident outfile)
        )
    )
)

(define (outident ident outfile)
    (newline outfile)
    (dooutident ident outfile)
)
(define (dooutident ident outfile)
    (cond
        ((not (> ident 0)) #t)
        (#t (display " " outfile) (dooutident (- ident 1) outfile))
    )
)

(define (printinglength form)
  (cond ((null? form) 3) ; (L)
        ((char? form) 3) ;;;;;;;;; FOR COMMON LISP ONLY !
        ((classicatom? form)
            (string-length 
                (symtabget (classicatomname form) 'cppname)
            )
        )
;        ((extvarnamep form) (string-length form))
        ((string? form) (+ 2 (string-length form)))
        ((number? form) 8) ; Need to fix this!
        ((eqv? form 'quote) 5)
        ((eq? form ()) 3)
        ((eq? form #t) 4) ; schT
        ((eq? form #f) 4) ; schF
        ((eqv? form 'lambda) 6) 
;        ((symbolp form) (string-length 
;           (first (gethash (chartran (symbolname form)) symbtable ()))
;        ))
        ((atom? form) (error 'err_internal "strange atom encountered" form))
        ((isdottedlist form)  ; dotted list ((L| A, B, C)|| D) 
            (+ (printinglengthofitems form) 10 (* (- (length form) 1) 2))
        )
        (#t                   ; proper list  (L| A, B, C, D)
            (+ (printinglengthofitems form) 5 (* (- (length form) 1) 2))
        )
  )
)
(define (printinglengthofitems items)
  (cond ((null? items) 0)
        ((atom? items) (printinglength items)) ; This is possible when 
                                              ; there was a dotted list
        (#t (+ (printinglength (car items)) 
              (printinglengthofitems (cdr items))
        ))
  )
)

(define (isdottedlist ls)
    (cond
        ((null? ls) #f)
        ((pair? ls) (isdottedlist (cdr ls)))
        (#t #t)
    )
)




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (cmdlinedirectives str)
  (let ((directives (saferead (plus "(" str ")") '%ERROR '%EOF ())))
    (cond ((eq? directives '%EOF) t) ; there was no directives
          ((eq? directives '%ERROR) 
              (error 'err_cmdline 
                     "Malformed directive at command line"
                     (readerror)
              )
          )
          (#t (interpretedirectives directives #f "*command line*"))
    )
  )
)

(define (ils_main srcfiles)
    (getsources srcfiles)
    (parsesources theprogram)
    (makeglobalnames)
    (makehxx hxxfilepath)
    (makecxx cxxfilepath) 
    ;(if debuggerfilename (makedebuggerfile debuggerfilename) #t)
)
