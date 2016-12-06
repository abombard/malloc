#include "malloc.h"

#define SIZE	200

int main()
{
	int i;

	char	*tiny[SIZE];
	char	*small[SIZE];
	char	*large[SIZE];

	for (i = 0; i < 200; i ++)
	{
		tiny[i] = malloc(12);
		small[i] = malloc(2000);
		large[i] = malloc(5000);
	}

	show_memory();

	for (i = 0; i < 200; i ++)
	{
		free(tiny[i]);
		free(small[i]);
		free(large[i]);
	}
	return 0;
}
