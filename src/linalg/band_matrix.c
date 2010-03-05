/************************************************************************/
/* Copyright J�r�me Lelong <jerome.lelong@gmail.com>                    */
/*                                                                      */
/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU Lesser General Public License as       */
/* published by the Free Software Foundation, either version 3 of the   */
/* License, or (at your option) any later version.                      */
/*                                                                      */
/* This program is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    */
/* Lesser General Public License for more details.                      */
/*                                                                      */
/* You should have received a copy of the GNU Lesser General Public     */
/* License along with this program.  If not, see                        */
/* <http://www.gnu.org/licenses/>.                                      */
/************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
  
#include "config.h"
#include "pnl_band_matrix.h"
#include "pnl_machine.h"
#include "pnl_matrix.h"
#include "pnl_mathtools.h"
#include "pnl_internals.h"
#include "clapack.h"

#define PNL_BMGET(BM,i,j) BM->array[(BM->nu+i-j) + BM->m_band * j] 
#define PNL_BMLET(BM,i,j) BM->array[(BM->nu+i-j) + BM->m_band * j] 

static double __op_plus(double a, double b) { return a+b; }
static double __op_minus(double a, double b) { return a-b; }
static double __op_mult(double a, double b) { return a*b; }
static double __op_div(double a, double b) { return a/b; }
static double __op_inv(double a) { return 1./a; }

/**  Creates a band matrix 
 *
 * @param m the number of rows.  
 * @param n the number of columns.  
 * @param nl the number of lower diagonals
 * @param nu the number of upper diagonals
 * @return a PnlBandMat.
 */
PnlBandMat* pnl_bandmat_create (int m, int n, int nl, int nu) 
{
  PnlBandMat *BM;

  if (m < 0 || n < 0|| nu > n || nl > m) return NULL;
  if ((BM = malloc (sizeof (PnlBandMat))) == NULL) return NULL;
  BM->m = m;
  BM->n = n;
  BM->nl = nl;
  BM->nu = nu;
  if (m == 0 || n == 0)
    {
      BM->m_band = BM->n_band = 0;
      BM->array = NULL; return BM;
    }
  BM->m_band = (nl+nu+1); BM->n_band = n;
  if ((BM->array = malloc ( BM->m_band * BM->n_band * sizeof(double))) == NULL)
    {
      free (BM);
      return NULL;
    }
  return BM;
}

/** 
 * Creates a band matrix from a PnlMat 
 *
 * @param M adress of a PnlMat
 * @param nl the number of lower diagonal
 * @param nu the number of upper diagonal
 * @return a PnlBandMat.
 */
PnlBandMat* pnl_bandmat_create_from_mat(const PnlMat *M,int nl, int nu)
{
  PnlBandMat *BM;
  int i, j;
  BM = pnl_bandmat_create (M->m, M->n, nl, nu);

  for ( j=0 ; j<BM->n ; j++ )
    {
      for ( i=MAX(0, j - nu) ; i<MIN(BM->m, j+nl+1) ; i++ )
        {
          PNL_BMLET (BM, i, j) = PNL_MGET (M, i, j);
        }
    }
  return BM;
}


/** 
 * Frees a PnlBandMat
 *
 * @param BM adress of a PnlBandMat.
 */
void pnl_bandmat_free(PnlBandMat **BM)
{
  if ( (*BM) != NULL && (*BM)->array != NULL )
    {
      free ((*BM)->array); 
      free (*BM);
    }
  *BM = NULL;
}

/**
 * Resizes a band matrix
 *
 * @param BM a PnlBandMat
 * @param m the new number of rows.  
 * @param n the new number of columns.  
 * @param nl the new number of lower diagonals
 * @param nu the new number of upper diagonals
 */
