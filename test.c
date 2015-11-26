// test filter correctness

// write my_qfminim in cpp that allows default parameters

#include <pari/pari.h>
#include <assert.h>

int main()
{
  pari_init(8000000,2);
  GEN x = zerocol(3);
  GEN y = mkcol3s(1,2,3);
  gel(x,2) = stoi(5); 
  x = gadd(x,y); 
  pari_printf("x = %Ps\n", x);
  pari_printf("y = %Ps\n", y);
  pari_close(); 
  return 0; 
}
