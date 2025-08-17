#lang racket

(provide caesar)

(define (caesar alphabet key)

  ; Długość listy 
  
  (define (lenght xs)
    (if (null? xs)
        0
        (+ 1 (lenght (cdr xs)))))

  ; Obróć listę cyklicznie w prawo 
     
  (define (rotate xs key)

    (define shift (modulo (- key) (lenght xs)))

    (define (iter xs ps k d)

      (cond [(and (= k 0) (not (null? ps)))
             (cons (car ps) (iter xs (cdr ps) 0 0))]
            [(and (null? ps) (< d shift))
             (cons (car xs) (iter (cdr xs) null 0 (+ 1 d)))]
            [(and (null? ps) (>= d shift)) null]
            [else (iter xs (cdr ps) (- k 1) 0)]))
    
    (iter xs xs shift 0))

  ; Połącz elementy na tych samych indeksach
  
  (define (zip xs ys)
    (if (null? xs)
        null
        (cons (cons (car xs) (car ys)) (zip (cdr xs) (cdr ys)))))


  ; Stwórz listę par kod-litera
  
  (define pary
    (zip (rotate alphabet key) alphabet))

  
  (define (encode x)
    (define (iter xs)
      (if (null? xs)
          null
          (if (eq? (caar xs) x)
              (cdar xs)
              (iter (cdr xs)))))
    (iter pary))

  (define (decode x)
    (define (iter xs)
      (if (null? xs)
          null
          (if (eq? (cdar xs) x)
              (caar xs)
              (iter (cdr xs)))))
    (iter pary))
  
  (cons encode decode ))
