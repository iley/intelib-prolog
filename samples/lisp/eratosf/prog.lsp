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




;
; Eratosfen 
(%%% (MODULE-NAME "prog")(DECLARE-PUBLIC eratosfen))


(defun list_all_numbers (max)
  (cond ((= 1 max) nil)
        (t (append (list_all_numbers (- max 1)) (list max)))
  )
)

(defun rm_from_list (num the_list)
  (cond
    ((null the_list) nil)
    ((= num (car the_list)) (cdr the_list))
    (t (cons (car the_list) (rm_from_list num (cdr the_list))))
  )
)

(defun rm_list_from_list (list1 list2)
  (cond ((null list1) list2)
        (t (rm_list_from_list (cdr list1) (rm_from_list (car list1) list2)))
  )
)

(defun list_multiples (num max)
  (make_list_multiples num num max)
)
(defun make_list_multiples (num min max) 
  (cond ((> min max) nil)
        (t (cons min (make_list_multiples num (+ num min) max)))
  )
)

(defun eratosfen (max)
   (do_eratosfen max (list_all_numbers max))
)

(defun do_eratosfen (max remain)
   ;;; uncomment this if you want interactive debugger demonstration
   ;(break "Entering the interactive debugger")
   (cond ((null remain) nil)
         (t (cons (car remain) 
                  (do_eratosfen max (rm_list_from_list 
                                      (list_multiples (car remain) max) 
                                      (cdr remain)
                                    )
                  )
             )
          )
    )
)


