#include "errors.h"
#include "libft.h"
#include <unistd.h>
#include "shell.h"

void shell_error_handler(int error_code, char *err_arg);

void shell_error_handler(int error_code, char *err_arg)
{
    char *error_message;

    error_message = NULL;
    if (error_code == IS_DIRECTORY)
        error_message = "No such file or directory";
    else if (error_code == NUMERIC_REQUIRED)
    {
        error_message = "numeric argument required";
    }
    else if (error_code == TOO_MANY_ARGS)
    {
        error_message = "too many arguments";
    }
    else if (error_code == UNKNOWN_COMMAND)
    {
        error_message = "command not found";
    }
    else if (error_code == INVALID_ARGUMENT)
    {
        error_message = "invalid argument";
    }
    else if (error_code == PERMISSION_DENIED)
    {
        error_message = "permission denied";
    }
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
}