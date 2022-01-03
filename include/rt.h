#ifndef _RT_H_
#define _RT_H_

#include <omp.h>

#define RT_SYNCHED 0

#define RT_WORKERS omp_get_max_threads()

#define RT_SET_WORKERS(n) omp_set_num_workers(n)

#endif
