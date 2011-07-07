/* dla_gerfsx_extended.f -- translated by f2c (version 20061008).
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
static double c_b6 = -1.;
static double c_b8 = 1.;

 int dla_gerfsx_extended__(int *prec_type__, int *
	trans_type__, int *n, int *nrhs, double *a, int *lda, 
	double *af, int *ldaf, int *ipiv, int *colequ, 
	double *c__, double *b, int *ldb, double *y, int *
	ldy, double *berr_out__, int *n_norms__, double *errs_n__,
	 double *errs_c__, double *res, double *ayb, double *
	dy, double *y_tail__, double *rcond, int *ithresh, 
	double *rthresh, double *dz_ub__, int *ignore_cwise__, 
	int *info)
{
    /* System generated locals */
    int a_dim1, a_offset, af_dim1, af_offset, b_dim1, b_offset, y_dim1, 
	    y_offset, errs_n_dim1, errs_n_offset, errs_c_dim1, errs_c_offset, 
	    i__1, i__2, i__3;
    double d__1, d__2;
    char ch__1[1];

    /* Local variables */
    double dxratmax, dzratmax;
    int i__, j;
    extern  int dla_geamv__(int *, int *, int *, 
	    double *, double *, int *, double *, int *, 
	    double *, double *, int *);
    int incr_prec__;
    double prev_dz_z__, yk, final_dx_x__;
    extern  int dla_wwaddw__(int *, double *, 
	    double *, double *);
    double final_dz_z__, prevnormdx;
    int cnt;
    double dyk, eps, incr_thresh__, dx_x__, dz_z__;
    extern  int dla_lin_berr__(int *, int *, int *
	    , double *, double *, double *);
    double ymin;
    extern  int blas_dgemv_x__(int *, int *, int *
	    , double *, double *, int *, double *, int *, 
	    double *, double *, int *, int *);
    int y_prec_state__;
    extern  int blas_dgemv2_x__(int *, int *, int 
	    *, double *, double *, int *, double *, 
	    double *, int *, double *, double *, int *, 
	    int *), dgemv_(char *, int *, int *, double *, 
	    double *, int *, double *, int *, double *, 
	    double *, int *), dcopy_(int *, double *, 
	    int *, double *, int *);
    double dxrat, dzrat;
    extern  int daxpy_(int *, double *, double *, 
	    int *, double *, int *);
    char trans[1];
    double normx, normy;
    extern double dlamch_(char *);
    extern  int dgetrs_(char *, int *, int *, 
	    double *, int *, int *, double *, int *, 
	    int *);
    double normdx;
    extern /* Character */ VOID chla_transtype__(char *, unsigned long, int *);
    double hugeval;
    int x_state__, z_state__;


/*     -- LAPACK routine (version 3.2.1)                                 -- */
/*     -- Contributed by James Demmel, Deaglan Halligan, Yozo Hida and -- */
/*     -- Jason Riedy of Univ. of California Berkeley.                 -- */
/*     -- April 2009                                                   -- */

/*     -- LAPACK is a software package provided by Univ. of Tennessee, -- */
/*     -- Univ. of California Berkeley and NAG Ltd.                    -- */

/*     .. */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DLA_GERFSX_EXTENDED improves the computed solution to a system of */
/*  linear equations by performing extra-precise iterative refinement */
/*  and provides error bounds and backward error estimates for the solution. */
/*  This subroutine is called by DGERFSX to perform iterative refinement. */
/*  In addition to normwise error bound, the code provides maximum */
/*  componentwise error bound if possible. See comments for ERR_BNDS_NORM */
/*  and ERR_BNDS_COMP for details of the error bounds. Note that this */
/*  subroutine is only resonsible for setting the second fields of */
/*  ERR_BNDS_NORM and ERR_BNDS_COMP. */

/*  Arguments */
/*  ========= */

/*     PREC_TYPE      (input) INTEGER */
/*     Specifies the intermediate precision to be used in refinement. */
/*     The value is defined by ILAPREC(P) where P is a CHARACTER and */
/*     P    = 'S':  Single */
/*          = 'D':  Double */
/*          = 'I':  Indigenous */
/*          = 'X', 'E':  Extra */

