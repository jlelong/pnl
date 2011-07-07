/* clarscl2.f -- translated by f2c (version 20061008).
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

 int clarscl2_(int *m, int *n, float *d__, complex *x, 
	int *ldx)
{
    /* System generated locals */
    int x_dim1, x_offset, i__1, i__2, i__3, i__4, i__5;
    complex q__1;

    /* Local variables */
    int i__, j;


/*     -- LAPACK routine (version 3.2.1)                               -- */
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

/*  CLARSCL2 performs a reciprocal diagonal scaling on an vector: */
/*    x <-- inv(D) * x */
/*  where the REAL diagonal matrix D is stored as a vector. */

/*  Eventually to be replaced by BLAS_cge_diag_scale in the new BLAS */
/*  standard. */

/*  Arguments */
/*  ========= */

/*     M       (input) INTEGER */
/*     The number of rows of D and X. M >= 0. */

/*     N       (input) INTEGER */
/*     The number of columns of D and X. N >= 0. */

/*     D       (input) REAL array, length M */
/*     Diagonal matrix D, stored as a vector of length M. */

/*     X       (input/output) COMPLEX array, dimension (LDX,N) */
/*     On entry, the vector X to be scaled by D. */
/*     On exit, the scaled vector. */

/*     LDX     (input) INTEGER */
/*     The leading dimension of the vector X. LDX >= 0. */

/*  ===================================================================== */

/*     .. Local Scalars .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --d__;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * x_dim1;
	    i__4 = i__ + j * x_dim1;
	    i__5 = i__;
	    q__1.r = x[i__4].r / d__[i__5], q__1.i = x[i__4].i / d__[i__5];
	    x[i__3].r = q__1.r, x[i__3].i = q__1.i;
	}
    }
    return 0;
} /* clarscl2_ */
