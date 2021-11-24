#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <libft.h>
#include <pipex.h>

#include <stdio.h>

static t_bool	delimiter_found(char *delimiter, char *line)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == OK)
		return (TRUE);
	return (FALSE);
}

int	make_here_doc(char *delimiter)
{
	int		fd;
	char	*line;

	fd = open(".here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	while (1)
	{
		line = ft_gnl(0);
		if (!line)
			break ;
		else if (delimiter_found(delimiter, line))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (open(".here_doc", O_RDONLY));
}
