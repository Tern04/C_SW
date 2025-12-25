(min 5 2 8 1 9)            ; 1
(max 10 20 5 15)           ; 20
(min -5 -10 0 5)           ; -10

(+ (min 10 20) (max 5 15)) ; 10 + 15 = 25

(list 1 2 3)               ; (1 2 3)
(list 10 "ahoj" 30)        ; (10 "ahoj" 30)

; Vnořené seznamy:
(list 1 (list 2 3) 4)      ; (1 (2 3) 4)


(atom 5)                   ; T
(atom "test")              ; T
(atom (list 1 2))          ; NIL
(atom nil)                 ; T

