/* clanhb.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "pnl/pnl_f2c.h"

/* Table of constant values */

static int c__1 = 1;

double clanhb_(char *norm, char *uplo, int *n, int *k, complex *
	ab, int *ldab, float *work)
{
    /* System generated locals */
    int ab_dim1, ab_offset, i__1, i__2, i__3, i__4;
    float ret_val, r__1, r__2, r__3;

    /* Builtin functions */
    double c_ABS(complex *), sqrt(double);

    /* Local variables */
    int i__, j, l;
    float sum, absa, scale;
    extern int lsame_(char *, char *);
    float value;
    extern  int classq_(int *, complex *, int *, float 
	    *, float *);


/*  -- LAPACK auxiliary routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CLANHB  returns the value of the one norm,  or the Frobenius norm, or */
/*  the  infinity norm,  or the element of  largest absolute value  of an */
/*  n by n hermitian band matrix A,  with k super-diagonals. */

/*  Description */
/*  =========== */

/*  CLANHB returns the value */

/*     CLANHB = ( MAX(ABS(A(i,j))), NORM = 'M' or 'm' */
/*              ( */
/*              ( norm1(A),         NORM = '1', 'O' or 'o' */
/*              ( */
/*              ( normI(A),         NORM = 'I' or 'i' */
/*              ( */
/*              ( normF(A),         NORM = 'F', 'f', 'E' or 'e' */

/*  where  norm1  denotes the  one norm of a matrix (maximum column sum), */
/*  normI  denotes the  infinity norm  of a matrix  (maximum row sum) and */
/*  normF  denotes the  Frobenius norm of a matrix (square root of sum of */
/*  squares).  Note that  MAX(ABS(A(i,j)))  is not a consistent matrix norm. */

/*  Arguments */
/*  ========= */

/*  NORM    (input) CHARACTER*1 */
/*          Specifies the value to be returned in CLANHB as described */
/*          above. */

/*  UPLO    (input) CHARACTER*1 */
/*          Specifies whether the upper or lower triangular part of the */
/*          band matrix A is supplied. */
/*          = 'U':  Upper triangular */
/*          = 'L':  Lower triangular */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0.  When N = 0, CLANHB is */
/*          set to zero. */

/*  K       (input) INTEGER */
/*          The number of super-diagonals or sub-diagonals of the */
/*          band matrix A.  K >= 0. */

/*  AB      (input) COMPLEX array, dimension (LDAB,N) */
/*          The upper or lower triangle of the hermitian band matrix A, */
/*          stored in the first K+1 rows of AB.  The j-th column of A is */
/*          stored in the j-th column of the array AB as follows: */
/*          if UPLO = 'U', AB(k+1+i-j,j) = A(i,j) for MAX(1,j-k)<=i<=j; */
/*          if UPLO = 'L', AB(1+i-j,j)   = A(i,j) for j<=i<=MIN(n,j+k). */
/*          Note that the imaginary parts of the diagonal elements need */
/*          not be set and are assumed to be zero. */

/*  LDAB    (input) INTEGER */
/*          The leading dimension of the array AB.  LDAB >= K+1. */

/*  WORK    (workspace) REAL array, dimension (MAX(1,LWORK)), */
/*          where LWORK >= N when NORM = 'I' or '1' or 'O'; otherwise, */
/*          WORK is not referenced. */

/* ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    --work;

    /* Function Body */
    if (*n == 0) {
	value = 0.f;
    } else if (lsame_(norm, "M")) {

/*        Find MAX(ABS(A(i,j))). */

	value = 0.f;
	if (lsame_(uplo, "U")) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
		i__2 = *k + 2 - j;
		i__3 = *k;
		for (i__ = MAX(i__2,1); i__ <= i__3; ++i__) {
/* Computing MAX */
		    r__1 = value, r__2 = c_ABS(&ab[i__ + j * ab_dim1]);
		    value = MAX(r__1,r__2);
/* L10: */
		}
/* Computing MAX */
		i__3 = *k + 1 + j * ab_dim1;
		r__2 = value, r__3 = (r__1 = ab[i__3].r, ABS(r__1));
		value = MAX(r__2,r__3);
/* L20: */
	    }
	} else {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
		i__3 = j * ab_dim1 + 1;
		r__2 = value, r__3 = (r__1 = ab[i__3].r, ABS(r__1));
		value = MAX(r__2,r__3);
/* Computing MIN */
		i__2 = *n + 1 - j, i__4 = *k + 1;
		i__3 = MIN(i__2,i__4);
		for (i__ = 2; i__ <= i__3; ++i__) {
/* Computing MAX */
		    r__1 = value, r__2 = c_ABS(&ab[i__ + j * ab_dim1]);
		    value = MAX(r__1,r__2);
/* L30: */
		}
/* L40: */
	    }
	}
    } else if (lsame_(norm, "I") || lsame_(norm, "O") || *(unsigned char *)norm == '1') {

/*        Find normI(A) ( = norm1(A), since A is hermitian). */

	value = 0.f;
	if (lsame_(uplo, "U")) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		sum = 0.f;
		l = *k + 1 - j;
/* Computing MAX */
		i__3 = 1, i__2 = j - *k;
		i__4 = j - 1;
		for (i__ = MAX(i__3,i__2); i__ <= i__4; ++i__) {
		    absa = c_ABS(&ab[l + i__ + j * ab_dim1]);
		    sum += absa;
		    work[i__] += absa;
/* L50: */
		}
		i__4 = *k + 1 + j * ab_dim1;
		work[j] = sum + (r__1 = ab[i__4].r, ABS(r__1));
/* L60: */
	    }
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
		r__1 = value, r__2 = work[i__];
		value = MAX(r__1,r__2);
