#ifndef _NONZEROS_H_
#define _NONZEROS_H_
/*
 * Most matrix types have a field named "nnz". As long as they do, these
 * functions will provide an access method. Those that do not have to define
 * their own "nonzeros" method for the generic code to work.
 */

template <class T, class IT,
         template <typename, typename> class MATRIX>
inline IT nonzeros(const MATRIX<T,IT> * const A){ return A->nnz; }

template <class T, class IT,
         template <typename, typename> class MATRIX>
inline IT nonzeros(const MATRIX<T,IT> A){ return A.nnz; }

template <class T, class IT, class SIT,
         template <typename, typename, typename> class MATRIX>
inline IT nonzeros(const MATRIX<T,IT,SIT> * const A){ return A->nnz; }

template <class T, class IT, class SIT,
         template <typename, typename, typename> class MATRIX>
inline IT nonzeros(const MATRIX<T,IT,SIT> A){ return A.nnz; }

#endif
