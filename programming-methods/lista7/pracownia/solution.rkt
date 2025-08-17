#lang racket

(provide (struct-out const) (struct-out binop) (struct-out var-expr)
		 (struct-out let-expr) (struct-out var-free) (struct-out var-bound) 
		 (struct-out pos) annotate-expression)

;; ---------------
;; Jezyk wejsciowy
;; ---------------

(struct pos (file line col)     #:transparent)
  
(struct const    (val)          #:transparent)
(struct binop    (op l r)       #:transparent)
(struct var-expr (id)           #:transparent)
(struct let-expr (loc id e1 e2) #:transparent)

(define (expr? e)
  (match e
    [(const n)      (number? n)]
    [(binop op l r) (and (symbol? op) (expr? l) (expr? r))]
    [(var-expr x)   (symbol? x)]
    [(let-expr loc x e1 e2)
     (and (pos? loc) (symbol? x) (expr? e1) (expr? e2))]
    [_ false]))

(define (make-pos s)
  (pos (syntax-source s)
       (syntax-line   s)
       (syntax-column s)))

(define (parse e)
  (let ([r (syntax-e e)])
    (cond
      [(number? r) (const r)]
      [(symbol? r) (var-expr r)]
      [(and (list? r) (= 3 (length r)))
       (match (syntax-e (car r))
         ['let (let* ([e-def (syntax-e (second r))]
                      [x     (syntax-e (first e-def))])
                 (let-expr (make-pos (first e-def))
                           (if (symbol? x) x (error "parse error!"))
                           (parse (second e-def))
                           (parse (third r))))]
         [op   (binop op (parse (second r)) (parse (third r)))])]
      [else (error "parse error!")])))

;; ---------------
;; Jezyk wyjsciowy
;; ---------------

(struct var-free  (id)     #:transparent)
(struct var-bound (pos id) #:transparent)

(define (expr-annot? e)
  (match e
    [(const n)         (number? n)]
    [(binop op l r)    (and (symbol? op) (expr-annot? l) (expr-annot? r))]
    [(var-free x)      (symbol? x)]
    [(var-bound loc x) (and (pos? loc) (symbol? x))]
    [(let-expr loc x e1 e2)
     (and (pos? loc) (symbol? x) (expr-annot? e1) (expr-annot? e2))]
    [_ false]))

(define (annotate-expression e)
  (define (search xs id)
    (if (null? xs)
        (cons null #f)
        (if (eq? id (caar xs))
            (cons (cdar xs) #t)
            (search (cdr xs) id))))

  (define (annotate e xs)
    (match e
      [(const n) (const n)]
      [(binop op e1 e2) (binop op (annotate e1 xs) (annotate e2 xs))]
      [(pos file line col) (pos file line col)]
      [(var-expr id) (let ([p (search xs id)]) (if (cdr p) (var-bound (car p) id) (var-free id)))]
      [(let-expr loc id e1 e2) (let-expr loc id (annotate e1 xs) (annotate e2 (cons (cons id loc) xs)))]
      [_ false]))
  (annotate e null))

(define test0 (parse #'(+ (* 2 3) x)))
(define test1 (parse #'(let [x 5] (* y x))))
(define test2 (parse #'(+ (let [x (* z 3)] (* y x)) (let [x 2] (+ (let [x 3] 4) (let [y (+ x 3)] 3))))))
