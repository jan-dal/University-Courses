#lang racket
(define/contract (suffixes xs) (parametric->/c [a] (-> (listof a) (listof (listof a))))
  (if (null? xs)
      null
      (cons xs (suffixes (cdr xs)))))


(define list-of-lists/c? (lambda (x) (andmap list? x)))



(define/contract (sublists xs) (parametric->/c [a] (-> (listof a) (listof (listof a))))
  (if (null? xs)
      (list null)
      (append-map
       (lambda (ys) (list (cons (car xs) ys) ys))
       (sublists (cdr xs)))))

(define c1 (parametric->/c [a b] (-> a b a)))
(define c2 (parametric->/c [a b c] (-> (-> a b c) (-> a b) a c)))
(define c3 (parametric->/c [a b c] (-> (-> b c) (-> a b) (-> a c))))
(define c4 (parametric->/c [a] (-> (-> (-> a a) a) a)))

(define/contract (id x y) c1 x)
(define/contract (con f g x) c2
  (f x (g x)))
(define/contract (concatenate f g) c3
  (lambda (x) (f (g x))))
(define/contract (proc4 f) c4
  (f (lambda (x) x)))


(define c5 (parametric->/c [a b] (-> a b)))

(define/contract (proc5 x) c5
  (proc5 x))