int pnl_bandmat_resize(PnlBandMat *BM, int m, int n, int nl, int nu)
{
  int size;
  if ( m < 0 || n< 0 || nu < 0 || nl < 0 ) return FAIL;
  if ( m == 0 || n == 0 )
    {
      if (BM->array != NULL) free (BM->array); 
      BM->array = NULL;
      BM->m = BM->n = BM->m_band = BM->n_band = 0;
      BM->nl = BM->nu = 0;
      return OK;
    }
  size = (nl + nu + 1) * n; 
  if ( BM->m_band * BM->n_band >= size ) 
    {
      BM->m=m; BM->n=n;
      BM->m_band = (nl + nu + 1);
      BM->n_band = n;
      BM->nl = nl; BM->nu = nu;
      return OK;
    }
  /* now BM->size < size */
  BM->m=m; BM->n=n;
  BM->m_band = (nl + nu + 1);
  BM->n_band = n;
  BM->nl = nl; BM->nu = nu;
  if (BM->array != NULL) free (BM->array);
  if ((BM->array = malloc (size * sizeof (double))) == NULL) return FAIL;
  return OK;
}


/**
 * Clones a PnlBandMat
 *
 * @param Bclone an already existing PnlBandMat.
 * @param BM a PnlBandMat
 */
void pnl_bandmat_clone(PnlBandMat * Bclone, const PnlBandMat * BM)
{
  pnl_bandmat_resize (Bclone, BM->m, BM->n, BM->nl, BM->nu);
  memcpy (Bclone->array, BM->array, BM->m_band * BM->n_band * sizeof(double));
}

/**
 * Copies a PnlBandMat
 *
 * @param BM a PnlBandMat
 */
PnlBandMat* pnl_bandmat_copy(const PnlBandMat * BM)
{
  PnlBandMat *Bcopy;
  Bcopy = pnl_bandmat_create (BM->m, BM->n, BM->nl, BM->nu);
  memcpy (Bcopy->array, BM->array, BM->m_band * BM->n_band * sizeof(double));
  return Bcopy;
}
/**
 * Creates a PnlMat from a PnlBandMat
 *
 * @param BM a PnlBandMat
 * @return a PnlMat
 */
PnlMat* pnl_bandmat_to_mat(const PnlBandMat *BM)
{
  PnlMat *M;
  int i, j;
  M = pnl_mat_create_from_double (BM->m, BM->n, 0.);
  for ( j=0 ; j<BM->n ; j++ )
    {
      for ( i=MAX(0, j - BM->nu) ; i<MIN(BM->m, j+BM->nl+1) ; i++ )
        {
          PNL_MLET (M, i, j) = PNL_BMGET (BM, i, j); 
        }
    }
  return M;
}

/** 
 * Prints a band matrix in a full format
 * 
 * @param BM a band matrix
 */
void pnl_bandmat_print_as_full (const PnlBandMat *BM)
{
  PnlMat *M = pnl_bandmat_to_mat (BM);
  pnl_mat_print_nsp (M);
  pnl_mat_free (&M);
}

/**
 * in-place map function
 * On exit  BM = f(BM)
 *
 * @param BM a PnlBandMat
 * @param f the function to be applied term by term
 * On exit  BM = f(BM)
 */
void pnl_bandmat_map_inplace(PnlBandMat *BM, double(*f)(double ))
{
  int i, j;
  for ( j=0 ; j<BM->n_band ; j++ )
    {
      for ( i=MAX(0, BM->nu-j) ; i<BM->m_band - MAX(BM->nl-BM->n_band+j+1, 0) ; i++)
        {
          BM->array[j*BM->m_band+i] = (*f)(BM->array[j*BM->m_band+i]);
        }
    }
}

/**
 * in-place map function
 * On exit  BM = f(BM)
 *
 * @param BA a PnlBandMat
 * @param BB a PnlBandMat
 * @param f the function to be applied term by term
 * On exit  BA = f(BA,BB)
 */
void pnl_bandmat_map_bandmat(PnlBandMat *BA, const PnlBandMat *BB, 
                             double(*f)(double, double ))
{
  int i, j;
  PNL_CHECK (BA->m != BB->m || BA->n != BB->n || BA->nu != BB->nu || BA->nl != BB->nl,
             "size mismatched", "bandmat_map_bandlat");
  for ( j=0 ; j<BA->n_band ; j++ )
    {
      for ( i=MAX(0, BA->nu-j) ; i<BA->m_band - MAX(BA->nl-BA->n_band+j+1, 0) ; i++)
        {
          BA->array[j*BA->m_band+i] = (*f)(BA->array[j*BA->m_band+i],BB->array[j*BB->m_band+i]);
        }
    }
}


