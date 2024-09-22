#ifndef _INPUT_H_
#define _INPUT_H_

#include <cassert>
#include <cstdio>

#include "mm_io/mm_io.h"
#include "common.h"
#include "matrix/coo.1.h"
#include "matrix/csr.1.h"
#include "matrix/csr.2.h"

/*
 * Read binary data from file. Used to read block sizes.
 */
template <class T>
int read_bin_array(const char * filename, T ** data, size_t * num )
{
  size_t size = get_file_size(filename) / sizeof(T);

  // Allocate memory
  *num = size;
  *data = (T *) malloc(size * sizeof(T));
  if (*data==NULL) return -1;

  // Read data from file
  FILE * f = fopen(filename, "rb");
  if (f==NULL) return -2;
  size_t ret = fread( *data, sizeof(T), size, f);
  if (ret < size) return -3;

  fclose(f);
  return 0;
}

/*
 * Function to read a COO matrix from a Matrix Market file. Makes use of the
 * library mm_io.c provided here :
 * https://math.nist.gov/MatrixMarket/mmio-c.html. It returns 0 if read was
 * successfull, a negative number otherwise. The code here is based on the
 * example code provided with the library (example_read.c)
 */
template <typename T, typename IT,
          template <typename, typename> class COO>
int read_mm_COO(COO<T,IT> * A, const char * filename)
{
  // Open file for reading
  MM_typecode matcode;
  FILE * f = fopen(filename, "r");
  int symflag = 0, ret;
  unsigned long r, c, counter, position;
  double val;

  if(f == NULL){
    fprintf(stderr, "Cannot open file.\n");
    return -1;
  }

  // Read header and perform type checks
  if (mm_read_banner(f, &matcode) != 0){
      fprintf(stderr, "Could not process Matrix Market banner.\n");
      return -2;
  }

  if (mm_is_complex(matcode) || !mm_is_coordinate(matcode) ){
      fprintf(stderr, "Sorry, this program does not support ");
      fprintf(stderr, "Market Market type: [%s]\n", mm_typecode_to_str(matcode));
      return -3;
  }

  if (mm_is_symmetric(matcode)) symflag = 1;

  // Read size and allocate memory. Following function only works with ints
  int nr, nc, nnz;
  int ret_code = mm_read_mtx_crd_size(f, &nr, &nc, &nnz);
  if (ret_code != 0) {
    fprintf(stderr, "Error while reading size information.\n");
    return -4;
  }
  A->rows = (IT) nr;
  A->columns = (IT) nc;
  A->nnz = (IT) nnz;

  if(symflag){
    fflush(f);
    position = ftell(f);
    counter = 0;
    for (IT i=0; i<2*nnz; i++){
        ret = fscanf(f, "%lu %lu %lg\n", &r, &c, &val);
        if(ret == EOF){
          break;
        }else{
          if(r==c){
            counter += 1;
          }else{
            counter += 2;
          }
        }
    }
    nnz = counter;
    A->nnz = (IT) nnz;
    fseek(f,position,SEEK_SET);
  }

  // Allocate memory
  allocate(A, (IT) nnz);

  // Read everything as unsigned long and double. Convert to appropriate type later.
  for (IT i=0; i<nnz; i++){
      ret = fscanf(f, "%lu %lu %lg\n", &r, &c, &val);
      if(ret == EOF){
        A->nnz = i;
        break;
      }else{
        set_point(A, (IT) i, (IT) (r-1), (IT) (c-1), (T) val);

        // If matrix is symmetric, add symmetric point
        if(symflag && r!=c){
          i++;
          set_point(A, (IT) i, (IT) (c-1), (IT) (r-1), (T) val);
        }
      }
  }

  fclose(f);
  return 0;
}

/*
 * Read a Coo matrix from three binary files (one for values, one for row
 * indexes and one for column indexes). Files need to store their content as
 * the datatype defined in Coo matrix. It returns 0 if read was successfull, a
 * negative number otherwise.
 */
