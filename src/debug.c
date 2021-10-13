#include <stdio.h>
#include <main.h>
#include <unistd.h>

void	print_envp(char **envp)
{
	int	i;

	i = 0;
	printf("\n******\n\n");
	while (envp[i])
		{
			printf("envp[%d] = %s\n", i, envp[i]);
			i++;
		}
	printf("\n******\n\n");
}

void	check_access(char *file)
{
	if (access(file, R_OK) == 0)
		printf("R_OK\n");
	if (access(file, W_OK) == 0)
		printf("W_OK\n");
	if (access(file, X_OK) == 0)
		printf("X_OK\n");
	if (access(file, F_OK) == 0)
		printf("F_OK\n");
	if (access(file, F_OK | R_OK) == 0)
		printf("F_OK | R_OK\n");
}