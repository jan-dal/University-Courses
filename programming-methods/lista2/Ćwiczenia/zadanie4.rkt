#lang racket



(define (f x)
  (if (= (modulo x 2) 1)
      (/ (+ x 1) (+ x 2))
      (/ (+ x 2) (+ x 1))
      )
  )

(define (prod-rek val next a b)
  (if (> a b)
      1
      (* (val a) (prod-rek val next (next a) b))
      )
  )


(define (prod-iter val next mult a b)
  (if (> a b)
      mult 
      (prod-iter val next (* mult (val a)) (next a) b)
      )
  )


(define (accumulate-rek combiner null-value term a next b)
  (if (> a b)
      null-value
      (combiner
       (term a) (accumulate-rek combiner null-value term (next a) next b)
       )
      )
  )

(define (accumulate-iter combiner null-value term a next b)
  (if (> a b)
      null-value
      (accumulate-iter combiner (combiner null-value (term a)) term (next a) next b)
      )
  )