/**
 * in-place matrix operator application
 * BM = BM op x 
 * 
 * @param BM : left hand side vector
 * @param x : double arg
 * @param op : a binary operator, given as a function
 */
static void __pnl_bandmat_apply_op(PnlBandMat *BM, double x, double (*op)(double, double))
{
  int i, j;
  for ( j=0 ; j<BM->n_band ; j++ )
    {
      for ( i=MAX(0, BM->nu-j) ; i<BM->m_band - MAX(BM->nl-BM->n_band+j+1, 0) ; i++)
        {
          BM->array[j*BM->m_band+i] = (*op)(BM->array[j*BM->m_band+i], x);
        }
    }
}


/**
 * in-place PnlBandMat scalar addition
 *
 * @param BM left hand side PnlBandMat
 * @param x scalar
 * @return  BM += x
 */
void pnl_bandmat_plus_double(PnlBandMat *BM , double x)
{
  __pnl_bandmat_apply_op (BM, x, __op_plus);
}

/**
 * in-place PnlBandMat scalar substraction
 *
 * @param BM left hand side PnlBandMat
 * @param x scalar
 * @return  BM -= x
 */
void pnl_bandmat_minus_double(PnlBandMat *BM , double x)
{
  __pnl_bandmat_apply_op (BM, x, __op_minus);
}

/**
 * in-place PnlBandMat scalar multiplication
 *
 * @param BM left hand side PnlBandMat
 * @param x scalar
 * @return  BM *= x
 */
void pnl_bandmat_mult_double(PnlBandMat *BM , double x)
{
  __pnl_bandmat_apply_op (BM, x, __op_mult);
}


/**
 * in-place PnlBandMat scalar division
 *
 * @param BM left hand side PnlBandMat
 * @param x scalar
 * @return  BM /= x
 */
void pnl_bandmat_div_double(PnlBandMat *BM , double x)
{
  __pnl_bandmat_apply_op (BM, x, __op_div);
}


/**
 * map PnlBandMat componentwise
 * On exit, lhs(i) = f(rhs(i))
 *
 * @param lhs a PnlBandMat
 * @param rhs a PnlBandMat
 * @param f real function 
 */
void pnl_bandmat_map(PnlBandMat *lhs, const PnlBandMat *rhs, double(*f)(double))
{
  pnl_bandmat_clone(lhs, rhs);
  pnl_bandmat_map_inplace(lhs, f);
}

/**
 * in-place PnlBandMat addition
 *
 * @param BA left hand side PnlBandMat
 * @param BB rigth hand side PnlBandMat
 * @return  BA += BB
 */
void pnl_bandmat_plus_bandmat(PnlBandMat *BA, const PnlBandMat *BB)
{
  pnl_bandmat_map_bandmat(BA, BB, __op_plus);
}

void pnl_bandmat_minus_bandmat(PnlBandMat *BA, const PnlBandMat *BB)
{
  pnl_bandmat_map_bandmat (BA, BB, __op_minus);
}

/**
 * in-place term by term PnlBandMat inverse
 *
 * @param BA left hand side PnlBandMat
 * @param BB right hand side PnlBandMat
 * @return  BA = BA ./ BB
 */
void pnl_bandmat_div_bandmat_term(PnlBandMat *BA, const PnlBandMat *BB)
{
  pnl_bandmat_map_bandmat (BA, BB, __op_div);
}

/**
 * in-place PnlBandMat term by term multiplication
 *
 * @param BA left hand side PnlBandMat
 * @param BB right hand side PnlBandMat
 * @return  BA = BA.*BB
 */
void pnl_bandmat_mult_bandmat_term(PnlBandMat *BA, const PnlBandMat *BB)
{
  pnl_bandmat_map_bandmat (BA, BB, __op_mult);
}

/** 
 * Get function
 *
 * @param BM a PnlBandMat
 * @param i an integer
 * @param j an integer
 * @return M(i,j).
 */
double pnl_bandmat_get(PnlBandMat *BM,int i,int j)
{ 
  PNL_CHECK ( j<0 || j>BM->n || i<MAX(0, j - BM->nu) || i>=MIN(BM->m, j+BM->nl+1),
              "index out of range", "bandmat_get");
  return PNL_BMGET(BM, i, j);
}

