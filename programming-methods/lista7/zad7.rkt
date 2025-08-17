#lang racket


; --------- ;
; Wyrazenia ;
; --------- ;

(struct const    (val)      #:transparent)
(struct binop    (op l r)   #:transparent)
(struct var-expr (id)       #:transparent)
(struct let-expr (id e1 e2) #:transparent)

(define (operator? x)
  (member x '(+ - * /)))

(define (expr? e)
  (match e
    [(const n) (number? n)]
    [(binop op l r) (and (operator? op) (expr? l) (expr? r))]
    [(var-expr x) (symbol? x)]
    [(let-expr x e1 e2) (and (symbol? x) (expr? e1) (expr? e2))]
    [_ false]))

(define (parse q)
  (cond
    [(number? q) (const q)]
    [(symbol? q) (var-expr q)]
    [(and (list? q) (eq? (length q) 3) (eq? (first q) 'let))
     (let-expr (first (second q))
               (parse (second (second q)))
               (parse (third q)))]
    [(and (list? q) (eq? (length q) 3) (operator? (first q)))
     (binop (first q)
            (parse (second q))
            (parse (third q)))]))

(define (test-parse) (parse '(let [x (+ 2 2)] (+ x 1))))

; ---------- ;
; Srodowiska ;
; ---------- ;

(struct environ (xs))

(define env-empty (environ null))
(define (env-add x v env)
  (environ (cons (cons x v) (environ-xs env))))
(define (env-lookup x env)
  (define (assoc-lookup xs)
    (cond [(null? xs) (error "Unknown identifier" x)]
          [(eq? x (car (car xs))) (cdr (car xs))]
          [else (assoc-lookup (cdr xs))]))
  (assoc-lookup (environ-xs env)))

; --------- ;
; Ewaluacja ;
; --------- ;

(define (value? v)
  (number? v))

(define (op->proc op)
  (match op ['+ +] ['- -] ['* *] ['/ /]))

(define (eval-env e env)
  (match e
    [(const n) n]
    [(binop op l r) ((op->proc op) (eval-env l env)
                                   (eval-env r env))]
    [(let-expr x e1 e2)
     (eval-env e2 (env-add x (eval-env e1 env) env))]
    [(var-expr x) (env-lookup x env)]))

(define (eval e) (eval-env e env-empty))

(define test-eval (eval (parse '(let [x 5] (let [y (let [z (+ 5 x)] z)] (+ y x))))))
(define test0 (parse '(let [x 5] (+ 3 5))))
(define test1 (let-expr'x (binop'+ (const  2) (const  2))(let-expr'y (binop'* (const  3) (var-expr'x))(binop'+ (const  7) (var-expr'x)))))
(define test2 (parse '(let [x 4] (let [x (+ x 2)] (+ 2 (let (+ y x) (+ 8 9)))))))

; ------- ;
; Zadanie ;
; ------- ;

(define (optimize e)

  (define (search e id)
    (match e
      [(const n) #f]
      [(binop op e1 e2) (or (search e1 id) (search e2 id))]
      [(var-expr x) (eq? x id)]
      [(let-expr x e1 e2) (if (eq? x id)
                               (search e1 id)
                               (or (search e1 id) (search e2 id)))]
      [_ false]))
  
    (match e
      [(const n) (const n)]
      [(binop op e1 e2) (binop op (optimize e1) (optimize e2))]
      [(var-expr id) (var-expr id)]
      [(let-expr id e1 e2) (if (search (optimize e2) id)
                               (let-expr id (optimize e1) (optimize e2))
                               (optimize e2))]
      [_ false]))


