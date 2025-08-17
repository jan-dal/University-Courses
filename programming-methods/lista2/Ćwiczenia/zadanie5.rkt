#lang racket
(define (square x)
  (* x x))

(define (fd x) 6)
(define (fn x)
  (square (- (* 2 x) 1))
  )

(define (cont-frac num den k)
  (if (= k 0)
      0
      (/ (num 1) (+ (den 1) (cont-frac (lambda (x) (num (+ x 1))) (lambda (x) (den (+ x 1))) (- k 1))))))

(define (atan-cf x k)
  (cont-frac (lambda (a)
               (if (= a 1)
                   x
                   (square (* (- a 1) x))
                   )
               )
             (lambda (a) (- (* 2 a) 1)) k)
  )

