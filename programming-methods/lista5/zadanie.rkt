#lang racket

(define leaf 'leaf)                ; konstruktor liścia

(define (leaf? t)                  ; predykat
  (eq? t 'leaf))

(define (node v l r)               ; konstruktor węzła
  (list 'node v l r))

(define (node? t)                  ; predykat
  (and (list? t)
       (= (length t) 4)
       (eq? (first t) 'node)))

(define (node-val t)               ; selektory
  (second t))

(define (node-left t)
  (third t))

(define (node-right t)
  (fourth t))

(define (tree? t)                  ; predykat definiujący
  (or (leaf? t)                    ; nasz główny typ danych!
      (and (node? t)
           (tree? (node-left  t))
           (tree? (node-right t)))))

(define (fold-tree f null t)
  (if (leaf? t)
      null
      (f (node-val t)
         (fold-tree f null (node-left t))
         (fold-tree f null (node-right t)))))

(define (tree-sum t)
  (if (leaf? t)
      0
      (+ (node-val t)
         (tree-sum (node-left  t))
         (tree-sum (node-right t)))))

(define (tree-sumf t) (fold-tree (lambda (x y z) (+ x y z)) 0 t))
(define (tree-flip t) (fold-tree (lambda (x y z) (node x y z)) leaf t))
(define (heightf t) (fold-tree (lambda (x y z) (if (< y z)
                                                   (+ 1 y)
                                                   (+ 1 z))) 1 t))


(define (flip t)
  (if (leaf? t)
      leaf
      (node (node-val t)
            (flip (node-right t))
            (flip (node-left t)))))
(define (height t)
  (if (leaf? t)
      1
      (if (< (height (node-right t)) (height (node-right t)))
          (+ 1 (height (node-left t)))
          (+ 1 (height (node-right t))))))

(define (sum-paths t)
  (define (tree t n)
    (if (leaf? t)
        leaf
        (node (+ n (node-val t))
              (tree (node-right t) (+ n (node-val t)))
              (tree (node-left t) (+ n (node-val t))))))
  (tree t 0))



(define big-tree
  (node 1 (node 2 (node 4 leaf leaf)
                  (node 5 leaf leaf))
          (node 3 (node 6 leaf leaf)
                  (node 7 leaf leaf))))

;;        1
;;       / \
;;      /   \
;;     2     3
;;    / \   / \
;;   4   5 6   7
;;  / \ /| |\ / \



;;        1
;;       / \
;;      /   \
;;     3     4
;;    / \   / \
;;   7   8 10   11
;;  / \ /| |\ / \







