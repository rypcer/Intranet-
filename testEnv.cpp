// crt_dupenv_s.c
#include  <stdlib.h>
#include <stdio.h>
int main(void)
{
	char* pValue;
	size_t len;
	errno_t err = _dupenv_s(&pValue, &len, "pathext");
	if (!err) {
		printf("pathext = %s\n", pValue);
		free(pValue);
	}
	
}