#lang plait
(define-type-alias Subst (Listof (Symbol * S-Exp)))
(define-type-alias Eq (S-Exp * S-Exp))
(define-type-alias Eqs (Listof Eq))


(define (zip xs ys)
  (if (or (empty? xs) (empty? ys))
      empty
      (cons (pair (first xs) (first ys)) (zip (rest xs) (rest ys)))))

(define (same? xs ys)
  (cond
    [(empty? xs) #t]
    [(and (equal? (fst (first xs)) (fst (first ys))) (equal? (snd (first xs)) (snd (first ys))))
     (same? (rest xs) (rest ys))]
    [else #f]))
    
(define (occurs x xs)
  (cond
    [(empty? xs) #f]
    [(s-exp-symbol? (first xs))
     (if (equal? x (first xs))
         #t
         (occurs x (rest xs)))]
    [(s-exp-list? (first xs))
     (or (occurs x (s-exp->list (first xs))) (occurs x (rest xs)))]
    [else (occurs x (rest xs))]))



(define (sublist x val xs)
  (type-case (Listof S-Exp) xs
    [empty '()]
    [(cons p ps)
     (cond
       [(s-exp-symbol? p)
        (if (equal? p x)
            (cons val (sublist x val ps))
            (cons p (sublist x val ps)))]
       [(s-exp-list? p)
        (let ([nl (sublist x val (s-exp->list p))])
          (append nl (sublist x val ps)))]
       [else (cons p (sublist x val ps))])]))



(define (substitute-single x val xs)
  (type-case Eqs xs
    [empty empty]
    [(cons p ps)
     (let ([el (fst p)]
           [er (snd p)])
       (cond
         [(and (s-exp-symbol? el) (equal? el x))
          (cons (pair val er) (substitute-single x val ps))]
         [(and (s-exp-symbol? er) (equal? er x))
          (cons (pair val el) (substitute-single x val ps))]
         [(s-exp-list? el) (cons (pair (list->s-exp (sublist x val (s-exp->list el))) er) (substitute-single x val ps))]
         [(s-exp-list? er) (cons (pair el (list->s-exp (sublist x val (s-exp->list er)))) (substitute-single x val ps))]
         [else (cons p (substitute-single x val ps))]))]))



(define (substitute-all xs newxs)
  (type-case Eqs xs
    [empty newxs]
    [(cons p ps)
     (let ([el (fst (first newxs))]
           [er (snd (first newxs))])
       (cond
         [(s-exp-symbol? el)
          (substitute-all ps (append (substitute-single el er (rest newxs)) (list (first newxs))))]
         [(s-exp-symbol? er)
          (substitute-all ps (append (substitute-single er el (rest newxs)) (list (first newxs))))]
         [else (substitute-all ps (append (rest newxs) (list (first newxs))))]))]))




; Empty - jeżeli istnieje rówanie sprzeczne
; Eqs - skrócone o równania 
(define (filterout xs acc pass)
  (type-case Eqs xs
    [empty acc]
    [(cons p ps)
     (let ([el (fst p)]
           [er (snd p)])
       (cond
         [(and (s-exp-number? el) (s-exp-number? er))
          (if (= (s-exp->number el) (s-exp->number er))
              (filterout ps acc pass)
              empty)]
         [(and (s-exp-boolean? el) (s-exp-boolean? er))
          (if (equal? (s-exp->boolean el) (s-exp->boolean er))
              (filterout ps acc pass)
              empty)]
         [(and (s-exp-list? el) (s-exp-list? er)
               (= (length (s-exp->list el)) (length (s-exp->list er))))
          (let* ([zp (zip (s-exp->list el) (s-exp->list er))]
                 [f (filterout zp empty pass)])
            (if (empty? f)
                empty
                (filterout ps (append acc f) pass)))]
         [(and (s-exp-symbol? el) (s-exp-symbol? er))
          (if (equal? (s-exp->symbol el) (s-exp->symbol er))
              (filterout ps acc pass)
              (if (= pass 2)
                  empty
                  (filterout ps (cons p acc) pass)))]
         [(and (s-exp-symbol? el) (s-exp-list? er))
          (if (occurs el (s-exp->list er))
              empty
              (filterout ps (cons p acc) pass))]
         [(and (s-exp-symbol? er) (s-exp-list? el))
          (if (occurs er (s-exp->list el))
              empty
              (filterout ps (cons p acc) pass))]
         [(and (not (s-exp-symbol? el)) (not (s-exp-symbol? er)))
          empty]
         [else (filterout ps (cons p acc) pass)]))]))
               
(define test (list (pair `3 `3) (pair `x `3) (pair `(p t y z 5 (z 4 t y)) `((3 4) (z y) 3 4 5 (v 4 t y)))))
(define test1 (list (pair `x `1)))
(define test2 (list (pair `x `1) (pair `y `#t) (pair `(z q) `(x y))))
(define test3 (list (pair `x `1) (pair `y `#t) (pair `(z z) `(x y))))
(define test4 (list (pair `x `y) (pair `x `3) (pair `(z x p) `(y 3 (x y z))) (pair `(3 3 3) `p)))


(define (unify eqs)
  (local ([define (iter xs)
             (let* ([sub (filterout xs empty 1)]
                    [p (filterout (substitute-all sub sub) empty 2)])
               (if (and (= (length p) (length xs)) (same? xs p))
                   p
                   (iter p)))])
    (let ([p (iter eqs)])
      (type-case Eqs p
        [empty (none)]
        [(cons d ds)
         (local [(define (iter xs)
                   (if (empty? xs)
                       empty
                       (let ([el (fst (first xs))]
                             [er (snd (first xs))])
                         (cond
                           [(s-exp-symbol? el) (cons (pair (s-exp->symbol el) er) (iter (rest xs)))]
                           [(s-exp-symbol? er) (cons (pair (s-exp->symbol er) el) (iter (rest xs)))]))))]
           (some (iter p)))]))))
