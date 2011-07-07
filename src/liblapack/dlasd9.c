
#include "pnl/pnl_f2c.h"

 int dlasd9_(int *icompq, int *ldu, int *k, 
	double *d__, double *z__, double *vf, double *vl, 
	double *difl, double *difr, double *dsigma, double *
	work, int *info)
{
/*  -- LAPACK auxiliary routine (version 3.0) --   
       Univ. of Tennessee, Oak Ridge National Lab, Argonne National Lab,   
       Courant Institute, NAG Ltd., and Rice University   
       June 30, 1999   


    Purpose   
    =======   

    DLASD9 finds the square roots of the roots of the secular equation,   
    as defined by the values in DSIGMA and Z.  It makes the   
    appropriate calls to DLASD4, and stores, for each  element in D,   
    the distance to its two nearest poles (elements in DSIGMA). It also   
    updates the arrays VF and VL, the first and last components of all   
    the right singular vectors of the original bidiagonal matrix.   

    DLASD9 is called from DLASD7.   

    Arguments   
    =========   

    ICOMPQ  (input) INTEGER   
            Specifies whether singular vectors are to be computed in   
            factored form in the calling routine:   

               ICOMPQ = 0             Compute singular values only.   

               ICOMPQ = 1             Compute singular vector matrices in   
                                      factored form also.   
    K       (input) INTEGER   
            The number of terms in the rational function to be solved by   
            DLASD4.  K >= 1.   

    D       (output) DOUBLE PRECISION array, dimension(K)   
            D(I) contains the updated singular values.   

    DSIGMA  (input) DOUBLE PRECISION array, dimension(K)   
            The first K elements of this array contain the old roots   
            of the deflated updating problem.  These are the poles   
            of the secular equation.   

    Z       (input) DOUBLE PRECISION array, dimension (K)   
            The first K elements of this array contain the components   
            of the deflation-adjusted updating row vector.   

    VF      (input/output) DOUBLE PRECISION array, dimension(K)   
            On entry, VF contains  information passed through SBEDE8.f   
            On exit, VF contains the first K components of the first   
            components of all right singular vectors of the bidiagonal   
            matrix.   

    VL      (input/output) DOUBLE PRECISION array, dimension(K)   
            On entry, VL contains  information passed through SBEDE8.f   
            On exit, VL contains the first K components of the last   
            components of all right singular vectors of the bidiagonal   
            matrix.   

    DIFL    (output) DOUBLE PRECISION array, dimension (K).   
            On exit, DIFL(I) = D(I) - DSIGMA(I).   

    DIFR    (output) DOUBLE PRECISION array,   
                                dimension (LDU, 2) if ICOMPQ =1 and   
                                dimension (K) if ICOMPQ = 0.   
            On exit, DIFR(I, 1) = D(I) - DSIGMA(I+1), DIFR(K, 1) is not   
            defined and will not be referenced.   

            If ICOMPQ = 1, DIFR(1:K, 2) is an array containing the   
            normalizing factors for the right singular vector matrix.   

    WORK    (workspace) DOUBLE PRECISION array,   
                                   dimension at least (3 * K)   
            Workspace.   

    INFO    (output) INTEGER   
            = 0:  successful exit.   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   
            > 0:  if INFO = 1, an singular value did not converge   

    Further Details   
    ===============   

    Based on contributions by   
       Ming Gu and Huan Ren, Computer Science Division, University of   
       California at Berkeley, USA   

    =====================================================================   


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static int c__1 = 1;
    static int c__0 = 0;
    static double c_b8 = 1.;
    
    /* System generated locals */
    int difr_dim1, difr_offset, i__1, i__2;
    double d__1, d__2;
    /* Builtin functions */
    double sqrt(double), d_sign(double *, double *);
    /* Local variables */
    extern double ddot_(int *, double *, int *, double *, 
	    int *);
    static double temp;
    extern double dnrm2_(int *, double *, int *);
    static int iwk2i, iwk3i, i__, j;
    static double diflj, difrj, dsigj;
    extern  int dcopy_(int *, double *, int *, 
	    double *, int *);
    extern double pnl_dlamc3(double *, double *);
    extern  int dlasd4_(int *, int *, double *, 
	    double *, double *, double *, double *, 
	    double *, int *);
    static double dj;
    extern  int dlascl_(char *, int *, int *, 
	    double *, double *, int *, int *, double *, 
	    int *, int *), dlaset_(char *, int *, int 
	    *, double *, double *, double *, int *), 
	    xerbla_(char *, int *);
    static double dsigjp, rho, djp1;
    static int iwk1, iwk2, iwk3;
#define difr_ref(a_1,a_2) difr[(a_2)*difr_dim1 + a_1]


    difr_dim1 = *ldu;
    difr_offset = 1 + difr_dim1 * 1;
    difr -= difr_offset;
    --d__;
    --z__;
    --vf;
    --vl;
    --difl;
    --dsigma;
    --work;

    /* Function Body */
    *info = 0;

    if (*icompq < 0 || *icompq > 1) {
	*info = -1;
    } else if (*k < 1) {
	*info = -3;
    } else if (*ldu < *k) {
	*info = -2;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DLASD9", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*k == 1) {
	d__[1] = ABS(z__[1]);
	difl[1] = d__[1];
	if (*icompq == 1) {
	    difl[2] = 1.;
	    difr_ref(1, 2) = 1.;
	}
	return 0;
    }

/*     Modify values DSIGMA(i) to make sure all DSIGMA(i)-DSIGMA(j) can   
       be computed with high relative accuracy (barring over/underflow).   
       This is a problem on machines without a guard digit in   
       add/subtract (Cray XMP, Cray YMP, Cray C 90 and Cray 2).   
       The following code replaces DSIGMA(I) by 2*DSIGMA(I)-DSIGMA(I),   
       which on any of these machines zeros out the bottommost   
       bit of DSIGMA(I) if it is 1; this makes the subsequent   
       subtractions DSIGMA(I)-DSIGMA(J) unproblematic when cancellation   
       occurs. On binary machines with a guard digit (almost all   
       machines) it does not change DSIGMA(I) at all. On hexadecimal   
       and decimal machines with a guard digit, it slightly   
       changes the bottommost bits of DSIGMA(I). It does not account   
       for hexadecimal or decimal machines without guard digits   
       (we know of none). We use a subroutine call to compute   
       2*DLAMBDA(I) to prevent optimizing compilers from eliminating   
       this code. */

    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	dsigma[i__] = pnl_dlamc3(&dsigma[i__], &dsigma[i__]) - dsigma[i__];
/* L10: */
    }

