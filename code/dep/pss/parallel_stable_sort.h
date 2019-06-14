/*
  Copyright (C) 2014 Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.
  * Neither the name of Intel Corporation nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
  WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _PSS_PUBLIC_HEADER_H_
#define _PSS_PUBLIC_HEADER_H_

#include <algorithm>
#include <cilk/cilk.h>

#include "pss_common.h"

namespace pss {

namespace internal {

// Merge sequences [xs,xe) and [ys,ye) to output sequence [zs,zs+(xe-xs)+(ye-ys))
// Destroy input sequence iff destroy==true
template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename RandomAccessIterator3, typename Compare>
void parallel_move_merge( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 ys, RandomAccessIterator2 ye, RandomAccessIterator3 zs, bool destroy, Compare comp ) {
    const size_t MERGE_CUT_OFF = 2000;
    while( (xe-xs) + (ye-ys) > MERGE_CUT_OFF ) {
        RandomAccessIterator1 xm;
        RandomAccessIterator2 ym;
        if( xe-xs < ye-ys  ) {
            ym = ys+(ye-ys)/2;
            xm = std::upper_bound(xs,xe,*ym,comp);
        } else {
            xm = xs+(xe-xs)/2;
            ym = std::lower_bound(ys,ye,*xm,comp);
        }
        cilk_spawn parallel_move_merge( xs, xm, ys, ym, zs, destroy, comp );
        zs += (xm-xs) + (ym-ys);
        xs = xm;
        ys = ym;
    }
    serial_move_merge( xs, xe, ys, ye, zs, comp );
    if( destroy ) {
        serial_destroy( xs, xe );
        serial_destroy( ys, ye );
    }
}

// Sorts [xs,xe), where zs[0:xe-xs) is temporary buffer supplied by caller.
// Result is in [xs,xe) if inplace==true, otherwise in [zs,zs+(xe-xs))
template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Compare>
void parallel_stable_sort_aux( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 zs, int inplace, Compare comp ) {
    typedef typename std::iterator_traits<RandomAccessIterator2>::value_type T;
    const size_t SORT_CUT_OFF = 500;
    if( xe-xs<=SORT_CUT_OFF ) {
        stable_sort_base_case(xs, xe, zs, inplace, comp); 
    } else {
        RandomAccessIterator1 xm = xs + (xe-xs)/2;
        RandomAccessIterator2 zm = zs + (xm-xs);
        RandomAccessIterator2 ze = zs + (xe-xs);
        cilk_spawn parallel_stable_sort_aux( xs, xm, zs, !inplace, comp );
        /*nospawn*/parallel_stable_sort_aux( xm, xe, zm, !inplace, comp );
        cilk_sync;
        if( inplace )
            parallel_move_merge( zs, zm, zm, ze, xs, inplace==2, comp );
        else
            parallel_move_merge( xs, xm, xm, xe, zs, false, comp );
   }
}

} // namespace internal

template<typename RandomAccessIterator, typename Compare>
void parallel_stable_sort( RandomAccessIterator xs, RandomAccessIterator xe, Compare comp ) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
    if( internal::raw_buffer z = internal::raw_buffer( sizeof(T)*(xe-xs) ) )
        internal::parallel_stable_sort_aux( xs, xe, (T*)z.get(), 2, comp );
    else
        // Not enough memory available - fall back on serial sort
        std::stable_sort( xs, xe, comp );
}

//! Wrapper for sorting with default comparator.
template<class RandomAccessIterator>
void parallel_stable_sort( RandomAccessIterator xs, RandomAccessIterator xe ) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
    parallel_stable_sort( xs, xe, std::less<T>() );
}

} // namespace pss

#endif
