#lang racket

; --------- ;
; Wyrazenia ;
; --------- ;


;; Potęgowanie a^b -> (^ a b) '^ oblicza się do expt
;; Suma wartości kolejnych wartości funkcji od i od n naturalnych

;; (let-expr (variable a b) (expr?))
;; Oblicza się do (binop (let (variable a) f) (binop (let (variable (+ 1 a))) ...
;; Let wyrażenie gdzie związana wartość jest iterowana przy obliczaniu

;; Całka oznaczona od a b -> (integral a b expr?)
;; Oblicza się od funkcji obliczającej całkę z expr? na przedziale a b

;; Minimum zbioru (min expr?)




(struct const    (val)    #:transparent)
(struct binop    (op l r) #:transparent)
(struct var-expr (n)      #:transparent)
(struct let-expr (e1 e2)  #:transparent)
(struct function (var exp) #:transparent)


(struct sum-expr (a b f) #:transparent)
(struct integral (a b f) #:transparent)
(struct minset (f) #:transparent)

(define (operator? x)
  (member x '(+ - * / ^)))

(define (expr? e)
  (match e
    [(const n) (number? n)]
    [(binop op l r) (and (operator? op) (expr? l) (expr? r))]
    [(var-expr x) (number? x)]
    [(let-expr e1 e2) (and (expr? e1) (expr? e2))]
    [(function var exp) (and (expr? var) (expr? exp))]
    [(minset f) (expr? f)]
    [(sum-expr a b f) (and (expr? a) (expr? b) (expr? f))]
    [_ false]))

;; parsowanie konwertuje nazwy zmiennych do reprezentacji de Bruijna
(define (parse q)
  (define (which x xs)
    (cond [(null? xs) (error "Unknown variable")]
          [(eq? x (car xs)) 0]
          [else (+ 1 (which x (cdr xs)))]))
  (define (parse-env q xs)
    (cond
      [(number? q) (const q)]
      [(symbol? q) (var-expr (which q xs))]
      [(and (list? q) (eq? (length q) 3) (eq? (first q) 'let))
       (let-expr (parse-env (second (second q)) xs)
                 (parse-env (third q) (cons (first (second q)) xs)))]
      [(and (list? q) (eq? (length q) 3) (operator? (first q)))
       (binop (first q)
              (parse-env (second q) xs)
              (parse-env (third q)  xs))]
      [(and (list? q) (eq? (length q) 4) (eq? (first q) 'sum)
            (and (number? (second q)) (number? (third q))))
       (sum-expr (const (second q)) (const (third q)) (parse-env (fourth q) xs))]
      [(and (list? q) (eq? (length q) 4) (eq? (first q) 'integral))
       (integral (const (second q)) (const (third q)) (parse-env (fourth q) xs))]
      [(and (list? q) (eq? (length q) 2) (eq? (first q) 'min))
       (minset (parse-env (second q) xs))]
      [(and (list? q) (eq? (length q) 3) (eq? (first q) 'f) (symbol? (second q)))
       (function (parse-env (third q) (cons (second q) xs)) (second q))]))
  (parse-env q null))

(define (test-parse) (parse '(let [x (let [z 2] (^ z 3))] (let [y 7] (+ x y)))))
(define test-new (parse '(min (integral 3 4 (f x (+ x (let [y 3] (* y x))))))))
; ---------- ;
; Srodowiska ;
; ---------- ;

(define (n-th n xs)
  (cond [(null? xs) (error "Unknown variable")]
        [(eq? n 0) (car xs)]
        [else (n-th (- n 1) (cdr xs))]))

(define env-empty  null)
(define env-add    cons)
(define env-lookup n-th)

; --------- ;
; Ewaluacja ;
; --------- ;

(define (value? v)
  (number? v))

(define (op->proc op)
  (match op ['+ +] ['- -] ['* *] ['/ /] ['^ expt]))

(define (eval-env e env)
  (match e
    [(const n) n]
    [(binop op l r) ((op->proc op) (eval-env l env)
                                   (eval-env r env))]
    [(let-expr e1 e2)
     (eval-env e2 (env-add (eval-env e1 env) env))]
    [(var-expr n) (env-lookup n env)]))

(define (eval e) (eval-env e env-empty))

(define (test-eval) (eval (parse '(let [x 5] (let [y (let [z (+ 5 x)] z)] (+ y x))))))
