/* zpprfs.f -- translated by f2c (version 20061008).
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

static doublecomplex c_b1 = {1.,0.};
static int c__1 = 1;

 int zpprfs_(char *uplo, int *n, int *nrhs, 
	doublecomplex *ap, doublecomplex *afp, doublecomplex *b, int *ldb, 
	 doublecomplex *x, int *ldx, double *ferr, double *berr, 
	doublecomplex *work, double *rwork, int *info)
{
    /* System generated locals */
    int b_dim1, b_offset, x_dim1, x_offset, i__1, i__2, i__3, i__4, i__5;
    double d__1, d__2, d__3, d__4;
    doublecomplex z__1;

    /* Builtin functions */
    double d_imag(doublecomplex *);

    /* Local variables */
    int i__, j, k;
    double s;
    int ik, kk;
    double xk;
    int nz;
    double eps;
    int kase;
    double safe1, safe2;
    extern int lsame_(char *, char *);
    int isave[3], count;
    int upper;
    extern  int zcopy_(int *, doublecomplex *, int *, 
	    doublecomplex *, int *), zhpmv_(char *, int *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, int *, 
	    doublecomplex *, doublecomplex *, int *), zaxpy_(
	    int *, doublecomplex *, doublecomplex *, int *, 
	    doublecomplex *, int *), zlacn2_(int *, doublecomplex *, 
	    doublecomplex *, double *, int *, int *);
    extern double dlamch_(char *);
    double safmin;
    extern  int xerbla_(char *, int *);
    double lstres;
    extern  int zpptrs_(char *, int *, int *, 
	    doublecomplex *, doublecomplex *, int *, int *);


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     Modified to call ZLACN2 in place of ZLACON, 10 Feb 03, SJH. */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  ZPPRFS improves the computed solution to a system of linear */
/*  equations when the coefficient matrix is Hermitian positive definite */
/*  and packed, and provides error bounds and backward error estimates */
/*  for the solution. */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangle of A is stored; */
/*          = 'L':  Lower triangle of A is stored. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  NRHS    (input) INTEGER */
/*          The number of right hand sides, i.e., the number of columns */
/*          of the matrices B and X.  NRHS >= 0. */

/*  AP      (input) COMPLEX*16 array, dimension (N*(N+1)/2) */
/*          The upper or lower triangle of the Hermitian matrix A, packed */
/*          columnwise in a linear array.  The j-th column of A is stored */
/*          in the array AP as follows: */
/*          if UPLO = 'U', AP(i + (j-1)*j/2) = A(i,j) for 1<=i<=j; */
/*          if UPLO = 'L', AP(i + (j-1)*(2n-j)/2) = A(i,j) for j<=i<=n. */

/*  AFP     (input) COMPLEX*16 array, dimension (N*(N+1)/2) */
/*          The triangular factor U or L from the Cholesky factorization */
/*          A = U**H*U or A = L*L**H, as computed by DPPTRF/ZPPTRF, */
/*          packed columnwise in a linear array in the same format as A */
/*          (see AP). */

/*  B       (input) COMPLEX*16 array, dimension (LDB,NRHS) */
/*          The right hand side matrix B. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B.  LDB >= MAX(1,N). */

/*  X       (input/output) COMPLEX*16 array, dimension (LDX,NRHS) */
/*          On entry, the solution matrix X, as computed by ZPPTRS. */
/*          On exit, the improved solution matrix X. */

/*  LDX     (input) INTEGER */
/*          The leading dimension of the array X.  LDX >= MAX(1,N). */

/*  FERR    (output) DOUBLE PRECISION array, dimension (NRHS) */
/*          The estimated forward error bound for each solution vector */
/*          X(j) (the j-th column of the solution matrix X). */
/*          If XTRUE is the true solution corresponding to X(j), FERR(j) */
/*          is an estimated upper bound for the magnitude of the largest */
/*          element in (X(j) - XTRUE) divided by the magnitude of the */
/*          largest element in X(j).  The estimate is as reliable as */
/*          the estimate for RCOND, and is almost always a slight */
/*          overestimate of the true error. */

/*  BERR    (output) DOUBLE PRECISION array, dimension (NRHS) */
/*          The componentwise relative backward error of each solution */
/*          vector X(j) (i.e., the smallest relative change in */
/*          any element of A or B that makes X(j) an exact solution). */

/*  WORK    (workspace) COMPLEX*16 array, dimension (2*N) */

/*  RWORK   (workspace) DOUBLE PRECISION array, dimension (N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */

/*  Internal Parameters */
/*  =================== */

/*  ITMAX is the maximum number of steps of iterative refinement. */

