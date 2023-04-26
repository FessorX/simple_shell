#include "main.h"

/**
 * without_comment - deletes comments from the input
 *
 * @in: input string
 * Return: input without comments
 */
char *without_comment(char *in)
{
	int i, till;

	till = 0;
	for (i = 0; in[i]; i++)
	{
		if (in[i] == '#')
		{
			if (i == 0)
			{
				free(in);
				return (NULL);
			}

			if (in[i - 1] == ' ' || in[i - 1] == '\t' || in[i - 1] == ';')
				till = i;
		}
	}

	if (till != 0)
	{
		in = _realloc(in, i, till + 1);
		in[till] = '\0';
	}

	return (in);
}

/**
 * shell_loop - Loop of shell
 * @datash: data relevant (av, input, args)
 *
 * Return: no return.
 */
void shell_loop(data_shell *datash)
{
	int loop, i_eof;
	char *input;

	loop = 1;
	while (loop == 1)
	{
		write(STDIN_FILENO, "($) ", 5);
		input = read_line(&i_eof);
		if (i_eof != -1)
		{
			input = without_comment(input);
			if (input == NULL)
				continue;

			if (check_syntax_error(datash, input) == 1)
			{
				datash->status = 2;
				free(input);
				continue;
			}
			input = rep_var(input, datash);
			loop = split_commands(datash, input);
			datash->counter += 1;
			free(input);
		}
		else
		{
			loop = 0;
			free(input);
		}
	}
}

/**
 * get_sigint - Handle the crtl + c call in prompt
 * @sig: Signal handler
 */
void get_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n($) ", 5);
}
