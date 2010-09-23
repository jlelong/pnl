#ifndef _PNL_RANDOM_H 
#define _PNL_RANDOM_H 


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "pnl/pnl_object.h"
#include "pnl/pnl_mathtools.h"
#include "pnl/pnl_types.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_list.h"

/**
 * \defgroup PnlRandom Random generators 
 */
/*@{*/

#define MC 0	
#define QMC 1	
#define CREATE 0 
#define RETRIEVE 1 

/* indices of random generators 
 * The first generators must be true MC generators and the last ones are QMC
 * Do NOT mix them otherwise the arrays  pnl_random and pnl_random_MC will be
 * wrong */
enum {
  PNL_RNG_KNUTH,
  PNL_RNG_MRGK3,
  PNL_RNG_MRGK5,
  PNL_RNG_SHUFL,
  PNL_RNG_LECUYER,
  PNL_RNG_TAUSWORTHE,
  PNL_RNG_MERSENNE,
  PNL_RNG_MERSENNE_RANDOM_SEED,
  /* beginning of QMC sequences */
  PNL_RNG_SQRT,
  PNL_RNG_HALTON,
  PNL_RNG_FAURE,
  PNL_RNG_SOBOL,
  PNL_RNG_SOBOL2,
  PNL_RNG_NIEDERREITER,
  /* end of static rng */
  PNL_RNG_DCMT
};

/**
 * RandomGenerators
 */
typedef unsigned long ulong;

typedef struct _PnlRng PnlRng;
struct _PnlRng
{
  PnlObject object;
  int type; /*!< generator type */
  int id; /*!< index of the generator in the list */
  void (*Compute)(PnlRng *g, double *sample); /*!< the function to compute the
                                                next number in the sequence */
  int rand_or_quasi; /*!< can be MC or QMC */
  int dimension; /*!< dimension of the space in which we draw the samples */
  int max_dim; /*!< maximum dimension of the space in which we draw the samples */
  int counter; /*!< counter = number of samples already drawn */
  int has_gauss; /*!< Is a gaussian deviate available? */
  double gauss; /*!< If has_gauss==1, gauss a gaussian sample */
  int size_state; /*!< size in bytes of the state variable */
  void *state; /*!< state of the random generator */
};

/*
 * This type is only for building a proper enumeration for Premia's list
 */
typedef struct _PnlRngTypes PnlRngTypes;
struct _PnlRngTypes
{
  enum_member base; /* {label, key} */
  PnlRng *rng;
};

/* extern PnlRng PnlRngKnuth;
 * extern PnlRng PnlRngMrgk3;
 * extern PnlRng PnlRngMrgk5;
 * extern PnlRng PnlRngShufl;
 * extern PnlRng PnlRngLecuyer;
 * extern PnlRng PnlRngTausworthe;
 * extern PnlRng PnlRngMersenne;
 * extern PnlRng PnlRngMersenneRandomSeed;
 * extern PnlRng PnlRngSqrt;
 * extern PnlRng PnlRngHalton;
 * extern PnlRng PnlRngFaure;
 * extern PnlRng PnlRngSobol;
 * extern PnlRng PnlRngSobol2;
 * extern PnlRng PnlRngNiederreiter; */

extern PnlRngTypes PnlRngArray[];
extern enum_members RNGs;
extern enum_members MC_RNGs;


extern int pnl_rand_init(int type_generator, int simulation_dim,long samples);
extern void pnl_rand_sseed (int type_generator, ulong seed);
extern int pnl_rand_or_quasi(int type_generator);

/*
 * Generation functions
 */
extern double pnl_rand_gauss(int, int, int, int);
extern int pnl_rand_bernoulli(double p, int generator);
extern long pnl_rand_poisson(double lambda, int type_generator);
extern long pnl_rand_poisson1(double lambda,double t, int type_generator);
extern double pnl_rand_exp(double lambda,int type_generator);
extern double pnl_rand_uni (int type_generator);
extern double pnl_rand_uni_ab (double a, double b, int type_generator);
extern double pnl_rand_normal (int type_generator);
extern void pnl_vect_rand_uni(PnlVect *G, int samples, double a, double b, int type_generator);
extern void pnl_vect_rand_uni_d(PnlVect *G, int dimension, double a, double b, int type_generator);
extern void pnl_vect_rand_normal(PnlVect *G, int samples, int generator);
extern void pnl_vect_rand_normal_d(PnlVect *G, int dimension, int generator);
extern void pnl_mat_rand_uni(PnlMat *M, int samples, int dimension, const PnlVect *a,
                             const PnlVect *b, int type_generator);