/*  ==================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --ap;
    --afp;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --ferr;
    --berr;
    --work;
    --rwork;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*nrhs < 0) {
	*info = -3;
    } else if (*ldb < MAX(1,*n)) {
	*info = -7;
    } else if (*ldx < MAX(1,*n)) {
	*info = -9;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZPPRFS", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0 || *nrhs == 0) {
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    ferr[j] = 0.;
	    berr[j] = 0.;
/* L10: */
	}
	return 0;
    }

/*     NZ = maximum number of nonzero elements in each row of A, plus 1 */

    nz = *n + 1;
    eps = dlamch_("Epsilon");
    safmin = dlamch_("Safe minimum");
    safe1 = nz * safmin;
    safe2 = safe1 / eps;

/*     Do for each right hand side */

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {

	count = 1;
	lstres = 3.;
L20:

/*        Loop until stopping criterion is satisfied. */

/*        Compute residual R = B - A * X */

	zcopy_(n, &b[j * b_dim1 + 1], &c__1, &work[1], &c__1);
	z__1.r = -1., z__1.i = -0.;
	zhpmv_(uplo, n, &z__1, &ap[1], &x[j * x_dim1 + 1], &c__1, &c_b1, &
		work[1], &c__1);

/*        Compute componentwise relative backward error from formula */

/*        MAX(i) ( ABS(R(i)) / ( ABS(A)*ABS(X) + ABS(B) )(i) ) */

/*        where ABS(Z) is the componentwise absolute value of the matrix */
/*        or vector Z.  If the i-th component of the denominator is less */
/*        than SAFE2, then SAFE1 is added to the i-th components of the */
/*        numerator and denominator before dividing. */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * b_dim1;
	    rwork[i__] = (d__1 = b[i__3].r, ABS(d__1)) + (d__2 = d_imag(&b[
		    i__ + j * b_dim1]), ABS(d__2));
/* L30: */
	}

/*        Compute ABS(A)*ABS(X) + ABS(B). */

	kk = 1;
	if (upper) {
	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		s = 0.;
		i__3 = k + j * x_dim1;
		xk = (d__1 = x[i__3].r, ABS(d__1)) + (d__2 = d_imag(&x[k + j *
			 x_dim1]), ABS(d__2));
		ik = kk;
		i__3 = k - 1;
		for (i__ = 1; i__ <= i__3; ++i__) {
		    i__4 = ik;
		    rwork[i__] += ((d__1 = ap[i__4].r, ABS(d__1)) + (d__2 = 
			    d_imag(&ap[ik]), ABS(d__2))) * xk;
		    i__4 = ik;
		    i__5 = i__ + j * x_dim1;
		    s += ((d__1 = ap[i__4].r, ABS(d__1)) + (d__2 = d_imag(&ap[
			    ik]), ABS(d__2))) * ((d__3 = x[i__5].r, ABS(d__3))
			     + (d__4 = d_imag(&x[i__ + j * x_dim1]), ABS(d__4)
			    ));
		    ++ik;
/* L40: */
		}
		i__3 = kk + k - 1;
		rwork[k] = rwork[k] + (d__1 = ap[i__3].r, ABS(d__1)) * xk + s;
		kk += k;
/* L50: */
	    }
	} else {
	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		s = 0.;
		i__3 = k + j * x_dim1;
		xk = (d__1 = x[i__3].r, ABS(d__1)) + (d__2 = d_imag(&x[k + j *
			 x_dim1]), ABS(d__2));
		i__3 = kk;
		rwork[k] += (d__1 = ap[i__3].r, ABS(d__1)) * xk;
		ik = kk + 1;
		i__3 = *n;
		for (i__ = k + 1; i__ <= i__3; ++i__) {
		    i__4 = ik;
		    rwork[i__] += ((d__1 = ap[i__4].r, ABS(d__1)) + (d__2 = 
			    d_imag(&ap[ik]), ABS(d__2))) * xk;
		    i__4 = ik;
		    i__5 = i__ + j * x_dim1;
		    s += ((d__1 = ap[i__4].r, ABS(d__1)) + (d__2 = d_imag(&ap[
			    ik]), ABS(d__2))) * ((d__3 = x[i__5].r, ABS(d__3))
			     + (d__4 = d_imag(&x[i__ + j * x_dim1]), ABS(d__4)
			    ));
		    ++ik;
/* L60: */
		}
		rwork[k] += s;
		kk += *n - k + 1;
/* L70: */
	    }
	}
	s = 0.;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (rwork[i__] > safe2) {
/* Computing MAX */
		i__3 = i__;
		d__3 = s, d__4 = ((d__1 = work[i__3].r, ABS(d__1)) + (d__2 = 
			d_imag(&work[i__]), ABS(d__2))) / rwork[i__];
		s = MAX(d__3,d__4);
	    } else {
/* Computing MAX */
		i__3 = i__;
		d__3 = s, d__4 = ((d__1 = work[i__3].r, ABS(d__1)) + (d__2 = 
			d_imag(&work[i__]), ABS(d__2)) + safe1) / (rwork[i__] 
			+ safe1);
		s = MAX(d__3,d__4);
	    }