/*     TRANS_TYPE     (input) INTEGER */
/*     Specifies the transposition operation on A. */
/*     The value is defined by ILATRANS(T) where T is a CHARACTER and */
/*     T    = 'N':  No transpose */
/*          = 'T':  Transpose */
/*          = 'C':  Conjugate transpose */

/*     N              (input) INTEGER */
/*     The number of linear equations, i.e., the order of the */
/*     matrix A.  N >= 0. */

/*     NRHS           (input) INTEGER */
/*     The number of right-hand-sides, i.e., the number of columns of the */
/*     matrix B. */

/*     A              (input) DOUBLE PRECISION array, dimension (LDA,N) */
/*     On entry, the N-by-N matrix A. */

/*     LDA            (input) INTEGER */
/*     The leading dimension of the array A.  LDA >= MAX(1,N). */

/*     AF             (input) DOUBLE PRECISION array, dimension (LDAF,N) */
/*     The factors L and U from the factorization */
/*     A = P*L*U as computed by DGETRF. */

/*     LDAF           (input) INTEGER */
/*     The leading dimension of the array AF.  LDAF >= MAX(1,N). */

/*     IPIV           (input) INTEGER array, dimension (N) */
/*     The pivot indices from the factorization A = P*L*U */
/*     as computed by DGETRF; row i of the matrix was interchanged */
/*     with row IPIV(i). */

/*     COLEQU         (input) LOGICAL */
/*     If .TRUE. then column equilibration was done to A before calling */
/*     this routine. This is needed to compute the solution and error */
/*     bounds correctly. */

/*     C              (input) DOUBLE PRECISION  array, dimension (N) */
/*     The column scale factors for A. If COLEQU = .FALSE., C */
/*     is not accessed. If C is input, each element of C should be a power */
/*     of the radix to ensure a reliable solution and error estimates. */
/*     Scaling by powers of the radix does not cause rounding errors unless */
/*     the result underflows or overflows. Rounding errors during scaling */
/*     lead to refining with a matrix that is not equivalent to the */
/*     input matrix, producing error estimates that may not be */
/*     reliable. */

/*     B              (input) DOUBLE PRECISION array, dimension (LDB,NRHS) */
/*     The right-hand-side matrix B. */

/*     LDB            (input) INTEGER */
/*     The leading dimension of the array B.  LDB >= MAX(1,N). */

/*     Y              (input/output) DOUBLE PRECISION array, dimension */
/*                    (LDY,NRHS) */
/*     On entry, the solution matrix X, as computed by DGETRS. */
/*     On exit, the improved solution matrix Y. */

/*     LDY            (input) INTEGER */
/*     The leading dimension of the array Y.  LDY >= MAX(1,N). */

/*     BERR_OUT       (output) DOUBLE PRECISION array, dimension (NRHS) */
/*     On exit, BERR_OUT(j) contains the componentwise relative backward */
/*     error for right-hand-side j from the formula */
/*         MAX(i) ( ABS(RES(i)) / ( ABS(op(A_s))*ABS(Y) + ABS(B_s) )(i) ) */
/*     where ABS(Z) is the componentwise absolute value of the matrix */
/*     or vector Z. This is computed by DLA_LIN_BERR. */

/*     N_NORMS        (input) INTEGER */
/*     Determines which error bounds to return (see ERR_BNDS_NORM */
/*     and ERR_BNDS_COMP). */
/*     If N_NORMS >= 1 return normwise error bounds. */
/*     If N_NORMS >= 2 return componentwise error bounds. */

/*     ERR_BNDS_NORM  (input/output) DOUBLE PRECISION array, dimension */
/*                    (NRHS, N_ERR_BNDS) */
/*     For each right-hand side, this array contains information about */
/*     various error bounds and condition numbers corresponding to the */
/*     normwise relative error, which is defined as follows: */

/*     Normwise relative error in the ith solution vector: */
/*             max_j (ABS(XTRUE(j,i) - X(j,i))) */
/*            ------------------------------ */
/*                  max_j ABS(X(j,i)) */

