#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	FILE *fd;
	fd = fopen("data", "w");
	if(fd == NULL)
	{
		printf("cannot open file");
		exit(EXIT_FAILURE);
	}
	if(fork() == 0)
	{
		printf("Parent process\n");
		fputs("first line\n", fd);
		fputs("second line\n", fd);
	}
	else
	{
		printf("child process\n");
		fputs("first line of child\n", fd);
		fputs("second line of child\n", fd);
	}
	return 0;
	
}
