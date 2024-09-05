#ifndef ERRORS_H
# define ERRORS_H

# define ERROR 1
# define SUCCESS 0

# define IS_DIRECTORY 256
# define NUMERIC_REQUIRED 257
# define TOO_MANY_ARGS 258
# define UNKNOWN_COMMAND 259
# define INVALID_ARGUMENT 260
# define PERMISSION_DENIED 261
# define ERROR_EXIT 262


/*  errors  */
void shell_error_handler(int error_code, char *str);

/*  errors_utils    */
int ft_perror(char *str);
int ft_isnumber(char *str);

#endif