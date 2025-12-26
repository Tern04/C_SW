(set 'arr '(5 1 7 3 2 6 4 9 8))
(set 'len (length arr))
(set 'swapped T)

(while swapped
    (set 'i 1)
    (set 'swapped nil)
    (while (< i len)
        (set 'num1 (nth (- i 1) arr))
        (set 'num2 (nth i arr))
        (if (> num1 num2)
            (while T
                (set (nth (- i 1) arr) num2)
                (set (nth i arr) num1)
                (set 'swapped T)
                (brk)
            )
        )
        (inc 'i 1)
    )
)

(print arr)
(quit)