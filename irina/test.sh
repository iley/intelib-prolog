#!/bin/sh

(2>/dev/null ls ./irina > /dev/null || (echo 'FAIL! No ./irina file'; false)) &&
(2>/dev/null ls ./wrap > /dev/null || (echo 'FAIL! No ./wrap file'; false)) &&
(
  (
    (
      (./irina > test1.log; ./wrap > test2.log; diff -bB test1.log test2.log) &&
      (./irina -h > test1.log; ./wrap -h > test2.log; diff -bB test1.log test2.log) &&
      ( ./irina --help > test1.log; ./wrap --help > test2.log; diff -bB test1.log test2.log) &&
      (./irina -a > test1.log; ./wrap -a > test2.log; diff -bB test1.log test2.log) &&
      (./irina --about > test1.log; ./wrap --about > test2.log; diff -bB test1.log test2.log) &&
      (./irina -V > test1.log; ./wrap -V > test2.log; diff -bB test1.log test2.log) &&
      (./irina --version > test1.log; ./wrap --version > test2.log; diff -bB test1.log test2.log) &&
      (./irina -l 1 -b 2 -l -b -l 3 -l -b -b 4 > test1.log; ./wrap -l 1 -b 2 -l -b -l 3 -l -b -b 4 > test2.log; diff -bB test1.log test2.log) &&
      (./irina -o -o hello > test1.log; ./wrap -o -o hello > test2.log; diff -bB test1.log test2.log) &&
      echo 'OK! ./wrap and ./irina give the same output'
    ) ||
    (echo -e '\nFAIL! ./wrap and ./irina do not give the same output')
  ) && rm test*.log
)
