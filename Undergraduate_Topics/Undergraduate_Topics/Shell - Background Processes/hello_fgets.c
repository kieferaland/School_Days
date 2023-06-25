#include <stdio.h>
// #include <stdlib.h>
#define MAX_LINE_LEN 1024

int main()
{
	printf("enter your name.\n");
	char line[MAX_LINE_LEN];
	if (fgets(line, sizeof(line), stdin) != NULL){
		printf("your name is: %s", line);
	}
	return 0;
}