#lang racket
(define (insertsort xs)
  (define (sort xs acc)
    (define (insert ys v)
      (cond
        [(null? ys) (cons v null)]
        [(>= (car ys) v)
         (cons v ys)]
        [else
         (cons (car ys) (insert (cdr ys) v))]))
    (if (null? xs)
        acc
        (sort (cdr xs) (insert acc (car xs)))))
  (sort xs null))

(define (long-list n k acc)
  (if (= n 0)
      acc
      (long-list (- n 1) k (cons (- k n) acc))))



'(letrec (insertsort
          (lambda (x)
            (letrec (sort
                     (lambda (xs acc)
                       (letrec (insert
                                (lambda (ys v)
                                  (if (null? ys)
                                      (pair v null)
                                      (if (not (< (fst ys) v))
                                          (pair v ys)
                                          (pair (fst ys) (insert (snd ys) v))))))
                         (if (null? xs)
                             acc
                             (sort (snd xs) (insert acc (fst xs)))))))
              (sort x null))))
   (letrec (long-list (lambda (n k acc)
                        (if (= n 0)
                            acc
                            (long-list (- n 1) k (pair (- k n) acc)))))
     (insertsort (long-list 100 100 null))))