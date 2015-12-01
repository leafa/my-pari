#include <pari/pari.h>
#include <assert.h>

long
simple_filter(GEN M, GEN args, long M_size)
{
  long filtered_size = 0; // 1 indexing
  long i = 1;

  for (i = 1; i <= M_size; i++)
  {
    if (itos(gcoeff(M,2,i)) % 5 == 1)
      gel(M,++filtered_size) = leafcopy(gel(M,i)); // filtered_size <= i
  }

  return filtered_size;
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

void apply_simple_filter_on_leech_lattice()
{
  pari_printf("test simple filter on leech lattice: save results whose first field is divisible by 3\n"); 
  GEN x = gp_read_file("leech_lattice");
  GEN filename = gp_read_str("simple_filter_on_leech_lattice_result"); 
  GEN args = mkvec2(filename, stoi(3)); 
  GEN d = qfminim0(x,NULL,NULL,3,(long)&simple_filter,args,DEFAULTPREC);
  pari_printf("%Ps\n", d);
}

int
main()
{
  pari_init(8000000,2);

  GEN x = gp_read_file("simple_input");
  GEN filename = gp_read_str("simple_filter_on_simple_lattice_result"); 
  GEN args = mkvec2(filename, stoi(3)); 
  GEN d = qfminim0(x,stoi(5),NULL,3,(long)&simple_filter,args,DEFAULTPREC);
  pari_printf("%Ps\n", d);
  
  pari_close();
  return 0;
}
