; +-------------------------------------------------------------------------+
; |               I__n__t__e__L__i__b           0.6.21 development          |
; | Copyright (c) Andrey Vikt. Stolyarov <crocodil_AT_croco.net> 2000-2008. |
; |                                                                         |
; | This is free software.  See GNU GENERAL PUBLIC LICENSE v.2 for details. |
; | GNU GPL v2  is  found  in docs/gnu_gpl2.txt,  or at  http://www.gnu.org |
; |     Please see also docs/readme.txt and visit http://www.intelib.org    |
; |                                                                         |
; | !!! THERE IS NO WARRANTY OF ANY KIND, NEITHER EXPRESSED NOR IMPLIED !!! |
; +-------------------------------------------------------------------------+





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
