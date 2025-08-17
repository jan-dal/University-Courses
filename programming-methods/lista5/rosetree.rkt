#lang racket

(define (leaf v) (cons 'leaf v))

(define (leaf? t)                
  (and (pair? t) (eq? 'leaf (car t))))

(define (node v r n)               ; konstruktor węzła
  (list 'node v r n))

(define (node? t)                  ; predykat
  (and (list? t)
       (= (length t) 4)
       (eq? (first t) 'node)))

(define (sel-node t n)               ; selektory
  (if (= n 0)
      (car n)
      (sel-node (cdr t) (- n 1))))
      


(define (tree? t)                  ; predykat definiujący
  (define (iter nodes)
    (if (tree?
  (or (leaf? t)                    ; nasz główny typ danych!
      (and (node? t)
           (iter nodes)))
          