/* L70: */
	    }
	} else {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		work[i__] = 0.f;
/* L80: */
	    }
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__4 = j * ab_dim1 + 1;
		sum = work[j] + (r__1 = ab[i__4].r, ABS(r__1));
		l = 1 - j;
/* Computing MIN */
		i__3 = *n, i__2 = j + *k;
		i__4 = MIN(i__3,i__2);
		for (i__ = j + 1; i__ <= i__4; ++i__) {
		    absa = c_ABS(&ab[l + i__ + j * ab_dim1]);
		    sum += absa;
		    work[i__] += absa;
/* L90: */
		}
		value = MAX(value,sum);
/* L100: */
	    }
	}
    } else if (lsame_(norm, "F") || lsame_(norm, "E")) {

/*        Find normF(A). */

	scale = 0.f;
	sum = 1.f;
	if (*k > 0) {
	    if (lsame_(uplo, "U")) {
		i__1 = *n;
		for (j = 2; j <= i__1; ++j) {
/* Computing MIN */
		    i__3 = j - 1;
		    i__4 = MIN(i__3,*k);
/* Computing MAX */
		    i__2 = *k + 2 - j;
		    classq_(&i__4, &ab[MAX(i__2, 1)+ j * ab_dim1], &c__1, &
			    scale, &sum);
/* L110: */
		}
		l = *k + 1;
	    } else {
		i__1 = *n - 1;
		for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
		    i__3 = *n - j;
		    i__4 = MIN(i__3,*k);
		    classq_(&i__4, &ab[j * ab_dim1 + 2], &c__1, &scale, &sum);
/* L120: */
		}
		l = 1;
	    }
	    sum *= 2;
	} else {
	    l = 1;
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__4 = l + j * ab_dim1;
	    if (ab[i__4].r != 0.f) {
		i__4 = l + j * ab_dim1;
		absa = (r__1 = ab[i__4].r, ABS(r__1));
		if (scale < absa) {
/* Computing 2nd power */
		    r__1 = scale / absa;
		    sum = sum * (r__1 * r__1) + 1.f;
		    scale = absa;
		} else {
/* Computing 2nd power */
		    r__1 = absa / scale;
		    sum += r__1 * r__1;
		}
	    }
/* L130: */
	}
	value = scale * sqrt(sum);
    }

    ret_val = value;
    return ret_val;

/*     End of CLANHB */

} /* clanhb_ */
