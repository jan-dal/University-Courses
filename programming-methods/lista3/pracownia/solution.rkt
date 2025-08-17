#lang racket

(define (cons a b)
  (lambda (x) (cond[(= x 0) a]
                   [(= x 1) b]
                   [else #f])))
(define (car p)
  (p 0))
(define (cdr p)
  (p 1))



(define (pair? p)
  (if (not (procedure? p)) #f
      (and (car p) (cdr p))))