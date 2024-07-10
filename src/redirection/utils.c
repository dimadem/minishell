#include "shell.h"
#include <fcntl.h>
#include "libft.h"
#include "tokens.h"

int open_file(t_ast *node, char *direction);
char        *ft_get_next_line(int fd);
static char	*extract_line(char **saved);
static char	*read_from_fd(int fd, char *saved, char *buf);
char        *ft_strjoin_gnl(char *s1, char const *s2);

/**
- @brief open file in the context of redirection
-       "<" - read
-       ">" - write
-       ">>" - append
-       "tty" - open /dev/tty
- 
- @param node current node in the AST
- @param direction type of redirection 
- @return int file descriptor
 */

int open_file(t_ast *node, char *direction)
{
    int fd;
    
    if ((ft_strcmp(direction, "<") == 0) || (ft_strcmp(direction, "read") == 0))
        fd = open(node->args[0], O_RDONLY);
    else if (ft_strcmp(direction, ">") == 0)
        fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if ((ft_strcmp(direction, ">>") == 0) || (ft_strcmp(direction, "temp") == 0))
        fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (ft_strcmp(direction, "tty") == 0)
        fd = open("/dev/tty", O_RDWR);
    else
        fd = -1;
    return (fd);
}

char	*ft_get_next_line(int fd)
{
	static char	*saved = NULL;
	char		*line;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE < 1 || fd > 4095)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	saved = read_from_fd(fd, saved, buf);
	if (!saved)
	{
		free(saved);
		saved = NULL;
		return (NULL);
	}
	line = extract_line(&saved);
	if (line == NULL)
	{
		free(saved);
		saved = NULL;
	}
	return (line);
}

char	*ft_strjoin_gnl(char *s1, char const *s2)
{
	char	*str;
	size_t	s1_len;
	size_t	s2_len;

	if (!s2)
		return (NULL);
	if (s1)
		s1_len = ft_strlen(s1);
	else
		s1_len = 0;
	s2_len = ft_strlen(s2);
	str = (char *)malloc(s1_len + s2_len + 1);
	if (!str)
		return (NULL);
	if (s1)
		ft_strlcpy(str, s1, s1_len + 1);
	else
		str[0] = '\0';
	ft_memcpy(str + s1_len, s2, s2_len + 1);
	return (str);
}

static char	*read_from_fd(int fd, char *saved, char *buf)
{
	int		chars_read;
	char	*temp;

	chars_read = read(fd, buf, BUFFER_SIZE);
	while (chars_read > 0)
	{
		buf[chars_read] = '\0';
		temp = saved;
		saved = ft_strjoin_gnl(saved, buf);
		free(temp);
		if (ft_strchr(saved, '\n'))
			break ;
		chars_read = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	if (chars_read < 0)
	{
		free(saved);
		saved = NULL;
		return (NULL);
	}
	return (saved);
}

static char	*extract_line(char **saved)
{
	char	*new_line_ptr;
	char	*line;
	char	*temp;

	new_line_ptr = ft_strchr(*saved, '\n');
	if (new_line_ptr)
	{
		line = ft_substr(*saved, 0, new_line_ptr - *saved + 1);
		temp = *saved;
		*saved = ft_strdup(new_line_ptr + 1);
		free(temp);
		return (line);
	}
	if (**saved)
	{
		line = ft_strdup(*saved);
		free(*saved);
		*saved = NULL;
		return (line);
	}
	return (NULL);
}