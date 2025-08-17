#lang racket
(define (funkcja x y z)
  (define (suma_kwadratow a b) (+ (* a a) (* b b)))
  (cond [(and (< x y) (< x z)) (suma_kwadratow y z)]
        [(and (< y x) (< y z)) (suma_kwadratow x z)]
        [(and (< z x) (< z y)) (suma_kwadratow x y)]
        )
  )