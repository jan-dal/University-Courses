#lang racket
(define (foldr f i xs)
  (if (null? xs)
      i
      (f (car xs) (foldr f i (cdr xs)))))
(define xs (list 1 2 3 5 6 10))

(define (wstaw a xs)
  (foldr (lambda (x y)
           (if (< a (cadr xs)) (cons a xs)
               (cons x y))) null xs))