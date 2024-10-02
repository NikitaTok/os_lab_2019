#include "swap.h"

void Swap(char *left, char *right)
{
	char one = *left;
	*left = *right;
	*right = one;
}
