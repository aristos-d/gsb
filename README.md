# Generalized Sparse Blocks

This library is the result of my diploma thesis for the Department of
Electrical and Computer Engineering of the Aristotle University of
Thessaloniki. It introduces a new sparse matrix storage format after which the
library is named. However, a number of other storage formats are implemented
and hopefully the library could be useful to anyone interested in research
related to the SpMV routine.

## Requirements

All executables are built with clang. Any compiler supporting OpenMP and C++11
could probably be sufficient but no other compiler has been tested.

## License

The licensing terms of this software library are described in LICENSE. These
terms do not apply to the files in the subdirectories of dep/. The files in
these subdirectories are works of others and are redistributed according to
terms of their respective copyright owners. They are:

* Intel for the parallel stable sort library
* NIST (probably, not sure) for the MatrixMarket IO library

