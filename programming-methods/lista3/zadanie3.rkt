#lang racket
(define (swap p)
  (cons (cdr p) (car p)))

(define (inc x) (+ 1 x))
(define (square x) (* x x))

(define (fun-product f g)
  (lambda (p) (cons (f (car p)) (g (cdr p)))))