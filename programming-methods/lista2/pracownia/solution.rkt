#lang racket

(provide integral)

(define (sum val next a b)
  (if (> a b)
      0
      (+ (val a) (sum val next (next a) b))))


(define (integral f prec)
  (lambda (a b)

    (let* ([przedzial (/ (abs(- b a)) (abs prec))]
           [prostokat (lambda (x) (* przedzial (f x)))]
           [next (lambda (x) (+ przedzial x))] 
           )
      (cond [(< a b) (sum prostokat next a (- b przedzial))]
            [(> a b) (- (sum prostokat next b (- a przedzial)))]
            [else 0]
            )
      )
    )
  )

(define isin (integral sin 1000))
(define ix (integral (lambda (x) x) 1000))
(define i0 (integral (lambda (x) 1) 1000))
(define itan (integral tan 1000))

(isin 0 pi)
(isin (- pi) (* 2 pi))
(isin (* 2 pi) pi)
(isin pi 0)
(isin pi (- (* 2 pi)))
(ix 0.0 10)
(ix -10.0 0)
(ix -20.0 -10)
(i0 -100 100)
(i0 1 1)
(i0 -2.0 1/2)
(i0 0.0 -1)
(i0 -1 0.0)
(itan (/ pi -2) (/ pi 2))
(itan 0 (/ pi 2)) ; Metoda nie przydaje się do liczenia całek nie zbieżnych