#lang racket
(define (power-close-to b n)
  (define (is_bigger? e)
    (> (expt b e) n)
    )
  (define (is_smaller? e)
    (< (expt b e) n)
    )
  (define (iter e)
    (cond [(and (is_bigger? e) (is_bigger? (- e 1)))
           (iter (- e 1))]
          [(is_smaller? e)
           (iter (+ e 1))]
          [else e])
    )
  
  (iter 1))