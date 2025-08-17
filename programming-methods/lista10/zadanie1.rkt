#lang racket

;; Składnia wyrażeń
(struct const    (val)           #:transparent)
(struct unop     (op expr)       #:transparent)
(struct binop    (op left right) #:transparent)
(struct lbinop   (op left right) #:transparent) ; leniwe operatory logiczne
(struct var-expr (id)            #:transparent)

(define (op-bin? op)
  (member op '(+ - * / = < > <=)))
(define (op-log? op)
  (member op '(and or)))
(define (op-un? op)
  (member op '(not -)))

(define (reserved? s)
  (or (op-bin? s)
      (op-un?  s)
      (member s '(if skip while true false quit)))) ; quit zarezerwowane dla repla

(define (name? s)
  (and (symbol? s)
       (not (reserved? s))))

(define (expr? e)
  (match e
    [(const v)     (number? v)]
    [(unop op e)   (and (op-un? op) (expr? e))]
    [(binop op el er)
     (and (op-bin? op)
          (expr? el)
          (expr? er))]
    [(lbinop op el er)
     (and (op-log? op)
          (expr? el)
          (expr? er))]
    [(var-expr x)  (name? x)]))


;; (for (initial) (cond) (next) (body))
;; Składnia poleceń
(struct assign     (left right) #:transparent)
(struct while-comm (expr body)  #:transparent)
(struct if-comm    (expr ct cf) #:transparent)
(struct comp-comm  (left right) #:transparent)
(struct skip       ()           #:transparent)
(struct for-comm   (init con next body) #:transparent)
(struct break      ()           #:transparent)

(define (command? c)
  (match c
    [(assign x e)
     (and (name? x)
          (expr? e))]
    [(while-comm e c)
     (and (expr? e)
          (command? c))]
    [(if-comm e ct cf)
     (and (expr? e)
          (command? ct)
          (command? cf))]
    [(comp-comm c1 c2)
     (and (command? c1)
          (command? c2))]
    [(skip) true]
    [_ false]))


;; Parser
(define (tagged-tuple? tag len tup)
  (and (list? tup)
       (pair? tup)
       (eq? tag (car tup))
       (= len (length (cdr tup)))))

(define (parse-expr e)
  (cond
   [(number? e) (const e)]
   [(symbol? e) (var-expr e)]
   [(and (list? e)
         (= 2 (length e))
         (op-un? (car e)))
    (unop (car e) (parse-expr (cadr e)))]
   [(and (list? e)
         (= 3 (length e))
         (op-bin? (car e)))
    (binop (car e) (parse-expr (cadr e)) (parse-expr (caddr e)))]
   [(and (list? e)
         (= 3 (length e))
         (op-log? (car e)))
    (lbinop (car e) (parse-expr (cadr e)) (parse-expr (caddr e)))]))

(define (make-comp c cs)
  (if (null? cs)
      c
      (comp-comm c (make-comp (car cs) (cdr cs)))))

(define (parse-comm c)
  (cond
   [(and (list? c)
         (= 1 (length c))
         (eq? (car c) 'break)
         (break))]
   [(and (list? c)
         (= 5 (length c))
         (eq? (first c) 'for)
         (for-comm (parse-comm (second c)) (parse-expr (third c)) (parse-comm (fourth c)) (parse-comm (fifth c))))]
   [(and (list? c)
         (= 3 (length c))
         (eq? (cadr c) ':=)
         (name? (car c)))
    (assign (car c) (parse-expr (caddr c)))]
   [(and (list? c)
         (= 3 (length c))
         (eq? (car c) 'while))
    (while-comm (parse-expr (cadr c)) (parse-comm (caddr c)))]
   [(and (list? c)
         (= 4 (length c))
         (eq? (car c) 'if))
    (if-comm (parse-expr (cadr c))
             (parse-comm (caddr c))
             (parse-comm (cadddr c)))]
   [(and (list? c)
         (= 3 (length c))
         (eq? (car c) 'if))
    (if-comm (parse-expr (cadr c))
             (parse-comm (caddr c))
             (skip))]
   [(eq? c 'skip) (skip)]
   ;; na koniec -- złożenie poleceń
   [(and (list? c)
         (pair? c))
    (make-comp (parse-comm (car c)) (map parse-comm (cdr c)))]))


;; Ewaluator

(define (op->proc op)
  (match op
    ['+   +]
    ['-   -] ; zarówno unarny jak i binarny, Racket sobie poradzi
    ['*   *]
    ['/   /] ; wypadałoby obsłużyć dzielenie przez 0, np. tak jak w języku letrec
    ['not not]
    ['=   (lambda (x y) (if (= x y) 1 0))]
    ['>   (lambda (x y) (if (> x y) 1 0))]
    ['<   (lambda (x y) (if (< x y) 1 0))]
    ['<=  (lambda (x y) (if (<= x y) 1 0))]))

(define (value? v)
  (number? v))

;; pamięć przechowuje aktualną wartość dla każdej zmiennej
;; przyjmujemy że nieznane zmienne mają wartość 0
(struct mem (assoc) #:transparent)
(define (mem-lookup m x)
  (define (aux xvs)
    (cond
     [(null? xvs)        0]
     [(eq? (caar xvs) x) (cdar xvs)]
     [else               (aux (cdr xvs))]))
  (aux (mem-assoc m)))
(define (mem-update m x v)
  (define (aux xvs)
    (cond
     [(null? xvs)        (list (cons x v))]
     [(eq? (caar xvs) x) (cons (cons x v) (cdr xvs))]
     [else               (cons (car xvs)  (aux (cdr xvs)))]))
  (mem (aux (mem-assoc m))))

;; Ewaluator dla wyrażeń przekształca wyrażenie i aktualny stan pamięci w wartość
(define (eval-expr e m)
  (match e
    [(const v)  v]
    [(unop op e)
     ((op->proc op) (eval-expr e m))]
    [(binop op e1 e2)
     (let ((v1 (eval-expr e1 m))
           (v2 (eval-expr e2 m)))
       ((op->proc op) v1 v2))]
    [(lbinop 'and e1 e2)
     (if (= 0 (eval-expr e1 m))
         0
         (eval-expr e2 m))]
    [(lbinop 'or e1 e2)
     (if (= 1 (eval-expr e1 m))
         1
         (eval-expr e2 m))]
    [(var-expr x)
     (mem-lookup m x)]))


;; Ewaluator dla poleceń przekształca program i początkowy stan
;; pamięci w końcowy stan pamięci
(define (eval-comm c m)
  (match c
    [(skip)    m]
    [(break)   (cons ('break m))]
    [(for-comm ini con next body)
     (let* ((v (eval-comm ini m))
           (bol (eval-expr con v)))
       (if (= bol 0)
           m
           (let ((b (eval-comm body v)))
             (if (and
                  (pair? b)
                  (eq? 'break (car b)))
                 (cdr b)
                 (eval-comm (for-comm next con next body) b)))))]
    [(comp-comm c1 c2)
     (let* ((m1 (eval-comm c1 m))
            (m2 (eval-comm c2 m1)))
       m2)]
    [(if-comm e ct cf)
     (let ((v (eval-expr e m)))
       (if (= v 0)
           (eval-comm cf m)
           (eval-comm ct m)))]
    [(while-comm e cb)
     (let ((v (eval-expr e m)))
       (if (= v 0)
           m
           (let ((m1 (eval-comm cb m)))
             (eval-comm c m1))))]
    [(assign x e)
     (let ((v (eval-expr e m)))
       (mem-update m x v))]))

(define test (parse-comm '((x := (+ 1 2)) (while (< x 10) (x := (+ x x ))))))


(define (fibonnaci n)
  (parse-comm `((x0 := 0) (x1 := 1) (s := 0) (n := ,n) (while (> n 0) ((n := (- n 1)) (s := (+ x0 x1)) (x0 := x1) (x1 := s))))))

(define (silnia n)
  (parse-comm `((n := 1) (for (x := 1) (<= x ,n) (x := (+ x 1)) (if (= x 10) (break) (n := (* n x)))))))
