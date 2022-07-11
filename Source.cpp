#include <stdio.h>

int main() {
	
	char x[5]; // { 'h','e','l','l','o' }
	scanf_s("%s",&x);
	const char* y = x;
	printf("%s",y);
}