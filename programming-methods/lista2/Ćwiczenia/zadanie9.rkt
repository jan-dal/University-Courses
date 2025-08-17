#lang racket
(define (build n d b)
  (/ n (+ d b)))

(define (identity x) x)

(define (compose f g)
  (lambda (x) (f (g x))
  ))

(define (repeated p n)
  (if (= n 0)
      p
      (compose (repeated p (- n 1)) p )
      )
  )

(define (repeated-build k n d b)
  (if (= k 0)
      0
   ((repeated (lambda (x) (build n d x)) (- k 1)) b)))
  