#ifndef _OUTPUT_H_
#define _OUTPUT_H_
#include <cstdio>

#include "mm_io/mm_io.h"
#include "common.h"
#include "matrix/coo.3.h"

/*
 * Write binary data to a file.
 */
template <class T>
int write_bin_array(const char * filename, T * data, size_t num )
{
  FILE *f;
  size_t ret;

  // Read data from file
  f = fopen(filename, "wb");
  if (f==NULL) return -1;
  ret = fwrite(data, sizeof(T), num, f);
  if (ret < num) return -2;

  fclose(f);
  return 0;
}

/*
 * Write a COO matrix to a MatrixMarket file. Files will use same indexing as
 * the COO matrix.  It returns 0 if write was successfull, a negative number
 * otherwise.
 */
template <typename T, typename IT,
          template <typename, typename> class COO>
int write_mm_COO(COO<T,IT> * A, const char * filename)
{
  MM_typecode matcode;
  T val;
  IT i;
  unsigned long r, c;
  unsigned long size = A->nnz;

  // Open file for writing
  FILE * f = fopen(filename, "w");
  if (f == NULL) {
    fprintf(stderr, "Cannot open output file.\n");
    return -1;
  }

  // Initialize typecode struct
  mm_initialize_typecode(&matcode);
  mm_set_matrix(&matcode);
  mm_set_coordinate(&matcode);
  mm_set_real(&matcode);
  mm_set_general(&matcode);

  // Write file header
  mm_write_banner(f, matcode);
  mm_write_mtx_crd_size(f, A->rows, A->columns, size);

  // Write one point at the time.
  for (i=0; i<size; i++)
  {
    r = A->get_row_index(i);
    c = A->get_column_index(i);
    val = A->get_value(i);

    // MM uses 1-based indexing
    fprintf(f, "%lu %lu %f\n", r+1, c+1, (double) val);
  }

  // Close file
  fclose(f);
  return 0;
}

/*
 * Write a COO matrix to three binary files (one for values, one for row
 * indexes and one for column indexes). Files will use the same datatype as the
 * COO matrix and zero based indexing . It returns 0 if write was successfull,
 * a negative number otherwise.
 */
template <typename T, typename IT,
          template <typename, typename> class COO>
int write_bin_COO(COO<T,IT> * A,
        const char * file_row, const char * file_col, const char * file_val)
{
  int ret;
  T val;
  IT i, r, c;
  IT nrows = A->rows;
  IT ncols = A->columns;
  IT size = A->nnz;

  // Open files for writing
  FILE * fv = fopen(file_val, "wb");
  FILE * fr = fopen(file_row, "wb");
  FILE * fc = fopen(file_col, "wb");

  if (fv == NULL || fr == NULL || fc == NULL) {
    fprintf(stderr, "Cannot open one of the files.\n");
    return -1;
  }

  // Write row file
  for (i=0; i<size; i++)
  {
    r = A->get_row_index(i);
    fwrite(&r, sizeof(IT), 1, fr);
  }
  fclose(fr);

  // Write column file
  for (i=0; i<size; i++)
  {
    c = A->get_column_index(i);
    fwrite(&c, sizeof(IT), 1, fc);
  }
  fclose(fc);

  // Write value file
  for (i=0; i<size; i++)
  {
    val = A->get_value(i);
    fwrite(&val, sizeof(T), 1, fv);
  }
  fclose(fv);

  return 0;
}

/*
 * Write a COO matrix on disk in a binary file. The file format is described
 * in "notes/File Formats.md"
 */
template <typename T, typename IT,
          template <typename, typename> class COO>
int write_bin_COO(COO<T,IT> A, const char * filename)
{
    T vbuffer;
    IT buffer;

    // Open file for writing
    FILE * f = fopen(filename, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot open output file.\n");
        return -1;
    }

    // Header section
    fwrite(&(A.rows), sizeof(IT), 1, f);
    fwrite(&(A.columns), sizeof(IT), 1, f);
    fwrite(&(A.nnz), sizeof(IT), 1, f);

    // Data section
    for (IT i=0; i<A.nnz; i++)
    {
        buffer = A.get_row_index(i);
        fwrite(&buffer, sizeof(IT), 1, f);
    }

    for (IT i=0; i<A.nnz; i++) {
        buffer = A.get_column_index(i);
        fwrite(&buffer, sizeof(IT), 1, f);
    }

    for (IT i=0; i<A.nnz; i++) {
        vbuffer = A.get_value(i);
        fwrite(&vbuffer, sizeof(T), 1, f);
    }

    fclose(f);
    return 0;
}

/*
 * Write a Coo3 matrix on disk in a blockes file. The file format is described
 * in "notes/File Formats.md"
 */
template <class T, class IT>
int write_bin_blocked(Coo3<T, IT> A, const char * filename)
{
  // Open file for writing
  FILE * f = fopen(filename, "wb");
  if(f == NULL){
    fprintf(stderr, "Cannot open one of the files.\n");
    return -1;
  }

  // Header section
  fwrite(&(A.rows), sizeof(IT), 1, f);
  fwrite(&(A.columns), sizeof(IT), 1, f);
  fwrite(&(A.nnz), sizeof(IT), 1, f);
  fwrite(&(A.blockrows), sizeof(IT), 1, f);
  fwrite(&(A.blockcols), sizeof(IT), 1, f);
  fwrite(&(A.nnzblocks), sizeof(IT), 1, f);

  // Data section
  for(IT i=0; i<A.nnz; i++)
  {
    fwrite(&(A.elements[i].block) , sizeof(IT), 1, f);
    fwrite(&(A.elements[i].row) , sizeof(IT), 1, f);
    fwrite(&(A.elements[i].col) , sizeof(IT), 1, f);
    fwrite(&(A.elements[i].val) , sizeof(T), 1, f);
  }

  fclose(f);
  return 0;
}

#endif
