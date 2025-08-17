#lang racket

(provide cube-root)

(define (cube-root x)

  (define (abs z)
    (cond [(>= z 0) z]
          [else (- z)]))
  
  (define (better y)
    (/ (+ (/ x (* y y)) (+ y y)) 3))
  
  (define (cube-iter y)
    (if (ok? y) y
        (cube-iter (better y))))
  
  (define (ok? y)
    (< (abs(- (* y y y) x)) 0.001))

  
  (cube-iter 1.0))

(cube-root 8)     ;2
(cube-root -8)    ;-2
(cube-root -64)   ;-4
(cube-root 121)   ;4.9460874...
(cube-root 4096)  ;16
(cube-root 9.78)  ;2.1385182...
(cube-root 0)     ;0
