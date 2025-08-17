#lang racket
(struct bdlist (v [prev #:mutable] [next #:mutable]) #:transparent)

(define (list->bdlist xs) 
  (define (make-bdlist xs beg p)
    (cond
      [(null? xs) beg]
      [(null? p)
       (let ((p (bdlist (car xs) null null)))
         (make-bdlist (cdr xs) p p))]
      [else
       (let ((q (bdlist (car xs) null null)))
         (begin
           (set-bdlist-next! p q)
           (set-bdlist-prev! q p)
           (make-bdlist (cdr xs) beg q)))]))
  (make-bdlist xs null null))

(define (bdfilter pred bdxs)
  (define (filter bdxs beg p)
    (cond
      [(null? bdxs) beg]
      [(and (pred (bdlist-v bdxs)) (null? p))
       (let ((p (bdlist (bdlist-v bdxs) null null)))
         (filter (bdlist-next bdxs) p p))]
      [(pred (bdlist-v bdxs))
       (let ((q (bdlist (bdlist-v bdxs) null null)))
         (begin
           (set-bdlist-next! p q)
           (set-bdlist-prev! q p)
           (filter (bdlist-next bdxs) beg q)))]
      [else
       (filter (bdlist-next bdxs) beg p)]))
  (filter bdxs null null))

(define (cycle-bdlist! bdxs)
  (define (cycle bdxs beg)
    (cond
      [(null? bdxs) null]
      [(null? beg)
       (cycle (bdlist-next bdxs) bdxs)]
      [(null? (bdlist-next bdxs))
       (begin
         (set-bdlist-next! bdxs beg)
         (set-bdlist-prev! beg bdxs))]
      [else
       (cycle (bdlist-next bdxs) beg)]))
  (cycle bdxs null))


(define (decycle-bdlist! bdxs)
  (let ((end (bdlist-prev bdxs)))
    (begin
    (set-bdlist-prev! (bdlist-next end) null)
    (set-bdlist-next! end null))))

(define (show-bdcycle cbdxs n)
  (if (= n 0)
      (display "")
      (begin
        (display (bdlist-v cbdxs))
        (show-bdcycle (bdlist-next cbdxs) (- n 1)))))


(define q (list->bdlist (list 1 2 3 4 5 6 7 8)))
(define even-q (bdfilter even? q))