/*     The array is indexed by the type of error information as described */
/*     below. There currently are up to three pieces of information */
/*     returned. */

/*     The first index in ERR_BNDS_NORM(i,:) corresponds to the ith */
/*     right-hand side. */

/*     The second index in ERR_BNDS_NORM(:,err) contains the following */
/*     three fields: */
/*     err = 1 "Trust/don't trust" boolean. Trust the answer if the */
/*              reciprocal condition number is less than the threshold */
/*              sqrt(n) * slamch('Epsilon'). */

/*     err = 2 "Guaranteed" error bound: The estimated forward error, */
/*              almost certainly within a factor of 10 of the true error */
/*              so long as the next entry is greater than the threshold */
/*              sqrt(n) * slamch('Epsilon'). This error bound should only */
/*              be trusted if the previous boolean is true. */

/*     err = 3  Reciprocal condition number: Estimated normwise */
/*              reciprocal condition number.  Compared with the threshold */
/*              sqrt(n) * slamch('Epsilon') to determine if the error */
/*              estimate is "guaranteed". These reciprocal condition */
/*              numbers are 1 / (norm(Z^{-1},inf) * norm(Z,inf)) for some */
/*              appropriately scaled matrix Z. */
/*              Let Z = S*A, where S scales each row by a power of the */
/*              radix so all absolute row sums of Z are approximately 1. */

/*     This subroutine is only responsible for setting the second field */
/*     above. */
/*     See Lapack Working Note 165 for further details and extra */
/*     cautions. */

/*     ERR_BNDS_COMP  (input/output) DOUBLE PRECISION array, dimension */
/*                    (NRHS, N_ERR_BNDS) */
/*     For each right-hand side, this array contains information about */
/*     various error bounds and condition numbers corresponding to the */
/*     componentwise relative error, which is defined as follows: */

/*     Componentwise relative error in the ith solution vector: */
/*                    ABS(XTRUE(j,i) - X(j,i)) */
/*             max_j ---------------------- */
/*                         ABS(X(j,i)) */

/*     The array is indexed by the right-hand side i (on which the */
/*     componentwise relative error depends), and the type of error */
/*     information as described below. There currently are up to three */
/*     pieces of information returned for each right-hand side. If */
/*     componentwise accuracy is not requested (PARAMS(3) = 0.0), then */
/*     ERR_BNDS_COMP is not accessed.  If N_ERR_BNDS .LT. 3, then at most */
/*     the first (:,N_ERR_BNDS) entries are returned. */

/*     The first index in ERR_BNDS_COMP(i,:) corresponds to the ith */
/*     right-hand side. */

/*     The second index in ERR_BNDS_COMP(:,err) contains the following */
/*     three fields: */
/*     err = 1 "Trust/don't trust" boolean. Trust the answer if the */
/*              reciprocal condition number is less than the threshold */
/*              sqrt(n) * slamch('Epsilon'). */

/*     err = 2 "Guaranteed" error bound: The estimated forward error, */
/*              almost certainly within a factor of 10 of the true error */
/*              so long as the next entry is greater than the threshold */
/*              sqrt(n) * slamch('Epsilon'). This error bound should only */
/*              be trusted if the previous boolean is true. */

/*     err = 3  Reciprocal condition number: Estimated componentwise */
/*              reciprocal condition number.  Compared with the threshold */
/*              sqrt(n) * slamch('Epsilon') to determine if the error */
/*              estimate is "guaranteed". These reciprocal condition */
/*              numbers are 1 / (norm(Z^{-1},inf) * norm(Z,inf)) for some */
/*              appropriately scaled matrix Z. */
/*              Let Z = S*(A*diag(x)), where x is the solution for the */
/*              current right-hand side and S scales each row of */
/*              A*diag(x) by a power of the radix so all absolute row */
/*              sums of Z are approximately 1. */

/*     This subroutine is only responsible for setting the second field */
/*     above. */
/*     See Lapack Working Note 165 for further details and extra */
/*     cautions. */

