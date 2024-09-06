#include "exit_status.h"
#include "libft.h"
#include <unistd.h>
#include "shell.h"

#include <stdio.h>

void exit_status_handler(t_ms_data *data, int status_code, char *err_arg);
void set_exit_status(int *exit_status, int status_code);

void exit_status_handler(t_ms_data *data, int status_code, char *err_arg)
{
    char *error_message;

    error_message = NULL;
    if (status_code == IS_DIRECTORY)
        error_message = "No such file or directory";
    else if (status_code == NUMERIC_REQUIRED)
        error_message = "numeric argument required";
    else if (status_code == TOO_MANY_ARGS)
        error_message = "too many arguments";
    else if (status_code == UNKNOWN_COMMAND)
        error_message = "command not found";
    else if (status_code == INVALID_ARGUMENT)
        error_message = "invalid argument";
    else if (status_code == PERMISSION_DENIED)
        error_message = "permission denied";
    else if (status_code == NOT_VALID_IDENTIFIER)
        error_message = "unset: not a valid identifier";
    else if (status_code == INVALID_OPTION)
        error_message = "invalid option";
    else
        error_message = "error";
    if (err_arg)
    {
        ft_putstr_fd("bash: ", STDERR_FILENO);
        ft_putstr_fd(err_arg, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putendl_fd(error_message, STDERR_FILENO);
    }
    else
    {
        ft_putstr_fd("bash: ", STDERR_FILENO);
        ft_putendl_fd(error_message, STDERR_FILENO);
    } 
    set_exit_status(&data->exit_status, status_code);
    set_shell_var(&data->shell_variables, "?", ft_itoa(data->exit_status));
    printf("status_code: %d\n", status_code);
    printf("get shell var: %s\n", get_shell_variable(data->shell_variables, "?"));
}


void    set_exit_status(int *exit_status, int status_code)
{
    *exit_status = status_code;
}