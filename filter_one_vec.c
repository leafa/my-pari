#include <pari/pari.h>
#include <assert.h>

// use this on large data to test memory usage
GEN
silly_filter(GEN v, GEN args)
{
  if (itos(gel(v,1)) != 0)
    v = NULL; // if v doesn't satisfy the filter criteria, set it to NULL
  return v; 
}

GEN
simple_filter(GEN v, GEN args) 
{
  long modulus = itos(gel(args,1)); 
  if (itos(gel(v,1)) % modulus != 1)
    v = NULL; // if v doesn't satisfy the filter criteria, set it to NULL
  return v; 
}

/*
  M is a list of vectors we want to filter
  args is a list of GEN elements. Here we expect a factor k, list of primes, and a bound M
*/
long
linear_form_in_log_filter(GEN M, GEN args, long M_size)
{
  GEN x = gel(M,1); // first vector in M
  long x_length = lg(x)-1; // effective length

  // first arg is reserved for file name
  // ignore real values for now - later change M to a real matrix
  GEN k = gel(args,2); // factor (t_REAL) 
  GEN P = gel(args,3); // primes (t_INT)
  GEN C = gel(args,4); // bound (t_REAL)

  long i = 1, j = 1; 
  long filtered_size = 0;
  GEN b, v;
  v = zerocol(x_length+1);

  // take log of primes in P
  /* expect this to be true assert(lg(P) == lg(x));  */
  for (i = 1; i <= x_length; i++)
      gel(P,i) = glog(gel(P,i),DEFAULTPREC); 

  for (i = 1; i <= M_size; i++)
    {
      x = gel(M,i); 
      for (j = 1; j <= x_length; j++) 
          gel(v,j) = gel(x,j);

      b = gen_0; 
      for (j = 1; j <= x_length; j++)
        b = addii(b, mulii(gel(x,j),gel(P,j)));
      gel(v,x_length+1) = mulii(k,b); 

      GEN norm = gnorml2(v);
      // keep vector x if norm <= C
      if (gcmp(norm,C) != 1)
        gel(M,++filtered_size) = leafcopy(x); 
    }
  
  return filtered_size;
}

void apply_filter_helper(long fptr, const char* in_file)
{
  GEN x = gp_read_file(in_file);
  GEN args = mkvec(stoi(3));
  // qfminim(sym_matrix, norm_bound, n_ret_vecs, flag, fptr, args, filename, prec)
  GEN d = qfminim0(x,stoi(10),NULL,3,fptr,args,NULL,DEFAULTPREC);
  pari_printf("apply filter result:\n%Ps\n", d);
}

int
main()
{
  pari_init(8000000,2);

  pari_printf("test simple filter on simple_input\n"); 
  apply_filter_helper((long)&simple_filter, "simple_input");

  pari_printf("test silly filter on leech_lattice\n"); 
  apply_filter_helper((long)&silly_filter, "leech_lattice"); 
  
  pari_close();
  return 0;
}