/*     RES            (input) DOUBLE PRECISION array, dimension (N) */
/*     Workspace to hold the intermediate residual. */

/*     AYB            (input) DOUBLE PRECISION array, dimension (N) */
/*     Workspace. This can be the same workspace passed for Y_TAIL. */

/*     DY             (input) DOUBLE PRECISION array, dimension (N) */
/*     Workspace to hold the intermediate solution. */

/*     Y_TAIL         (input) DOUBLE PRECISION array, dimension (N) */
/*     Workspace to hold the trailing bits of the intermediate solution. */

/*     RCOND          (input) DOUBLE PRECISION */
/*     Reciprocal scaled condition number.  This is an estimate of the */
/*     reciprocal Skeel condition number of the matrix A after */
/*     equilibration (if done).  If this is less than the machine */
/*     precision (in particular, if it is zero), the matrix is singular */
/*     to working precision.  Note that the error may still be small even */
/*     if this number is very small and the matrix appears ill- */
/*     conditioned. */

/*     ITHRESH        (input) INTEGER */
/*     The maximum number of residual computations allowed for */
/*     refinement. The default is 10. For 'aggressive' set to 100 to */
/*     permit convergence using approximate factorizations or */
/*     factorizations other than LU. If the factorization uses a */
/*     technique other than Gaussian elimination, the guarantees in */
/*     ERR_BNDS_NORM and ERR_BNDS_COMP may no longer be trustworthy. */

/*     RTHRESH        (input) DOUBLE PRECISION */
/*     Determines when to stop refinement if the error estimate stops */
/*     decreasing. Refinement will stop when the next solution no longer */
/*     satisfies norm(dx_{i+1}) < RTHRESH * norm(dx_i) where norm(Z) is */
/*     the infinity norm of Z. RTHRESH satisfies 0 < RTHRESH <= 1. The */
/*     default value is 0.5. For 'aggressive' set to 0.9 to permit */
/*     convergence on extremely ill-conditioned matrices. See LAWN 165 */
/*     for more details. */

/*     DZ_UB          (input) DOUBLE PRECISION */
/*     Determines when to start considering componentwise convergence. */
/*     Componentwise convergence is only considered after each component */
/*     of the solution Y is stable, which we definte as the relative */
/*     change in each component being less than DZ_UB. The default value */
/*     is 0.25, requiring the first bit to be stable. See LAWN 165 for */
/*     more details. */

/*     IGNORE_CWISE   (input) LOGICAL */
/*     If .TRUE. then ignore componentwise convergence. Default value */
/*     is .FALSE.. */

/*     INFO           (output) INTEGER */
/*       = 0:  Successful exit. */
/*       < 0:  if INFO = -i, the ith argument to DGETRS had an illegal */
/*             value */

/*  ===================================================================== */

/*     .. Local Scalars .. */
/*     .. */
/*     .. Parameters .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    errs_c_dim1 = *nrhs;
    errs_c_offset = 1 + errs_c_dim1;
    errs_c__ -= errs_c_offset;
    errs_n_dim1 = *nrhs;
    errs_n_offset = 1 + errs_n_dim1;
    errs_n__ -= errs_n_offset;
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    af_dim1 = *ldaf;
    af_offset = 1 + af_dim1;
    af -= af_offset;
    --ipiv;
    --c__;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    y_dim1 = *ldy;
    y_offset = 1 + y_dim1;
    y -= y_offset;
    --berr_out__;
    --res;
    --ayb;
    --dy;
    --y_tail__;

    /* Function Body */
    if (*info != 0) {
	return 0;
    }
    chla_transtype__(ch__1, (unsigned long)1, trans_type__);
    *(unsigned char *)trans = *(unsigned char *)&ch__1[0];
    eps = dlamch_("Epsilon");
    hugeval = dlamch_("Overflow");
/*     Force HUGEVAL to Inf */
    hugeval *= hugeval;
