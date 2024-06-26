![image](https://github.com/dimadem/minishell/assets/50800563/01e0c354-b0f2-49de-a4ef-8194baca2a33)


![Norminette workflow](https://github.com/dimadem/minishell/actions/workflows/norminette.yml/badge.svg)
![Compilation workflow](https://github.com/dimadem/minishell/actions/workflows/compile.yml/badge.svg)
![Valgrind workflow](https://github.com/dimadem/minishell/actions/workflows/valgrind.yml/badge.svg)


build/: Directory for object files and other intermediate build artifacts.

docs/: Documentation for the project.

inc/: Header files for the project, defining interfaces for different modules.
- builtins.h: Declarations for built-in shell commands.
- execute.h: Declarations for command execution functions.
- parser.h: Declarations for command-line parsing functions.
- pipe.h: Declarations for piping functions.
- redirection.h: Declarations for redirection functions.
- shell.h: General declarations for the shell.
- tokens.h: Declarations for parsing functions.

lib/: Directory for third-party libraries, such as libft.

src/: Source files for the project, organized into subdirectories by functionality.
- builtins/: Implementation of built-in shell commands.
- execute/: Implementation of command execution logic.
- parser/: Implementation of command-line parsing.
- pipe/: Implementation of piping between commands.
- test/: Tests
- redirection/: Implementation of input/output redirection.
- utils/: Implementation of auxiliary functions.
- main.c: Entry point of the shell application.
