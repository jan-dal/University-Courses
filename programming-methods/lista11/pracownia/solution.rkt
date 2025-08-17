#lang racket
(provide (contract-out
          [with-labels  with-labels/c]
          [foldr-map  foldr-map/c]
          [pair-from  pair-from/c ]))
(provide with-labels/c  foldr-map/c  pair-from/c)

(define with-labels/c (parametric->/c [a b] (-> (-> a b) (listof a) (listof (list/c b a)))))
(define foldr-map/c (parametric->/c [a b c] (-> (-> a b (cons/c c b)) b (listof a) (cons/c (listof c) b))))
(define pair-from/c (parametric->/c [a b c] (-> (-> c a) (-> c b) (-> c (cons/c a b)))))


; Przyjmuje procedurę i listę, zwraca listę list
(define (with-labels f xs) with-labels/c
  (if (null? xs)
      null
      (cons (list (f (car xs)) (car xs)) (with-labels f (cdr xs)))))


;; Przyjmuje procedurę zwracającą parę, wartość początkową i listę
;; Zwraca parę 
(define (foldr-map f start xs) foldr-map/c
  (define (wrapper ys acc xacc)
    (cond
      [(null? ys) (cons xacc acc)]
      [else (let ((v (f (car ys) acc)))
              (wrapper (cdr ys) (cdr v) (cons (car v) xacc)))]))
  (wrapper (reverse xs) start null))


;; Przyjmuje dwie procedury nieokreślone dalej
;; Zwraca procedurę przyjmującą argument i zwraca parę 
(define (pair-from f g) pair-from/c
  (lambda (x) (cons (f x) (g x))))
