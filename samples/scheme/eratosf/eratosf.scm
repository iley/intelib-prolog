;   InteLib                                    http://www.intelib.org
;   The file samples/scheme/eratosf/eratosf.scm
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
  (MODULE-NAME "eratosf")
  (DECLARE-PUBLIC eratosfen)
)

(define (list_all_numbers max)
  (cond ((= 1 max) '())
        (#t (append (list_all_numbers (- max 1)) (list max)))
  )
)

(define (rm_from_list num the_list)
  (cond
    ((null? the_list) '())
    ((= num (car the_list)) (cdr the_list))
    (#t (cons (car the_list) (rm_from_list num (cdr the_list))))
  )
)

(define (rm_list_from_list  list1 list2)
  (cond ((null? list1) list2)
        (#t (rm_list_from_list (cdr list1) (rm_from_list (car list1) list2)))
  )
)

(define (list_multiples  num max)
  (make_list_multiples num num max)
)
(define (make_list_multiples  num min max) 
  (cond ((> min max) '())
        (#t (cons min (make_list_multiples num (+ num min) max)))
  )
)

(define (eratosfen  max)
   (do_eratosfen max (list_all_numbers max))
)

(define (do_eratosfen  max remain)
   (cond ((null? remain) '())
         (#t (cons (car remain) 
                  (do_eratosfen max (rm_list_from_list 
                                      (list_multiples (car remain) max) 
                                      (cdr remain)
                                    )
                  )
             )
          )
    )
)

