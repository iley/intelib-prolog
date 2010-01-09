;   InteLib                                    http://www.intelib.org
;   The file ill/ill.lsp
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
     (MODULE-NAME "ill")
     (INIT-FUNCTION-NAME "TheLispProgram")
   ; Interface to the module
     (DECLARE-PUBLIC lispentry)
     (DECLARE-PUBLIC lispstdout)
     (DECLARE-PUBLIC handlecommandlinedirectives)
     (DECLARE-PUBLIC error_explanation)
)


(defvar chartrantable (makehashtable)) ; Character translation rules
(defvar symbtable (makehashtable)) ; Symbols
       ; key: name_in_lisp (translated according to chartrantable)
       ; val: (<name_in_c++> <package_id> <classname>
       ;       <is_already_defined>  <is_public> <is_used>)
; (defvar packagetable (makehashtable)) ; Packages
(defvar theprogram ()) ; Program text to be translated
(defvar othermodules ()) ; header files of other modules to be included 
(defvar libheaders ()) ; header files of the InteLib library to be included
; (defvar debuggersymbols ()) ; symbols to be exported into the debuger, if any
;                        ; by default, all publics and externs, and also those 
;                        ; defined by toplevel defun and defvar
                             
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;     global parameters
; default values
(defconstant definitfunctionname "LispInit_") 
(defconstant deflisppackagename "PackageLisp_") 
(defconstant defuserpackagename "PackageUser_") 
(defconstant defcxxfilepath ".cxx") 
(defconstant defhxxfilepath ".hxx")
(defconstant defhxxfilename ".hxx")
(defconstant defdebuggerfilename "_debugger.cxx")
(defconstant defmodulename "lisppart")
;
; variables for actual values
(defvar initfunctionname ()) ; name of the initialization function
(defvar lisppackagename ()) ; name of the object of package
(defvar userpackagename ()) ; name of the object of package
(defvar cxxfilepath ()) ; path of CXX file 
(defvar hxxfilepath ()) ; path of HXX file
(defvar hxxfilename ()) ; name of HXX file for the #include directive
(defvar debuggerfilename ()) ; name for debugger module
(defvar modulename ()) ; Module Id 


(defvar rightmargin 75) ; Max number of symbols per line
(defvar identstep 4) ; How far an inner construct is indented to the right

(defvar lispstdout ()) ; Stdout - to be initialized by the main program

    ; <package_id> ::= 'pkg_intelib | 'pkg_lisp | 'pkg_user | 'pkg_unknown
    ;   Respectively: symbols defined by the library itself,
    ;                 symbols of library functions (e.g. CAR, CONS etc)
    ;                 symbols used in the file and still undeclared
    ;                 symbols declared elsewhere
       

;(defconstant atomquote (makeclassicatom "QUOTE"))
;(defconstant atomfunction (makeclassicatom "FUNCTION"))
(defconstant atomquote (car ''(1)))
               ;;;;;;;; to be fixed -- '#'a doesn't work! ;;;;;;;;;
