#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

int ft_perror(char *str);
int ft_isnumber(char *str);

/**
- @brief return error message and exit
- 
- @param str paste the error message
- @return int return status:
- 				- 1: ERROR
 */

int	ft_perror(char *str)
{
	perror (str);
	exit(ERROR);
}

int ft_isnumber(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}