/*     Using HUGEVAL may lead to spurious underflows. */
    incr_thresh__ = (double) (*n) * eps;

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {
	y_prec_state__ = 1;
	if (y_prec_state__ == 2) {
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		y_tail__[i__] = 0.;
	    }
	}
	dxrat = 0.;
	dxratmax = 0.;
	dzrat = 0.;
	dzratmax = 0.;
	final_dx_x__ = hugeval;
	final_dz_z__ = hugeval;
	prevnormdx = hugeval;
	prev_dz_z__ = hugeval;
	dz_z__ = hugeval;
	dx_x__ = hugeval;
	x_state__ = 1;
	z_state__ = 0;
	incr_prec__ = FALSE;
	i__2 = *ithresh;
	for (cnt = 1; cnt <= i__2; ++cnt) {

/*         Compute residual RES = B_s - op(A_s) * Y, */
/*             op(A) = A, A**T, or A**H depending on TRANS (and type). */

	    dcopy_(n, &b[j * b_dim1 + 1], &c__1, &res[1], &c__1);
	    if (y_prec_state__ == 0) {
		dgemv_(trans, n, n, &c_b6, &a[a_offset], lda, &y[j * y_dim1 + 
			1], &c__1, &c_b8, &res[1], &c__1);
	    } else if (y_prec_state__ == 1) {
		blas_dgemv_x__(trans_type__, n, n, &c_b6, &a[a_offset], lda, &
			y[j * y_dim1 + 1], &c__1, &c_b8, &res[1], &c__1, 
			prec_type__);
	    } else {
		blas_dgemv2_x__(trans_type__, n, n, &c_b6, &a[a_offset], lda, 
			&y[j * y_dim1 + 1], &y_tail__[1], &c__1, &c_b8, &res[
			1], &c__1, prec_type__);
	    }
/*        XXX: RES is no longer needed. */
	    dcopy_(n, &res[1], &c__1, &dy[1], &c__1);
	    dgetrs_(trans, n, &c__1, &af[af_offset], ldaf, &ipiv[1], &dy[1], 
		    n, info);

/*         Calculate relative changes DX_X, DZ_Z and ratios DXRAT, DZRAT. */

	    normx = 0.;
	    normy = 0.;
	    normdx = 0.;
	    dz_z__ = 0.;
	    ymin = hugeval;

	    i__3 = *n;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		yk = (d__1 = y[i__ + j * y_dim1], ABS(d__1));
		dyk = (d__1 = dy[i__], ABS(d__1));
		if (yk != 0.) {
/* Computing MAX */
		    d__1 = dz_z__, d__2 = dyk / yk;
		    dz_z__ = MAX(d__1,d__2);
		} else if (dyk != 0.) {
		    dz_z__ = hugeval;
		}
		ymin = MIN(ymin,yk);
		normy = MAX(normy,yk);
		if (*colequ) {
/* Computing MAX */
		    d__1 = normx, d__2 = yk * c__[i__];
		    normx = MAX(d__1,d__2);
/* Computing MAX */
		    d__1 = normdx, d__2 = dyk * c__[i__];
		    normdx = MAX(d__1,d__2);
		} else {
		    normx = normy;
		    normdx = MAX(normdx,dyk);
		}
	    }
	    if (normx != 0.) {
		dx_x__ = normdx / normx;
	    } else if (normdx == 0.) {
		dx_x__ = 0.;
	    } else {
		dx_x__ = hugeval;
	    }
	    dxrat = normdx / prevnormdx;
	    dzrat = dz_z__ / prev_dz_z__;

