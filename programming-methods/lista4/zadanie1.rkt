#lang racket

(define (foldr f i xs)
  (if (null? xs)
      i
      (f (car xs)
         (foldr f i (cdr xs)))))


(define xs (list 1 2 3 4 5 6 7 6 7))
(define ys (list 2 4 6 8 10 12 14 16))

(define (elem x xs)
  (if (null? xs)
      #f
      (if (eq? x (car xs))
          #t
          (elem x (cdr xs)))))

(define (elemf x xs)
  (foldr (lambda (a b) (if (eq? x a) #t b)) #f xs))

(define (filter p xs)
  (if (null? xs)
      null
      (if (p (car xs))
          (cons (car xs) (filter p (cdr xs)))
          (filter p (cdr xs)))))
(define (filterf p xs)
  (foldr (lambda (a b) (if (p a) (cons a b) b)) null xs))

(define (maximum xs)
  (if (null? xs)
      (error "Error")
      (cond [(null? (cdr xs)) (car xs)]
            [(> (car xs) (maximum (cdr xs))) (car xs)]
            [else (maximum (cdr xs))])))

(define (maximumf xs)
  (foldr (lambda (a b) (cond [(null? (cdr xs)) a]
                             [(> a b) a]
                             [else b])) (error "Error") xs))

(define (zip xs ys)
  (if (null? xs)
      null
      (cons (cons (car xs) (car ys)) (zip (cdr xs) (cdr ys)))))

(define (every-other xs)
  (if (null? xs)
      null
      (cons (car xs) (if (not (null? (cdr xs))) (every-other (cddr xs)) null))))
      

(define (every-otherf xs)
  (foldr (lambda (a b) (cons a (cdr b) ))) null xs)

