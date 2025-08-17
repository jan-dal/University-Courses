#lang racket
(define sudoku '((5 3 _ _ 7 _ _ _ _)
 (6 _ _ 1 9 5 _ _ _)
 (_ 9 8 _ _ _ _ 6 _)
 (8 _ _ _ 6 _ _ _ 3)
 (4 _ _ 8 _ 3 _ _ 1)
 (7 _ _ _ 2 _ _ _ 6)
 (_ 6 _ _ _ _ 2 8 _)
 (_ _ _ 4 1 9 _ _ 5)
 (_ _ _ _ 8 _ _ 7 9)))

(define (lenght row)
  (if (null? row)
      0
      (+ 1 (lenght (cdr row)))))
(define (rows->board xss)
  (define n
    (sqrt (lenght (car xss))))

  (define (get-k x y)
   (+ (* n (floor (/ y n))) (floor (/ x n))))

  
  (define (f row x y k)
    (if (null? row)
        null
        (cons (list x y k (car row)) (f (cdr row) (+ 1 x) y (get-k (+ 1 x) y)))))
  
  (define (combine y xss)
    (if (null? xss)
        null
        (cons (f (car xss) 0 y (get-k 0 y)) (combine (+ 1 y) (cdr xss)))))
  (combine 0 xss))


(define (board-rows board)
  (if (null? board)
      null
  (cons (map fourth (car board)) (board-rows (cdr board)))))
      
(define (board-collumns board)

  (define (nth row k)
    (if (null? row)
        null
        (if (<= k 0)
            (fourth (car row))
            (nth (cdr row) (- k 1)))))
  
  (define (collumn k board)
    (if (null? board)
        null
        (cons (nth (car board) k) (collumn k (cdr board)))))

  (define (iter c)
    (if (= (length (car board)) c)
        null
        (cons (collumn c board) (iter (+ 1 c)))))

  (iter 0))

(define (board-boxes board)

  (define (box-row k row)
    (if (null? row)
        null
        (if (= (third (car row)) k)
           (cons (fourth (car row)) (box-row k (cdr row)))
           (box-row k (cdr row)))))
  
  (define (box k board)
    (if (null? board)
        null
        (if (null? (box-row k (car board)))  
          (box k (cdr board))
          (append (box-row k (car board)) (box k (cdr board))))))

  (define (iter b)
    (if (= (length (car board)) b)
        null
        (cons (box b board) (iter (+ 1 b)))))
  
  (iter 0))
  
(define (partial-solution-ok? initial-board elems rows)
  (error ""))

(define s (rows->board sudoku))
;; Lista poprawnych częściowych wierszy po dodaniu
;; nowego znaku do jakiegoś częściowego rozwiązania
;; (elems, rows)
(define (new-elems initial-board elems rows)
  (filter
   (λ(es) (partial-solution-ok? initial-board es rows))
   (map (λ(c) (cons c elems)) alphabet)))

;; Lista poprawnych wierszy możliwych do dodania do
;; częściowego rozwiązania zawierającego tylko pełne
;; wiersze (rows)
(define (new-rows initial-board rows)
  (map (λ(r) (cons r rows))
       (foldr (λ(_ ess)
                (append-map
                 (λ(es) (new-elems initial-board es rows))
                 ess))
              (list null)
              alphabet)))

;; Lista poprawnych pełnych rozwiązań
(define (solutions initial-board)
  (map (λ(s) (rows->board s))
       (foldr (λ(_ rss)
                (append-map
                 (λ(rs) (new-rows initial-board rs))
                 rss))
              (list null)
              alphabet)))
