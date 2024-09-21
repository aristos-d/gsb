# Generalized Sparse Blocks

This library is the result of my diploma thesis for the Department of
Electrical and Computer Engineering of the Aristotle University of
Thessaloniki. It introduces a new sparse matrix storage format after which the
library is named. However, a number of other storage formats are implemented
and hopefully the library could be useful to anyone interested in research
related to the SpMV routine.

The fastest way to test the library is to run the SpMV routine using multiple
defined matrix formats. To do this, build the binary `bin/test/result` and run
it with the filename of a sparse matrix as its argument:

```
$ make bin/test/result
...

$ ./bin/test/result ~/data/cage14.mtx
```

The binary (as most binaries built by the Makefile) accepts two input formats
for matrices. The MatrixMarket format and a custom binary format (for filenames
ending with `.bin`). The custom binary format is as follows:

```
INDEXTYPE number_of_rows
INDEXTYPE number_of_columns
INDEXTYPE number_of_nonzeros
INDEXTYPE row_indeces[number_of_nonzeros]
INDEXTYPE column_indeces[number_of_nonzeros]
VALUETYPE nonzeros[number_of_nonzeros]
```

`INDEXTYPE` and `VALUETYPE` are defined at build time and are `uin32_t` and
`float` by default.


## Requirements

All executables are built with clang. Any compiler supporting OpenMP and C++17
could probably be sufficient but no other compiler has been tested.


## License

The licensing terms of this software library are described in LICENSE. These
terms do not apply to the files in the subdirectories of dep/. The files in
these subdirectories are works of others and are redistributed according to
terms of their respective copyright owners. They are:

* Intel for the parallel stable sort library
* NIST (probably, not sure) for the MatrixMarket IO library

