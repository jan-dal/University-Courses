#lang plait

; wyrażenia

(define-type ArithExpr
  (const [val : Number])
  (binop [op : Symbol] [l : ArithExpr] [r : ArithExpr])
  (var-expr [id : Symbol])
  (let-expr [id : Symbol] [e1 : ArithExpr] [e2 : ArithExpr])
  (if-expr [eb : ArithExpr] [et : ArithExpr] [ef : ArithExpr]))

(define-type Type
  (number-type)
  (boolean-type))

(define binary-operators '(+ - * / = < and or))

(define (binary-operator? op)
  (member op binary-operators))
(define (arith-op? op)
  (member op '(+ - * /)))
(define (bool-op? op)
  (member op '(and or)))
(define (comp-op? op)
  (member op '(= <)))

(define (typecheck expr)
  (type-check-env expr empty))

(define (type-check-env expr type-env)
  (type-case ArithExpr expr
    [(const n) (some (number-type))]      ; Stała jest typu liczbowego
    [(binop op el er)
     (let ((l (type-check-env el type-env))       ; Sprawdza typy podwyrażeń
           (r (type-check-env er type-env)))
       (cond
         [(or (none? l) (none? r)) (none)]
         [(and (equal? (some-v l) (number-type))            ; Jeśli dwa typy liczbowe to
               (equal? (some-v r) (number-type)))           ; zwraca odpowiedni typ na podst.
               (cond                                        ; predykatu
                 [(comp-op? op) (some (boolean-type))]
                 [(arith-op? op) (some (number-type))]
                 [else (none)])]
         [(and (equal? (some-v l) (boolean-type))      ; Jeśli dwa typy boolowskie
               (equal? (some-v r) (boolean-type))      ; Zwraca typ boolowski (jeśli op. log.)
               (bool-op? op))
          (some (boolean-type))]
         [else (none)]))]
    [(var-expr x) (env-type-lookup x type-env)]   ; Zwraca typ zapisany w środowisku lub none jeśli go nie ma
    [(let-expr id e1 e2)
     (let ((el (type-check-env e1 type-env)))         ; Oblicza typ zmiennej
       (cond
         [(none? el) (none)]
         [else (type-check-env e2 (env-add id el type-env))]))] ; Oblicza typ ciała z typem zmiennej w środowisku
    [(if-expr eb et ef)
     (let ((b (type-check-env eb type-env))
           (t (type-check-env et type-env))
           (f (type-check-env ef type-env))) ; Sprawdza typy wyrażeń eb et ef
       (cond
         [(none? b) (none)]
         [(and (equal? (some-v b) (boolean-type)) ; eb musi być typu boolean
               (equal? t f))                      ; et ef muszą być tego samego typu
          t]
         [else (none)]))]))

(define (parse q)
  (cond
    [(s-exp-number? q) (const (s-exp->number q))]
    [(s-exp-symbol? q) (var-expr (s-exp->symbol q))]
    [(s-exp-list? q)
     (let ([ql (s-exp->list q)])
       (cond
         [(and (= (length ql) 3)
               (equal? (first ql) `let))
          (let ([ll (s-exp->list (second ql))])
            (let-expr (s-exp->symbol (first ll))
                     (parse (second ll))
                      (parse (third ql))))]
         [(and (= (length ql) 4)
               (equal? (first ql) `if))
          (if-expr (parse (second ql))
                   (parse (third ql))
                   (parse (fourth ql)))]
         [(and (= (length ql) 3)
               (binary-operator? (s-exp->symbol (first ql))))
          (binop (s-exp->symbol (first ql))
                 (parse (second ql))
                 (parse (third ql)))]))]))


; środowiska

(define-type-alias (Env 'a) (Listof (Symbol * 'a)))

(env-empty : (Env 'a))
(define env-empty empty)

(define (env-add x v env) (cons (pair x v) env))

(define (env-type-lookup x env)
  (type-case (Env Type) env
    [empty (none)]
    [(cons p ps)
     (if (eq? (fst p) x)
         (snd p)
         (env-type-lookup x (rest env)))]))


(define (env-lookup x env)
  (type-case (Env 'a) env
    [empty (error 'env-lookup (string-append "Unknown identifier "
                                             (to-string x)))]
    [(cons p ps)
     (if (eq? (fst p) x)
         (snd p)
         (env-lookup x (rest env)))]))

; ewaluacja

(define-type Value
  (number-val [v : Number])
  (boolean-val [v : Boolean]))

(define (arith-op f)
  (lambda (x y)
    (number-val (f (number-val-v x) (number-val-v y)))))

(define (comp-op f)
  (lambda (x y)
    (boolean-val (f (number-val-v x) (number-val-v y)))))

(define (bool-op f)
  (lambda (x y)
    (boolean-val (f (boolean-val-v x) (boolean-val-v y)))))

(define (op->proc op)
  (case op
    ['+ (arith-op +)]
    ['- (arith-op -)]
    ['* (arith-op *)]
    ['/ (arith-op /)]
    ['= (comp-op =)]
    ['< (comp-op <)]
    ['and (bool-op (lambda (x y) (and x y)))]
    ['or (bool-op (lambda (x y) (or x y)))]))

(define (eval-env e env)
  (type-case ArithExpr e
    [(const n) (number-val n)]
    [(binop op l r) ((op->proc op) (eval-env l env) (eval-env r env))]
    [(var-expr x) (env-lookup x env)]
    [(let-expr x e1 e2) (eval-env e2 (env-add x (eval-env e1 env) env))]
    [(if-expr eb et ef)
     (if (boolean-val-v (eval-env eb env))
         (eval-env et env)
         (eval-env ef env))]))

(define (eval e) (eval-env e env-empty))