extern void pnl_mat_rand_uni2(PnlMat *M, int samples, int dimension,
                              double a, double b, int type_generator);
extern void pnl_mat_rand_normal(PnlMat *M, int samples, int dimension, int type_generator);
extern double pnl_rand_gamma (double a, double b, int gen);
extern double pnl_rand_chi2  (double nu, int gen);

extern double pnl_rng_gauss(int, int, int, PnlRng *rng);
extern int pnl_rng_bernoulli(double p, PnlRng *rng);
extern long pnl_rng_poisson(double lambda, PnlRng *rng);
extern long pnl_rng_poisson1(double lambda,double t, PnlRng *rng);
extern double pnl_rng_exp(double lambda,PnlRng *rng);
extern double pnl_rng_uni (PnlRng *rng);
extern double pnl_rng_uni_ab (double a, double b, PnlRng *rng);
extern double pnl_rng_normal (PnlRng *rng);
extern void pnl_vect_rng_uni(PnlVect *G, int samples, double a, double b, PnlRng *rng);
extern void pnl_vect_rng_uni_d(PnlVect *G, int dimension, double a, double b, PnlRng *rng);
extern void pnl_vect_rng_normal(PnlVect *G, int samples, PnlRng *rng);
extern void pnl_vect_rng_normal_d(PnlVect *G, int dimension, PnlRng *rng);
extern void pnl_mat_rng_uni(PnlMat *M, int samples, int dimension, const PnlVect *a,
                             const PnlVect *b, PnlRng *rng);
extern void pnl_mat_rng_uni2(PnlMat *M, int samples, int dimension,
                              double a, double b, PnlRng *rng);
extern void pnl_mat_rng_normal(PnlMat *M, int samples, int dimension, PnlRng *rng);
extern double pnl_rng_gamma (double a, double b, PnlRng *rng);
extern double pnl_rng_chi2  (double nu, PnlRng *rng);


/*
 * MT
 */
#define MT_N 624

typedef struct 
{
  unsigned long mt[MT_N];
  int mti;
} mt_state;

extern void pnl_mt_sseed(mt_state *state, unsigned long int s);
extern unsigned long pnl_mt_genrand (mt_state *state);
extern double pnl_mt_genrand_double (mt_state *state);



/*
 * DCMT
 */
#define DCMT_N 17 /* N = p / w + 1; p = 521; w = 32; */

typedef struct
{
  ulong aaa;
  int mm,nn,rr,ww;
  ulong wmask,umask,lmask;
  int shift0, shift1, shiftB, shiftC;
  ulong maskB, maskC;
  int i;
  ulong state[DCMT_N];
} dcmt_state;

extern dcmt_state* pnl_dcmt_get_parameter(ulong seed);
extern dcmt_state** pnl_dcmt_create_array(int n, ulong seed, int *count);
extern void pnl_dcmt_sseed (dcmt_state *mts, ulong s);
extern double pnl_dcmt_genrand_double(dcmt_state *mts);
extern void pnl_dcmt_free(dcmt_state **mts);
extern void pnl_dcmt_free_array(dcmt_state **mts, int count);
extern PnlRng** pnl_rng_dcmt_create_array (int n, ulong seed, int *count);

/*
 * Dynamic List of Rngs
 */
/* extern PnlList *PnlRngList; */
extern int pnl_rand_add (PnlRng *rng);
extern int pnl_rand_add_dcmt (dcmt_state *mts);
extern void pnl_rand_remove (int id);

/*@}*/

/*
 * PnlRng interface
 */
extern void pnl_rng_free(PnlRng **);
extern PnlRng* pnl_rng_new ();
extern PnlRng* pnl_rng_create (int type);
extern void pnl_rng_init (PnlRng *rng, int type);
PnlRng* pnl_rng_get_from_id (int type_generator);
extern void pnl_rng_sseed(PnlRng *rng, unsigned long int s);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _PNL_RANDOM_H */ 