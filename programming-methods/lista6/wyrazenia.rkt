#lang racket
(struct const (val) #:transparent)
(struct binop (op l r) #:transparent)
(struct unop (op e) #:transparent)
(struct variable () #:transparent)
(define (expr? e)
  (match e
    [(const n) (number? n)]
    [(binop op l r) (and (symbol? op) (expr? l) (expr? r))]
    [(unop op e) (and (symbol? op) (expr? e))]
    [(variable) true]
    [_ false]))

(define (value? v) (number? v))

(define (op->proc op)
  (match op ['+ +] ['- -] ['* *] ['/ /] ['^ expt] ['abs abs] ['∂ ∂]))

(define (eval e x)
  (match e
    [(const n) n]
    [(binop op l r) ((op->proc op) (eval l x) (eval r x))]
    [(unop '∂ e) (eval (∂ e) x)]
    [(variable) x]))

(define (parse q)
  (cond [(number? q) (const q)]
        [(and (list? q) (eq? (length q) 2) (symbol? (first q))
         (unop (first q) (parse (second q))))]
        [(and (list? q) (eq? (length q) 3) (symbol? (first q)))
         (binop (first q) (parse (second q)) (parse (third q)))]
        [(eq? q 'x) (variable)]))

(define (pi-expr n)
  (define (iter n k e)
    (if (= n 0)
        (binop '+ (const 3) e)
        (if (const? e)
            (iter (- n 1) (- k 2) (binop '/ (binop '* (const k) (const k)) (const 6)))
            (iter (- n 1) (- k 2) (binop '/ (binop '* (const k) (const k)) (binop '+ (const 6) e))))))
  (iter n (+ (* 2 (- n 1)) 1) (const 0)))

(define (pretty-print e)
  (match e
    [(const n) (number->string n)]
    [(binop op l r) (string-append "(" (pretty-print l) (symbol->string op) (pretty-print r) ")")]
    [(unop op e) (string-append "|" (pretty-print e) "|")]))

(define (∂ e)
  (match e
    [(const n) (const 0)]
    [(variable) (const 1)]
    [(binop '+ l r) (binop '+ (∂ l) (∂ r))]
    [(binop '* l r) (binop '+ (binop '* (∂ l) r) (binop '* l (∂ r)))]
    [(unop '∂ e) (∂ (∂ e))]))

(define pi (parse '(+ x x)))
(define w (parse '(∂ (∂ (+ x (∂ (* x (* x x))))))))
