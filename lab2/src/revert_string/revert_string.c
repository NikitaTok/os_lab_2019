#include "revert_string.h"
#include <string.h>

void RevertString(char *str)
{
	int l = strlen(str);
    
	for (int i=0, j=l-1; i<j; i++, j--)
	{
        char one = str[i];
        str[i] = str[j];
        str[j] = one;
    }
}

