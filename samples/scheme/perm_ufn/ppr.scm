
(%%% 
   (MODULE-NAME "ppr")
   (DECLARE-PUBLIC schememain)
   (DECLARE-EXTERNAL create_pg)
   (DECLARE-EXTERNAL next_permutation)
   (DECLARE-USED-MODULE "symbs.hpp")
)

(define (do_out generator out)
    (let ((perm (next_permutation generator)))
        (cond ((null? perm) #t)
              (#t (write perm out) (newline out) 
                  (break (list "Current permutation is" perm 
                               "Entering interactive interpreter loop")) 
                  (do_out generator out))
        )
    )
)

(define (schememain n outstream) 
    (let ((pg (create_pg n)))
        (do_out pg outstream)
    )
)
