#lang racket

(define (identity x) x)

(define (compose f g)
  (lambda (x) (f (g x)))
  )

(define (repeated p n)
  (if (= n 0)
      (identity p)
      (compose (repeated p (- n 1)) p )
      )
  )