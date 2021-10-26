#include <libft.h>
#include <pipex.h>

int	create_path_split(char **envp, char ***path_split)
{
	char	*curr_envp;
	int		i;

	i = 0;
	while (envp && envp[i])
	{
		curr_envp = ft_strnstr(envp[i], "PATH=", 5);
		if (curr_envp)
			break;
		i++;
	}
	*path_split = ft_split(curr_envp + 5, ':');
	if (*path_split)
		return (0);
	return (-1);
}