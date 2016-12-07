#include <stdlib.h>

int main()
{
  void *addr1;

  addr1 = malloc(16);
  free(addr1);
  return 0;
}
