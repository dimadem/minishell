/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rocky <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:46:24 by rocky             #+#    #+#             */
/*   Updated: 2024/09/06 13:46:25 by rocky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "tokens.h"
#include "redirection.h"
#include "execute.h"
#include <fcntl.h>
#include <sys/wait.h>
#include "signals.h"

// GLOBAL VAR
// volatile tells compiler the var can change at compilation between accesses.
// sig_atomic_t used for global_vars
volatile sig_atomic_t	g_heredoc_interrupted = 0;

static int	open_tmp_file(const char *type)
{
	int	file_fd;

	file_fd = -1;
	if (ft_strcmp(type, "w") == 0)
		file_fd = open("/tmp/heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strcmp(type, "r") == 0)
		file_fd = open("/tmp/heredoc", O_RDONLY);
	if (file_fd < 0)
		ft_perror("open");
	return (file_fd);
}

static void	setup_sigint_handler(struct sigaction *sa_old)
{
	struct sigaction	sa_new;

	sa_new.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, sa_old);
}

static int	handle_heredoc_interruption(char *line, char *eof, int file_fd, \
		struct sigaction *sa_old)
{
	free(line);
	free(eof);
	close(file_fd);
	unlink("/tmp/heredoc");
	g_heredoc_interrupted = 0;
	sigaction(SIGINT, sa_old, NULL);
	return (1);
}

static void	execute_child(t_ast *node, t_ms_data *data, int *file_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_perror("fork");
	if (pid == 0)
	{
		data->std_in = dup(*file_fd);
		execute_ast(node, data);
		exit(0);
	}
	close(*file_fd);
	waitpid(pid, &data->exit_status, 0);
}

int	redirect_here_doc(t_ast *node, t_ms_data *data)
{
	char				*line;
	char				*eof;
	int					file_fd;
	struct sigaction	sa_old;

	line = NULL;
	if (node->right->args[0] == NULL)
		return (1);
	setup_sigint_handler(&sa_old);
	file_fd = open_tmp_file("w");
	eof = ft_strdup(node->right->args[0]);
	line = readline("> ");
	write_heredoc_lines(&line, file_fd, eof);
	if (g_heredoc_interrupted)
		return (handle_heredoc_interruption(line, eof, file_fd, &sa_old));
	free(line);
	free(eof);
	close(file_fd);
	sigaction(SIGINT, &sa_old, NULL);
	file_fd = open_tmp_file("r");
	execute_child(node->left, data, &file_fd);
	unlink("/tmp/heredoc");
	return (0);
}