/** 
 * Lget function
 *
 * @param BM a PnlBandMat
 * @param i an integer
 * @param j an integer
 * @return &(M(i,j)).
 */
double* pnl_bandmat_lget(PnlBandMat *BM,int i,int j)
{ 
  PNL_CHECK ( j<0 || j>BM->n || i<MAX(0, j - BM->nu) || i>=MIN(BM->m, j+BM->nl+1),
              "index out of range", "bandmat_get");
  return &(PNL_BMGET(BM, i, j));
}

/** 
 * Set function
 * On exit,  M(i,j) = x 
 *
 * @param BM PnlBandMat
 * @param i an integer
 * @param j an integer
 * @param x a real. 
 */
void pnl_bandmat_set(PnlBandMat * BM,int i,int j,double x)
{ 
  PNL_CHECK ( j<0 || j>BM->n || i<MAX(0, j - BM->nu) || i>=MIN(BM->m, j+BM->nl+1),
              "index out of range", "bandmat_get");
  PNL_BMLET(BM, i, j) = x;
}


/** 
 * pnl_bandmat_set_double
 * put value x for all entries of M,
 *
 * @param BM a PnlBandMat.
 * @param x double.
 */
void pnl_bandmat_set_double(PnlBandMat*  BM,double x)
{
  int i, j;
  for ( j=0 ; j<BM->n_band ; j++ )
    {
      for ( i=MAX(0, BM->nu-j) ; i<BM->m_band - MAX(BM->nl-BM->n_band+j+1, 0) ; i++ )
        {
          BM->array[j*BM->m_band+i] = x;
        }
    }
}


/**
 *  in place matrix vector multiplication
 *
 * @param y a vector
 * @param BM  a Band matrix
 * @param x a vector
 * @return  y = BM * x
 */
void pnl_bandmat_mult_vect_inplace(PnlVect *y, const PnlBandMat *BM, const PnlVect *x)
{
  pnl_bandmat_lAxpby (1., BM, x, 0., y);
}

/**
 * compute y=l * BM * x + b * y 
 *
 * @param l  a real 
 * @param BM a BandMat
 * @param x a vector
 * @param b a real
 * @param y a vector
 */
void pnl_bandmat_lAxpby(double l, const PnlBandMat *BM, const PnlVect *x, double b, PnlVect * y)
{
  int m, n, kl, ku, lda, incx, incy;

  PNL_CHECK ( (b != 0. && y->size != BM->m) || x->size != BM->n,
              "size mismatched", "bandmat_lAxpby");
  m = BM->m;
  n = BM->n;
  kl = BM->nl;
  ku = BM->nu;
  lda = BM->m_band;
  incx = incy = 1;

  if ( b == 0. )
    {
      pnl_vect_resize (y, BM->m);
    }
  C2F(dgbmv) ("N", &m, &n, &kl, &ku, &l, BM->array, &lda, x->array, &incx, &b, y->array, &incy); 
}



/** 
 * Enlarges a band matrix to store its LU decomposition 
 *  
 * @param BM
 */
static void pnl_bandmat_enlarge_for_lu (PnlBandMat *BM)
{
  int rows;
  int i, j;
  double *AB;

  AB = MALLOC_DOUBLE (BM->n * (2 * BM->nl + BM->nu + 1));
  rows = BM->m_band + BM->nl;
  for ( j=0 ; j<BM->n ; j++)
    {
      for ( i=MAX(0, BM->nu-j) ; i<BM->m_band - MAX(BM->nl-BM->n_band+j+1, 0) ; i++)
        {
          AB[i+BM->nl+j*rows] = BM->array[j*BM->m_band + i];
        }
    }
  free (BM->array);
  BM->array = AB;
  BM->m_band = rows;
}

/**
 * LU factorization of a Band matrix
 *
 * @param BM a PnlBandMat 
 * @param p a PnlPermutation (output parameter) to store the permutation used to
 * compute the LU decomposition
 * @warning we use the Lapack convention for p.  For 0 <= i < N, row i of the
 * matrix was interchanged with row p(i) with the Fortran convention  1 <= p(i)
 * <= N
 */
