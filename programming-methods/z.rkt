#lang racket

(define xs (list 1 2 3 4 5))

(define (lenght xs)
  (if (null? xs)
      0
      (+ 1 (lenght (cdr xs)))))

(define (rotate xs key)

  (define shift (modulo key (lenght xs)))

  (define (iter xs ps k d)

    (cond [(and (= k 0) (not (null? ps)))
           (cons (car ps) (iter xs (cdr ps) 0 0))]
          [(and (null? ps) (< d shift))
           (cons (car xs) (iter (cdr xs) null 0 (+ 1 d)))]
          [(and (null? ps) (>= d shift)) null]
          [else (iter xs (cdr ps) (- k 1) 0)]))
    
  (iter xs xs shift 0))
    

(define (zip xs ys)
    (if (null? xs)
        null
        (cons (cons (car xs) (car ys)) (zip (cdr xs) (cdr ys)))))