template <class T, class IT>
int read_bin_COO(Coo<T,IT,IT> * A, const char * file_row, const char * file_col, const char * file_val)
{
  T val;
  IT size, r, c;
  IT max_row = 0, max_col = 0;
  FILE *fr, *fc, *fv;

  // Read row file to the end to get size
  size = (IT) get_file_size(file_val) / sizeof(T);

  // Allocate memory
  allocate(A, size);

  // Read row file
  fr = fopen(file_row, "rb");
  if (fr == NULL) {
    fprintf(stderr, "Cannot open row file.\n");
    return -1;
  }

  fread(A->I, sizeof(IT), size, fr);
  for (IT i=0; i<size; i++){
    if(A->I[i] > max_row) max_row = A->I[i];
  }

  fclose(fr);

  // Read column file
  fc = fopen(file_col, "rb");
  if (fc == NULL) {
    fprintf(stderr, "Cannot open column file.\n");
    return -1;
  }

  fread(A->J, sizeof(IT), size, fc);
  for (IT i=0; i<size; i++){
    if(A->J[i] > max_col) max_col = A->J[i];
  }

  fclose(fc);

  // Read value file
  fv = fopen(file_val, "rb");
  if (fc == NULL) {
    fprintf(stderr, "Cannot open values file.\n");
    return -1;
  }

  fread(A->val, sizeof(T), size, fv);
  fclose(fc);

  // Set number or rows and columns
  A->rows = max_row + 1;
  A->columns = max_col + 1;
  A->nnz = size;

  return 0;
}

/*
 * Read a COO matrix from three binary files (one for values, one for row
 * indexes and one for column indexes). Files need to store their content as
 * the datatype defined in COO matrix. It returns 0 if read was successfull, a
 * negative number otherwise.
 */
template <typename T, typename IT,
          template <typename, typename> class COO>
int read_bin_COO(COO<T, IT> * A, const char * file_row, const char * file_col, const char * file_val)
{
  T vbuffer;
  IT size, counter, buffer;
  IT max_row, max_col;
  FILE *fr, *fc, *fv;

  // Read values file to the end to get size
  size = (IT) get_file_size(file_val) / sizeof(T);

  // Allocate memory
  allocate(A, size);

  // Read row file
  fr = fopen(file_row, "rb");
  if (fr == NULL) {
    fprintf(stderr, "Cannot open row file.\n");
    return -1;
  }

  for (IT i=0; i<size; i++)
  {
    counter += fread(&buffer, sizeof(IT), 1, fr);
    A->set_row_index(i, buffer);
  }

  fclose(fr);

  // Read column file
  fc = fopen(file_col, "rb");
  if (fc == NULL)
  {
    fprintf(stderr, "Cannot open column file.\n");
    return -1;
  }

  for (IT i=0; i<size; i++)
  {
    counter += fread(&buffer, sizeof(IT), 1, fc);
    A->set_column_index(i, buffer);
  }

  fclose(fc);

  // Read value file
  fv = fopen(file_val, "rb");
  if (fv == NULL)
  {
    fprintf(stderr, "Cannot open values file.\n");
    return -1;
  }

  for (IT i=0; i<size; i++)
  {
    counter += fread(&vbuffer, sizeof(T), 1, fv);
    A->set_value(i, vbuffer);
  }

  fclose(fc);

  // Set number or rows and columns
  A->rows = max_row + 1;
  A->columns = max_col + 1;
  A->nnz = size;
  return 0;
}

/*
 * Read a COO matrix from a binary file (.bin).
 */
template <typename T, typename IT,
          template <typename, typename> class COO>
int read_bin_COO(COO<T,IT> * A, const char * filename)
{
    IT counter, buffer;
    T vbuffer;
    FILE * f;

    f = fopen(filename, "rb");
    if (f == NULL) {
        fprintf(stderr, "Cannot open matrix file.\n");
        return -1;
    }

    // Reading header
    counter = 0;
    counter += fread(&(A->rows), sizeof(IT), 1, f);
    counter += fread(&(A->columns), sizeof(IT), 1, f);
    counter += fread(&(A->nnz), sizeof(IT), 1, f);
    if (counter != 3) {
        fprintf(stderr, "Failed to read matrix header.\n");
        return -2;
    }

    // Memory allocation
    allocate(A, A->nnz);

    // Reading rest of the file
    counter = 0;
    for (IT i=0; i<A->nnz; i++)
    {
        counter += fread(&buffer, sizeof(IT), 1, f);
        A->set_row_index(i, buffer);
    }

    for (IT i=0; i<A->nnz; i++)
    {
        counter += fread(&buffer, sizeof(IT), 1, f);
        A->set_column_index(i, buffer);
    }

    for (IT i=0; i<A->nnz; i++)
    {
        counter += fread(&vbuffer, sizeof(T), 1, f);
        A->set_value(i, vbuffer);
    }

    if (counter != 3 * A->nnz)
    {
        fprintf(stderr, "Failed to read matrix.\n");
        return -4;
    }

    fclose(f);
    return 0;
}

