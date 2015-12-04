// test filter correctness

// write my_qfminim in cpp that allows default parameters

#include <pari/pari.h>
#include <assert.h>

int main()
{
  pari_init(8000000,2);
  
  pari_sp av = avma;
  
  GEN m = zeromat(2,2);
  gel(m,1) = mkcol2s(1,2);
  gel(m,2) = mkcol2s(3,4);
  pari_printf("m = %Ps\n", m);
  GEN a = gclone(m);
  avma = av;
  pari_printf("a = %Ps\n", a);

  long i;
  GEN b = zeromat(2,4);
  
  for (i = 1; i <= 2; i++) 
    gel(b,i) = gcopy(gel(a,i));
  for (i = 1; i <= 2; i++)
    gel(b,i+2) = gcopy(gel(a,i));
  
  gunclone(a);
  a = gclone(b);
  avma = av;
  pari_printf("a = %Ps\n", a);
    
  pari_close(); 
  return 0; 
}
