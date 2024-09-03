/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmdemirk <dmdemirk@student.42london.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:32:29 by dmdemirk          #+#    #+#             */
/*   Updated: 2024/07/11 16:00:30 by dmdemirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "tokens.h"
#include "redirection.h"
#include "execute.h"
#include <fcntl.h>
#include <sys/wait.h>

int			redirect_here_doc(t_ast *node, t_ms_data *data);
static int	open_tmp_file(const char *type);
static void	execute_child(t_ast *node, t_ms_data *data, int *file_fd);

/**
  - @brief This function handle "<<" heredoc functionality
  - 
  - @param node current node in the AST
  - @param data minishell data structure
  - @return status:
  - 0: success
  - 1: error
 */

char *process_and_reassemble(char *line, t_ms_data *data) {
    char **tokens;
    char *token;
    char *result;
    size_t line_len = strlen(line);
    size_t result_len = 0;
    int i = 0;
    
    // Allocate memory for token array (assuming a reasonable max number of tokens)
    tokens = malloc((line_len / 2 + 1) * sizeof(char *));
    if (!tokens) {
        perror("malloc");
        return NULL;
    }
    
    // Split the input line into tokens by spaces
    token = strtok(line, " ");
    while (token != NULL) {
        // Apply the processing logic to each token
        tokens[i] = expand_env_var(line, data);
        result_len += strlen(tokens[i]) + 1; // +1 for the space
        i++;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL; // Null-terminate the token array

    // Allocate memory for the final result string
    result = malloc(result_len + 1); // +1 for the null terminator
    if (!result) {
        perror("malloc");
        free(tokens);
        return NULL;
    }

    // Concatenate the tokens back into a single string
    strcpy(result, tokens[0]);
    for (int j = 1; tokens[j] != NULL; j++) {
        strcat(result, " ");
        strcat(result, tokens[j]);
    }

    // Free the memory allocated for the tokens
    free(tokens);

    return result;
}

int	redirect_here_doc(t_ast *node, t_ms_data *data)
{
	char	*line;
	char	*eof;
	int		file_fd;

	line = NULL;
	if (node->right->args[0] == NULL)
		return (1);
	file_fd = open_tmp_file("w");
	eof = ft_strdup(node->right->args[0]);
	line = process_and_reassemble(readline("> "), data);
	while (line && (ft_strcmp(line, eof) != 0))
	{
		write(file_fd, line, ft_strlen(line));
		write(file_fd, "\n", 1);
		free(line);
		line = process_and_reassemble(readline("> "), data);
	}
	free(line);
	free(eof);
	close(file_fd);
	file_fd = open_tmp_file("r");
	execute_child(node->left, data, &file_fd);
	unlink("/tmp/heredoc");
	return (0);
}

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
