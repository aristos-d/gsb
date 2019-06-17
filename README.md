# Generalized Sparse Blocks

This library is the result of my diploma thesis for the Department of
Electrical and Computer Engineering of the Aristotle University of
Thessaloniki. It introduces a new sparse matrix storage format after which the
library is named. However, a number of other storage formats are implemented
and hopefully the library could be useful to anyone interested in research
related to the SpMV routine.

## Requirements

All executables are built with Intel's compiler. Any compiler supporting Cilk
Plus could probably be sufficient but no testing has been performed. Finally,
Mathwork's toolchain is required to build Matlab bindings. The installation
directories of these toolchains might have to be adjusted in the Makefile.