void pnl_bandmat_lu (PnlBandMat * BM, PnlVectInt *p)
{
  /* dgbtrf */
  int N, kl, ku, ldab, info;
  
  PNL_CHECK (BM->m != BM->n, "Matrix is not squared", "bandmat_lu");
  pnl_bandmat_enlarge_for_lu (BM);
  N = BM->m;
  kl = BM->nl;
  ku = BM->nu;
  ldab = BM->m_band;
  pnl_vect_int_resize (p, N);

  C2F(dgbtrf) (&N, &N, &kl, &ku, BM->array, &ldab, p->array, &info);
  if (info != 0)
    {
      PNL_ERROR ("LU decomposition cannot be computed", "bandmat_lu");
    }
}

/**
 * solves the linear system M x = b with M PnlBand Matrix.
 *
 * @param BM a PnlBandMat 
 * @param b right hand side member, used to the store solution on exit.
 */
void pnl_bandmat_syslin_inplace (PnlBandMat *BM, PnlVect *b)
{
  int N, kl, nrhs, ku, ldab, ldb, info;
  int *invpiv;
  
  PNL_CHECK (BM->m != BM->n, "Matrix is not squared", "bandmat_syslin_inplace");
  PNL_CHECK (BM->m != b->size, "size mismatched", "bandmat_syslin_inplace");
  pnl_bandmat_enlarge_for_lu (BM);
  N = BM->m;
  nrhs = 1;
  kl = BM->nl;
  ku = BM->nu;
  ldab = BM->m_band;
  ldb = b->size;
  invpiv = MALLOC_INT(N);

  C2F(dgbsv) (&N, &kl, &ku, &nrhs, BM->array, &ldab, invpiv, b->array, &ldb, &info);
  if (info != 0)
    {
      PNL_ERROR ("Error code", "bandmat_lu");
    }
  free (invpiv);
}

/**
 * Solves the linear system M x = b with M a PnlBand Matrix.
 *
 * @param BM a PnlBandMat
 * @param x a vector containing the solution on exit
 * @param b right hand side vector
 */
void pnl_bandmat_syslin (PnlVect *x, PnlBandMat *BM, const PnlVect *b)
{
  pnl_vect_clone (x, b);
  pnl_bandmat_syslin_inplace (BM, x);
}

/**
 * solves the linear system M x = x with a M PnlBand Matrix.
 *
 * @param BM the LU decomposition of a PnlBandMat as computed by pnl_bandmat_lu 
 * @param b right hand side member, used to the store solution on exit.
 * @param p a PnlPermutation (output parameter) to store the permutation used to
 * compute the LU decomposition
 * @warning we use the Lapack convention for p.  For 0 <= i < N, row i of the
 * matrix was interchanged with row p(i) with the Fortran convention  1 <= p(i)
 * <= N
 */
void pnl_bandmat_lu_syslin_inplace (const PnlBandMat *BM, const PnlVectInt *p, PnlVect *b)
{
  int N, kl, nrhs, ku, ldab, ldb, info;
  
  PNL_CHECK (BM->m != BM->n, "Matrix is not squared", "bandmat_syslin_inplace");
  PNL_CHECK (BM->m != b->size, "size mismatched", "bandmat_syslin_inplace");
  N = BM->m;
  nrhs = 1;
  kl = BM->nl;
  ku = BM->nu;
  ldab = BM->m_band;
  ldb = b->size;

  C2F(dgbtrs) ("N", &N, &kl, &ku, &nrhs, BM->array, &ldab, p->array, b->array, &ldb, &info);
  if (info != 0)
    {
      PNL_ERROR ("Error code", "bandmat_lu_syslin");
    }
}

/**
 * Solves the linear system M x = b with M a PnlBand Matrix.
 *
 * @param BM the LU decomposition of a PnlBandMat as computed by pnl_bandmat_lu 
 * @param p a vector of integers used to store the permutation
 * @param x a vector containing the solution on exit
 * @param b right hand side vector
 */
void pnl_bandmat_lu_syslin (PnlVect *x, const PnlBandMat *BM, const PnlVectInt *p, const PnlVect *b)
{
  pnl_vect_clone (x, b);
  pnl_bandmat_lu_syslin_inplace (BM, p, x);
} 


#undef PNL_BMGET
#undef PNL_BMLET