/*     Book keeping. */

    iwk1 = 1;
    iwk2 = iwk1 + *k;
    iwk3 = iwk2 + *k;
    iwk2i = iwk2 - 1;
    iwk3i = iwk3 - 1;

/*     Normalize Z. */

    rho = dnrm2_(k, &z__[1], &c__1);
    dlascl_("G", &c__0, &c__0, &rho, &c_b8, k, &c__1, &z__[1], k, info);
    rho *= rho;

/*     Initialize WORK(IWK3). */

    dlaset_("A", k, &c__1, &c_b8, &c_b8, &work[iwk3], k);

/*     Compute the updated singular values, the arrays DIFL, DIFR,   
       and the updated Z. */

    i__1 = *k;
    for (j = 1; j <= i__1; ++j) {
	dlasd4_(k, &j, &dsigma[1], &z__[1], &work[iwk1], &rho, &d__[j], &work[
		iwk2], info);

/*        If the root finder fails, the computation is terminated. */

	if (*info != 0) {
	    return 0;
	}
	work[iwk3i + j] = work[iwk3i + j] * work[j] * work[iwk2i + j];
	difl[j] = -work[j];
	difr_ref(j, 1) = -work[j + 1];
	i__2 = j - 1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    work[iwk3i + i__] = work[iwk3i + i__] * work[i__] * work[iwk2i + 
		    i__] / (dsigma[i__] - dsigma[j]) / (dsigma[i__] + dsigma[
		    j]);
/* L20: */
	}
	i__2 = *k;
	for (i__ = j + 1; i__ <= i__2; ++i__) {
	    work[iwk3i + i__] = work[iwk3i + i__] * work[i__] * work[iwk2i + 
		    i__] / (dsigma[i__] - dsigma[j]) / (dsigma[i__] + dsigma[
		    j]);
/* L30: */
	}
/* L40: */
    }

/*     Compute updated Z. */

    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	d__2 = sqrt((d__1 = work[iwk3i + i__], ABS(d__1)));
	z__[i__] = d_sign(&d__2, &z__[i__]);
/* L50: */
    }

/*     Update VF and VL. */

    i__1 = *k;
    for (j = 1; j <= i__1; ++j) {
	diflj = difl[j];
	dj = d__[j];
	dsigj = -dsigma[j];
	if (j < *k) {
	    difrj = -difr_ref(j, 1);
	    djp1 = d__[j + 1];
	    dsigjp = -dsigma[j + 1];
	}
	work[j] = -z__[j] / diflj / (dsigma[j] + dj);
	i__2 = j - 1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    work[i__] = z__[i__] / (pnl_dlamc3(&dsigma[i__], &dsigj) - diflj) / (
		    dsigma[i__] + dj);
/* L60: */
	}
	i__2 = *k;
	for (i__ = j + 1; i__ <= i__2; ++i__) {
	    work[i__] = z__[i__] / (pnl_dlamc3(&dsigma[i__], &dsigjp) + difrj) / 
		    (dsigma[i__] + dj);
/* L70: */
	}
	temp = dnrm2_(k, &work[1], &c__1);
	work[iwk2i + j] = ddot_(k, &work[1], &c__1, &vf[1], &c__1) / temp;
	work[iwk3i + j] = ddot_(k, &work[1], &c__1, &vl[1], &c__1) / temp;
	if (*icompq == 1) {
	    difr_ref(j, 2) = temp;
	}
/* L80: */
    }

    dcopy_(k, &work[iwk2], &c__1, &vf[1], &c__1);
    dcopy_(k, &work[iwk3], &c__1, &vl[1], &c__1);

    return 0;

/*     End of DLASD9 */

} /* dlasd9_ */

#undef difr_ref


