#include <pari/pari.h>
#include <assert.h>

/*
GP;install("extgcd", "GG&&", "gcdex", "./libextgcd.so");
*/

/* return d = gcd(a,b), sets u, v such that au + bv = gcd(a,b) */
GEN
extgcd(GEN A, GEN B, GEN *U, GEN *V)
{
  pari_sp av = avma;
  GEN ux = gen_1, vx = gen_0, a = A, b = B;

  if (typ(a) != t_INT) pari_err_TYPE("extgcd",a);
  if (typ(b) != t_INT) pari_err_TYPE("extgcd",b);
  if (signe(a) < 0) { a = negi(a); ux = negi(ux); }
  while (!gequal0(b))
  {
    GEN r, q = dvmdii(a, b, &r), v = vx;

    vx = subii(ux, mulii(q, vx));
    ux = v; a = b; b = r;
  }
  *U = ux;
  *V = diviiexact( subii(a, mulii(A,ux)), B );
  gerepileall(av, 3, &a, U, V); return a;
}

void
cp_vec_to_matrix(GEN** M, GEN x, long idx)
{
  assert(M!=NULL && *M!=NULL);
  assert(lg(*M) > 0); 
  assert(lg(x) == lg((*M)[1])); // length of x = length of M's column vectors
  
  long n = lg(x), i; // i = row idx, idx = col index
  for (i = 0; i < n; i++)
    (*M)[idx][i] = x[i]; 
}

long
simple_filter(GEN** M, GEN* args)
{
  long M_size = lg(*M);
  long filtered_size = 0;
  long i = 0; 
  
  for (i = 0; i < M_size; i++)
  {
    if ((*M)[i][1] % 2) // check the first value in i'th column is even
    {
      cp_vec_to_matrix(M, (*M)[i], filtered_size); 
      filtered_size++;
    }
  }
  
  return filtered_size;
}

/*
  M is a list of vectors we want to filter
  args is a list of GEN elements. Here we expect a factor k, list of primes, and a bound M
*/
/* long */
/* filter(GEN** M, GEN* args) */
/* { */
/*   pari_sp av = avma; // need to unwind stack by myself */
/*   long M_size = lg(*M); */
/*   if (M_size < 1) return M_size; */

/*   GEN x = (*M)[1];  */
/*   long x_length = lg(x); */

/*   GEN k = args[1]; // factor (t_REAL) */
/*   GEN* P = (GEN*)args[2]; // primes (t_INT ?)  */
/*   GEN C = args[3]; // bound (t_REAL) */

/*   long i = 0, j = 0, l = 0, k = 0; // j represents position in return list */
/*   GEN b = cgetg(x_length+1, t_VECSMALL); // initialize to zero (length = n + 1) */
/*   GEN v = cgetg(x_length+1, t_VECSMALL); // initialize to zero */
/*   for (i = 0; i < x_length+1; i++) */
/*     v[i+1] = 0; // one indexing */

/*   while (l < M_size) */
/*   { */
/*     x = (*M)[l];  */
/*     while (i < x_length) // zero or one indexed? assuming one */
/*     { */
/*       for (k = 0; k < x_length+1; k++) */
/*         b[k+1] = 0; */

/*       // ----------------THESE NEED TO BE REAL NUMBERS----------------------- */
/*       // USE MATRIX AND VECTOR MULTIPLICATION FUNCTIONS */

// ASSUME RATIONAL OPERATIONS, since that's what happens in minim0_dolll

      
/*       b[i+1] = 1; */
/*       // b[x_length+1] = k * log(P[i]); */
/*       mulrrz(k, glog(P[i],28), &b[x_length+1]); // glog(t_REAL,prec), where prec=28 is the default */
/*       // v = v + x[i] * b; */
/*       for (k = 0; k < x_length+1; k++) */
/*         v[k+1] = v[k+1] + x[i] * b[k+1];  */
/*       addrrz(v[k+1]  */
/*       i++; */
/*     } */

/*     GEN norm = norm(v);  */
/*     if (norm < C) */
/*     { */
/*       M[j] = x; */
/*       j++; */
/*     } */
/*     l++;  */
/*   } */
    
/*   avma = av; // unwind stack */
/*   return filtered_size;  */
/* } */

int
main()
{
  GEN d; 
  pari_init(1000000,2);
  // set flag to 3 = output to file, apply simple filter
  /* d = qfminim0(matid(3),NULL,NULL,3,(long)simple_filter,NULL,28); */
  d = qfminim0(matid(3),NULL,NULL,0,0,NULL,28);
  pari_printf("%Ps\n", d);
  pari_close();
  return 0;
}
