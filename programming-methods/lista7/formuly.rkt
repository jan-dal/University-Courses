#lang racket
(struct forall (var expr) #:transparent)
(struct exists (var expr) #:transparent)
(struct var (x) #:transparent)
(struct binop (op e1 e2) #:transparent)
(struct unop (op e1) #:transparent)
(struct val (x) #:transparent)

;; (forall x (|| x (! x)))
;; (forall y (&& (exists x (=> y x)) (<> y (|| y y)))

(define (operator? x)
  (member x '(=> <> || && !)))


(define (expr? q)
  (match q
    [(forall v expr) (and (var? v) (expr? expr))]
    [(exists v expr) (and (var? v) (expr? expr))]
    [(var v) (symbol? v)]
    [(binop op e1 e2) (and (operator? op) (expr? e1) (expr? e2))]
    [(unop op expr) (and (operator? op) (expr? expr))]
    [(val e) (and (boolean? e) (or (eq? e #f) (eq? e #t)))]
    [_ false]))

(define (parse e)
  (cond
   [(and (list? e) (eq? (length e) 3) (symbol? (second e)) (eq? (first e) 'forall))
    (forall (parse (second e)) (parse (third e)))]
   [(and (list? e) (eq? (length e) 3) (symbol? (second e)) (eq? (first e) 'exists))
    (exists (var (second e)) (parse (third e)))]
   [(and (list? e) (eq? (length e) 3) (operator? (first e)))
    (binop (first e) (parse (second e)) (parse (third e)))]
   [(and (list? e) (eq? (length e) 2) (operator? (first e)))
    (unop (first e) (parse (second e)))]
   [(and (boolean? e) (or (eq? e #f) (eq? e #t))) (val e)]
   [(symbol? e) (var e)]))

(define test-expr '(forall y (&& (exists x (=> y x)) (<> y (|| #f y)))))
(define f '(forall x (|| x (! x))))
(define g '(|| #t #f))
(define h '(<> #t #f))



(define (&& x y)
  (and x y))
(define (|| x y)
  (or x y))
(define (=> x y)
  (implies x y))
(define (<> x y)
  (and (=> x y) (=> y x)))


(define (op->proc op)
  (match op ['<> <>] ['=> =>] ['&& &&] ['|| ||] ['! not]))

(define (eval e)
  (eval-e e null))

(define (eval-e e xs)
  (define (where x xs)
    (cond
      [(null? xs) "Unbound variable"]
      [(and (pair? (car xs)) (boolean? (cdar xs)) (eq? x (caar xs))) (cdar xs)]
      [else (where x (cdr xs))]))
  (match e
    [(val tf) tf]
    [(binop op e1 e2) ((op->proc op) (eval-e e1 xs) (eval-e e2 xs))]
    [(unop op e1) ((op->proc op) (eval-e e1 xs))]
    [(forall x e1) (and (eval-e e1 (cons (cons (var-x x) #t) xs)) (eval-e e1 (cons (cons (var-x x) #f) xs)))]
    [(exists x e1) (or (eval-e e1 (cons (cons (var-x x) #t) xs)) (eval-e e1 (cons (cons (var-x x) #f) xs)))]
    [(var x) (where (var-x x) xs)]))
    
    



