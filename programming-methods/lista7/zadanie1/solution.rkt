#lang racket

(provide (struct-out const) (struct-out binop) rpn->arith)

;; -------------------------------
;; Wyrazenia w odwr. not. polskiej
;; -------------------------------

(define (rpn-expr? e)
  (and (list? e)
       (pair? e)
       (andmap (lambda (x) (or (number? x) (member x '(+ - * /))))
               e)))

;; ----------------------
;; Wyrazenia arytmetyczne
;; ----------------------

(struct const (val)    #:transparent)
(struct binop (op l r) #:transparent)

(define (arith-expr? e)
  (match e
    [(const n) (number? n)]
    [(binop op l r)
     (and (symbol? op) (arith-expr? l) (arith-expr? r))]
    [_ false]))

;; ----------
;; Kompilacja
;; ----------

(define (rpn->arith e)
  (define (eval-am e s)
  (cond
   [(and (null? e)
         (cons? s)
         (null? (cdr s)))
    (car s)]
   [(null? e)
    (error "Wrong number of arguments on the stack")]
   [(number? (car e))
    (eval-am (cdr e) (cons (const (car e)) s))]
   [(and (operator? (car e))
         (or (null? s)
             (null? (cdr s))))
    (error "Wrong number of arguments on the stack")]
   [(operator? (car e))
    (let* ((x (car  s))
           (y (cadr s))
           (s (cddr s))
           (s (cons (binop (car e) y x) s)))
      (eval-am (cdr e) s))]))

   (define (run-am e)
      (eval-am e null))
   (run-am e))

; Mozesz tez dodac jakies procedury pomocnicze i testy

(define (operator? x)
  (if (list? (member x '(+ - / *))) #t #f))

(define rpn '(8 2 3 + *))