(defconstant atomfunction (car (quote #'(1))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Processing options that may appear at command line
(defun handlecommandlinedirectives (str)
  (let ((directives (saferead (plus "(" str ")") '%ERROR '%EOF nil)))
    (cond ((eql directives '%EOF) t) ; there was no directives
          ((eql directives '%ERROR) 
              (error 'err_cmdline 
                     "Malformed directive at command line"
                     (readerror)
              )
          )
          (t (interpretedirectives directives nil "*command line*"))
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; READING SOURCE FILES

; open file, suck it and close
(defun readsrcfile (name)
  (let ((infile (lfopen name))) 
             ; "r" not specified here to make line num info available
     (cond ((null infile) (error 'err_nofile "Couldn't open file" name))
           (t
              (suckfile infile name 'theprogram)
              (lfclose infile)
           )
     )
  )
) 

; suck file into the variable referenced by thevar (MUST BE DYNAMIC)
; the variable must already have a value (possibly NIL), and the sucked
; file is appended to the existing value
(defun suckfile (infile filename thevar)
  (set thevar (append (eval thevar) (processfile infile filename))) 
)

; process given file, interprete '%%%' toplevels and return other text 
(defun processfile (infile filename)
  (let ((form (saferead infile '%ERROR '%EOF nil)))
    ; (print form lispstdout) (terpri lispstdout)
    (cond ((eq form '%ERROR)       
            (error 'err_readfile "reading file" filename (readerror))
          )
          ((eq form '%EOF)
             nil
          )
          ((atom form)
             (cons form (processfile infile filename))
          ) 
          ((and (classicatomp (car form)) 
                (equal (classicatomname (car form)) "%%%")
           )
             (interpretedirectives (cdr form) infile filename)
             (processfile infile filename)
          )
          (t
;              ; extract symbols for the debugger
;              (if (and (classicatomp (car form)) 
;                       (memberp (classicatomname (car form))
;                           '("DEFUN" "DEFMACRO" "DEFVAR" "DEFCONST")
;                       )
;                  )
;                  (setq debuggersymbols 
;                      (cons (chartran (classicatomname (second form))) 
;                            debuggersymbols
;                      )    
;                  )
;              )
             ; leave the form as is and continue
             (cons form (processfile infile filename))
          )
     )
  )
)

; process ILL directive section (%%% pseudoform)
(defun interpretedirectives (directives infile infilename)
   (cond ((null directives) nil)
         ((or (atom directives) (atom (car directives)))
             (error 'err_atomdirective "strange directive" directives
                    infilename 
                    (if infile (getcurrentline infile) -1)
             )
         )
         (t 
            ; (print directives lispstdout) (terpri lispstdout)
            (interpretedirective (car directives) infile infilename) 
            (interpretedirectives (cdr directives) infile infilename)
         )
   )
)
(defun interpretedirective (directive infile infilename)
   (let ((dirname (classicatomname (car directive))))
      ;(princ "*" lispstdout) (print directive lispstdout) 
      ;(terpri lispstdout)
      (cond 
         ((equal dirname "DECLARE-CHAR")
              ; declares character translation, e.g. (D-C #\@ "at")
              ; NOTE character declarations should go _before_ 
              ; everything else to make the chartran function work
           (setf (gethash (second directive) chartrantable) 
                 (third directive)
           )
         )
         ((equal dirname "DECLARE-FUNCTION")
              ; declares function available with the library
              ; (D-F "Class"  "CLNAME")
              ; (D-F "LFunctionIf" "IF") 
           (let ((d3 (third directive)))
             (if (stringp d3)
               (let ((name (chartran d3)))
                 (touchsymbol name)
                 (let ((data (gethash name symbtable nil))) 
                    (setf (second data) 'pkg_lisp) ; package
                    (setf (third data) 
                          (functionalsymbname (second directive)))
                    (setf (fourth data) nil) ; Already defined?  
                    (setf (fifth data) nil) ; Public?
                 )
               )
               ;(print "Ignored " lispstdout)
             )
           )
         )
         ((equal dirname "DECLARE-SYMBOL")
              ; declares symbol available from the library
              ; (D-S <C++-obj> <CLNAME>  <_>    )
              ; (D-S QUOTE     "QUOTE"   "QUOTE")
              ; normally, only nil, t, quote and lambda are provided
           (let ((name (chartran (third directive))))
             (touchsymbol name)
;             (let ((data (gethash name symbtable))) 
;                (setf (first data) (classicatomname (second directive)))
;                (setf (second data) 'pkg_intelib)
;                (setf (third data) nil)
;                (setf (fourth data) t)
;                (setf (fifth data) t)
;             )
             (setf (gethash name symbtable) 
                   (list (classicatomname (second directive))
                         'pkg_intelib nil t t t)
             )
           )
         )
         ((equal dirname "DECLARE-NAME")
              ; declares name translation, e.g. (D-N "NULL" "lNULL") 
              ;         or (D-N null "lNULL"
           (let ((name (if (classicatomp (second directive))
                           (chartran (classicatomname (second directive)))
                           (chartran (second directive))
                       )
                ))
             (touchsymbol name)
             (setf (first (gethash name symbtable)) 
                   (third directive)
             )
           )
         )
         ((equal dirname "DECLARE-PUBLIC")
              ; declares a symbol which must be made public 
              ; (that is, available for other modules 
              ; including the main program)
              ; (D-P MY-SYMBOL)
           (let ((name (chartran (classicatomname (second directive)))))
             (touchsymbol name)
             (setf (fifth (gethash name symbtable)) t) ; now it's public
             (setf (sixth (gethash name symbtable)) t) ; assume it IS used
;             (setq debuggersymbols (cons name debuggersymbols))
           )
         )
         ((equal dirname "DECLARE-EXTERNAL")
              ; declares a symbol which is public in another module 
              ; so there's no need to define it
              ; (D-E MY-SYMBOL)
           (let ((name (chartran (classicatomname (second directive)))))
             (touchsymbol name)
             (setf (fourth (gethash name symbtable)) t)
;             (setq debuggersymbols (cons name debuggersymbols))
           )
         )
         ((equal dirname "MODULE-NAME")
              ; sets the name of the module 
              ; (M-N "mymodule")
           (if (not modulename)
               (setq modulename (second directive))   
           )
         )
         ((equal dirname "CXX-FILE-PATH")
              ; sets the .cxx file to generate
              ; (C-F-N "myfile.cxx")
           (if (not cxxfilepath)
               (setq cxxfilepath (second directive))   
           )       
         )
         ((equal dirname "HXX-FILE-PATH")
              ; sets the .hxx file to generate
              ; (H-F-N "myfile.hxx")
           (if (not hxxfilepath)
               (setq hxxfilepath (second directive))   
           )       
         )
         ((equal dirname "HXX-FILE-NAME")
              ; sets the name of the .hxx file as it appears in the #include
              ; (H-F-N "myfile.hxx")
           (if (not hxxfilename)
               (setq hxxfilename (second directive))   
           )       
         )
         ((equal dirname "DEBUGGER-FILE")
              ; sets the name of the .hxx file as it appears in the #include
              ; (H-F-N "myfile.hxx")
           (setq debuggerfilename
              (if (null (second directive)) t (second directive))       
           )
         )
         ((equal dirname "LISP-PACKAGE-NAME")
              ; sets the name of the package which contains 
              ;   standard lisp functions
              ; (L-P-N "MyLispPackage")
           (if (not lisppackagename)
               (setq lisppackagename (second directive))   
           )
         )
         ((equal dirname "USER-PACKAGE-NAME")
              ; sets the name of the package which contains 
              ;   symbols invented by the user 
              ; (L-P-N "MyUserPackage")
           (if (not userpackagename)
               (setq userpackagename (second directive))   
           )
         )
         ((equal dirname "INIT-FUNCTION-NAME")
              ; sets the name of the initialization function name
              ; (I-F-N "mymodule")
           (if (not initfunctionname)
               (setq initfunctionname (second directive))   
           )
         )
         ((equal dirname "DECLARE-USED-MODULE")
              ; makes the compiler to insert an #include directive
              ; at the beginning of the .cxx file
              ; (D-U-M "othermdl.hxx")
              (setq othermodules 
                    (append othermodules (list (second directive))))
         )
         ((equal dirname "ADD-LIB-HEADERS")
              ; makes the compiler to insert an #include directive(s)
              ; at the beginning of the .cxx file
              ; _before_ those required by DECLARE-USED-MODULE directive
              ; (A-L-H "header1.hpp" "header2.hpp" ...)
              (setq libheaders
                    (append libheaders (cdr directive)))
         )
         (t (error 'err_unknowndirective "unknown directive" dirname
                    infilename
                    (if infile (getcurrentline infile) -1)
            )
         )
      )
   )
)


(defun getsources (srcfiles)
  (cond ((null srcfiles) t)
        (t (readsrcfile (car srcfiles)) (getsources (cdr srcfiles))) 
  )
)


;;;;;;;;;;;;
; 

(defun touchsymbol (name)
  (if (not (gethash name symbtable nil))
      (setf (gethash name symbtable)  
         (list name 'pkg_user "LSymbol" nil nil nil)
      )
  )
)

(defun functionalsymbname (fname)
  (list2string
    (append (string2list "LFunctionalSymbol<")
            (string2list fname)
            '(#\>)
    )
  )
)

(defun chartran (name) ; character translation of a symbol name
   (list2string (chartran_d (chartran_l (string2list name))))
)
(defun chartran_l (name)
  (cond ((null name) nil)
        (t (let ((chr (gethash (car name) chartrantable nil)))
              (if (null chr) 
                 (cons (car name) (chartran_l (cdr name)))
                 (append (string2list chr) (chartran_l (cdr name)))
              )
           )
        )
  )
)
(defun chartran_d (name)
  (cond ((memberp (car name) '(#\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9))
           (cons #\d name)
        )
        (t name)
  )
)


(defun makeglobalnames ()
   (if (not modulename)
     (setq modulename defmodulename)
   )
   (if (not initfunctionname)
     (setq initfunctionname (plus definitfunctionname modulename))
   ) 
   (if (not lisppackagename)
     (setq lisppackagename (plus deflisppackagename modulename))
   ) 
   (if (not userpackagename)
     (setq userpackagename (plus defuserpackagename modulename))
   ) 
   (if (not cxxfilepath)
     (setq cxxfilepath (plus modulename defcxxfilepath))
   )
   (if (not hxxfilename)
     (setq hxxfilename (plus modulename defhxxfilename))
     ; else, that is, hxxfilename explicitly defined
     (if (not hxxfilepath)
       (setq hxxfilepath hxxfilename)
     )
   )
   (if (not hxxfilepath)
     (setq hxxfilepath (plus modulename defhxxfilepath))
   )
   (if (eq debuggerfilename t)
     (setq debuggerfilename (plus modulename defdebuggerfilename))
   )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PARSING THE CODE

; translate all symbols via chartran;
; register all symbols that aren't registered yet.
(defun parsepass1 (text level)
  (cond ((null text) nil)
        ((atom text) 
          (if (classicatomp text)
              (let ((theatomname (chartran (classicatomname text))))
                 (touchsymbol theatomname)
                 (setf (sixth (gethash theatomname symbtable)) t) 
                           ; it definitely IS used
                 theatomname
              ) 
              text 
          )
        )
        (t (cons (parsepass1 (car text) (+ level 1)) 
                 (parsepass1 (cdr text) (+ level 1))
           )
        ) 
  )
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CODE GENERATION
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun makeincludesection (headers outfile)
  (mapcar 
    #'(lambda (name)
      (princ "#include \"" outfile)
      (princ name outfile)
      (princ "\"" outfile)
      (terpri outfile)
    )
    headers
  )
)

(defun makesysincludesection (headers outfile)
  (mapcar 
    #'(lambda (name)
      (princ "#include <intelib/" outfile)
      (princ name outfile)
      (princ ">" outfile)
      (terpri outfile)
    )
    headers
  )
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; HEADER (.hxx) FILE GENERATION

(defun makehxx (name)
  (let ((outfile (lfopen name "w")))
     (cond ((null outfile) (error 'err_nofile "Couldn't open file" name))
           (t
              (hxxdescription outfile)
              (hxxheading outfile)
              (hxxdefinitions outfile)
              (hxxending outfile)
              (lfclose outfile)
           )
     )
  )
)

(defun hxxdescription (outfile) 
  (princ "/* Header file GENERATED with ILL */" outfile) (terpri outfile)
)

(defun hxxheading (outfile) 
  (terpri outfile)
  (terpri outfile)
  (princ (plus "#ifndef ILL_GENERATED_SENTRY_" modulename " // sentry")
         outfile)
  (terpri outfile)
  (princ (plus "#define ILL_GENERATED_SENTRY_" modulename)
         outfile)
  (terpri outfile)
  (terpri outfile)
  (makesysincludesection libheaders outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun packagegeninits (startn finishn outfile)
  (princ "   void PackageInitFunction" outfile)
  (princ startn outfile)
  (princ "(); " outfile)
  (terpri outfile)
  (if (< startn finishn) (packagegeninits (+ startn 1) finishn outfile))
)

(defun packagedefinition (pkgid classname parentstr numinits outfile)
  (princ (plus "class " classname parentstr " { public: ") outfile) 
  (terpri outfile)
  (sortedmaphash 
    #'  
      (lambda (key data)
       ; key: name_in_lisp (translated according to chartrantable)
       ; val: (<name_in_c++> <package_id> <classname>
       ;       <is_already_defined>  <is_public> <is_used>)
        (if (and (eq (second data) pkgid) ; Belongs to this package
                 (not (fourth data))      ; isn't defined elsewhere
                 (sixth data)             ; is actually in use
                 (not (fifth data))       ; isn't public 
                                          ;  (publics defined elsewhere)
            )      
          (progn
            (princ (plus "   " (third data) "  " (first data) ";") outfile)
            (terpri outfile)
          )
        )
      )
    symbtable
  )    
  (princ (plus "   " classname "();") outfile)
  (terpri outfile)
  (princ (plus "   ~" classname "(){}") outfile)
  (terpri outfile)
  (cond ((> numinits 0)
         (princ " private:" outfile)
         (terpri outfile)
         (packagegeninits 1 numinits outfile)
        ) 
        (t nil)
  )
  (princ (plus "}; ") outfile) (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun hxxpublicsymbols (outfile)
  (terpri outfile)
  (princ "// Now public symbols go..." outfile)
  (terpri outfile)
  (terpri outfile)
  (sortedmaphash 
    #'
      (lambda (key data)
       ; key: name_in_lisp (translated according to chartrantable)
       ; val: (<name_in_c++> <package_id> <classname>
       ;       <is_already_defined>  <is_public> <is_used>)
        (if (and (not (fourth data))      ; isn't defined elsewhere
                 (sixth data)             ; is actually in use
                 (fifth data)             ; IS public 
            )    
          (progn   
            (princ (plus "   extern " 
                         (third data) 
                         "  " 
                         (first data) 
                         ";" ) 
                   outfile
            )
            (terpri outfile)
          )
        )
      )
    symbtable
  )    
  (terpri outfile)
  (princ "// End of public symbols section" outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun hxxdefinitions (outfile) 
  ; First, generate the pkg_lisp definition
  (packagedefinition 'pkg_lisp lisppackagename "" 0 outfile)
  ; Second, generate the pkg_user definition
  (packagedefinition 'pkg_user 
                     userpackagename 
                     (plus " : public " lisppackagename " ")
                     (length theprogram)
                     outfile)
  ; Declare a pointer to the instance of User package
  (princ (plus "extern " userpackagename " *p" userpackagename ";") outfile)
  (terpri outfile)
  ; Now, public symbols are to go...
  (hxxpublicsymbols outfile)
  ; Finally, declare the initialization function
  (princ "// The initialization function" outfile)
  (terpri outfile)
  (princ (plus "void " initfunctionname "();") outfile) 
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun hxxending (outfile) 
  (terpri outfile)
  (terpri outfile)
  (princ "#endif // sentry" outfile)
  (terpri outfile)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CPP (.cxx) FILE GENERATION

(defun makecxx (name)
  (let ((outfile (lfopen name "w")))
     (cond ((null outfile) (error 'err_nofile "Couldn't open file" name))
           (t
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

(defun cxxdescription (outfile) 
  (princ "/* C++ file GENERATED with ILL */" outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun cxxheading (outfile) 
  (makesysincludesection libheaders outfile)
  (makeincludesection (append othermodules (list hxxfilename)) outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
  (princ "// The pointer to the User package instance" outfile)
  (terpri outfile)
  (princ (plus userpackagename " *p" userpackagename " = 0;") outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
  (princ "// The LListConstructor instance" outfile)
  (terpri outfile)
  (princ "static LListConstructor L;" outfile)
  (terpri outfile)
  (princ "// The LFunctionConstructor instance" outfile)
  (terpri outfile)
  (princ "static LFunctionConstructor F;" outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun packageimplementation (pkgid classname numinits outfile)
  (princ (plus classname "::" classname "()") outfile)
  (let ((firstone t))
    (sortedmaphash 
       #'
        (lambda (key data)
         ; key: name_in_lisp (translated according to chartrantable)
         ; val: (<name_in_c++> <package_id> <classname>
         ;       <is_already_defined>  <is_public> <is_used>)
          (cond ((and (eq (second data) pkgid) ; Belongs to this package
                   (not (fourth data))         ; isn't defined elsewhere
                   (sixth data)                ; is actually in use
                   (not (fifth data))          ; isn't public 
                                               ; (publics defined elsewhere)
                 )      
                  (cond
                     (firstone
                        (setq firstone nil)
                        (princ " : " outfile)
                        (terpri outfile)
                     )
                     (t
                        (princ "," outfile)
                        (terpri outfile)
                     )
                  )
                  (princ (plus "            "
                               (first data) 
                               "(\"" (first data) "\")") 
                               outfile)
                )
                (t nil)
          )
        )
      symbtable
    )    
  )
  (cond 
    ((> numinits 0) 
      (terpri outfile)
      (princ "{" outfile)
      (terpri outfile)
      (geninitsinconstructor 1 numinits outfile)
      (princ "}" outfile)
      (terpri outfile)
    )
    (t (princ "  {}" outfile))
  )
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun geninitsinconstructor (startn finishn outfile)
  (princ "   PackageInitFunction" outfile)
  (princ startn outfile)
  (princ "(); " outfile)
  (terpri outfile)
  (if (< startn finishn) 
      (geninitsinconstructor (+ startn 1) finishn outfile)
  )
)

(defun cxxpublicsymbols (outfile)
  (terpri outfile)
  (princ "// Now public symbols go..." outfile)
  (terpri outfile)
  (terpri outfile)
  (sortedmaphash 
     #'
      (lambda (key data)
       ; key: name_in_lisp (translated according to chartrantable)
       ; val: (<name_in_c++> <package_id> <classname>
       ;       <is_already_defined>  <is_public> <is_used>)
        (if (and (not (fourth data))      ; isn't defined elsewhere
                 (sixth data)             ; is actually in use
                 (fifth data)             ; IS public 
            )      
          (progn 
            (princ (plus "    " 
                         (third data) 
                         "  " 
                         (first data) 
                         "(\"" (first data) "\")"
                         ";") 
                   outfile)
            (terpri outfile)
          )
        )
      )
    symbtable
  )    
  (terpri outfile)
  (princ "// End of public symbols section" outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun cxxdefinitions (outfile) 
  (packageimplementation 'pkg_lisp lisppackagename 0 outfile)
  (packageimplementation 'pkg_user userpackagename 
                         (length theprogram) outfile)
  (cxxpublicsymbols outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun cxxtoplevels (outfile) 
  (outtoplevels (plus "void " userpackagename "::PackageInitFunction")
                1
                theprogram
                outfile) 
)

(defun cxxinitfunction (outfile) 
  (terpri outfile)
  (terpri outfile)
  (princ "// The initialization function" outfile)
  (terpri outfile)
  (princ (plus "void " initfunctionname "()" #\Newline "{" #\Newline) 
         outfile) 
  (princ (plus "  p" userpackagename " = new " userpackagename ";") outfile)
  (terpri outfile)
  (princ "}" outfile)
  (terpri outfile)
)

(defun cxxending (outfile) 
  (terpri outfile)
  (terpri outfile)
  (princ "// END OF GENERATED FILE" outfile)
  (terpri outfile)
)

(defun outtoplevels (funprefix startnum formsleft outfile)
  (cond ((null formsleft) t)
        (t
          (princ funprefix outfile)
          (princ startnum outfile) 
          (princ "() { " outfile)
          (doouttoplevel (car formsleft) 0 t outfile)
          (terpri outfile)
          (princ "  .Evaluate();" outfile)
          (terpri outfile)
          (princ "}" outfile)
          (terpri outfile)
          (terpri outfile)
          (outtoplevels funprefix (+ startnum 1) (cdr formsleft) outfile)
        )
  )
)

(defun atomprint (a outfile)
    (cond
        ((stringp a) (princ (string2c a) outfile))
        ((characterp a) (princ (char2c a) outfile))
        (t (prin1 a outfile))
    )
)

(defun doouttoplevel (form ident isnewline outfile)
  ;(print form outfile)
  (if isnewline (outident ident outfile))
  (cond
    ((null form) (princ "(L)" outfile)) 
    ((classicatomp form) 
       (princ (first (gethash 
                       (chartran (classicatomname form)) 
                       symbtable nil
                     )
              ) 
              outfile
       )
    )
    ((extvarnamep form) 
       (princ (plus "F&" form) outfile)
    )
    ((atom form) (atomprint form outfile))
    ((and                              ; quote form? 
        (eql atomquote (car form))
        (eql 2 (length form))
      )
      (let ((form2 (second form)))
        (if (or (numberp form2) (stringp form2))
          () ; don't do ~2 or ~"some_string", it will be no good
          (princ "~" outfile)
        )
        (doouttoplevel form2 ident nil outfile)
      )
    )  
    ((and                              ; function form? 
        (eql atomfunction (car form))
        (eql 2 (length form))
      )
      (princ "F^" outfile)
      (doouttoplevel (second form) ident nil outfile)
    )  ; Otherwise, it's a list...
;    ((not isnewline) ; this means it MUST fit in the line...
;      (outlisthere form outfile)
;    )
    ((< (+ (printinglength form) ident) rightmargin) ; looks like will fit
      (outlisthere form outfile)
    )
    (t ; doesnt fit in a single line - let's print one item per line
      (outlonglist form ident outfile)
    )
  )
)

(defun outlisthere (form outfile)
  (dooutlist form (not (isdottedlist form)) nil 0 outfile) 
)

(defun outlonglist (form ident outfile)
  (dooutlist form (not (isdottedlist form)) t (+ ident identstep) outfile)
)

(defun dooutlist (form isnormal islong ident outfile)
  (if isnormal 
     (princ "(L| " outfile)
     (princ "((L| " outfile)
  )
         ; first element's always at the same line, 
         ; but it might be a long list itself...
  (if (and islong   ; if the list is not long, it's element cannot be long
           (not (null (car form)))
           (listp (car form)) ; only a non-empty list can be a long list
           (> (+ (printinglength (car form)) ident 5) rightmargin) ; long
      )
    (outlonglist (car form) ident outfile)
    (doouttoplevel (car form) 0 nil outfile)
  ) 
         ; The rest of elements...
  (dooutrestoflist (cdr form) islong ident outfile)
)

(defun dooutrestoflist (form islong ident outfile)
  (cond ((null form) ; normal list's ending
          (princ ")" outfile)
        )
        ((atom form) ; dotted list's ending
          (princ ")|| " outfile)
          (doouttoplevel form ident islong outfile)
          (princ ")" outfile)
        )
        (t           ; middle of the list
          (princ ", " outfile)
          (doouttoplevel (car form) ident islong outfile)
          (dooutrestoflist (cdr form) islong ident outfile)
        )
  )
)

(defun outident (ident outfile)
  (terpri outfile)
  (dooutident ident outfile)
)
(defun dooutident (ident outfile)
  (cond ((not (> ident 0)) t)
        (t (princ " " outfile) (dooutident (- ident 1) outfile))
  )
)


; (defun printinglength (form)
;   (let ((q (printinglength_imp form)))
;     (princ "@* " lispstdout)
;     (princ q lispstdout)
;     (princ " ---> " lispstdout)
;     (prin1 form lispstdout)
;     (terpri lispstdout)
;     q
;   ) 
; )

(defun printinglength (form)
  (cond ((null form) 3) ; (L)
        ((characterp form) 3) ;;;;;;;;; FOR COMMON LISP ONLY !
        ((classicatomp form) (stringlength 
           (first (gethash (chartran (classicatomname form)) symbtable nil))
        ))
        ((extvarnamep form) (stringlength form))
        ((stringp form) (+ 2 (stringlength form)))
        ((numberp form) 8) ; Need to fix this!
        ((eql form 'quote) 5)
        ((eql form 'nil) 3)
        ((eql form 't) 1)
        ((eql form 'lambda) 6) 
        ((symbolp form) (stringlength 
           (first (gethash (chartran (symbolname form)) symbtable nil))
        ))
        ((atom form) (error 'err_internal "strange atom encountered" form))
        ((isdottedlist form)  ; dotted list ((L| A, B, C)|| D) 
           (+ (printinglengthofitems form) 10 (* (- (length form) 1) 2))
        )
        (t                   ; proper list  (L| A, B, C, D)
           (+ (printinglengthofitems form) 5 (* (- (length form) 1) 2))
        )
  )
)
(defun printinglengthofitems (items)
  (cond ((null items) 0)
        ((atom items) (printinglength items)) ; This is possible when 
                                              ; there was a dotted list
        (t (+ (printinglength (car items)) 
              (printinglengthofitems (cdr items))
        ))
  )
)

(defun isdottedlist (list)
  (if (null (cdr (last list))) nil t)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Debugger (_debugger.cxx) file generation

(defun makedebuggerfile (name)
  (let ((outfile (lfopen name "w"))
        (classname (plus "IntelibLispLoop_" modulename))
       )
     (cond ((null outfile) (error 'err_nofile "Couldn't open file" name))
           (t
              (debuggerfiledescription outfile)
              (debuggerfileheading outfile)
              (debuggerfileclass classname outfile)
              (debuggerfilestaticpart classname outfile)
              (debuggerfileending outfile)
              (lfclose outfile)
           )
     )
  )
)

(defun debuggerfiledescription (outfile) 
  (princ "/* Lisp part debugger assistance GENERATED with ILL */" outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun debuggerfileheading (outfile) 
  (princ "#include <stdio.h>" outfile)
  (terpri outfile)
  (makesysincludesection
    (append libheaders
            (list "lisp/lpackage.hpp" "ill/ill_loop.hpp")
    )
    outfile
  )
  (makeincludesection (append othermodules (list hxxfilename)) outfile)
  (terpri outfile)
  (terpri outfile)
  (terpri outfile)
)

(defun debuggerfileclass (classname outfile)
   (princ (plus "class " classname " : public IntelibLispLoop { public: ") 
          outfile
   )
   (terpri outfile)
   (princ (plus "    " classname "(const SReference& pkg)") outfile)
   (terpri outfile)
   (princ (plus "         : IntelibLispLoop(pkg) {") outfile)
   (terpri outfile)
   (debuggerfile_imports outfile)
   (terpri outfile)
   (princ "    }" outfile)
   (terpri outfile)
   (princ "};" outfile)
   (terpri outfile)
)

(defun debuggerfile_imports (outfile) 
   (sortedmaphash 
      #'(lambda (symbol data)
         ; val: (<name_in_c++> <package_id> <classname>
         ;       <is_already_defined>  <is_public> <is_used>)
         (cond
            ((eql (second data) 'pkg_intelib) nil) ; already there
            ((not (sixth data)) nil)               ; unused
            ((or (fourth data) (fifth data))       ; just by name
               ; because it is either public or external
               (princ (plus "        ImportSymbol(" (first data) ");")
                      outfile)
               (terpri outfile)
            )
            ((member (second data) '(pkg_lisp pkg_user))
               (princ (plus 
                        "        ImportSymbol(" 
                        "p" userpackagename "->"
                        (first data) ");"
                      )
                      outfile
               )
               (terpri outfile)
            )
         )
      )
      symbtable
   )
)

(defun debuggerfilestaticpart (classname outfile)
  (terpri outfile)
  (terpri outfile)

  (princ
    "static void FunBreak(const SReference &ref, IntelibContinuation *lf) {"
    outfile) 
  (terpri outfile)
  (princ (plus "    static " classname " *p = 0;") outfile)
  (terpri outfile)
  (princ "    if(!p) {" outfile)
  (terpri outfile)
  (princ "        SReference pkg(new LExpressionPackageIntelib);" outfile) 
  (terpri outfile)
  (princ (plus "        p = new " classname "(pkg);") outfile) 
  (terpri outfile)
  (princ "    }" outfile)  (terpri outfile)
  (terpri outfile)
  (princ "    printf(\"%s\\n\", ref->TextRepresentation().c_str());" outfile) 
  (terpri outfile)
  (princ "    p->Go(lf);" outfile) (terpri outfile)
  (princ "}" outfile) (terpri outfile)
  (terpri outfile)

  (princ "void LispBreak() { FunBreak(\"LispBreak called\", 0); }" outfile) 
  (terpri outfile)
  (terpri outfile)

  (princ "// TheLispBreakFunction is implemented in lisp/library/std/break.cpp"
         outfile)
  (terpri outfile)
  (princ "extern void (*TheLispBreakFunction)" outfile)
  (princ         "(const SReference &ref, IntelibContinuation *lfp);" outfile)
  (terpri outfile)
  (terpri outfile)
  (princ "class RegisterBreakFunction { public:" outfile) (terpri outfile)
  (princ "    RegisterBreakFunction() { TheLispBreakFunction = FunBreak; }" 
         outfile) 
  (terpri outfile)
  (princ "};" outfile) (terpri outfile)
  (terpri outfile)
  (princ "static RegisterBreakFunction br;" outfile) (terpri outfile)
)

(defun debuggerfileending (outfile) 
  (terpri outfile)
  (terpri outfile)
  (princ "// END OF GENERATED FILE" outfile)
  (terpri outfile)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
(defun sortedmaphash (fun table)
  (let ((thelist ()))
    (maphash 
      #'
        (lambda (key data) 
          (setq thelist (cons (cons key data) thelist))
        )
      table
    )
    (mapcar 
      #'(lambda (item) 
          (funcall fun (car item) (cdr item))
        )
      (sort thelist #'(lambda (x y) (lessp (car x) (car y))))
    )
  )
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; function called from the main program
(defun lispentry (srcfiles)
;  (getsources (cons illdefsfilename srcfiles))
  (getsources srcfiles)
  (parsepass1 theprogram 0)
;  (sortedmaphash 
;            #'(lambda (key data)
;                 (princ key lispstdout)
;                 (princ data lispstdout)
;                 (terpri lispstdout)
;              )
;           symbtable          
;  )
;  (princ hxxfilename lispstdout)
;  (princ cxxfilename lispstdout)
  (makeglobalnames)
  (makehxx hxxfilepath)
  (makecxx cxxfilepath) 
  (if debuggerfilename (makedebuggerfile debuggerfilename) t)
)


(defun error_explanation (error_list stream)
    (cond
        ((eq (car error_list) 'err_cmdline)
            (princ (second error_list) stream) 
            (princ ": " stream)
            (princ (first (third error_list)) stream)
            (terpri stream)
        )
        ((eq (car error_list) 'err_nofile)
            (princ "Can't open file " stream)
            (princ (third error_list) stream)
            (terpri stream)
        )
        ((eq (car error_list) 'err_readfile)
            (let (
                (filename (third error_list)) 
                (rderr (fourth error_list))
            )
                (princ filename stream)
                (princ ":" stream)
                (princ (second rderr) stream)
                (princ ": reader error " stream)
                (princ (first rderr) stream)
                (terpri stream)
                (if (and (third rderr) (> (third rderr) -1)) (progn
                    (princ filename stream)
                    (princ ":" stream)
                    (princ (third rderr) stream)
                    (princ ": in expression started here" stream)
                    (terpri stream)
                ))
            )
        )
        ((eq (car error_list) 'err_atomdirective)
            (let (
                (dirname (third error_list))
                (filename (fourth error_list)) 
                (linenum (fifth error_list))
            )
                (princ filename stream)
                (princ ":" stream)
                (if (> linenum 0) (progn
                    (princ linenum stream)
                    (princ ":" stream)
                ))
                (princ " malformed directive " stream)
                (princ dirname stream)
                (terpri stream)
            )
        )
        ((eq (car error_list) 'err_unknowndirective)
            (let (
                (dirname (third error_list))
                (filename (fourth error_list)) 
                (linenum (fifth error_list))
            )
                (princ filename stream)
                (princ ":" stream)
                (if (> linenum 0) (progn
                    (princ linenum stream)
                    (princ ":" stream)
                ))
                (princ " unknown directive " stream)
                (princ dirname stream)
                (terpri stream)
            )
        )
        ((eq (car error_list) 'err_internal)
            (princ "INTERNAL ERROR: " stream)
            (princ (third error_list) stream)
            (princ ": " stream)
            (princ (fourth error_list) stream)
            (terpri stream)
        )
        (t
            (princ (plus "Error: " (second error_list) #\Newline) stream)
        )
    )
)
