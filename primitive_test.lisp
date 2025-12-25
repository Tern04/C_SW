(set 'puvodni (list (list 1 2) (list 3 4) 5))
(set 'zbytek (cdr puvodni))         ; ((3 4) 5)
(set 'hlava (car puvodni))          ; (1 2)

(print "Test 1 - Deep Copy Integrity:")
(car (car zbytek))                  ; 3
(length (car zbytek))               ; 2
(car hlava)                         ; 1

(print "Test 2 - Boundary Conditions:")
(length nil)                        ; 0
(car nil)                           ; NIL
(cdr nil)                           ; NIL
(cdr (list 1))                      ; NIL
(nth 0 nil)                         ; NIL
(nth 100 (list 1 2 3))              ; NIL

(print "Test 3 - Arithmetic Mix:")
(+ 10 (length (list 1 2 3 4 5)))    ; 15
(- (max 10 20) (min 5 15))          ; 15
(* (nth 1 (list 1 5 10)) 2)         ; 10
(/ (length (list 1 2 3 4 5 6)) 2)   ; 3

(print "Test 4 - Atom vs List Logic:")
(atom (list))                       ; T
(atom (cdr (list 1 2)))             ; NIL
(atom (car (list (list 1 2) 3)))    ; NIL
(atom (nth 1 (list (list 1) 42)))   ; T - 42

(print "Test 5 - Deep Nesting:")
(car (car (car (list (list (list "Trefa!")))))) ;"Trefa!"

(print "Test 6 - Symbols and Quotes:")
(set 'seznam-symbolu (quote (A B C)))
(nth 1 seznam-symbolu)              ; B
(atom (car seznam-symbolu))         ; T