/*
 * Read a COO matrix in either binary (.bin) or MatrixMarket (.mtx) format.
 * Use filename suffix to decide.
 */
template <typename T, typename IT,
          template <typename, typename> class COO>
int read_COO(COO<T,IT> * A, const char * filename)
{
    if (string_ends_with(filename, ".bin")) {
        return read_bin_COO(A, filename);
    } else if (string_ends_with(filename, ".mtx")) {
        return read_mm_COO(A, filename);
    }

    fprintf(stderr, "Unknown file extension. Use \".bin\" or \".mtx\"\n");
    return -1;
}

/*
 * Read a sparse matrix from a binary triples file. The file is read to a CSR
 * matrix.
 * WARNING : The file MUST be sorted according to row, column.
 */
template <class T, class IT>
int read_bin_CSR(Csr<T, IT> * A, const char * filename)
{
  IT current_row, nnz;

  // Open file for reading
  FILE * f = fopen(filename, "rb");
  if(f == NULL){
    fprintf(stderr, "Cannot open input file.\n");
    return -1;
  }

  // Read header
  fread(&(A->rows), sizeof(IT), 1, f);
  fread(&(A->columns), sizeof(IT), 1, f);
  fread(&nnz, sizeof(IT), 1, f);

  DEBUG(puts("Reading header done"));

  // Allocate one memory block for all arrays
  A->val = new T[nnz];
  A->col_ind = new IT[nnz];
  A->row_ptr = new IT[A->rows + 1]();

  DEBUG(puts("Memory allocation done"));

  // Read data section
  for(IT i=0; i<nnz; i++){
    fread(&current_row, sizeof(IT), 1, f);
    A->row_ptr[current_row+1]++;  // Count non-zeros per row
  }
  fread(A->col_ind, sizeof(IT), nnz, f);
  fread(A->val, sizeof(T), nnz, f);
  fclose(f);

  DEBUG(puts("Reading data done"));

  // Get row pointers from row non-zero counters
  for(IT i=0; i<A->rows; i++){
    A->row_ptr[i+1] = A->row_ptr[i+1] + A->row_ptr[i];
  }

  assert(A->row_ptr[A->rows] == nnz);
  return 0;
}

/*
 * Read a sparse matrix from a binary triples file. The file is read to a CSR
 * matrix.
 * WARNING : The file MUST be sorted according to row, column.
 */
template <class T, class IT>
int read_bin_CSR(Csr2<T, IT> * A, const char * filename)
{
  IT current_row, nnz;

  // Open file for writing
  FILE * f = fopen(filename, "rb");
  if(f == NULL){
    fprintf(stderr, "Cannot open input file.\n");
    return -1;
  }

  // Read header
  fread(&(A->rows), sizeof(IT), 1, f);
  fread(&(A->columns), sizeof(IT), 1, f);
  fread(&nnz, sizeof(IT), 1, f);

  // Allocate memory
  A->nonzeros = new NonZero<T,IT>[nnz];
  A->row_ptr = new IT[A->rows]();

  // Read data section
  for(IT i=0; i<nnz; i++){
    fread(&current_row, sizeof(IT), 1, f);
    A->row_ptr[current_row+1]++;  // Count non-zeros per row
  }

  for(IT i=0; i<nnz; i++)
    fread(&(A->nonzeros[i].index), sizeof(IT), 1, f);

  for(IT i=0; i<nnz; i++)
    fread(&(A->nonzeros[i].val), sizeof(IT), 1, f);

  fclose(f);

  // Get row pointers from row non-zero counters
  for(IT i=0; i<A->rows; i++){
    A->row_ptr[i+1] = A->row_ptr[i+1] + A->row_ptr[i];
  }

  assert(A->row_ptr[A->rows] == nnz);
  return 0;
}

#endif
