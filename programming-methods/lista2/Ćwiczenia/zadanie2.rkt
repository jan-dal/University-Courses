#lang racket

(define (square x) (* x x))
(define (cube x) (* x x x))

(define (compose f g)
  (lambda (x) (f (g x)))
  )