/*         Check termination criteria */

	    if (! (*ignore_cwise__) && ymin * *rcond < incr_thresh__ * normy 
		    && y_prec_state__ < 2) {
		incr_prec__ = TRUE;
	    }
	    if (x_state__ == 3 && dxrat <= *rthresh) {
		x_state__ = 1;
	    }
	    if (x_state__ == 1) {
		if (dx_x__ <= eps) {
		    x_state__ = 2;
		} else if (dxrat > *rthresh) {
		    if (y_prec_state__ != 2) {
			incr_prec__ = TRUE;
		    } else {
			x_state__ = 3;
		    }
		} else {
		    if (dxrat > dxratmax) {
			dxratmax = dxrat;
		    }
		}
		if (x_state__ > 1) {
		    final_dx_x__ = dx_x__;
		}
	    }
	    if (z_state__ == 0 && dz_z__ <= *dz_ub__) {
		z_state__ = 1;
	    }
	    if (z_state__ == 3 && dzrat <= *rthresh) {
		z_state__ = 1;
	    }
	    if (z_state__ == 1) {
		if (dz_z__ <= eps) {
		    z_state__ = 2;
		} else if (dz_z__ > *dz_ub__) {
		    z_state__ = 0;
		    dzratmax = 0.;
		    final_dz_z__ = hugeval;
		} else if (dzrat > *rthresh) {
		    if (y_prec_state__ != 2) {
			incr_prec__ = TRUE;
		    } else {
			z_state__ = 3;
		    }
		} else {
		    if (dzrat > dzratmax) {
			dzratmax = dzrat;
		    }
		}
		if (z_state__ > 1) {
		    final_dz_z__ = dz_z__;
		}
	    }

/*           Exit if both normwise and componentwise stopped working, */
/*           but if componentwise is unstable, let it go at least two */
/*           iterations. */

	    if (x_state__ != 1) {
		if (*ignore_cwise__) {
		    goto L666;
		}
		if (z_state__ == 3 || z_state__ == 2) {
		    goto L666;
		}
		if (z_state__ == 0 && cnt > 1) {
		    goto L666;
		}
	    }
	    if (incr_prec__) {
		incr_prec__ = FALSE;
		++y_prec_state__;
		i__3 = *n;
		for (i__ = 1; i__ <= i__3; ++i__) {
		    y_tail__[i__] = 0.;
		}
	    }
	    prevnormdx = normdx;
	    prev_dz_z__ = dz_z__;

/*           Update soluton. */

	    if (y_prec_state__ < 2) {
		daxpy_(n, &c_b8, &dy[1], &c__1, &y[j * y_dim1 + 1], &c__1);
	    } else {
		dla_wwaddw__(n, &y[j * y_dim1 + 1], &y_tail__[1], &dy[1]);
	    }
	}
/*        Target of "IF (Z_STOP .AND. X_STOP)".  Sun's f77 won't EXIT. */
L666:

/*     Set final_* when cnt hits ithresh. */

	if (x_state__ == 1) {
	    final_dx_x__ = dx_x__;
	}
	if (z_state__ == 1) {
	    final_dz_z__ = dz_z__;
	}

/*     Compute error bounds */

	if (*n_norms__ >= 1) {
	    errs_n__[j + (errs_n_dim1 << 1)] = final_dx_x__ / (1 - dxratmax);
	}
	if (*n_norms__ >= 2) {
	    errs_c__[j + (errs_c_dim1 << 1)] = final_dz_z__ / (1 - dzratmax);
	}

/*     Compute componentwise relative backward error from formula */
/*         MAX(i) ( ABS(R(i)) / ( ABS(op(A_s))*ABS(Y) + ABS(B_s) )(i) ) */
/*     where ABS(Z) is the componentwise absolute value of the matrix */
/*     or vector Z. */

/*         Compute residual RES = B_s - op(A_s) * Y, */
/*             op(A) = A, A**T, or A**H depending on TRANS (and type). */

	dcopy_(n, &b[j * b_dim1 + 1], &c__1, &res[1], &c__1);
	dgemv_(trans, n, n, &c_b6, &a[a_offset], lda, &y[j * y_dim1 + 1], &
		c__1, &c_b8, &res[1], &c__1);
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ayb[i__] = (d__1 = b[i__ + j * b_dim1], ABS(d__1));
	}

/*     Compute ABS(op(A_s))*ABS(Y) + ABS(B_s). */

	dla_geamv__(trans_type__, n, n, &c_b8, &a[a_offset], lda, &y[j * 
		y_dim1 + 1], &c__1, &c_b8, &ayb[1], &c__1);
	dla_lin_berr__(n, n, &c__1, &res[1], &ayb[1], &berr_out__[j]);

/*     End of loop for each RHS. */

    }

    return 0;
} /* dla_gerfsx_extended__ */
