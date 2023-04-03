#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

int	fatal_error(char *msg) __attribute__((noreturn));

int	fatal_error(char *msg) {
	dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
	exit(1);
}

int	interpret(char* line) {
	extern char	**environ;
	char		*argv[] = {line, NULL};
	pid_t		pid;
	int			wstatus;

	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	// child process
	else if (pid == 0) {
		execve(line, argv, environ);
		fatal_error("execve");
	}
	// parent process
	else {
		wait(&wstatus);
		return (WEXITSTATUS(wstatus));
	}
}

int	main() {
	int		status;
	char	*line;

	rl_outstream = stderr;
	while (1) {
		line = readline("minishell$ ");
		if (line == NULL)
			break;
		if (*line)
			add_history(line);
		status = interpret(line);
		free(line);
	}
	exit(status);
}
