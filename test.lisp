(+ 2 5 (* 2 3))
(- 100 (* 5 10) (+ 5 5))
(/ 100 2 5)
(* 1 2 3 4 5)
'(5 1 7 3 2 6 4 9 8)
(quote (a b 2))
(quote A)
(set 'A 10) ;10
(print A)
(set 'A (+ A 5)) ; 15
(print A)
(set 'B (* A 2)) ; 30
(print B)
(set 'A 10) ;10
(inc 'A (+ 2 3)) ; 15
(print A)
(dec 'A A) ; 0
(print A)
(= 1 1) ;T
(= 1 2) ;NIL
(/= 1 2) ;T
(/= 1 1) ;NIL
(< 1 2) ;T
(< 2 1) ;NIL
(> 2 1) ;T
(> 1 2) ;NIL
(<= 1 1) ;T
(<= 2 1) ;NIL
(>= 2 2) ;T
(>= 1 2) ;NIL

(set 'x 10)
(while (> x 0)
    (if (= x 5)
        (brk)
        (print x))
    (set 'x (- x 1)))


(quit)
(quit)