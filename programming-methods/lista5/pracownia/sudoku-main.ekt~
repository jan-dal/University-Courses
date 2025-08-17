#lang racket

(require "solution.rkt")

(let ([b (solutions (rows->board (read)))])
  (display (if (null? b)
               "no solution"
               (board-rows (car b)))))