;   InteLib                                    http://www.intelib.org
;   The file samples/lisp/permut/ppr.lsp
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
   (MODULE-NAME "ppr")
   (DECLARE-PUBLIC lispmain)
   (DECLARE-EXTERNAL create_pg)
   (DECLARE-EXTERNAL next_permutation)
   (DECLARE-USED-MODULE "symbs.hpp")
)

(defun do_out (generator out)
    (let ((perm (next_permutation generator)))
        (cond ((null perm) t)
              (t (prin1 perm out) (terpri out) 
                 (break (list "Current permutation is" perm 
                              "Entering interactive interpreter loop")) 
                 (do_out generator out))
        )
    )
)

(defun lispmain (n outstream) 
    (let ((pg (create_pg n)))
        (do_out pg outstream)
    )
)
