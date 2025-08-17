#lang racket

(define (matrix a b c d)
  (cons (cons a b) (cons c d)))
(define (matrix-at m x y)
  (if (= x 1)
      (if (= y 1)
          (caar m)
          (cdar m))
      (if (= y 1)
          (cadr m)
          (cddr m))))
(define (matrix? m)
  (if (and (pair? m) (pair? (car m)) (pair? (cdr m)))
      true
      false))