/* L80: */
	}
	berr[j] = s;

/*        Test stopping criterion. Continue iterating if */
/*           1) The residual BERR(J) is larger than machine epsilon, and */
/*           2) BERR(J) decreased by at least a factor of 2 during the */
/*              last iteration, and */
/*           3) At most ITMAX iterations tried. */

	if (berr[j] > eps && berr[j] * 2. <= lstres && count <= 5) {

/*           Update solution and try again. */

	    zpptrs_(uplo, n, &c__1, &afp[1], &work[1], n, info);
	    zaxpy_(n, &c_b1, &work[1], &c__1, &x[j * x_dim1 + 1], &c__1);
	    lstres = berr[j];
	    ++count;
	    goto L20;
	}

/*        Bound error from formula */

/*        norm(X - XTRUE) / norm(X) .le. FERR = */
/*        norm( ABS(inv(A))* */
/*           ( ABS(R) + NZ*EPS*( ABS(A)*ABS(X)+ABS(B) ))) / norm(X) */

/*        where */
/*          norm(Z) is the magnitude of the largest component of Z */
/*          inv(A) is the inverse of A */
/*          ABS(Z) is the componentwise absolute value of the matrix or */
/*             vector Z */
/*          NZ is the maximum number of nonzeros in any row of A, plus 1 */
/*          EPS is machine epsilon */

/*        The i-th component of ABS(R)+NZ*EPS*(ABS(A)*ABS(X)+ABS(B)) */
/*        is incremented by SAFE1 if the i-th component of */
/*        ABS(A)*ABS(X) + ABS(B) is less than SAFE2. */

/*        Use ZLACN2 to estimate the infinity-norm of the matrix */
/*           inv(A) * diag(W), */
/*        where W = ABS(R) + NZ*EPS*( ABS(A)*ABS(X)+ABS(B) ))) */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (rwork[i__] > safe2) {
		i__3 = i__;
		rwork[i__] = (d__1 = work[i__3].r, ABS(d__1)) + (d__2 = 
			d_imag(&work[i__]), ABS(d__2)) + nz * eps * rwork[i__]
			;
	    } else {
		i__3 = i__;
		rwork[i__] = (d__1 = work[i__3].r, ABS(d__1)) + (d__2 = 
			d_imag(&work[i__]), ABS(d__2)) + nz * eps * rwork[i__]
			 + safe1;
	    }
/* L90: */
	}

	kase = 0;
L100:
	zlacn2_(n, &work[*n + 1], &work[1], &ferr[j], &kase, isave);
	if (kase != 0) {
	    if (kase == 1) {

/*              Multiply by diag(W)*inv(A'). */

		zpptrs_(uplo, n, &c__1, &afp[1], &work[1], n, info)
			;
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i__3 = i__;
		    i__4 = i__;
		    i__5 = i__;
		    z__1.r = rwork[i__4] * work[i__5].r, z__1.i = rwork[i__4] 
			    * work[i__5].i;
		    work[i__3].r = z__1.r, work[i__3].i = z__1.i;
/* L110: */
		}
	    } else if (kase == 2) {

/*              Multiply by inv(A)*diag(W). */

		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i__3 = i__;
		    i__4 = i__;
		    i__5 = i__;
		    z__1.r = rwork[i__4] * work[i__5].r, z__1.i = rwork[i__4] 
			    * work[i__5].i;
		    work[i__3].r = z__1.r, work[i__3].i = z__1.i;
/* L120: */
		}
		zpptrs_(uplo, n, &c__1, &afp[1], &work[1], n, info)
			;
	    }
	    goto L100;
	}

/*        Normalize error. */

	lstres = 0.;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
	    i__3 = i__ + j * x_dim1;
	    d__3 = lstres, d__4 = (d__1 = x[i__3].r, ABS(d__1)) + (d__2 = 
		    d_imag(&x[i__ + j * x_dim1]), ABS(d__2));
	    lstres = MAX(d__3,d__4);
/* L130: */
	}
	if (lstres != 0.) {
	    ferr[j] /= lstres;
	}

/* L140: */
    }

    return 0;

/*     End of ZPPRFS */

} /* zpprfs_ */
