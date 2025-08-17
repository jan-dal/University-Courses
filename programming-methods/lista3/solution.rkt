#lang racket

(provide null null? cons pair? car cdr)


(define null
  (lambda (a)
    (cond [(= a 2) 0])))

(define (null? x)
  (if (procedure? x)
      (cond [(= 0 (x 2)) #t]
            [else #f])
      #f))

(define (cons x y)
  (lambda (a)
    (cond [(= a 0) x]
          [(= a 1) y]
          [(= a 2) 1])))

(define (pair? x)
  (if (procedure? x)
      (cond [(= 1 (x 2)) #t]
            [else #f])
      #f))

(define (car x)
  (x 0))

(define (cdr x)
  (x 1))

(define lista1 (cons 1 (cons 2 (cons 3 (cons 4 null)))))

(define(list-sum  xs)
  (if(null? xs)
     0
     (+ (car xs)(list-sum (cdr xs)))))

(define (last-element xs)
  (if (null? (cdr xs))
      (car xs)
      (last-element (cdr xs))))

(define (map-list f xs)
  (if (null? xs)
      xs
      (cons (f (car xs)) (map-list f (cdr xs)))))




(list-sum lista1)
(last-element lista1)
(define square (map-list (lambda (x) (* x x)) lista1))
(car square)
(car (cdr square))
(car (cdr (cdr square)))
(car (cdr (cdr (cdr square))))
